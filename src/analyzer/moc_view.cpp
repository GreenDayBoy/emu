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
    QByteArrayData data[15];
    char stringdata[241];
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
QT_MOC_LITERAL(3, 40, 5),
QT_MOC_LITERAL(4, 46, 10),
QT_MOC_LITERAL(5, 57, 29),
QT_MOC_LITERAL(6, 87, 14),
QT_MOC_LITERAL(7, 102, 17),
QT_MOC_LITERAL(8, 120, 16),
QT_MOC_LITERAL(9, 137, 8),
QT_MOC_LITERAL(10, 146, 10),
QT_MOC_LITERAL(11, 157, 12),
QT_MOC_LITERAL(12, 170, 20),
QT_MOC_LITERAL(13, 191, 21),
QT_MOC_LITERAL(14, 213, 26)
    },
    "eMU::analyzer::View\0displayReadPayload\0"
    "\0index\0disconnect\0resizeWritePayloadFieldsCount\0"
    "numberOfFields\0parseNumericField\0"
    "parseStringField\0sendDump\0sendFields\0"
    "generateDump\0clearReadPayloadView\0"
    "clearWritePayloadView\0calculatehWritePayloadSize\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_eMU__analyzer__View[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x0a,
       4,    0,   72,    2, 0x0a,
       5,    1,   73,    2, 0x0a,
       7,    0,   76,    2, 0x0a,
       8,    0,   77,    2, 0x0a,
       9,    0,   78,    2, 0x0a,
      10,    0,   79,    2, 0x0a,
      11,    0,   80,    2, 0x0a,
      12,    0,   81,    2, 0x0a,
      13,    0,   82,    2, 0x0a,
      14,    0,   83,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void eMU::analyzer::View::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        View *_t = static_cast<View *>(_o);
        switch (_id) {
        case 0: _t->displayReadPayload((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->disconnect(); break;
        case 2: _t->resizeWritePayloadFieldsCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->parseNumericField(); break;
        case 4: _t->parseStringField(); break;
        case 5: _t->sendDump(); break;
        case 6: _t->sendFields(); break;
        case 7: _t->generateDump(); break;
        case 8: _t->clearReadPayloadView(); break;
        case 9: _t->clearWritePayloadView(); break;
        case 10: _t->calculatehWritePayloadSize(); break;
        default: ;
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
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
