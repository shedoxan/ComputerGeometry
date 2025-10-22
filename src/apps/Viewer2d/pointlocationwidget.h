#ifndef POINTLOCATIONWIDGET_H
#define POINTLOCATIONWIDGET_H

#include <QPointF>
#include <QPolygonF>
#include <QString>
#include <QVector>
#include <QWidget>

#include "PlaneGeometry/PlaneOperations.h"

class QMouseEvent;
class QPaintEvent;

class PointLocationWidget : public QWidget {
    Q_OBJECT

public:
    explicit PointLocationWidget(QWidget* parent = nullptr);

    QVector<QPointF> rawPoints() const;
    QVector<QPointF> convexHull() const;
    bool polygonReady() const;
    bool hasTestPoint() const;

    void clearAll();
    void finalizePolygon();
    void removeTestPoint();
    void setDraggingEnabled(bool enabled);

signals:
    void polygonPointsChanged(const QVector<QPointF>& points);
    void polygonFinalized(const QVector<QPointF>& hull);
    void testPointChanged(const QPointF& point);
    void classificationChanged(plane_geometry::PointClassification classification,
                               const QString& description);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    enum class Stage {
        CollectPolygon,
        PolygonReady
    };

    enum class DisplayState {
        Unknown,
        Inside,
        Outside,
        OnBoundary,
        NearBoundary
    };

    void appendPoint(const QPointF& position);
    void updatePoint(int index, const QPointF& position);
    int hitTestPoint(const QPointF& position) const;
    void rebuildHull();
    QPointF eventPosition(const QMouseEvent* event) const;
    void evaluateTestPoint();
    double computeDelta() const;
    double distanceToPolygonBoundary(const QPointF& position) const;
    static QString describeClassification(plane_geometry::PointClassification classification,
                                          DisplayState state,
                                          double distance,
                                          double delta);

    QVector<QPointF> m_rawPoints;
    QPolygonF m_convexHull;
    Stage m_stage = Stage::CollectPolygon;

    bool m_dragEnabled = true;
    bool m_dragging = false;
    int m_dragIndex = -1;

    bool m_testPointDragging = false;
    QPointF m_testPoint;
    bool m_hasTestPoint = false;

    plane_geometry::PointClassification m_lastClassification =
        plane_geometry::PointClassification::Outside;
    DisplayState m_displayState = DisplayState::Unknown;
};

#endif
