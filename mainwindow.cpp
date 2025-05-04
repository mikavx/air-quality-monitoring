#include "mainwindow.h"
#include "ui_mainwindow.h" // Ważny include

// Includy Qt
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>          // Dla qWarning
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QPainter>
#include <QTextEdit>       // Potrzebne dla analysisResultsTextEdit
#include <QPushButton>     // Potrzebne dla przycisku w QMessageBox
#include <QFileInfo>       // Dla pobrania nazwy pliku w błędach

// Includy QtCharts
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

// Includy standardowe
#include <stdexcept>       // Dla std::exception i std::runtime_error
#include <algorithm>       // Dla std::sort
#include <string>          // Dla std::to_string

// Konstruktor
mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow) // Poprawna inicjalizacja UI
{
    ui->setupUi(this); // Konfiguracja UI z pliku .ui

    apiClient = new GiosApiClient(this);

    // --- Połączenia sygnałów i slotów ---
    connect(apiClient, &GiosApiClient::stationsFetched, this, &mainWindow::handleStationsFetched);
    connect(apiClient, &GiosApiClient::networkError, this, &mainWindow::handleNetworkError);
    connect(apiClient, &GiosApiClient::sensorsFetched, this, &mainWindow::handleSensorsFetched);
    connect(apiClient, &GiosApiClient::measurementDataFetched, this, &mainWindow::handleMeasurementDataFetched);

    if (ui->listWidget) {
        connect(ui->listWidget, &QListWidget::itemClicked, this, &mainWindow::on_listWidget_itemClicked);
    } else { qWarning() << "listWidget not found in UI."; }

    if (ui->sensorsListWidget) {
        connect(ui->sensorsListWidget, &QListWidget::itemClicked, this, &mainWindow::on_sensorsListWidget_itemClicked);
    } else { qWarning() << "sensorsListWidget not found in UI."; }

    // Sprawdzenie istnienia innych kluczowych widgetów (można usunąć po debugowaniu)
    if (!ui->chartView) { qWarning() << "chartView not found in UI.";}
    if (!ui->measurementDataTextEdit) { qWarning() << "measurementDataTextEdit not found in UI.";}
    // Sprawdź nazwę widgetu na wyniki analizy (zakładam, że to QTextEdit)
    if (!ui->analysisResultsTextEdit) { qWarning() << "analysisResultsTextEdit not found in UI.";}
    if (!ui->analyzeButton) { qWarning() << "analyzeButton not found in UI."; }
    if (!ui->cityFilterLineEdit) { qWarning() << "cityFilterLineEdit not found in UI."; }

    if (statusBar()) {
        statusBar()->showMessage("Aplikacja gotowa.", 3000);
    }
}

// Destruktor
mainWindow::~mainWindow()
{
    delete ui; // Usuwamy obiekt UI
}

// === Funkcje Prywatne ===

/**
 * @brief Rysuje wykres danych pomiarowych w widżecie chartView.
 *
 * Tworzy nową serię danych, osie i obiekt QChart na podstawie danych
 * przechowywanych w `currentMeasurementData`. Poprzedni wykres jest automatycznie usuwany.
 * Jeśli brak poprawnych danych, wyświetla pusty wykres z informacją.
 */
