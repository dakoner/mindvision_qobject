/****************************************************************************
** Meta object code from reading C++ file 'MindVisionCamera.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../MindVisionCamera.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MindVisionCamera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CameraWorker_t {
    QByteArrayData data[9];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraWorker_t qt_meta_stringdata_CameraWorker = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CameraWorker"
QT_MOC_LITERAL(1, 13, 10), // "frameReady"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 5), // "image"
QT_MOC_LITERAL(4, 31, 10), // "fpsChanged"
QT_MOC_LITERAL(5, 42, 3), // "fps"
QT_MOC_LITERAL(6, 46, 8), // "finished"
QT_MOC_LITERAL(7, 55, 7), // "process"
QT_MOC_LITERAL(8, 63, 4) // "stop"

    },
    "CameraWorker\0frameReady\0\0image\0"
    "fpsChanged\0fps\0finished\0process\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,
       6,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   46,    2, 0x0a /* Public */,
       8,    0,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CameraWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CameraWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->fpsChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->finished(); break;
        case 3: _t->process(); break;
        case 4: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CameraWorker::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraWorker::frameReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CameraWorker::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraWorker::fpsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CameraWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraWorker::finished)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CameraWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CameraWorker.data,
    qt_meta_data_CameraWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CameraWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CameraWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CameraWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CameraWorker::frameReady(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CameraWorker::fpsChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CameraWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_MindVisionCamera_t {
    QByteArrayData data[31];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MindVisionCamera_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MindVisionCamera_t qt_meta_stringdata_MindVisionCamera = {
    {
QT_MOC_LITERAL(0, 0, 16), // "MindVisionCamera"
QT_MOC_LITERAL(1, 17, 10), // "frameReady"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 5), // "image"
QT_MOC_LITERAL(4, 35, 10), // "fpsChanged"
QT_MOC_LITERAL(5, 46, 3), // "fps"
QT_MOC_LITERAL(6, 50, 13), // "errorOccurred"
QT_MOC_LITERAL(7, 64, 7), // "message"
QT_MOC_LITERAL(8, 72, 4), // "open"
QT_MOC_LITERAL(9, 77, 5), // "close"
QT_MOC_LITERAL(10, 83, 5), // "start"
QT_MOC_LITERAL(11, 89, 4), // "stop"
QT_MOC_LITERAL(12, 94, 15), // "setAutoExposure"
QT_MOC_LITERAL(13, 110, 7), // "enabled"
QT_MOC_LITERAL(14, 118, 15), // "setExposureTime"
QT_MOC_LITERAL(15, 134, 14), // "exposureTimeMs"
QT_MOC_LITERAL(16, 149, 13), // "setAnalogGain"
QT_MOC_LITERAL(17, 163, 4), // "gain"
QT_MOC_LITERAL(18, 168, 15), // "getAutoExposure"
QT_MOC_LITERAL(19, 184, 15), // "getExposureTime"
QT_MOC_LITERAL(20, 200, 13), // "getAnalogGain"
QT_MOC_LITERAL(21, 214, 20), // "getExposureTimeRange"
QT_MOC_LITERAL(22, 235, 7), // "double&"
QT_MOC_LITERAL(23, 243, 5), // "minMs"
QT_MOC_LITERAL(24, 249, 5), // "maxMs"
QT_MOC_LITERAL(25, 255, 18), // "getAnalogGainRange"
QT_MOC_LITERAL(26, 274, 4), // "int&"
QT_MOC_LITERAL(27, 279, 3), // "min"
QT_MOC_LITERAL(28, 283, 3), // "max"
QT_MOC_LITERAL(29, 287, 6), // "setRoi"
QT_MOC_LITERAL(30, 294, 6) // "enable"

    },
    "MindVisionCamera\0frameReady\0\0image\0"
    "fpsChanged\0fps\0errorOccurred\0message\0"
    "open\0close\0start\0stop\0setAutoExposure\0"
    "enabled\0setExposureTime\0exposureTimeMs\0"
    "setAnalogGain\0gain\0getAutoExposure\0"
    "getExposureTime\0getAnalogGain\0"
    "getExposureTimeRange\0double&\0minMs\0"
    "maxMs\0getAnalogGainRange\0int&\0min\0max\0"
    "setRoi\0enable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MindVisionCamera[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       4,    1,   97,    2, 0x06 /* Public */,
       6,    1,  100,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       8,    0,  103,    2, 0x02 /* Public */,
       9,    0,  104,    2, 0x02 /* Public */,
      10,    0,  105,    2, 0x02 /* Public */,
      11,    0,  106,    2, 0x02 /* Public */,
      12,    1,  107,    2, 0x02 /* Public */,
      14,    1,  110,    2, 0x02 /* Public */,
      16,    1,  113,    2, 0x02 /* Public */,
      18,    0,  116,    2, 0x02 /* Public */,
      19,    0,  117,    2, 0x02 /* Public */,
      20,    0,  118,    2, 0x02 /* Public */,
      21,    2,  119,    2, 0x02 /* Public */,
      25,    2,  124,    2, 0x02 /* Public */,
      29,    1,  129,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::QString,    7,

 // methods: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::Bool,   13,
    QMetaType::Bool, QMetaType::Double,   15,
    QMetaType::Bool, QMetaType::Int,   17,
    QMetaType::Bool,
    QMetaType::Double,
    QMetaType::Int,
    QMetaType::Void, 0x80000000 | 22, 0x80000000 | 22,   23,   24,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 26,   27,   28,
    QMetaType::Bool, QMetaType::Bool,   30,

       0        // eod
};

void MindVisionCamera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MindVisionCamera *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->fpsChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: { bool _r = _t->open();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->close(); break;
        case 5: { bool _r = _t->start();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->stop(); break;
        case 7: { bool _r = _t->setAutoExposure((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->setExposureTime((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->setAnalogGain((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->getAutoExposure();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { double _r = _t->getExposureTime();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->getAnalogGain();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->getExposureTimeRange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 14: _t->getAnalogGainRange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: { bool _r = _t->setRoi((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MindVisionCamera::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MindVisionCamera::frameReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MindVisionCamera::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MindVisionCamera::fpsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MindVisionCamera::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MindVisionCamera::errorOccurred)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MindVisionCamera::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MindVisionCamera.data,
    qt_meta_data_MindVisionCamera,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MindVisionCamera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MindVisionCamera::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MindVisionCamera.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MindVisionCamera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void MindVisionCamera::frameReady(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MindVisionCamera::fpsChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MindVisionCamera::errorOccurred(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
