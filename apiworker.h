#ifndef APIWORKER_H
#define APIWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrl>
#include <QJsonParseError>
#include <QVariant>
#include <QDateTime>

struct StationInfo {
    int id;
    QString stationName;
    QString cityName;
};

struct SensorInfo {
    int id;
    int stationId;
    QString paramName;
    QString paramFormula;
    QString paramCode;
    int idParam;
};

struct Measurement {
    QDateTime date;
    QVariant value;
};

struct MeasurementData {
    QString key;
    QList<Measurement> values;
};

class ApiWorker : public QObject
{
    Q_OBJECT

public:
    explicit ApiWorker(QObject *parent = nullptr);
    ~ApiWorker();

public slots:
    // Sloty wywoływane z wątku głównego do rozpoczęcia pracy
    void doFetchAllStations();
    void doFetchSensorsForStation(int stationId);
    void doFetchMeasurementData(int sensorId);

signals:
    // Sygnały emitowane z wątku pracownika do wątku głównego z wynikami
    void stationsReady(const QList<StationInfo>& stations);
    void sensorsReady(const QList<SensorInfo>& sensors);
    void measurementDataReady(const MeasurementData& data);
    void errorOccurred(const QString& errorString); // Sygnał błędu

private slots:
    // Prywatne sloty do obsługi odpowiedzi sieciowych W WĄTKU PRACOWNIKA
    void onFetchStationsFinished(QNetworkReply *reply);
    void onFetchSensorsFinished(QNetworkReply *reply);
    void onFetchMeasurementDataFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager; // Każdy wątek powinien mieć swój NAM

    // Metody parsowania (przeniesione z GiosApiClient)
    void parseStationsJson(const QByteArray& jsonData);
    void parseSensorsJson(const QByteArray& jsonData, int stationId);
    void parseMeasurementDataJson(const QByteArray& jsonData);
};

#endif // APIWORKER_H
