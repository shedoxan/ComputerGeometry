#ifndef CONVEXHULLWIDGET_H
#define CONVEXHULLWIDGET_H

#include <QPointF>
#include <QVector>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;

class ConvexHullWidget : public QWidget {
    Q_OBJECT

public:
    explicit ConvexHullWidget(QWidget* parent = nullptr);

    QVector<QPointF> points() const;
    void clearPoints();
    void setConvexHull(const QVector<QPointF>& hull);
    void clearConvexHull();
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
    QVector<QPointF> m_convexHull;
    bool m_showConvexHull = false;
    bool m_dragEnabled = false;
    bool m_dragging = false;
    int m_dragIndex = -1;
};

#endif