void mainWindow::displayChart()
{
    if (!ui->chartView) {
        qWarning() << "Nie można wyświetlić wykresu - brak widgetu chartView.";
        return;
    }

    // Tworzenie nowej serii
    QLineSeries *series = new QLineSeries();
    series->setName(currentMeasurementData.key.isEmpty() ? "Dane" : currentMeasurementData.key);

    // Wypełnianie serii poprawnymi danymi
    int validPoints = 0;
    for (const Measurement& m : currentMeasurementData.values) {
        if (m.date.isValid() && !m.value.isNull()) {
            series->append(m.date.toMSecsSinceEpoch(), m.value.toDouble());
            validPoints++;
        }
    }

    // Tworzenie nowego wykresu
    QChart *chart = new QChart(); // Nowy wykres przy każdym rysowaniu
    chart->setTitle("Dane pomiarowe dla: " + currentMeasurementData.key);
    chart->setAnimationOptions(QChart::SeriesAnimations); // Prosta animacja

    if (validPoints == 0) {
        // Jeśli nie ma punktów, wyświetl komunikat i pusty wykres
        qWarning() << "Brak poprawnych danych do wyświetlenia na wykresie dla klucza:" << currentMeasurementData.key;
        chart->setTitle("Brak poprawnych danych do wyświetlenia");
        delete series; // Usuń pustą serię, bo QChart jej nie przejmie na własność bez addSeries
    } else {
        // Jeśli są punkty, dodaj serię i osie
        chart->addSeries(series); // Chart przejmuje serię na własność
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        // Konfiguracja osi X (Data i Czas)
        QDateTimeAxis *axisX = new QDateTimeAxis;
        axisX->setFormat("yyyy-MM-dd HH:mm");
        axisX->setTitleText("Data pomiaru");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX); // Powiąż serię z osią

        // Konfiguracja osi Y (Wartości)
        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText("Wartość [" + currentMeasurementData.key + "]");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    }

    // Ustawienie nowego wykresu w widoku QChartView
    // Stary wykres zostanie automatycznie usunięty przez QChartView
    ui->chartView->setChart(chart); // QChartView przejmuje chart na własność
    ui->chartView->setRenderHint(QPainter::Antialiasing); // Wygładzanie
}

/**
 * @brief Filtruje listę stacji wyświetlaną w ui->listWidget na podstawie podanego tekstu.
 * @param cityText Tekst wpisany przez użytkownika w polu filtra miejscowości.
 *                 Filtrowanie ignoruje wielkość liter i białe znaki na brzegach.
 */
void mainWindow::filterStationsByCity(const QString &cityText)
{
    if (!ui->listWidget) {
        qWarning() << "Cannot filter stations: listWidget is null.";
        return;
    }

    ui->listWidget->clear(); // Wyczyść listę przed filtrowaniem
    QString filter = cityText.trimmed().toLower(); // Przygotuj filtr
    int itemsAdded = 0;

    // Iteracja po pełnej liście stacji przechowywanej w allStationsList
    for (const StationInfo& station : allStationsList) {
        // Warunek dopasowania: filtr pusty LUB nazwa miasta zawiera filtr (ignorując wielkość liter)
        bool match = filter.isEmpty() || (!station.cityName.isEmpty() && station.cityName.toLower().contains(filter));

        if (match) {
            // Utwórz nowy element listy
            QListWidgetItem *item = new QListWidgetItem(QString("%1 (%2) [ID: %3]") // Użyj [] dla ID
                                                            .arg(station.stationName, station.cityName)
                                                            .arg(station.id));
            item->setData(Qt::UserRole, station.id); // Zapisz ID stacji w danych elementu
            ui->listWidget->addItem(item);
            itemsAdded++;
        }
    }

    // Jeśli nic nie pasowało do filtra (a filtr nie był pusty), pokaż komunikat
    if (itemsAdded == 0 && !filter.isEmpty()) {
        ui->listWidget->addItem(QString("Nie znaleziono stacji dla: '%1'").arg(cityText)); // Najpierw dodaj tekst
        if (ui->listWidget->count() > 0) { // Sprawdź czy element został dodany (zawsze powinien)
            // Pobierz ostatnio dodany element
            QListWidgetItem *item = ui->listWidget->item(ui->listWidget->count() - 1);
            if (item) { // Dodatkowe sprawdzenie wskaźnika
                // Ustaw flagę, aby był nieaktywny
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            }
        }
    }
}

// === Implementacje Slotów ===

void mainWindow::on_fetchStationsButton_clicked()
{
    // Wyczyszczenie kontrolek
    if (ui->listWidget) ui->listWidget->clear();
    if (ui->sensorsListWidget) ui->sensorsListWidget->clear();
    if (ui->chartView) ui->chartView->setChart(new QChart()); // Ustaw nowy pusty wykres
    if (ui->measurementDataTextEdit) ui->measurementDataTextEdit->clear();
    if (ui->analysisResultsTextEdit) ui->analysisResultsTextEdit->clear(); // Czyszczenie wyników analizy
    if (ui->selectedStationLabel) {
        ui->selectedStationLabel->setText("Wybierz stację..."); // Ustaw placeholder
        ui->selectedStationLabel->setToolTip("");
    }

    if (statusBar()) statusBar()->showMessage("Pobieranie listy stacji...");
    apiClient->fetchAllStations(); // Wywołaj pobieranie
}

