#include "convexhullwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPalette>

namespace {
constexpr double kHitRadius = 6.0;
constexpr int kPointRadius = 5;
}

ConvexHullWidget::ConvexHullWidget(QWidget* parent) : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

QVector<QPointF> ConvexHullWidget::points() const {
    return m_points;
}

void ConvexHullWidget::clearPoints() {
    if (m_points.isEmpty() && m_convexHull.isEmpty()) {
        return;
    }

    m_points.clear();
    clearConvexHull();
    update();
    emit pointsChanged(m_points);
}

void ConvexHullWidget::setConvexHull(const QVector<QPointF>& hull) {
    m_convexHull = hull;
    m_showConvexHull = m_convexHull.size() >= 2;
    update();
}

void ConvexHullWidget::clearConvexHull() {
    m_convexHull.clear();
    m_showConvexHull = false;
    update();
}

void ConvexHullWidget::setDraggingEnabled(bool enabled) {
    if (!enabled) {
        m_dragging = false;
        m_dragIndex = -1;
    }
    m_dragEnabled = enabled;
}

void ConvexHullWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        clearPoints();
        return;
    }

    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    const QPointF pos = eventPosition(event);
    const int index = findPointIndex(pos);

    if (index != -1 && m_dragEnabled) {
        m_dragging = true;
        m_dragIndex = index;
        return;
    }

    m_points.append(pos);
    clearConvexHull();
    update();
    emit pointsChanged(m_points);
}

void ConvexHullWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragEnabled || !m_dragging || m_dragIndex < 0 || m_dragIndex >= m_points.size()) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    updatePoint(m_dragIndex, eventPosition(event));
}

void ConvexHullWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        m_dragIndex = -1;
    }

    QWidget::mouseReleaseEvent(event);
}

void ConvexHullWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_showConvexHull && m_convexHull.size() >= 2) {
        painter.setPen(QPen(QColor(0, 120, 215), 2));
        painter.drawPolygon(QPolygonF(m_convexHull));
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::red);
    const QPointF offset{kPointRadius + 3.0, -kPointRadius};

    for (int i = 0; i < m_points.size(); ++i) {
        const QPointF& point = m_points[i];
        painter.drawEllipse(point, kPointRadius, kPointRadius);
        painter.drawText(point + offset, QString::number(i + 1));
    }
}

int ConvexHullWidget::findPointIndex(const QPointF& position) const {
    for (int i = 0; i < m_points.size(); ++i) {
        const QPointF delta = m_points[i] - position;
        if (QPointF::dotProduct(delta, delta) <= kHitRadius * kHitRadius) {
            return i;
        }
    }
    return -1;
}

void ConvexHullWidget::updatePoint(int index, const QPointF& position) {
    if (index < 0 || index >= m_points.size()) {
        return;
    }

    m_points[index] = position;
    clearConvexHull();
    update();
    emit pointsChanged(m_points);
}

QPointF ConvexHullWidget::eventPosition(const QMouseEvent* event) const {
    return event->position();
}
