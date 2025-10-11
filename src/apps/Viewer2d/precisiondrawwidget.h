#ifndef PRECISIONDRAWWIDGET_H
#define PRECISIONDRAWWIDGET_H

#include <QPointF>
#include <QVector>
#include <QWidget>

#include "PlaneGeometry/PlaneOperations.h"

class QMouseEvent;
class QPaintEvent;

class PrecisionDrawWidget : public QWidget {
    Q_OBJECT

public:
    explicit PrecisionDrawWidget(QWidget* parent = nullptr);

    void setSegment(const plane_geometry::Segment2D<plane_geometry::ExactScalar>& segment);
    void setPoint(const plane_geometry::Point2D<plane_geometry::ExactScalar>& point);
    void clear();
    void setDragEnabled(bool enabled);

signals:
    void canvasPointsChanged(QVector<plane_geometry::Point2D<plane_geometry::ExactScalar>> points);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void updateExactStateFromPoints();
    void emitPointsChanged();
    int findPointIndex(const QPointF& position) const;

    QVector<QPointF> m_points;
    bool m_dragging = false;
    int m_dragIndex = -1;
    bool m_dragEnabled = true;

    bool m_hasSegment = false;
    bool m_hasPoint = false;
    plane_geometry::Segment2D<plane_geometry::ExactScalar> m_segment{};
    plane_geometry::Point2D<plane_geometry::ExactScalar> m_point{};
};

#endif
