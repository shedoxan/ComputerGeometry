#ifndef TRIANGULATIONWIDGET_H
#define TRIANGULATIONWIDGET_H

#include <QPointF>
#include <QPolygonF>
#include <QVector>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;

class TriangulationWidget : public QWidget {
    Q_OBJECT

public:
    explicit TriangulationWidget(QWidget* parent = nullptr);

    QVector<QPointF> points() const;
    void clearPoints();
    void setTriangles(const QVector<QPolygonF>& triangles);
    void clearTriangles();
    void setDraggingEnabled(bool enabled);

signals:
    void pointsChanged(const QVector<QPointF>& points);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    int findPointIndex(const QPointF& position) const;
    void updatePoint(int index, const QPointF& position);
    QPointF eventPosition(const QMouseEvent* event) const;

    QVector<QPointF> m_points;
    QVector<QPolygonF> m_triangles;
    bool m_dragEnabled = true;
    bool m_dragging = false;
    int m_dragIndex = -1;
};

#endif
