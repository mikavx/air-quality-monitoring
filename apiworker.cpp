#include "apiworker.h"
#include <QNetworkRequest>
#include <QThread> // Potrzebne dla QThread::currentThreadId()
#include <QDebug>
#include <algorithm> // Dla std::sort
#include <QTimeZone>   // Potrzebne do parsowania daty

ApiWorker::ApiWorker(QObject *parent) : QObject(parent)
{
    qDebug() << "ApiWorker created in thread:" << QThread::currentThreadId();
    // Ważne: networkManager jest tworzony w tym wątku, w którym stworzono ApiWorker
    // Zostanie użyty w wątku, do którego przeniesiemy obiekt ApiWorker.
    // Tworzymy go BEZ RODZICA, bo jego życiem zarządza destruktor ApiWorker.
    networkManager = new QNetworkAccessManager();
}

ApiWorker::~ApiWorker()
{
    qDebug() << "ApiWorker destroyed in thread:" << QThread::currentThreadId();
    // Usuń networkManagera przy niszczeniu workera
    delete networkManager;
    networkManager = nullptr; // Dobra praktyka ustawić wskaźnik na null po usunięciu
}

// --- Sloty publiczne (uruchamiane przez sygnały z wątku głównego) ---

void ApiWorker::doFetchAllStations()
{
    qDebug() << "ApiWorker: doFetchAllStations in thread:" << QThread::currentThreadId();
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/station/findAll");
    QNetworkRequest request(url);
    qDebug() << "ApiWorker: Sending request for all stations to URL:" << url.toString();
    QNetworkReply *reply = networkManager->get(request);

    // Sprawdź, czy reply nie jest null (bardzo mało prawdopodobne, ale bezpieczniej)
    if (!reply) {
        qWarning() << "ApiWorker: FATAL - networkManager->get() returned nullptr for stations!";
        emit errorOccurred("Krytyczny błąd sieciowy (worker nie mógł utworzyć odpowiedzi).");
        return;
    }

    // Połącz finished z prywatnym slotem TEJ klasy
    connect(reply, &QNetworkReply::finished, this, [this, reply](){ this->onFetchStationsFinished(reply); });

    // Opcjonalnie: Połącz errorOccurred dla szybszego logowania błędów sieciowych
    connect(reply, &QNetworkReply::errorOccurred, this, [reply](QNetworkReply::NetworkError code){
        // Tylko loguj, główna obsługa błędu jest w on...Finished
        qWarning() << "ApiWorker: Network errorOccurred signal for stations:" << code << reply->errorString() << "URL:" << reply->url().toString();
    });
}

void ApiWorker::doFetchSensorsForStation(int stationId)
{
    qDebug() << "ApiWorker: doFetchSensorsForStation for ID" << stationId << "in thread:" << QThread::currentThreadId();
    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/station/sensors/%1").arg(stationId));
    QNetworkRequest request(url);
    qDebug() << "ApiWorker: Sending request for sensors to URL:" << url.toString();
    QNetworkReply *reply = networkManager->get(request);

    if (!reply) {
        qWarning() << "ApiWorker: FATAL - networkManager->get() returned nullptr for sensors! Station ID:" << stationId;
        emit errorOccurred("Krytyczny błąd sieciowy (worker nie mógł utworzyć odpowiedzi).");
        return;
    }

    connect(reply, &QNetworkReply::finished, this, [this, reply](){ this->onFetchSensorsFinished(reply); });
    connect(reply, &QNetworkReply::errorOccurred, this, [reply](QNetworkReply::NetworkError code){
        qWarning() << "ApiWorker: Network errorOccurred signal for sensors:" << code << reply->errorString() << "URL:" << reply->url().toString();
    });
}

