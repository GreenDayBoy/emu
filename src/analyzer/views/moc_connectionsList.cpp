/****************************************************************************
** Meta object code from reading C++ file 'connectionsList.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "connectionsList.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connectionsList.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_eMU__analyzer__views__ConnectionsList_t {
    QByteArrayData data[13];
    char stringdata[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_eMU__analyzer__views__ConnectionsList_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_eMU__analyzer__views__ConnectionsList_t qt_meta_stringdata_eMU__analyzer__views__ConnectionsList = {
    {
QT_MOC_LITERAL(0, 0, 37),
QT_MOC_LITERAL(1, 38, 18),
QT_MOC_LITERAL(2, 57, 0),
QT_MOC_LITERAL(3, 58, 11),
QT_MOC_LITERAL(4, 70, 12),
QT_MOC_LITERAL(5, 83, 16),
QT_MOC_LITERAL(6, 100, 10),
QT_MOC_LITERAL(7, 111, 17),
QT_MOC_LITERAL(8, 129, 18),
QT_MOC_LITERAL(9, 148, 19),
QT_MOC_LITERAL(10, 168, 9),
QT_MOC_LITERAL(11, 178, 18),
QT_MOC_LITERAL(12, 197, 10)
    },
    "eMU::analyzer::views::ConnectionsList\0"
    "onConnectionAccept\0\0std::string\0"
    "connectionId\0onPayloadReceive\0payloadIds\0"
    "onConnectionClose\0insertConnectionId\0"
    "insertReadPayloadId\0payloadId\0"
    "removeConnectionId\0disconnect\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_eMU__analyzer__views__ConnectionsList[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06,
       5,    2,   52,    2, 0x06,
       7,    1,   57,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       8,    1,   60,    2, 0x08,
       9,    2,   63,    2, 0x08,
      11,    1,   68,    2, 0x08,
      12,    0,   71,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    6,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,   10,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void eMU::analyzer::views::ConnectionsList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConnectionsList *_t = static_cast<ConnectionsList *>(_o);
        switch (_id) {
        case 0: _t->onConnectionAccept((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 1: _t->onPayloadReceive((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 2: _t->onConnectionClose((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 3: _t->insertConnectionId((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 4: _t->insertReadPayloadId((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 5: _t->removeConnectionId((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 6: _t->disconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConnectionsList::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectionsList::onConnectionAccept)) {
                *result = 0;
            }
        }
        {
            typedef void (ConnectionsList::*_t)(const std::string & , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectionsList::onPayloadReceive)) {
                *result = 1;
            }
        }
        {
            typedef void (ConnectionsList::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectionsList::onConnectionClose)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject eMU::analyzer::views::ConnectionsList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_eMU__analyzer__views__ConnectionsList.data,
      qt_meta_data_eMU__analyzer__views__ConnectionsList,  qt_static_metacall, 0, 0}
};


const QMetaObject *eMU::analyzer::views::ConnectionsList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *eMU::analyzer::views::ConnectionsList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_eMU__analyzer__views__ConnectionsList.stringdata))
        return static_cast<void*>(const_cast< ConnectionsList*>(this));
    return QObject::qt_metacast(_clname);
}

int eMU::analyzer::views::ConnectionsList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void eMU::analyzer::views::ConnectionsList::onConnectionAccept(const std::string & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void eMU::analyzer::views::ConnectionsList::onPayloadReceive(const std::string & _t1, const std::string & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void eMU::analyzer::views::ConnectionsList::onConnectionClose(const std::string & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
