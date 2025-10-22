#include "triangulationwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QtMath>

namespace {
constexpr double kHitRadius = 6.0;
constexpr int kPointRadius = 4;
constexpr qreal kTriangleOpacity = 0.25;
}

TriangulationWidget::TriangulationWidget(QWidget* parent)
    : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

QVector<QPointF> TriangulationWidget::points() const {
    return m_points;
}

void TriangulationWidget::clearPoints() {
    if (m_points.isEmpty()) {
        clearTriangles();
        return;
    }
    m_points.clear();
    clearTriangles();
    update();
    emit pointsChanged(m_points);
}

void TriangulationWidget::setTriangles(const QVector<QPolygonF>& triangles) {
    m_triangles = triangles;
    update();
}

void TriangulationWidget::clearTriangles() {
    if (m_triangles.isEmpty()) {
        return;
    }
    m_triangles.clear();
    update();
}

void TriangulationWidget::setDraggingEnabled(bool enabled) {
    if (!enabled) {
        m_dragging = false;
        m_dragIndex = -1;
    }
    m_dragEnabled = enabled;
}

void TriangulationWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        const int index = findPointIndex(eventPosition(event));
        if (index >= 0) {
            m_points.removeAt(index);
            clearTriangles();
            update();
            emit pointsChanged(m_points);
        }
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
    clearTriangles();
    update();
    emit pointsChanged(m_points);
}

void TriangulationWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragEnabled || !m_dragging) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    updatePoint(m_dragIndex, eventPosition(event));
}

void TriangulationWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        m_dragIndex = -1;
    }
    QWidget::mouseReleaseEvent(event);
}

void TriangulationWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_triangles.isEmpty()) {
        QPen trianglePen(QColor(30, 144, 255));
        trianglePen.setWidthF(1.5);
        painter.setPen(trianglePen);
        QColor fillColor = trianglePen.color();
        fillColor.setAlphaF(kTriangleOpacity);
        painter.setBrush(fillColor);

        for (const QPolygonF& polygon : m_triangles) {
            painter.drawPolygon(polygon);
        }
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

int TriangulationWidget::findPointIndex(const QPointF& position) const {
    for (int i = 0; i < m_points.size(); ++i) {
        const QPointF delta = m_points[i] - position;
        if (QPointF::dotProduct(delta, delta) <= kHitRadius * kHitRadius) {
            return i;
        }
    }
    return -1;
}

void TriangulationWidget::updatePoint(int index, const QPointF& position) {
    if (index < 0 || index >= m_points.size()) {
        return;
    }

    m_points[index] = position;
    clearTriangles();
    update();
    emit pointsChanged(m_points);
}

QPointF TriangulationWidget::eventPosition(const QMouseEvent* event) const {
    return event->position();
}