void mainWindow::handleStationsFetched(const QList<StationInfo>& stations)
{
    this->allStationsList = stations; // Zapisz pobraną listę jako pełną listę
    if (statusBar()) statusBar()->showMessage(QString("Pobrano %1 stacji.").arg(stations.count()), 5000);

    // Pobierz aktualny tekst filtra i zastosuj filtrowanie
    QString currentFilterText = ui->cityFilterLineEdit ? ui->cityFilterLineEdit->text() : QString();
    filterStationsByCity(currentFilterText);
}

void mainWindow::handleNetworkError(const QString& errorString)
{
    QString userMessage = "Wystąpił błąd sieci lub danych API:\n" + errorString;
    if (statusBar()) statusBar()->showMessage("Błąd pobierania danych.", 5000);

    // Pokaż okno błędu z opcją wczytania pliku
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Błąd");
    msgBox.setText(userMessage + "\n\nCzy chcesz spróbować wczytać dane z pliku?");
    msgBox.addButton("OK", QMessageBox::AcceptRole); // Przycisk domyślny
    QPushButton *loadButton = msgBox.addButton("Wczytaj z pliku", QMessageBox::ActionRole);

    msgBox.exec(); // Wyświetl i czekaj na wybór

    // Jeśli użytkownik kliknął "Wczytaj z pliku"
    if (msgBox.clickedButton() == loadButton) {
        on_loadDataButton_clicked(); // Wywołaj slot wczytywania
    }
}

/**
 * @brief Slot wywoływany po kliknięciu elementu na liście stacji (ui->listWidget).
 * Odczytuje ID wybranej stacji, aktualizuje etykietę informacyjną
 * i inicjuje pobieranie listy sensorów dla tej stacji przez GiosApiClient.
 * @param item Wskaźnik na kliknięty element QListWidgetItem.
 */
void mainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // Sprawdź, czy kliknięty element jest prawidłowy i aktywny
    if (!item || !item->flags().testFlag(Qt::ItemIsEnabled)) {
        // Jeśli kliknięto pusty obszar lub element nieaktywny,
        // wyczyść informacje o wybranej stacji i dalsze kontrolki.
        qDebug() << "Item clicked is null or disabled.";
        if (ui->selectedStationLabel) {
            ui->selectedStationLabel->setText("Wybierz stację z listy..."); // Ustaw placeholder
            ui->selectedStationLabel->setToolTip(""); // Wyczyść podpowiedź
        }
        if (ui->sensorsListWidget) ui->sensorsListWidget->clear();
        if (ui->chartView) ui->chartView->setChart(new QChart());
        if (ui->measurementDataTextEdit) ui->measurementDataTextEdit->clear();
        if (ui->analysisResultsTextEdit) ui->analysisResultsTextEdit->clear();
        return; // Zakończ działanie slotu
    }

    // Spróbuj odczytać ID stacji zapisane w danych elementu listy
    bool idReadOk;
    int stationId = item->data(Qt::UserRole).toInt(&idReadOk);

    // Jeśli udało się odczytać poprawne ID
    if (idReadOk) {
        // Pobierz tekst klikniętego elementu (np. "Nazwa Stacji (Miasto) [ID: 123]")
        QString selectedStationListText = item->text();
        qDebug() << "Station selected:" << selectedStationListText << "ID:" << stationId;

        if (ui->selectedStationLabel) {
            // Ustaw tekst etykiety, możesz użyć tekstu z listy lub sformatować inaczej
            ui->selectedStationLabel->setText(QString("Wybrana stacja: %1").arg(selectedStationListText));
            // Ustaw podpowiedź (tooltip) z samym ID
            ui->selectedStationLabel->setToolTip(QString("ID stacji: %1").arg(stationId));
        } else {
            qWarning() << "Nie znaleziono widgetu 'selectedStationLabel' w UI.";
        }

        // Wyczyść kontrolki związane z sensorami i danymi przed nowym zapytaniem
        if (ui->sensorsListWidget) {
            ui->sensorsListWidget->clear();
            ui->sensorsListWidget->addItem("Pobieranie sensorów..."); // Pokaż status ładowania
            if(ui->sensorsListWidget->count() > 0)
                ui->sensorsListWidget->item(0)->setFlags(ui->sensorsListWidget->item(0)->flags() & ~Qt::ItemIsEnabled);
        }
        if (ui->chartView) {
            ui->chartView->setChart(new QChart()); // Wyzeruj wykres
        }
        if (ui->measurementDataTextEdit) {
            ui->measurementDataTextEdit->clear(); // Wyzeruj pole tekstowe danych
            ui->measurementDataTextEdit->setPlaceholderText("");
        }
        if (ui->analysisResultsTextEdit) {
            ui->analysisResultsTextEdit->clear(); // Wyzeruj pole analizy
            ui->analysisResultsTextEdit->setPlaceholderText("");
        }

        // Poinformuj użytkownika na pasku statusu
        if (statusBar()) {
            statusBar()->showMessage(QString("Pobieranie sensorów dla stacji ID: %1...").arg(stationId));
        }
        // Wywołaj metodę API do pobrania sensorów dla wybranej stacji
        apiClient->fetchSensorsForStation(stationId);

    } else {
        // Jeśli nie udało się odczytać ID stacji
        qWarning() << "Nie udało się odczytać poprawnego ID stacji z klikniętego elementu:" << item->text();
        // Wyczyść etykietę wybranej stacji w razie błędu
        if (ui->selectedStationLabel) {
            ui->selectedStationLabel->setText("Błąd odczytu ID stacji!");
            ui->selectedStationLabel->setToolTip("");
        }
        // Pokaż błąd na pasku statusu
        if (statusBar()) {
            statusBar()->showMessage("Błąd: Nieprawidłowe dane dla wybranej stacji.", 3000);
        }
        if (ui->sensorsListWidget) ui->sensorsListWidget->clear();
        if (ui->chartView) ui->chartView->setChart(new QChart());
        if (ui->measurementDataTextEdit) ui->measurementDataTextEdit->clear();
        if (ui->analysisResultsTextEdit) ui->analysisResultsTextEdit->clear();
    }
}

