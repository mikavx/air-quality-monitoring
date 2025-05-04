#include "giosapiclient.h"

// Includy Qt
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QUrl>
#include <QDebug>        // Dla qWarning
#include <algorithm>     // Dla std::sort
#include <QScopedPointer> // Dla bezpiecznego zarządzania QNetworkReply

// Konstruktor
GiosApiClient::GiosApiClient(QObject *parent)
    : QObject(parent)
{
    // Inicjalizujemy managera sieciowego, ustawiając rodzica,
    // aby Qt zarządzało jego pamięcią.
    networkManager = new QNetworkAccessManager(this);
}

// === Metody publiczne inicjujące żądania ===

void GiosApiClient::fetchAllStations()
{
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/station/findAll");
    QNetworkRequest request(url);
    qDebug() << "GiosApiClient: Wysyłanie żądania stacji...";
    QNetworkReply *reply = networkManager->get(request);
    // Łączymy sygnał finished z odpowiednim slotem obsługującym
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { this->onFetchStationsFinished(reply); });
}

void GiosApiClient::fetchSensorsForStation(int stationId)
{
    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/station/sensors/%1").arg(stationId));
    QNetworkRequest request(url);
    qDebug() << "GiosApiClient: Wysyłanie żądania sensorów dla stacji ID:" << stationId;
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { this->onFetchSensorsFinished(reply); });
}

void GiosApiClient::fetchMeasurementData(int sensorId)
{
    QUrl url(QString("https://api.gios.gov.pl/pjp-api/rest/data/getData/%1").arg(sensorId));
    QNetworkRequest request(url);
    qDebug() << "GiosApiClient: Wysyłanie żądania danych dla sensora ID:" << sensorId;
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { this->onFetchMeasurementDataFinished(reply); });
}

// === Sloty prywatne obsługujące odpowiedzi sieciowe ===

void GiosApiClient::onFetchStationsFinished(QNetworkReply *reply)
{
    // Używamy QScopedPointer, aby zapewnić automatyczne wywołanie deleteLater()
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyGuard(reply);
    if (!reply) {
        qWarning() << "GiosApiClient: onFetchStationsFinished - pusty reply!";
        return;
    }

    // Sprawdzamy błąd sieciowy
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = "Błąd sieciowy (stacje): " + reply->errorString();
        qWarning() << errorMsg << "URL:" << reply->url().toString();
        emit networkError(errorMsg);
        return;
    }

    // Odczyt i parsowanie danych
    QByteArray jsonData = reply->readAll();
    parseStationsJson(jsonData);
}

void GiosApiClient::onFetchSensorsFinished(QNetworkReply *reply)
{
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyGuard(reply);
    if (!reply) return;

    // Wyodrębnij ID stacji z URL
    int stationIdFromUrl = -1;
    QStringList parts = reply->url().path().split('/');
    if (!parts.isEmpty()) { bool ok; int id = parts.last().toInt(&ok); if (ok) stationIdFromUrl = id; }
    if(stationIdFromUrl == -1) qWarning() << "GiosApiClient: Nie można wyodrębnić ID stacji z URL:" << reply->url();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = QString("Błąd sieciowy (sensory, URL: %1): %2")
                               .arg(reply->url().toString()).arg(reply->errorString());
        qWarning() << errorMsg;
        emit networkError(errorMsg);
        return;
    }

    QByteArray jsonData = reply->readAll();
    parseSensorsJson(jsonData, stationIdFromUrl);
}

void GiosApiClient::onFetchMeasurementDataFinished(QNetworkReply *reply)
{
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyGuard(reply);
    if (!reply) return;

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = QString("Błąd sieciowy (dane pomiarowe, URL: %1): %2")
                               .arg(reply->url().toString()).arg(reply->errorString());
        qWarning() << errorMsg;
        emit networkError(errorMsg);
        return;
    }

    QByteArray jsonData = reply->readAll();
    parseMeasurementDataJson(jsonData);
}


// === Prywatne metody parsowania JSON ===

