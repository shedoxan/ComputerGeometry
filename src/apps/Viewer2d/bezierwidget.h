#ifndef BEZIERWIDGET_H
#define BEZIERWIDGET_H

#include <QPointF>
#include <QVector>
#include <QWidget>

#include "PlaneGeometry/PlaneOperations.h"

class QMouseEvent;
class QPaintEvent;

class BezierWidget : public QWidget {
    Q_OBJECT

public:
    explicit BezierWidget(QWidget* parent = nullptr);

    int degree() const;
    void setDegree(int degree);

    int sampleCount() const;
    void setSampleCount(int count);

    QVector<QPointF> controlPoints() const;
    QVector<QPointF> curvePoints() const;

    void clearControlPoints();
    void setDraggingEnabled(bool enabled);

signals:
    void controlPointsChanged(const QVector<QPointF>& points);
    void curveUpdated(const QVector<QPointF>& curve);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void rebuildCurve();
    int requiredPointCount() const;
    int hitTestPoint(const QPointF& position) const;
    QPointF eventPosition(const QMouseEvent* event) const;

    int m_degree = 1;
    int m_sampleCount = 100;
    QVector<QPointF> m_controlPoints;
    QVector<QPointF> m_curvePoints;

    bool m_dragEnabled = true;
    bool m_dragging = false;
    int m_dragIndex = -1;
};

#endif
