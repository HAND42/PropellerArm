/****************************************************************************
** Meta object code from reading C++ file 'view_setting_sample_time.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MovingArmProject/view_setting_sample_time.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view_setting_sample_time.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.0. It"
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
struct qt_meta_stringdata_CLASSview_setting_sample_timeENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSview_setting_sample_timeENDCLASS = QtMocHelpers::stringData(
    "view_setting_sample_time",
    "modification_valeurs",
    "",
    "on_apply_button_clicked",
    "on_cancel_button_clicked",
    "on_sample_time_step_value_editingFinished",
    "on_sample_time_simulator_step_value_editingFinished",
    "on_sample_time_controller_step_value_editingFinished",
    "on_sample_time_graph_step_value_editingFinished"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSview_setting_sample_timeENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    0,   58,    2, 0x08,    3 /* Private */,
       5,    0,   59,    2, 0x08,    4 /* Private */,
       6,    0,   60,    2, 0x08,    5 /* Private */,
       7,    0,   61,    2, 0x08,    6 /* Private */,
       8,    0,   62,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject view_setting_sample_time::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSview_setting_sample_timeENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSview_setting_sample_timeENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSview_setting_sample_timeENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<view_setting_sample_time, std::true_type>,
        // method 'modification_valeurs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_apply_button_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cancel_button_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sample_time_step_value_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sample_time_simulator_step_value_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sample_time_controller_step_value_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sample_time_graph_step_value_editingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void view_setting_sample_time::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<view_setting_sample_time *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->modification_valeurs(); break;
        case 1: _t->on_apply_button_clicked(); break;
        case 2: _t->on_cancel_button_clicked(); break;
        case 3: _t->on_sample_time_step_value_editingFinished(); break;
        case 4: _t->on_sample_time_simulator_step_value_editingFinished(); break;
        case 5: _t->on_sample_time_controller_step_value_editingFinished(); break;
        case 6: _t->on_sample_time_graph_step_value_editingFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (view_setting_sample_time::*)();
            if (_t _q_method = &view_setting_sample_time::modification_valeurs; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *view_setting_sample_time::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *view_setting_sample_time::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSview_setting_sample_timeENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int view_setting_sample_time::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void view_setting_sample_time::modification_valeurs()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
