#ifndef GIOSAPICLIENT_H
#define GIOSAPICLIENT_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QVariant>

// === POTRZEBNE FORWARD DECLARATIONS ===
class QNetworkAccessManager;
class QNetworkReply;
class QJsonObject;
class QJsonArray;
class QByteArray;
// =====================================

/**
 * @file giosapiclient.h
 * @brief Definicja klasy GiosApiClient oraz powiązanych struktur danych.
 * @author Olga Baran
 */

/**
 * @struct StationInfo
 * @brief Przechowuje podstawowe informacje o stacji pomiarowej GIOŚ.
 */
struct StationInfo {
    int id = -1;                ///< Unikalne ID stacji w systemie GIOŚ.
    QString stationName;        ///< Oficjalna nazwa stacji pomiarowej.
    QString cityName;           ///< Nazwa miejscowości, w której znajduje się stacja.
};

/**
 * @struct SensorInfo
 * @brief Przechowuje informacje o pojedynczym sensorze (stanowisku pomiarowym) na stacji.
 */
struct SensorInfo {
    int id = -1;                ///< Unikalne ID sensora (stanowiska pomiarowego).
    int stationId = -1;         ///< ID stacji GIOŚ, do której przypisany jest sensor.
    QString paramName;          ///< Pełna nazwa mierzonego parametru (np. "Dwutlenek azotu").
    QString paramFormula;       ///< Wzór chemiczny lub symbol parametru (np. "NO2").
    QString paramCode;          ///< Krótki kod identyfikujący parametr (np. "NO2").
    int idParam = -1;           ///< Wewnętrzne ID parametru w systemie GIOŚ.
};

/**
 * @struct Measurement
 * @brief Reprezentuje pojedynczy odczyt pomiaru z sensora.
 */
struct Measurement {
    QDateTime date;             ///< Data i czas dokonania pomiaru.
    QVariant value;             ///< Zmierzona wartość. Może być typu double lub pusta (QVariant()), jeśli wartość była null.
};

/**
 * @struct MeasurementData
 * @brief Kontener na serię danych pomiarowych dla konkretnego parametru.
 */
struct MeasurementData {
    QString key;                ///< Klucz (kod) identyfikujący mierzony parametr (np. "PM10", "SO2").
    QList<Measurement> values;  ///< Lista odczytów (obiektów Measurement) dla tego parametru.
};

/**
 * @class GiosApiClient
 * @brief Odpowiada za komunikację z publicznym API GIOŚ PJP.
 *
 * Pobiera dane o stacjach pomiarowych, sensorach na tych stacjach oraz
 * historyczne dane pomiarowe z wybranych sensorów. Komunikacja odbywa się
 * asynchronicznie za pomocą QNetworkAccessManager. Wyniki zwracane są
 * poprzez sygnały. Klasa obsługuje również podstawowe błędy sieciowe
 * oraz błędy parsowania odpowiedzi JSON.
 */
class GiosApiClient : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor. Tworzy instancję QNetworkAccessManager.
     * @param parent Wskaźnik na obiekt rodzica.
     */
    explicit GiosApiClient(QObject *parent = nullptr);

    // === Metody publiczne inicjujące żądania ===

    /**
     * @brief Inicjuje pobranie listy wszystkich dostępnych stacji pomiarowych.
     * Po zakończeniu operacji emitowany jest sygnał stationsFetched() lub networkError().
     */
    void fetchAllStations();

    /**
     * @brief Inicjuje pobranie listy sensorów dla określonej stacji pomiarowej.
     * @param stationId ID stacji, której sensory mają zostać pobrane.
     * Po zakończeniu operacji emitowany jest sygnał sensorsFetched() lub networkError().
     */
    void fetchSensorsForStation(int stationId);

    /**
     * @brief Inicjuje pobranie danych pomiarowych z określonego sensora.
     * @param sensorId ID sensora, z którego dane mają zostać pobrane.
     * Po zakończeniu operacji emitowany jest sygnał measurementDataFetched() lub networkError().
     */
    void fetchMeasurementData(int sensorId);

signals:
    // === Sygnały informujące o wynikach ===

    /**
     * @brief Emitowany po pomyślnym pobraniu i przetworzeniu listy stacji.
     * @param stations Lista obiektów StationInfo zawierająca dane stacji.
     */
    void stationsFetched(const QList<StationInfo>& stations);

    /**
     * @brief Emitowany po pomyślnym pobraniu i przetworzeniu listy sensorów dla stacji.
     * @param sensors Lista obiektów SensorInfo zawierająca dane sensorów.
     */
    void sensorsFetched(const QList<SensorInfo>& sensors);

    /**
     * @brief Emitowany po pomyślnym pobraniu i przetworzeniu danych pomiarowych.
     * @param data Obiekt MeasurementData z kluczem parametru i listą odczytów.
     */
    void measurementDataFetched(const MeasurementData& data);

    /**
     * @brief Emitowany, gdy wystąpi błąd podczas komunikacji sieciowej lub parsowania odpowiedzi.
     * @param errorString Komunikat opisujący błąd.
     */
    void networkError(const QString& errorString);

private slots:
    /** @brief Slot wewnętrzny, odbiera sygnał finished() dla odpowiedzi na żądanie stacji. */
    void onFetchStationsFinished(QNetworkReply *reply);
    /** @brief Slot wewnętrzny, odbiera sygnał finished() dla odpowiedzi na żądanie sensorów. */
    void onFetchSensorsFinished(QNetworkReply *reply);
    /** @brief Slot wewnętrzny, odbiera sygnał finished() dla odpowiedzi na żądanie danych pomiarowych. */
    void onFetchMeasurementDataFinished(QNetworkReply *reply);

private:
    // === Metody pomocnicze (parsowanie) ===
    /** @brief Parsuje odpowiedź JSON zawierającą listę stacji. */
    void parseStationsJson(const QByteArray& jsonData);
    /** @brief Parsuje odpowiedź JSON zawierającą listę sensorów. */
    void parseSensorsJson(const QByteArray& jsonData, int stationId);
    /** @brief Parsuje odpowiedź JSON zawierającą dane pomiarowe. */
    void parseMeasurementDataJson(const QByteArray& jsonData);

    // === Pola klasy ===
    QNetworkAccessManager *networkManager; ///< Manager Qt do obsługi operacji sieciowych.
};

#endif // GIOSAPICLIENT_H
