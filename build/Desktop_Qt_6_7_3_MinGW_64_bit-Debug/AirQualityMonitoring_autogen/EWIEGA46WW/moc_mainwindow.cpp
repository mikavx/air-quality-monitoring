/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSmainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSmainWindowENDCLASS = QtMocHelpers::stringData(
    "mainWindow",
    "on_fetchStationsButton_clicked",
    "",
    "on_listWidget_itemClicked",
    "QListWidgetItem*",
    "item",
    "on_sensorsListWidget_itemClicked",
    "on_saveDataButton_clicked",
    "on_loadDataButton_clicked",
    "on_analyzeButton_clicked",
    "on_cityFilterLineEdit_textChanged",
    "text",
    "handleStationsFetched",
    "QList<StationInfo>",
    "stations",
    "handleSensorsFetched",
    "QList<SensorInfo>",
    "sensors",
    "handleMeasurementDataFetched",
    "MeasurementData",
    "measurementResult",
    "handleNetworkError",
    "errorString"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSmainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    1,   81,    2, 0x08,    2 /* Private */,
       6,    1,   84,    2, 0x08,    4 /* Private */,
       7,    0,   87,    2, 0x08,    6 /* Private */,
       8,    0,   88,    2, 0x08,    7 /* Private */,
       9,    0,   89,    2, 0x08,    8 /* Private */,
      10,    1,   90,    2, 0x08,    9 /* Private */,
      12,    1,   93,    2, 0x08,   11 /* Private */,
      15,    1,   96,    2, 0x08,   13 /* Private */,
      18,    1,   99,    2, 0x08,   15 /* Private */,
      21,    1,  102,    2, 0x08,   17 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString,   22,

       0        // eod
};

Q_CONSTINIT const QMetaObject mainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSmainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSmainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSmainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<mainWindow, std::true_type>,
        // method 'on_fetchStationsButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_listWidget_itemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_sensorsListWidget_itemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_saveDataButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_loadDataButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_analyzeButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cityFilterLineEdit_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleStationsFetched'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<StationInfo> &, std::false_type>,
        // method 'handleSensorsFetched'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<SensorInfo> &, std::false_type>,
        // method 'handleMeasurementDataFetched'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeasurementData &, std::false_type>,
        // method 'handleNetworkError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void mainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_fetchStationsButton_clicked(); break;
        case 1: _t->on_listWidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 2: _t->on_sensorsListWidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 3: _t->on_saveDataButton_clicked(); break;
        case 4: _t->on_loadDataButton_clicked(); break;
        case 5: _t->on_analyzeButton_clicked(); break;
        case 6: _t->on_cityFilterLineEdit_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->handleStationsFetched((*reinterpret_cast< std::add_pointer_t<QList<StationInfo>>>(_a[1]))); break;
        case 8: _t->handleSensorsFetched((*reinterpret_cast< std::add_pointer_t<QList<SensorInfo>>>(_a[1]))); break;
        case 9: _t->handleMeasurementDataFetched((*reinterpret_cast< std::add_pointer_t<MeasurementData>>(_a[1]))); break;
        case 10: _t->handleNetworkError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *mainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSmainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int mainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