void GiosApiClient::parseStationsJson(const QByteArray& jsonData)
{
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = "Błąd parsowania JSON (stacje): " + parseError.errorString();
        qWarning() << errorMsg; emit networkError(errorMsg); return;
    }
    if (!jsonDoc.isArray()) {
        QString errorMsg = "Błąd formatu JSON (stacje): Oczekiwano tablicy.";
        qWarning() << errorMsg; emit networkError(errorMsg); return;
    }

    QList<StationInfo> stationsList;
    QJsonArray stationsArray = jsonDoc.array();
    stationsList.reserve(stationsArray.count());

    for (const QJsonValue &value : stationsArray) {
        if (value.isObject()) {
            QJsonObject stationObj = value.toObject();
            StationInfo station;

            if (stationObj.contains("id") && stationObj["id"].isDouble()) {
                station.id = static_cast<int>(stationObj["id"].toDouble());
            } else {
                qWarning() << "GiosApiClient: Pomijam stację - brak/niepoprawne 'id'"; continue;
            }

            station.stationName = stationObj.value("stationName").toString(QString("Stacja bez nazwy (ID: %1)").arg(station.id));

            if (stationObj.contains("city") && stationObj["city"].isObject()) {
                station.cityName = stationObj["city"].toObject().value("name").toString("Nieznane");
            } else {
                station.cityName = "Nieznane";
            }
            stationsList.append(station);
        }
    }
    emit stationsFetched(stationsList);
}

void GiosApiClient::parseSensorsJson(const QByteArray& jsonData, int stationId)
{
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = "Błąd parsowania JSON (sensory): " + parseError.errorString();
        qWarning() << errorMsg; emit networkError(errorMsg); return;
    }
    if (!jsonDoc.isArray()) {
        if (jsonDoc.isNull() || (jsonDoc.isArray() && jsonDoc.array().isEmpty())) {
            emit sensorsFetched({}); return;
        } else {
            QString errorMsg = "Błąd formatu JSON (sensory): Oczekiwano tablicy.";
            qWarning() << errorMsg; emit networkError(errorMsg); return;
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
                qWarning() << "GiosApiClient: Pomijam sensor - brak/niepoprawne 'id' dla stacji" << stationId; continue;
            }

            if (sensorObj.contains("param") && sensorObj["param"].isObject()) {
                QJsonObject paramObj = sensorObj["param"].toObject();
                sensor.paramName = paramObj.value("paramName").toString("Brak nazwy");
                sensor.paramFormula = paramObj.value("paramFormula").toString("?");
                sensor.paramCode = paramObj.value("paramCode").toString("Brak kodu");
                sensor.idParam = static_cast<int>(paramObj.value("idParam").toDouble(-1.0));
            } else {
                qWarning() << "GiosApiClient: Brak obiektu 'param' dla sensora id:" << sensor.id;
                sensor.paramName = "Brak danych"; sensor.paramFormula = "?"; sensor.paramCode = "ERROR"; sensor.idParam = -1;
            }
            sensorsList.append(sensor);
        }
    }
    emit sensorsFetched(sensorsList);
}

void GiosApiClient::parseMeasurementDataJson(const QByteArray& jsonData)
{
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = "Błąd parsowania JSON (dane): " + parseError.errorString();
        qWarning() << errorMsg; emit networkError(errorMsg); return;
    }
    if (!jsonDoc.isObject()) {
        QString errorMsg = "Błąd formatu JSON (dane): Oczekiwano obiektu.";
        qWarning() << errorMsg; emit networkError(errorMsg); return;
    }

    QJsonObject mainObj = jsonDoc.object();
    MeasurementData measurementData;
    measurementData.key = mainObj.value("key").toString("Nieznany");

    if (mainObj.contains("values") && mainObj["values"].isArray()) {
        QJsonArray valuesArray = mainObj["values"].toArray();
        measurementData.values.reserve(valuesArray.count());

        for (const QJsonValue &value : valuesArray) {
            if (value.isObject()) {
                QJsonObject mObj = value.toObject();
                Measurement m;

                if (mObj.contains("date") && mObj["date"].isString()) {
                    m.date = QDateTime::fromString(mObj["date"].toString(), "yyyy-MM-dd HH:mm:ss");
                    if (!m.date.isValid()) {
                        qWarning() << "GiosApiClient: Pomijam pomiar - niepoprawny format daty:" << mObj["date"].toString(); continue;
                    }
                } else continue; // Pomiń bez daty

                if (mObj.contains("value")) {
                    if (mObj["value"].isNull()) { m.value = QVariant(); }
                    else if (mObj["value"].isDouble()) { m.value = mObj["value"].toDouble(); }
                    else {
                        qWarning() << "GiosApiClient: Pomiar - wartość nie jest double ani null, traktuję jako null. Typ:" << mObj["value"].type();
                        m.value = QVariant();
                    }
                } else continue; // Pomiń bez wartości

                measurementData.values.append(m);
            }
        }
        // Sortowanie
        if (!measurementData.values.isEmpty()) {
            std::sort(measurementData.values.begin(), measurementData.values.end(),
                      [](const Measurement& a, const Measurement& b) { return a.date < b.date; });
        }
    } else {
        qWarning() << "GiosApiClient: Brak tablicy 'values' w danych pomiarowych dla klucza" << measurementData.key;
    }
    emit measurementDataFetched(measurementData);
}