void ApiWorker::doFetchMeasurementData(int sensorId)
{
    qDebug() << "ApiWorker: doFetchMeasurementData for ID" << sensorId << "in thread:" << QThread::currentThreadId();
    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/data/getData/%1").arg(sensorId));
    QNetworkRequest request(url);
    qDebug() << "ApiWorker: Sending request for measurements to URL:" << url.toString();
    QNetworkReply *reply = networkManager->get(request);

    if (!reply) {
        qWarning() << "ApiWorker: FATAL - networkManager->get() returned nullptr for measurements! Sensor ID:" << sensorId;
        emit errorOccurred("Krytyczny błąd sieciowy (worker nie mógł utworzyć odpowiedzi).");
        return;
    }

    connect(reply, &QNetworkReply::finished, this, [this, reply](){ this->onFetchMeasurementDataFinished(reply); });
    connect(reply, &QNetworkReply::errorOccurred, this, [reply](QNetworkReply::NetworkError code){
        qWarning() << "ApiWorker: Network errorOccurred signal for measurements:" << code << reply->errorString() << "URL:" << reply->url().toString();
    });
}


// --- Sloty prywatne (obsługa odpowiedzi sieciowej w wątku pracownika) ---

void ApiWorker::onFetchStationsFinished(QNetworkReply *reply)
{
    qDebug() << "ApiWorker: onFetchStationsFinished in thread:" << QThread::currentThreadId() << "for URL:" << reply->url().toString();
    // Przenieś wskaźnik reply do QScopedPointer, aby zapewnić deleteLater nawet przy błędach/return
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyGuard(reply);
    if (!reply) {
        qWarning() << "ApiWorker: onFetchStationsFinished called with nullptr reply (should not happen).";
        return; // Guard
    }


    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = "Błąd sieciowy (Worker): " + reply->errorString();
        qWarning() << errorMsg << "URL:" << reply->url().toString();
        emit errorOccurred(errorMsg); // Wyemituj błąd
        // replyGuard zajmie się deleteLater
        return;
    }

    QByteArray jsonData = reply->readAll();
    qDebug() << "ApiWorker: Received" << jsonData.size() << "bytes for stations.";
    // replyGuard zajmie się deleteLater
    parseStationsJson(jsonData); // Parsuj i emituj wynik/błąd
}

void ApiWorker::onFetchSensorsFinished(QNetworkReply *reply)
{
    qDebug() << "ApiWorker: onFetchSensorsFinished in thread:" << QThread::currentThreadId() << "for URL:" << reply->url().toString();
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyGuard(reply);
    if (!reply) return;

    int stationIdFromUrl = -1;
    QString path = reply->url().path();
    QStringList parts = path.split('/');
    if (parts.size() > 0) { bool ok; int id = parts.last().toInt(&ok); if (ok) stationIdFromUrl = id; }
    if(stationIdFromUrl == -1) {
        qWarning() << "ApiWorker: Could not extract stationId from URL:" << reply->url().toString();
    }


    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = QString("Błąd sieciowy sensorów (Worker, URL: %1): %2")
                               .arg(reply->url().toString()).arg(reply->errorString());
        qWarning() << errorMsg;
        emit errorOccurred(errorMsg);
        return; // replyGuard zajmie się deleteLater
    }
    QByteArray jsonData = reply->readAll();
    qDebug() << "ApiWorker: Received" << jsonData.size() << "bytes for sensors, Station ID:" << stationIdFromUrl;
    parseSensorsJson(jsonData, stationIdFromUrl);
}

void ApiWorker::onFetchMeasurementDataFinished(QNetworkReply *reply)
{
    qDebug() << "ApiWorker: onFetchMeasurementDataFinished in thread:" << QThread::currentThreadId() << "for URL:" << reply->url().toString();
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyGuard(reply);
    if (!reply) return;

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = QString("Błąd sieciowy danych (Worker, URL: %1): %2")
                               .arg(reply->url().toString()).arg(reply->errorString());
        qWarning() << errorMsg;
        emit errorOccurred(errorMsg);
        return; // replyGuard zajmie się deleteLater
    }
    QByteArray jsonData = reply->readAll();
    qDebug() << "ApiWorker: Received" << jsonData.size() << "bytes for measurements.";
    parseMeasurementDataJson(jsonData);
}

