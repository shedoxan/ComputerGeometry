#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPolygonF>
#include <QString>
#include <QVector>
#include <vector>

#include "PlaneGeometry/PlaneOperations.h"
#include "precisionconverters.h"
#include "convexhullwidget.h"
#include "precisiondrawwidget.h"
#include "triangulationwidget.h"
#include "convexpolygonboolewidget.h"
#include "pointlocationwidget.h"
#include "bezierwidget.h"

using PlaneScalar = double;
using PlanePointD = plane_geometry::Point2D<PlaneScalar>;
using PlaneSegmentD = plane_geometry::Segment2D<PlaneScalar>;
using SegmentIntersectionResultD = plane_geometry::SegmentIntersectionResult<PlaneScalar>;

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void updatePoints(const QVector<QPointF>& points);
    void onCheckOrientation();

    void onSegmentsChanged(const QVector<QPointF>& points);
    void onComputeIntersectionClicked();
    void onLiveIntersectionToggled(bool checked);
    void onClearSegmentsClicked();

    void onPrecisionCheckButtonClicked();
    void onPrecisionClearButtonClicked();
    void onPrecisionCanvasPointsChanged(const QVector<plane_geometry::Point2D<plane_geometry::ExactScalar>>& points);

    void onHullPointsChanged(const QVector<QPointF>& points);
    void onBuildHullClicked();
    void onLiveHullToggled(bool checked);
    void onClearHullClicked();

    void onTriangulationPointsChanged(const QVector<QPointF>& points);
    void onTriangulationComputeClicked();
    void onTriangulationLiveToggled(bool checked);
    void onTriangulationDragToggled(bool checked);
    void onTriangulationClearClicked();

    void onBooleanPolygonsChanged();
    void onBooleanFinalizeFirstClicked();
    void onBooleanFinalizeSecondClicked();
    void onBooleanComputeClicked();
    void onBooleanOperationChanged(int index);
    void onBooleanLiveToggled(bool checked);
    void onBooleanClearClicked();
    void onBooleanPhaseChanged(ConvexPolygonBooleWidget::Phase phase);

    void onPointLocationPointsChanged(const QVector<QPointF>& points);
    void onPointLocationHullReady(const QVector<QPointF>& hull);
    void onPointLocationTestPointChanged(const QPointF& point);
    void onPointLocationSummaryChanged(plane_geometry::PointClassification classification,
                                       const QString& description);
    void onPointLocationFinalizeClicked();
    void onPointLocationClearClicked();
    void onPointLocationRemoveTestClicked();
    void onPointLocationDragToggled(bool checked);

    void onBezierControlPointsChanged(const QVector<QPointF>& points);
    void onBezierCurveUpdated(const QVector<QPointF>& curve);
    void onBezierDegreeChanged(int index);
    void onBezierSamplesChanged(int value);
    void onBezierDragToggled(bool checked);
    void onBezierClearClicked();

private:
    void applyOrientationResult(bool hasResult, plane_geometry::Orientation orientation);
    QString orientationDescription(plane_geometry::Orientation orientation) const;
    void updatePointsSummary();

    void updateHullSummary();
    void rebuildConvexHull();
    void clearConvexHull();

    void updateIntersectionSummary();
    void applyIntersectionResult();
    bool intersectionSegmentsReady() const;
    SegmentIntersectionResultD evaluateIntersection() const;

    void updateTriangulationSummary();
    void computeDelaunayTriangulation();

    enum class BooleanOperation {
        Intersection,
        Union,
        Difference
    };
    void updateBooleanSummary();
    void recomputeBooleanOperation();
    BooleanOperation currentBooleanOperation() const;
    void updateBooleanControls();
    static QVector<QPointF> toQPointVector(const std::vector<plane_geometry::Point2D<double>>& polygon);
    static std::vector<plane_geometry::Point2D<double>> toGeometryPoints(const QVector<QPointF>& points);

    void updatePointLocationSummary(const QString& description);
    void updateBezierInfo();

    Ui::MainWindow* ui = nullptr;

    QVector<QPointF> m_orientationPoints;
    bool m_hasOrientationResult = false;
    plane_geometry::Orientation m_lastOrientation = plane_geometry::Orientation::OnSegment;
    QString m_lastResultSummary;

    QVector<QPointF> m_segmentPoints;
    bool m_liveIntersection = false;
    bool m_hasIntersectionResult = false;
    SegmentIntersectionResultD m_lastIntersection{};
    QString m_lastIntersectionSummary;

    QVector<QPointF> m_hullInputPoints;
    QVector<QPointF> m_convexHullPoints;
    bool m_hasConvexHull = false;
    bool m_liveConvexHull = false;
    QString m_convexHullSummary;

    ConvexHullWidget* m_hullCanvas = nullptr;
    PrecisionDrawWidget* m_precisionCanvas = nullptr;

    TriangulationWidget* m_triangulationCanvas = nullptr;
    QVector<QPolygonF> m_triangulationTriangles;
    bool m_triangulationLive = false;
    QString m_triangulationSummary;

    ConvexPolygonBooleWidget* m_booleanCanvas = nullptr;
    bool m_booleanLive = false;
    QVector<QPolygonF> m_booleanOuters;
    QVector<QPolygonF> m_booleanHoles;
    QString m_booleanSummary;
    QString m_booleanStatusMessage;

    PointLocationWidget* m_pointLocationCanvas = nullptr;
    QString m_pointLocationSummary;
    QString m_pointLocationStatusText;
    QPointF m_pointLocationTestPoint{};
    bool m_pointLocationHasTestPoint = false;

    BezierWidget* m_bezierCanvas = nullptr;
    QVector<QPointF> m_bezierCurvePoints;
    QString m_bezierInfo;
};

#endif
