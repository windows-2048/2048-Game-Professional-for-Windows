/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Mon 1. Dec 23:26:51 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x09,
      33,   11,   11,   11, 0x09,
      54,   11,   11,   11, 0x09,
      77,   11,   11,   11, 0x09,
      98,   11,   11,   11, 0x09,
     116,   11,   11,   11, 0x09,
     142,   11,   11,   11, 0x09,
     161,   11,   11,   11, 0x09,
     186,  179,   11,   11, 0x09,
     206,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0onTriggered4x4(bool)\0"
    "onTriggered8x8(bool)\0onTriggered16x16(bool)\0"
    "onTriggeredNewGame()\0onTriggeredHelp()\0"
    "onTriggeredCheckUpdates()\0onTriggeredAbout()\0"
    "onTriggeredExit()\0nScore\0onFieldChanged(int)\0"
    "onShowWindowTopLevel()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->onTriggered4x4((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onTriggered8x8((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onTriggered16x16((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onTriggeredNewGame(); break;
        case 4: _t->onTriggeredHelp(); break;
        case 5: _t->onTriggeredCheckUpdates(); break;
        case 6: _t->onTriggeredAbout(); break;
        case 7: _t->onTriggeredExit(); break;
        case 8: _t->onFieldChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onShowWindowTopLevel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
