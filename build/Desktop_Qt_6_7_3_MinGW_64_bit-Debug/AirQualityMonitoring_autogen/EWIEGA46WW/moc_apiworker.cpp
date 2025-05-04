/****************************************************************************
** Meta object code from reading C++ file 'apiworker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../apiworker.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'apiworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSApiWorkerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSApiWorkerENDCLASS = QtMocHelpers::stringData(
    "ApiWorker",
    "stationsReady",
    "",
    "QList<StationInfo>",
    "stations",
    "sensorsReady",
    "QList<SensorInfo>",
    "sensors",
    "measurementDataReady",
    "MeasurementData",
    "data",
    "errorOccurred",
    "errorString",
    "doFetchAllStations",
    "doFetchSensorsForStation",
    "stationId",
    "doFetchMeasurementData",
    "sensorId",
    "onFetchStationsFinished",
    "QNetworkReply*",
    "reply",
    "onFetchSensorsFinished",
    "onFetchMeasurementDataFinished"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSApiWorkerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,
       5,    1,   77,    2, 0x06,    3 /* Public */,
       8,    1,   80,    2, 0x06,    5 /* Public */,
      11,    1,   83,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    0,   86,    2, 0x0a,    9 /* Public */,
      14,    1,   87,    2, 0x0a,   10 /* Public */,
      16,    1,   90,    2, 0x0a,   12 /* Public */,
      18,    1,   93,    2, 0x08,   14 /* Private */,
      21,    1,   96,    2, 0x08,   16 /* Private */,
      22,    1,   99,    2, 0x08,   18 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::QString,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 19,   20,

       0        // eod
};

Q_CONSTINIT const QMetaObject ApiWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSApiWorkerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSApiWorkerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSApiWorkerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ApiWorker, std::true_type>,
        // method 'stationsReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<StationInfo> &, std::false_type>,
        // method 'sensorsReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<SensorInfo> &, std::false_type>,
        // method 'measurementDataReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeasurementData &, std::false_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'doFetchAllStations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'doFetchSensorsForStation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'doFetchMeasurementData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onFetchStationsFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'onFetchSensorsFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'onFetchMeasurementDataFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>
    >,
    nullptr
} };

void ApiWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ApiWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stationsReady((*reinterpret_cast< std::add_pointer_t<QList<StationInfo>>>(_a[1]))); break;
        case 1: _t->sensorsReady((*reinterpret_cast< std::add_pointer_t<QList<SensorInfo>>>(_a[1]))); break;
        case 2: _t->measurementDataReady((*reinterpret_cast< std::add_pointer_t<MeasurementData>>(_a[1]))); break;
        case 3: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->doFetchAllStations(); break;
        case 5: _t->doFetchSensorsForStation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->doFetchMeasurementData((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onFetchStationsFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 8: _t->onFetchSensorsFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 9: _t->onFetchMeasurementDataFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ApiWorker::*)(const QList<StationInfo> & );
            if (_t _q_method = &ApiWorker::stationsReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ApiWorker::*)(const QList<SensorInfo> & );
            if (_t _q_method = &ApiWorker::sensorsReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ApiWorker::*)(const MeasurementData & );
            if (_t _q_method = &ApiWorker::measurementDataReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ApiWorker::*)(const QString & );
            if (_t _q_method = &ApiWorker::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *ApiWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApiWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSApiWorkerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ApiWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ApiWorker::stationsReady(const QList<StationInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ApiWorker::sensorsReady(const QList<SensorInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ApiWorker::measurementDataReady(const MeasurementData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ApiWorker::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