void mainWindow::handleSensorsFetched(const QList<SensorInfo>& sensors)
{
    if (statusBar()) statusBar()->showMessage(QString("Pobrano %1 sensorów.").arg(sensors.count()), 5000);
    if (!ui->sensorsListWidget) return;

    ui->sensorsListWidget->clear();

    if (sensors.isEmpty()) {
        ui->sensorsListWidget->addItem("Brak dostępnych sensorów."); // Najpierw dodaj tekst
        if (ui->sensorsListWidget->count() > 0) { // Sprawdź czy element został dodany
            // Pobierz ostatnio dodany element
            QListWidgetItem *item = ui->sensorsListWidget->item(ui->sensorsListWidget->count() - 1);
            if (item) { // Dodatkowe sprawdzenie wskaźnika
                // Ustaw flagę, aby był nieaktywny
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            }
        }
    } else {
        for (const auto& sensor : sensors) {
            // Wyświetl nazwę, kod i ID sensora
            QListWidgetItem *item = new QListWidgetItem(QString("%1 (%2) [ID: %3]")
                                                            .arg(sensor.paramName, sensor.paramCode).arg(sensor.id));
            item->setData(Qt::UserRole, sensor.id); // Zapisz ID sensora
            ui->sensorsListWidget->addItem(item);
        }
    }
}

void mainWindow::on_sensorsListWidget_itemClicked(QListWidgetItem *item)
{
    if (!item || !item->flags().testFlag(Qt::ItemIsEnabled)) return;

    bool ok;
    int sensorId = item->data(Qt::UserRole).toInt(&ok);

    if (ok) {
        // Wyczyszczenie kontrolek przed pobraniem danych pomiarowych
        if (ui->chartView) ui->chartView->setChart(new QChart());
        if (ui->measurementDataTextEdit) {
            ui->measurementDataTextEdit->clear();
            ui->measurementDataTextEdit->setPlaceholderText(QString("Pobieranie danych dla sensora ID: %1...").arg(sensorId));
        }
        if (ui->analysisResultsTextEdit) ui->analysisResultsTextEdit->clear();

        if (statusBar()) statusBar()->showMessage(QString("Pobieranie danych dla sensora ID: %1...").arg(sensorId));
        apiClient->fetchMeasurementData(sensorId);
    } else {
        qWarning() << "Nie udało się odczytać ID sensora z elementu:" << item->text();
        if (statusBar()) statusBar()->showMessage("Błąd: Nieprawidłowe ID sensora.", 3000);
    }
}

