/****************************************************************************
** Meta object code from reading C++ file 'view.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_eMU__analyzer__View_t {
    QByteArrayData data[26];
    char stringdata[451];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_eMU__analyzer__View_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_eMU__analyzer__View_t qt_meta_stringdata_eMU__analyzer__View = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 18),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 11),
QT_MOC_LITERAL(4, 52, 12),
QT_MOC_LITERAL(5, 65, 15),
QT_MOC_LITERAL(6, 81, 24),
QT_MOC_LITERAL(7, 106, 10),
QT_MOC_LITERAL(8, 117, 16),
QT_MOC_LITERAL(9, 134, 18),
QT_MOC_LITERAL(10, 153, 5),
QT_MOC_LITERAL(11, 159, 10),
QT_MOC_LITERAL(12, 170, 29),
QT_MOC_LITERAL(13, 200, 14),
QT_MOC_LITERAL(14, 215, 17),
QT_MOC_LITERAL(15, 233, 16),
QT_MOC_LITERAL(16, 250, 20),
QT_MOC_LITERAL(17, 271, 8),
QT_MOC_LITERAL(18, 280, 10),
QT_MOC_LITERAL(19, 291, 12),
QT_MOC_LITERAL(20, 304, 20),
QT_MOC_LITERAL(21, 325, 21),
QT_MOC_LITERAL(22, 347, 26),
QT_MOC_LITERAL(23, 374, 17),
QT_MOC_LITERAL(24, 392, 27),
QT_MOC_LITERAL(25, 420, 29)
    },
    "eMU::analyzer::View\0connectionAccepted\0"
    "\0std::string\0connectionId\0payloadReceived\0"
    "std::vector<std::string>\0payloadIds\0"
    "connectionClosed\0displayReadPayload\0"
    "index\0disconnect\0resizeWritePayloadFieldsCount\0"
    "numberOfFields\0parseNumericField\0"
    "parseStringField\0parseWideStringField\0"
    "sendDump\0sendFields\0generateDump\0"
    "clearReadPayloadView\0clearWritePayloadView\0"
    "calculatehWritePayloadSize\0displayConnection\0"
    "removeConnectionFromDisplay\0"
    "displayConnectionReadPayloads\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_eMU__analyzer__View[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06,
       5,    2,  107,    2, 0x06,
       8,    1,  112,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       9,    1,  115,    2, 0x08,
      11,    0,  118,    2, 0x08,
      12,    1,  119,    2, 0x08,
      14,    0,  122,    2, 0x08,
      15,    0,  123,    2, 0x08,
      16,    0,  124,    2, 0x08,
      17,    0,  125,    2, 0x08,
      18,    0,  126,    2, 0x08,
      19,    0,  127,    2, 0x08,
      20,    0,  128,    2, 0x08,
      21,    0,  129,    2, 0x08,
      22,    0,  130,    2, 0x08,
      23,    1,  131,    2, 0x08,
      24,    1,  134,    2, 0x08,
      25,    2,  137,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6,    4,    7,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6,    4,    7,

       0        // eod
};

void eMU::analyzer::View::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        View *_t = static_cast<View *>(_o);
        switch (_id) {
        case 0: _t->connectionAccepted((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 1: _t->payloadReceived((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::vector<std::string>(*)>(_a[2]))); break;
        case 2: _t->connectionClosed((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 3: _t->displayReadPayload((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->disconnect(); break;
        case 5: _t->resizeWritePayloadFieldsCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->parseNumericField(); break;
        case 7: _t->parseStringField(); break;
        case 8: _t->parseWideStringField(); break;
        case 9: _t->sendDump(); break;
        case 10: _t->sendFields(); break;
        case 11: _t->generateDump(); break;
        case 12: _t->clearReadPayloadView(); break;
        case 13: _t->clearWritePayloadView(); break;
        case 14: _t->calculatehWritePayloadSize(); break;
        case 15: _t->displayConnection((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 16: _t->removeConnectionFromDisplay((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 17: _t->displayConnectionReadPayloads((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::vector<std::string>(*)>(_a[2]))); break;
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
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::vector<std::string> >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::string >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< std::vector<std::string> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (View::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&View::connectionAccepted)) {
                *result = 0;
            }
        }
        {
            typedef void (View::*_t)(const std::string & , const std::vector<std::string> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&View::payloadReceived)) {
                *result = 1;
            }
        }
        {
            typedef void (View::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&View::connectionClosed)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject eMU::analyzer::View::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_eMU__analyzer__View.data,
      qt_meta_data_eMU__analyzer__View,  qt_static_metacall, 0, 0}
};


const QMetaObject *eMU::analyzer::View::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *eMU::analyzer::View::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_eMU__analyzer__View.stringdata))
        return static_cast<void*>(const_cast< View*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int eMU::analyzer::View::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void eMU::analyzer::View::connectionAccepted(const std::string & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void eMU::analyzer::View::payloadReceived(const std::string & _t1, const std::vector<std::string> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void eMU::analyzer::View::connectionClosed(const std::string & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
