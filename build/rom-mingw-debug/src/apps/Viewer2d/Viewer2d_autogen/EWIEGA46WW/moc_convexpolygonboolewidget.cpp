/****************************************************************************
** Meta object code from reading C++ file 'convexpolygonboolewidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/apps/Viewer2d/convexpolygonboolewidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'convexpolygonboolewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN24ConvexPolygonBooleWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ConvexPolygonBooleWidget::qt_create_metaobjectdata<qt_meta_tag_ZN24ConvexPolygonBooleWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ConvexPolygonBooleWidget",
        "polygonsChanged",
        "",
        "phaseChanged",
        "ConvexPolygonBooleWidget::Phase",
        "phase"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'polygonsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'phaseChanged'
        QtMocHelpers::SignalData<void(ConvexPolygonBooleWidget::Phase)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ConvexPolygonBooleWidget, qt_meta_tag_ZN24ConvexPolygonBooleWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ConvexPolygonBooleWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24ConvexPolygonBooleWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24ConvexPolygonBooleWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN24ConvexPolygonBooleWidgetE_t>.metaTypes,
    nullptr
} };

void ConvexPolygonBooleWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ConvexPolygonBooleWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->polygonsChanged(); break;
        case 1: _t->phaseChanged((*reinterpret_cast< std::add_pointer_t<ConvexPolygonBooleWidget::Phase>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ConvexPolygonBooleWidget::*)()>(_a, &ConvexPolygonBooleWidget::polygonsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConvexPolygonBooleWidget::*)(ConvexPolygonBooleWidget::Phase )>(_a, &ConvexPolygonBooleWidget::phaseChanged, 1))
            return;
    }
}

const QMetaObject *ConvexPolygonBooleWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConvexPolygonBooleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24ConvexPolygonBooleWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ConvexPolygonBooleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ConvexPolygonBooleWidget::polygonsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConvexPolygonBooleWidget::phaseChanged(ConvexPolygonBooleWidget::Phase _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
