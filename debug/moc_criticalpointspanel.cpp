/****************************************************************************
** Meta object code from reading C++ file 'criticalpointspanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../criticalpointspanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'criticalpointspanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CriticalPointsPanel_t {
    QByteArrayData data[5];
    char stringdata[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CriticalPointsPanel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CriticalPointsPanel_t qt_meta_stringdata_CriticalPointsPanel = {
    {
QT_MOC_LITERAL(0, 0, 19), // "CriticalPointsPanel"
QT_MOC_LITERAL(1, 20, 17), // "sig_methodChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 25), // "sig_smoothingTimesChanged"
QT_MOC_LITERAL(4, 65, 24) // "sig_smoothingTypeChanged"

    },
    "CriticalPointsPanel\0sig_methodChanged\0"
    "\0sig_smoothingTimesChanged\0"
    "sig_smoothingTypeChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CriticalPointsPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    1,   32,    2, 0x06 /* Public */,
       4,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void CriticalPointsPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CriticalPointsPanel *_t = static_cast<CriticalPointsPanel *>(_o);
        switch (_id) {
        case 0: _t->sig_methodChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sig_smoothingTimesChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sig_smoothingTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CriticalPointsPanel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CriticalPointsPanel::sig_methodChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CriticalPointsPanel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CriticalPointsPanel::sig_smoothingTimesChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (CriticalPointsPanel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CriticalPointsPanel::sig_smoothingTypeChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject CriticalPointsPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CriticalPointsPanel.data,
      qt_meta_data_CriticalPointsPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CriticalPointsPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CriticalPointsPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CriticalPointsPanel.stringdata))
        return static_cast<void*>(const_cast< CriticalPointsPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int CriticalPointsPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CriticalPointsPanel::sig_methodChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CriticalPointsPanel::sig_smoothingTimesChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CriticalPointsPanel::sig_smoothingTypeChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