// --- Metody parsowania (kod przeniesiony z GiosApiClient) ---

void ApiWorker::parseStationsJson(const QByteArray& jsonData)
{
    qDebug() << "ApiWorker: parseStationsJson in thread:" << QThread::currentThreadId();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = "Błąd parsowania JSON stacji (Worker): " + parseError.errorString();
        qWarning() << errorMsg; emit errorOccurred(errorMsg); return;
    }
    if (!jsonDoc.isArray()) {
        QString errorMsg = "Błąd formatu JSON stacji (Worker): Oczekiwano tablicy.";
        qWarning() << errorMsg; emit errorOccurred(errorMsg); return;
    }

    QList<StationInfo> stationsList;
    QJsonArray stationsArray = jsonDoc.array();
    stationsList.reserve(stationsArray.count()); // Prealokacja

    for (const QJsonValue &value : stationsArray) {
        if (value.isObject()) {
            QJsonObject stationObj = value.toObject();
            StationInfo station;

            if (stationObj.contains("id") && stationObj["id"].isDouble()) {
                station.id = static_cast<int>(stationObj["id"].toDouble());
            } else {
                qWarning() << "ApiWorker: Skipping station due to missing/invalid 'id'"; continue;
            }

            if (stationObj.contains("stationName") && stationObj["stationName"].isString()) {
                station.stationName = stationObj["stationName"].toString();
            } else {
                qWarning() << "ApiWorker: Missing/invalid 'stationName' for station id:" << station.id;
                station.stationName = QString("Stacja bez nazwy (ID: %1)").arg(station.id);
            }

            // Odczytaj cityName (zakładając, że pole dodano do StationInfo)
            if (stationObj.contains("city") && stationObj["city"].isObject()) {
                QJsonObject cityObj = stationObj["city"].toObject();
                if (cityObj.contains("name") && cityObj["name"].isString()) {
                    station.cityName = cityObj["name"].toString();
                } else { station.cityName = "Nieznane"; }
            } else { station.cityName = "Nieznane"; }

            stationsList.append(station);
        } else {
            qWarning() << "ApiWorker: Skipping non-object item in stations array.";
        }
    }

    qDebug() << "ApiWorker: Emitting stationsReady signal with" << stationsList.count() << "stations.";
    emit stationsReady(stationsList); // Emituj wynik
}

void ApiWorker::parseSensorsJson(const QByteArray& jsonData, int stationId)
{
    qDebug() << "ApiWorker: parseSensorsJson for station" << stationId << "in thread:" << QThread::currentThreadId();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = "Błąd parsowania JSON sensorów (Worker): " + parseError.errorString();
        qWarning() << errorMsg; emit errorOccurred(errorMsg); return;
    }
    if (!jsonDoc.isArray()) {
        if (jsonDoc.isNull() || (jsonDoc.isArray() && jsonDoc.array().isEmpty())) {
            qDebug() << "ApiWorker: Emitting sensorsReady signal with 0 sensors (empty array) for station" << stationId;
            emit sensorsReady({}); return;
        } else {
            QString errorMsg = "Błąd formatu JSON sensorów (Worker): Oczekiwano tablicy.";
            qWarning() << errorMsg << "Received:" << jsonData.left(100); emit errorOccurred(errorMsg); return;
        }
    }

    QList<SensorInfo> sensorsList;
    QJsonArray sensorsArray = jsonDoc.array();
    sensorsList.reserve(sensorsArray.count());

    for (const QJsonValue &value : sensorsArray) {
        if (value.isObject()) {
            QJsonObject sensorObj = value.toObject();
            SensorInfo sensor;
            sensor.stationId = stationId;

            if (sensorObj.contains("id") && sensorObj["id"].isDouble()) {
                sensor.id = static_cast<int>(sensorObj["id"].toDouble());
            } else {
                qWarning() << "ApiWorker: Skipping sensor due to missing/invalid 'id' for station" << stationId; continue;
            }

            if (sensorObj.contains("param") && sensorObj["param"].isObject()) {
                QJsonObject paramObj = sensorObj["param"].toObject();
                sensor.paramName = paramObj.value("paramName").toString("Brak nazwy");
                sensor.paramFormula = paramObj.value("paramFormula").toString("?");
                sensor.paramCode = paramObj.value("paramCode").toString("Brak kodu");
                sensor.idParam = static_cast<int>(paramObj.value("idParam").toDouble(-1.0)); // Domyślnie -1
            } else {
                qWarning() << "ApiWorker: Missing/invalid 'param' object for sensor id:" << sensor.id;
                sensor.paramName = "Brak danych"; sensor.paramFormula = "?"; sensor.paramCode = "ERROR"; sensor.idParam = -1;
            }
            sensorsList.append(sensor);
        } else {
            qWarning() << "ApiWorker: Skipping non-object item in sensors array for station" << stationId;
        }
    }

    qDebug() << "ApiWorker: Emitting sensorsReady signal with" << sensorsList.count() << "sensors for station" << stationId;
    emit sensorsReady(sensorsList); // Emituj wynik
}

