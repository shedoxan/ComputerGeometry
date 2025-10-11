#include "segmentintersectionwidget.h"

#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QStringList>

namespace {
constexpr double kHitRadius = 6.0;
constexpr int kPointRadius = 5;
}

SegmentIntersectionWidget::SegmentIntersectionWidget(QWidget* parent) : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

QVector<QPointF> SegmentIntersectionWidget::points() const {
    return m_points;
}

void SegmentIntersectionWidget::clearSegments() {
    if (m_points.isEmpty() && m_highlightPoints.isEmpty()) {
        return;
    }

    m_points.clear();
    setIntersectionNone();
    emit segmentsChanged(m_points);
}

void SegmentIntersectionWidget::setIntersectionNone() {
    m_highlightPoints.clear();
    m_showOverlap = false;
    update();
}

void SegmentIntersectionWidget::setIntersectionPoint(const QPointF& point) {
    m_highlightPoints = {point};
    m_showOverlap = false;
    update();
}

void SegmentIntersectionWidget::setIntersectionOverlap(const QPointF& start, const QPointF& end)
{
    m_highlightPoints = {start, end};
    m_showOverlap = true;
    update();
}

void SegmentIntersectionWidget::setDragEnabled(bool enabled)
{
    if (!enabled) {
        m_dragging = false;
        m_dragIndex = -1;
    }
    m_dragEnabled = enabled;
}

void SegmentIntersectionWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        clearSegments();
        return;
    }

    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    const QPointF pos = eventPosition(event);
    const int index = findPointIndex(pos);

    if (index != -1) {
        if (!m_dragEnabled) {
            QWidget::mousePressEvent(event);
            return;
        }
        m_dragging = true;
        m_dragIndex = index;
        return;
    }

    if (m_points.size() < 4) {
        appendPoint(pos);
    }
}

void SegmentIntersectionWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragEnabled || !m_dragging || m_dragIndex < 0 || m_dragIndex >= m_points.size()) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    updatePoint(m_dragIndex, eventPosition(event));
}

void SegmentIntersectionWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        m_dragIndex = -1;
    }

    QWidget::mouseReleaseEvent(event);
}

void SegmentIntersectionWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QPen segmentPen(Qt::black, 2);
    const QPen secondSegmentPen(QColor(0, 191, 255), 2);

    if (m_points.size() >= 2) {
        painter.setPen(segmentPen);
        painter.drawLine(m_points[0], m_points[1]);
    }
    
    if (m_points.size() >= 4) {
        painter.setPen(secondSegmentPen);
        painter.drawLine(m_points[2], m_points[3]);
    }

    const QStringList labels{"A", "B", "C", "D"};

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::red);

    for (int i = 0; i < m_points.size(); ++i) {
        painter.drawEllipse(m_points[i], kPointRadius, kPointRadius);
        painter.setPen(Qt::black);
        painter.drawText(m_points[i] + QPointF(kPointRadius + 3, -kPointRadius),
                         i < labels.size() ? labels[i] : QStringLiteral("P%1").arg(i + 1));
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(Qt::red);
    }

    if (!m_highlightPoints.isEmpty()) {
        if (m_showOverlap && m_highlightPoints.size() == 2) {
            painter.setPen(QPen(QColor(50, 205, 50), 4, Qt::DashLine));
            painter.drawLine(m_highlightPoints[0], m_highlightPoints[1]);
        } else {
            painter.setPen(QPen(Qt::black, 1));
            painter.setBrush(QColor(50, 205, 50));
            for (const auto& highlightPoint : m_highlightPoints) {
                painter.drawEllipse(highlightPoint, kPointRadius + 2, kPointRadius + 2);
            }
        }
    }
}

int SegmentIntersectionWidget::findPointIndex(const QPointF& position) const {
    for (int i = 0; i < m_points.size(); ++i) {
        const QPointF delta = m_points[i] - position;
        const double distanceSquared = QPointF::dotProduct(delta, delta);
        if (distanceSquared <= kHitRadius * kHitRadius) {
            return i;
        }
    }

    return -1;
}

void SegmentIntersectionWidget::appendPoint(const QPointF& position) {
    m_points.append(position);
    emit segmentsChanged(m_points);
    update();
}

void SegmentIntersectionWidget::updatePoint(int index, const QPointF& position) {
    if (index < 0 || index >= m_points.size()) {
        return;
    }

    m_points[index] = position;
    emit segmentsChanged(m_points);
    update();
}

QPointF SegmentIntersectionWidget::eventPosition(const QMouseEvent* event) const {
    return event->position();
}
