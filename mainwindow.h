#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include "giosapiclient.h" // Dołącz definicje struktur (StationInfo itp.)

// === POTRZEBNE FORWARD DECLARATIONS ===
class QListWidgetItem;
namespace Ui { class mainWindow; } // Deklaracja wyprzedzająca dla UI
// Deklaracje z QtCharts (jeśli nie używasz using namespace w cpp)
namespace QtCharts {
class QChartView;
class QChart;
}
// =====================================

/**
 * @file mainwindow.h
 * @brief Definicja klasy mainWindow - głównego okna aplikacji.
 * @author Olga Baran
 */

/**
 * @class mainWindow
 * @brief Główne okno aplikacji monitorującej jakość powietrza.
 *
 * Odpowiada za interfejs użytkownika, w tym wyświetlanie list stacji i sensorów,
 * prezentację danych pomiarowych na wykresie i w polu tekstowym, obsługę
 * filtrowania stacji, zapisu/odczytu danych do/z pliku JSON oraz wyświetlanie
 * wyników prostej analizy danych. Komunikuje się z GiosApiClient w celu
 * pobierania danych z sieci.
 */
class mainWindow : public QMainWindow
{
    Q_OBJECT // Makro wymagane dla sygnałów i slotów

public:
    /**
     * @brief Konstruktor głównego okna.
     * Inicjalizuje interfejs użytkownika z pliku .ui, tworzy obiekt GiosApiClient
     * i konfiguruje połączenia sygnał-slot.
     * @param parent Wskaźnik na widget rodzica (zwykle nullptr).
     */
    explicit mainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor. Zwalnia obiekt interfejsu użytkownika.
     */
    ~mainWindow();

private slots:
    // === SLOTY OBSŁUGUJĄCE INTERAKCJĘ UŻYTKOWNIKA ===
    // Te sloty są prawdopodobnie połączone automatycznie przez mechanizm `connectSlotsByName`
    // dzięki konwencji nazewnictwa `on_nazwaWidgetu_nazwaSygnalu`.

    /** @brief Wywoływany po kliknięciu przycisku "Pobierz stacje" (fetchStationsButton). */
    void on_fetchStationsButton_clicked();
    /** @brief Wywoływany po kliknięciu elementu na liście stacji (listWidget). */
    void on_listWidget_itemClicked(QListWidgetItem *item);
    /** @brief Wywoływany po kliknięciu elementu na liście sensorów (sensorsListWidget). */
    void on_sensorsListWidget_itemClicked(QListWidgetItem *item);
    /** @brief Wywoływany po kliknięciu przycisku "Zapisz dane" (saveDataButton). */
    void on_saveDataButton_clicked();
    /** @brief Wywoływany po kliknięciu przycisku "Wczytaj dane" (loadDataButton). */
    void on_loadDataButton_clicked();
    /** @brief Wywoływany po kliknięciu przycisku "Analizuj Dane" (analyzeButton). */
    void on_analyzeButton_clicked();
    /** @brief Wywoływany po zmianie tekstu w polu filtra miejscowości (cityFilterLineEdit). */
    void on_cityFilterLineEdit_textChanged(const QString &text);

    // === SLOTY OBSŁUGUJĄCE SYGNAŁY Z GiosApiClient ===
    /**
     * @brief Odbiera listę stacji z GiosApiClient, zapisuje ją i aktualizuje widok listy.
     * @param stations Lista informacji o stacjach.
     */
    void handleStationsFetched(const QList<StationInfo>& stations);
    /**
     * @brief Odbiera listę sensorów z GiosApiClient i aktualizuje widok listy sensorów.
     * @param sensors Lista informacji o sensorach.
     */
    void handleSensorsFetched(const QList<SensorInfo>& sensors);
    /**
      * @brief Odbiera dane pomiarowe z GiosApiClient, zapisuje je i aktualizuje wykres oraz pole tekstowe.
      * @param measurementResult Dane pomiarowe dla jednego parametru.
      */
    void handleMeasurementDataFetched(const MeasurementData& measurementResult);
    /**
     * @brief Odbiera informację o błędzie z GiosApiClient, wyświetla komunikat i oferuje wczytanie danych.
     * @param errorString Tekst błędu.
     */
    void handleNetworkError(const QString& errorString);


private:
    // === METODY POMOCNICZE ===
    /**
     * @brief Aktualizuje widżet wykresu (ui->chartView) na podstawie danych w `currentMeasurementData`.
     * Tworzy nowy obiekt QChart z odpowiednimi seriami i osiami. Poprzedni QChart jest usuwany.
     */
    void displayChart();

    /**
     * @brief Filtruje listę stacji (ui->listWidget) na podstawie podanego tekstu.
     * Używa pełnej listy stacji przechowywanej w `allStationsList`.
     * @param cityText Tekst do filtrowania nazw miejscowości.
     */
    void filterStationsByCity(const QString &cityText);

    // === POLA KLASY ===
    Ui::mainWindow *ui;              ///< Wskaźnik na obiekt UI zarządzający widgetami z pliku .ui.
    GiosApiClient *apiClient;        ///< Obiekt odpowiedzialny za pobieranie danych z API.
    MeasurementData currentMeasurementData; ///< Bufor na ostatnio pobrane lub wczytane dane pomiarowe.
    QList<StationInfo> allStationsList; ///< Pełna lista stacji pobrana z API (używana do filtrowania).
};
#endif // MAINWINDOW_H
