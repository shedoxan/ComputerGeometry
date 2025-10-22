/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/apps/Viewer2d/mainwindow.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "updatePoints",
        "",
        "QList<QPointF>",
        "points",
        "onCheckOrientation",
        "onSegmentsChanged",
        "onComputeIntersectionClicked",
        "onLiveIntersectionToggled",
        "checked",
        "onClearSegmentsClicked",
        "onPrecisionCheckButtonClicked",
        "onPrecisionClearButtonClicked",
        "onPrecisionCanvasPointsChanged",
        "QList<plane_geometry::Point2D<plane_geometry::ExactScalar>>",
        "onHullPointsChanged",
        "onBuildHullClicked",
        "onLiveHullToggled",
        "onClearHullClicked",
        "onTriangulationPointsChanged",
        "onTriangulationComputeClicked",
        "onTriangulationLiveToggled",
        "onTriangulationDragToggled",
        "onTriangulationClearClicked",
        "onBooleanPolygonsChanged",
        "onBooleanFinalizeFirstClicked",
        "onBooleanFinalizeSecondClicked",
        "onBooleanComputeClicked",
        "onBooleanOperationChanged",
        "index",
        "onBooleanLiveToggled",
        "onBooleanClearClicked",
        "onBooleanPhaseChanged",
        "ConvexPolygonBooleWidget::Phase",
        "phase",
        "onPointLocationPointsChanged",
        "onPointLocationHullReady",
        "hull",
        "onPointLocationTestPointChanged",
        "point",
        "onPointLocationSummaryChanged",
        "plane_geometry::PointClassification",
        "classification",
        "description",
        "onPointLocationFinalizeClicked",
        "onPointLocationClearClicked",
        "onPointLocationRemoveTestClicked",
        "onPointLocationDragToggled",
        "onBezierControlPointsChanged",
        "onBezierCurveUpdated",
        "curve",
        "onBezierDegreeChanged",
        "onBezierSamplesChanged",
        "value",
        "onBezierDragToggled",
        "onBezierClearClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'updatePoints'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'onCheckOrientation'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSegmentsChanged'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'onComputeIntersectionClicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLiveIntersectionToggled'
        QtMocHelpers::SlotData<void(bool)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onClearSegmentsClicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPrecisionCheckButtonClicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPrecisionClearButtonClicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPrecisionCanvasPointsChanged'
        QtMocHelpers::SlotData<void(const QVector<plane_geometry::Point2D<plane_geometry::ExactScalar>> &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 14, 4 },
        }}),
        // Slot 'onHullPointsChanged'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'onBuildHullClicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLiveHullToggled'
        QtMocHelpers::SlotData<void(bool)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onClearHullClicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTriangulationPointsChanged'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'onTriangulationComputeClicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTriangulationLiveToggled'
        QtMocHelpers::SlotData<void(bool)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onTriangulationDragToggled'
        QtMocHelpers::SlotData<void(bool)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onTriangulationClearClicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBooleanPolygonsChanged'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBooleanFinalizeFirstClicked'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBooleanFinalizeSecondClicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBooleanComputeClicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBooleanOperationChanged'
        QtMocHelpers::SlotData<void(int)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 29 },
        }}),
        // Slot 'onBooleanLiveToggled'
        QtMocHelpers::SlotData<void(bool)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onBooleanClearClicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBooleanPhaseChanged'
        QtMocHelpers::SlotData<void(ConvexPolygonBooleWidget::Phase)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 33, 34 },
        }}),
        // Slot 'onPointLocationPointsChanged'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'onPointLocationHullReady'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 37 },
        }}),
        // Slot 'onPointLocationTestPointChanged'
        QtMocHelpers::SlotData<void(const QPointF &)>(38, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPointF, 39 },
        }}),
        // Slot 'onPointLocationSummaryChanged'
        QtMocHelpers::SlotData<void(plane_geometry::PointClassification, const QString &)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 41, 42 }, { QMetaType::QString, 43 },
        }}),
        // Slot 'onPointLocationFinalizeClicked'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPointLocationClearClicked'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPointLocationRemoveTestClicked'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPointLocationDragToggled'
        QtMocHelpers::SlotData<void(bool)>(47, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onBezierControlPointsChanged'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(48, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'onBezierCurveUpdated'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 50 },
        }}),
        // Slot 'onBezierDegreeChanged'
        QtMocHelpers::SlotData<void(int)>(51, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 29 },
        }}),
        // Slot 'onBezierSamplesChanged'
        QtMocHelpers::SlotData<void(int)>(52, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 53 },
        }}),
        // Slot 'onBezierDragToggled'
        QtMocHelpers::SlotData<void(bool)>(54, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onBezierClearClicked'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->updatePoints((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 1: _t->onCheckOrientation(); break;
        case 2: _t->onSegmentsChanged((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 3: _t->onComputeIntersectionClicked(); break;
        case 4: _t->onLiveIntersectionToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->onClearSegmentsClicked(); break;
        case 6: _t->onPrecisionCheckButtonClicked(); break;
        case 7: _t->onPrecisionClearButtonClicked(); break;
        case 8: _t->onPrecisionCanvasPointsChanged((*reinterpret_cast< std::add_pointer_t<QList<plane_geometry::Point2D<plane_geometry::ExactScalar>>>>(_a[1]))); break;
        case 9: _t->onHullPointsChanged((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 10: _t->onBuildHullClicked(); break;
        case 11: _t->onLiveHullToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 12: _t->onClearHullClicked(); break;
        case 13: _t->onTriangulationPointsChanged((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 14: _t->onTriangulationComputeClicked(); break;
        case 15: _t->onTriangulationLiveToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 16: _t->onTriangulationDragToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 17: _t->onTriangulationClearClicked(); break;
        case 18: _t->onBooleanPolygonsChanged(); break;
        case 19: _t->onBooleanFinalizeFirstClicked(); break;
        case 20: _t->onBooleanFinalizeSecondClicked(); break;
        case 21: _t->onBooleanComputeClicked(); break;
        case 22: _t->onBooleanOperationChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 23: _t->onBooleanLiveToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 24: _t->onBooleanClearClicked(); break;
        case 25: _t->onBooleanPhaseChanged((*reinterpret_cast< std::add_pointer_t<ConvexPolygonBooleWidget::Phase>>(_a[1]))); break;
        case 26: _t->onPointLocationPointsChanged((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 27: _t->onPointLocationHullReady((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 28: _t->onPointLocationTestPointChanged((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1]))); break;
        case 29: _t->onPointLocationSummaryChanged((*reinterpret_cast< std::add_pointer_t<plane_geometry::PointClassification>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 30: _t->onPointLocationFinalizeClicked(); break;
        case 31: _t->onPointLocationClearClicked(); break;
        case 32: _t->onPointLocationRemoveTestClicked(); break;
        case 33: _t->onPointLocationDragToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 34: _t->onBezierControlPointsChanged((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 35: _t->onBezierCurveUpdated((*reinterpret_cast< std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 36: _t->onBezierDegreeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 37: _t->onBezierSamplesChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 38: _t->onBezierDragToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 39: _t->onBezierClearClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        case 26:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        case 27:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        case 34:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        case 35:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPointF> >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
    return _id;
}
QT_WARNING_POP