void mainWindow::handleMeasurementDataFetched(const MeasurementData& measurementResult)
{
    this->currentMeasurementData = measurementResult; // Zapisz aktualne dane

    if (statusBar()) {
        statusBar()->showMessage(QString("Pobrano %1 pomiarów dla %2.")
                                     .arg(currentMeasurementData.values.count())
                                     .arg(currentMeasurementData.key), 5000);
    }

    // Aktualizuj wykres
    displayChart();

    // Aktualizuj pole tekstowe z danymi
    if (ui->measurementDataTextEdit) {
        ui->measurementDataTextEdit->clear();
        ui->measurementDataTextEdit->setPlaceholderText(""); // Usuń placeholder
        // Użyj HTML dla pogrubienia tytułu
        ui->measurementDataTextEdit->append(QString("<b>Dane dla: %1</b>").arg(currentMeasurementData.key));
        ui->measurementDataTextEdit->append("------------------------------------");
        if (currentMeasurementData.values.isEmpty()) {
            ui->measurementDataTextEdit->append("Brak danych pomiarowych.");
        } else {
            for (const Measurement& m : currentMeasurementData.values) {
                QString valueStr = m.value.isNull() ? "[brak]" : QString::number(m.value.toDouble());
                ui->measurementDataTextEdit->append(QString("%1: %2")
                                                        .arg(m.date.toString("yyyy-MM-dd HH:mm:ss"))
                                                        .arg(valueStr));
            }
        }
        ui->measurementDataTextEdit->append("------------------------------------");
    } else {
        qWarning() << "measurementDataTextEdit not found. Cannot display text data.";
    }

    // Wyczyść wyniki poprzedniej analizy przy ładowaniu nowych danych
    if(ui->analysisResultsTextEdit) {
        ui->analysisResultsTextEdit->clear();
        ui->analysisResultsTextEdit->setPlaceholderText("Kliknij 'Analizuj Dane'"); // Dodaj placeholder
    }
}

void mainWindow::on_saveDataButton_clicked()
{
    if (currentMeasurementData.key.isEmpty() || currentMeasurementData.values.isEmpty()) {
        QMessageBox::information(this, "Brak danych", "Brak danych pomiarowych do zapisania.");
        return;
    }

    // Okno dialogowe wyboru pliku
    QString defaultFileName = QString("dane_%1_%2.json").arg(currentMeasurementData.key).arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getSaveFileName(this, "Zapisz dane", documentsPath + "/" + defaultFileName, "Pliki JSON (*.json)");

    if (fileName.isEmpty()) return; // Anulowano

    // Przygotowanie struktury JSON
    QJsonObject rootObject;
    rootObject["key"] = currentMeasurementData.key;
    QJsonArray valuesArray;
    for (const Measurement& m : currentMeasurementData.values) {
        QJsonObject mObj;
        mObj["date"] = m.date.toString(Qt::ISODate); // Zapisz w standardzie ISO
        mObj["value"] = m.value.isNull() ? QJsonValue::Null : m.value.toDouble();
        valuesArray.append(mObj);
    }
    rootObject["values"] = valuesArray;
    QJsonDocument jsonDoc(rootObject);

    // Zapis do pliku z obsługą wyjątków
    try {
        QFile file(fileName);
        // Otwórz plik (rzuca wyjątek w razie błędu)
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw std::runtime_error(file.errorString().toStdString());
        }
        // Zapisz (rzuca wyjątek w razie błędu)
        if (file.write(jsonDoc.toJson()) == -1) {
            file.close(); // Zamknij przed rzuceniem
            throw std::runtime_error(file.errorString().toStdString());
        }
        file.close(); // Zamknij po udanym zapisie
        if (statusBar()) statusBar()->showMessage(QString("Dane zapisano do: %1").arg(QFileInfo(fileName).fileName()), 5000);

    } catch (const std::exception &e) {
        QString errorMsg = QString("Błąd zapisu do pliku '%1':\n%2")
                               .arg(QFileInfo(fileName).fileName()).arg(QString::fromStdString(e.what()));
        qWarning() << "Wyjątek podczas zapisu pliku:" << e.what();
        QMessageBox::critical(this, "Błąd Zapisu", errorMsg);
        if (statusBar()) statusBar()->showMessage("Błąd zapisu pliku.", 5000);
    } catch (...) { // Łapanie nieznanych wyjątków
        QMessageBox::critical(this, "Nieznany Błąd", "Wystąpił nieznany błąd podczas zapisu pliku.");
        if (statusBar()) statusBar()->showMessage("Nieznany błąd zapisu.", 5000);
    }
}