void ApiWorker::parseMeasurementDataJson(const QByteArray& jsonData)
{
    qDebug() << "ApiWorker: parseMeasurementDataJson in thread:" << QThread::currentThreadId();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = "Błąd parsowania JSON danych (Worker): " + parseError.errorString();
        qWarning() << errorMsg; emit errorOccurred(errorMsg); return;
    }
    if (!jsonDoc.isObject()) {
        QString errorMsg = "Błąd formatu JSON danych (Worker): Oczekiwano obiektu.";
        qWarning() << errorMsg; emit errorOccurred(errorMsg); return;
    }

    QJsonObject mainObj = jsonDoc.object();
    MeasurementData measurementData;

    measurementData.key = mainObj.value("key").toString("Nieznany");
    if (measurementData.key == "Nieznany") {
        qWarning() << "ApiWorker: Missing/invalid 'key' in measurement data.";
    }

    if (mainObj.contains("values") && mainObj["values"].isArray()) {
        QJsonArray valuesArray = mainObj["values"].toArray();
        measurementData.values.reserve(valuesArray.count());

        for (const QJsonValue &value : valuesArray) {
            if (value.isObject()) {
                QJsonObject mObj = value.toObject();
                Measurement m;

                if (mObj.contains("date") && mObj["date"].isString()) {
                    // Użyj formatu oczekiwanego przez API
                    m.date = QDateTime::fromString(mObj["date"].toString(), "yyyy-MM-dd HH:mm:ss");
                    if (!m.date.isValid()) {
                        qWarning() << "ApiWorker: Skipping measurement due to invalid date format:" << mObj["date"].toString(); continue;
                    }
                } else { continue; }

                if (mObj.contains("value")) {
                    if (mObj["value"].isNull()) { m.value = QVariant(); }
                    else if (mObj["value"].isDouble()) { m.value = mObj["value"].toDouble(); }
                    else { m.value = QVariant(); } // Traktuj inne typy jako null
                } else { continue; }

                measurementData.values.append(m);
            } else {
                qWarning() << "ApiWorker: Skipping non-object item in 'values' array for key" << measurementData.key;
            }
        }

        if (!measurementData.values.isEmpty()) {
            qDebug() << "ApiWorker: Sorting" << measurementData.values.count() << "measurements by date for key" << measurementData.key;
            std::sort(measurementData.values.begin(), measurementData.values.end(),
                      [](const Measurement& a, const Measurement& b) { return a.date < b.date; });
        }

    } else {
        qWarning() << "ApiWorker: Missing or invalid 'values' array in measurement data for key" << measurementData.key;
        // Emituj puste dane, jeśli nie ma tablicy 'values'
    }

    qDebug() << "ApiWorker: Emitting measurementDataReady signal for key" << measurementData.key << "with" << measurementData.values.count() << "values.";
    emit measurementDataReady(measurementData); // Emituj wynik (nawet jeśli pusty)
}
