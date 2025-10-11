#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>

#include "PlaneGeometry/PlaneOperations.h"
#include "precisionconverters.h"

class PrecisionDrawWidget;

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
        ~MainWindow();

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

    private:
        void applyOrientationResult(bool hasResult, plane_geometry::Orientation orientation);
        QString orientationDescription(plane_geometry::Orientation orientation) const;
        void updatePointsSummary();

        void updateIntersectionSummary();
        void applyIntersectionResult();
        bool intersectionSegmentsReady() const;
        SegmentIntersectionResultD evaluateIntersection() const;

        Ui::MainWindow* ui;                                                                         // указатель на сгенерированный класс UI
        QVector<QPointF> m_points;                                                                  // точки из DrawWidget (A,B — отрезок, C — тест)
        bool m_hasOrientationResult = false;                                                        // есть ли кэш результата ориентации?
        plane_geometry::Orientation m_lastOrientation = plane_geometry::Orientation::OnSegment;     // последний результат ориентации 
        QString m_lastResultSummary;                                                                // строка-резюме для textBrowser

        QVector<QPointF> m_segmentPoints;                                                           // точки из SegmentIntersectionWidget (A, B, C, D)
        bool m_liveIntersection = false;                                                            // живой пересчёт при каждом движении точки?
        bool m_hasIntersectionResult = false;                                                       // есть ли кэш результата пересечения?  
        SegmentIntersectionResultD m_lastIntersection{};                                            // последний результат 
        QString m_lastIntersectionSummary;                                                          // описание результата для UI 

        PrecisionDrawWidget* m_precisionCanvas = nullptr;
};

#endif
