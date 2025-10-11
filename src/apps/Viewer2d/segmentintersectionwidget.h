#ifndef SEGMENTINTERSECTIONWIDGET_H
#define SEGMENTINTERSECTIONWIDGET_H

#include <QPointF>
#include <QVector>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;

class SegmentIntersectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SegmentIntersectionWidget(QWidget* parent = nullptr);

    QVector<QPointF> points() const;
    void clearSegments();

    void setIntersectionNone();
    void setIntersectionPoint(const QPointF& point);
    void setIntersectionOverlap(const QPointF& start, const QPointF& end);
    void setDragEnabled(bool enabled);

signals:
    void segmentsChanged(const QVector<QPointF>& points);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    int findPointIndex(const QPointF& position) const;
    void appendPoint(const QPointF& position);
    void updatePoint(int index, const QPointF& position);
    QPointF eventPosition(const QMouseEvent* event) const;

    QVector<QPointF> m_points;   // [A, B, C, D]
    bool m_dragging = false;
    int m_dragIndex = -1;
    QVector<QPointF> m_highlightPoints;
    bool m_showOverlap = false;
    bool m_dragEnabled = false;
};

#endif