void mainWindow::on_loadDataButton_clicked()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(this, "Wczytaj dane", documentsPath, "Pliki JSON (*.json)");
    if (fileName.isEmpty()) return; // Anulowano

    try {
        // Otwarcie i odczyt pliku
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Nie można otworzyć pliku: " + file.errorString().toStdString());
        }
        QByteArray jsonData = file.readAll();
        file.close();

        // Parsowanie JSON
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            throw std::runtime_error("Błąd parsowania JSON: " + parseError.errorString().toStdString() + " (pozycja: " + std::to_string(parseError.offset) + ")");
        }
        if (!jsonDoc.isObject()) { throw std::runtime_error("Nieprawidłowy format JSON (oczekiwano obiektu)."); }

        QJsonObject rootObject = jsonDoc.object();
        MeasurementData loadedData;

        // Odczyt klucza
        if (rootObject.contains("key") && rootObject["key"].isString()) {
            loadedData.key = rootObject["key"].toString();
        } else { throw std::runtime_error("Brak/niepoprawny klucz 'key' w JSON."); }

        // Odczyt wartości
        if (rootObject.contains("values") && rootObject["values"].isArray()) {
            QJsonArray valuesArray = rootObject["values"].toArray();
            loadedData.values.reserve(valuesArray.count()); // Optymalizacja
            for(const QJsonValue& val : valuesArray) {
                if (!val.isObject()) { qWarning() << "Pomijam element niebędący obiektem w tablicy 'values'"; continue; }
                QJsonObject mObj = val.toObject();
                Measurement m;
                // Data (oczekiwany format ISO)
                if (mObj.contains("date") && mObj["date"].isString()) {
                    m.date = QDateTime::fromString(mObj["date"].toString(), Qt::ISODate); // Wczytaj ISO
                    if (!m.date.isValid()) { qWarning() << "Pomijam pomiar - niepoprawny format daty ISO:" << mObj["date"].toString(); continue; }
                } else { qWarning() << "Pomijam pomiar - brak pola 'date'"; continue; }
                // Wartość (null lub double)
                if (mObj.contains("value")) {
                    if (mObj["value"].isNull()) { m.value = QVariant(); }
                    else if (mObj["value"].isDouble()) { m.value = mObj["value"].toDouble(); }
                    else { qWarning() << "Pomijam pomiar - wartość nie jest double ani null. Typ:" << mObj["value"].type(); m.value = QVariant(); }
                } else { qWarning() << "Pomijam pomiar - brak pola 'value'"; continue; }
                loadedData.values.append(m);
            }
            // Sortowanie
            if(!loadedData.values.isEmpty()) {
                std::sort(loadedData.values.begin(), loadedData.values.end(),
                          [](const Measurement& a, const Measurement& b) { return a.date < b.date; });
            }
        } else { throw std::runtime_error("Brak/niepoprawna tablica 'values' w JSON."); }

        // Aktualizacja danych i UI
        currentMeasurementData = loadedData;
        handleMeasurementDataFetched(currentMeasurementData); // Wywołaj slot do aktualizacji UI

        if (statusBar()) statusBar()->showMessage(QString("Dane wczytano z: %1").arg(QFileInfo(fileName).fileName()), 5000);

    } catch (const std::exception &e) {
        QString errorMsg = QString("Błąd wczytywania pliku '%1':\n%2")
                               .arg(QFileInfo(fileName).fileName()).arg(QString::fromStdString(e.what()));
        qWarning() << "Wyjątek podczas wczytywania pliku:" << e.what();
        QMessageBox::critical(this, "Błąd Wczytywania", errorMsg);
        if (statusBar()) statusBar()->showMessage("Błąd wczytywania pliku.", 5000);
        // Wyczyść dane w przypadku błędu
        currentMeasurementData = MeasurementData();
        handleMeasurementDataFetched(currentMeasurementData); // Aktualizuj UI
    } catch (...) {
        QMessageBox::critical(this, "Nieznany Błąd", "Wystąpił nieznany błąd podczas wczytywania pliku.");
        if (statusBar()) statusBar()->showMessage("Nieznany błąd wczytywania.", 5000);
        // Wyczyść dane w przypadku błędu
        currentMeasurementData = MeasurementData();
        handleMeasurementDataFetched(currentMeasurementData);
    }
}


void mainWindow::on_analyzeButton_clicked()
{
    if (currentMeasurementData.values.isEmpty()) {
        QMessageBox::information(this, "Brak danych", "Brak danych pomiarowych do analizy.");
        // Użyj pola tekstowego do wyświetlenia komunikatu
        if (ui->analysisResultsTextEdit) ui->analysisResultsTextEdit->setPlainText("Brak danych do analizy.");
        return;
    }

    // Zmienne i logika obliczeń
    double minValue = 1e100, maxValue = -1e100, sum = 0.0;
    int validCount = 0;
    QDateTime minDate, maxDate, firstValidDate, lastValidDate;
    double firstValidValue = 0.0, lastValidValue = 0.0;
    bool isFirstValidFound = false;

    // Iteracja tylko po poprawnych danych
    for (const Measurement& m : currentMeasurementData.values) {
        if (m.date.isValid() && !m.value.isNull()) {
            double currentValue = m.value.toDouble();
            validCount++;
            sum += currentValue;
            if (currentValue < minValue) { minValue = currentValue; minDate = m.date; }
            if (currentValue > maxValue) { maxValue = currentValue; maxDate = m.date; }
            if (!isFirstValidFound) { firstValidValue = currentValue; firstValidDate = m.date; isFirstValidFound = true; }
            lastValidValue = currentValue; lastValidDate = m.date;
        }
    }

    // Formatowanie wyników jako HTML
    QString analysisHtmlText;
    if (validCount > 0) {
        double average = sum / validCount; // Oblicz średnią
        analysisHtmlText = QString("<b>Analiza dla: %1</b><br>").arg(currentMeasurementData.key);
        analysisHtmlText += QString("Liczba pomiarów: %1<br>").arg(validCount);
        analysisHtmlText += QString("Min: %1 (%2)<br>").arg(minValue).arg(minDate.toString("yyyy-MM-dd HH:mm"));
        analysisHtmlText += QString("Max: %1 (%2)<br>").arg(maxValue).arg(maxDate.toString("yyyy-MM-dd HH:mm"));
        analysisHtmlText += QString("Średnia: %1<br>").arg(average);
        if (validCount > 1) { // Oblicz trend tylko jeśli są co najmniej 2 punkty
            analysisHtmlText += "<br>"; // Odstęp
            analysisHtmlText += QString("Pierwszy pomiar (%1): %2<br>").arg(firstValidDate.toString("yy-MM-dd HH:mm")).arg(firstValidValue);
            analysisHtmlText += QString("Ostatni pomiar (%1): %2<br>").arg(lastValidDate.toString("yy-MM-dd HH:mm")).arg(lastValidValue);
            if (lastValidValue > firstValidValue) analysisHtmlText += "<b>Trend: Wzrostowy</b>";
            else if (lastValidValue < firstValidValue) analysisHtmlText += "<b>Trend: Spadkowy</b>";
            else analysisHtmlText += "<b>Trend: Stabilny</b>";
        } else {
            analysisHtmlText += "<br>Trend: Zbyt mało danych.";
        }
    } else {
        analysisHtmlText = "Brak poprawnych danych do analizy.";
    }

    // Wyświetlanie w QTextEdit
    if (ui->analysisResultsTextEdit) {
        ui->analysisResultsTextEdit->setHtml(analysisHtmlText);
    } else {
        qWarning() << "analysisResultsTextEdit widget not found!";
        // Awaryjne okno (usuń HTML)
        QString plainText = analysisHtmlText;
        plainText.replace("<br>", "\n"); plainText.remove("<b>"); plainText.remove("</b>");
        QMessageBox::information(this, "Wyniki Analizy", plainText);
    }
}

void mainWindow::on_cityFilterLineEdit_textChanged(const QString &text)
{
    // Wywołaj filtrowanie przy każdej zmianie tekstu
    filterStationsByCity(text);
}

