#include "bezierwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>

#include <algorithm>

namespace {
constexpr double kHitRadius = 6.0;
constexpr int kPointRadius = 5;

QPointF toPointF(const plane_geometry::Point2D<double>& point) {
    return {point.x, point.y};
}
}  // namespace

BezierWidget::BezierWidget(QWidget* parent)
    : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

int BezierWidget::degree() const {
    return m_degree;
}

void BezierWidget::setDegree(int degree) {
    const int clamped = std::clamp(degree, 1, 3);
    if (m_degree == clamped) {
        return;
    }

    m_degree = clamped;
    const int required = requiredPointCount();
    if (m_controlPoints.size() > required) {
        m_controlPoints.resize(required);
        emit controlPointsChanged(m_controlPoints);
    }
    rebuildCurve();
}

int BezierWidget::sampleCount() const {
    return m_sampleCount;
}

void BezierWidget::setSampleCount(int count) {
    const int clamped = std::max(2, count);
    if (m_sampleCount == clamped) {
        return;
    }
    m_sampleCount = clamped;
    rebuildCurve();
}

QVector<QPointF> BezierWidget::controlPoints() const {
    return m_controlPoints;
}

QVector<QPointF> BezierWidget::curvePoints() const {
    return m_curvePoints;
}

void BezierWidget::clearControlPoints() {
    if (m_controlPoints.isEmpty()) {
        return;
    }
    m_controlPoints.clear();
    m_curvePoints.clear();
    m_dragging = false;
    m_dragIndex = -1;
    update();
    emit controlPointsChanged(m_controlPoints);
    emit curveUpdated(m_curvePoints);
}

void BezierWidget::setDraggingEnabled(bool enabled) {
    if (!enabled) {
        m_dragging = false;
        m_dragIndex = -1;
    }
    m_dragEnabled = enabled;
}

void BezierWidget::mousePressEvent(QMouseEvent* event) {
    const QPointF pos = eventPosition(event);

    if (event->button() == Qt::RightButton) {
        const int index = hitTestPoint(pos);
        if (index >= 0) {
            m_controlPoints.removeAt(index);
            emit controlPointsChanged(m_controlPoints);
            rebuildCurve();
        }
        return;
    }

    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    const int index = hitTestPoint(pos);
    if (index >= 0 && m_dragEnabled) {
        m_dragging = true;
        m_dragIndex = index;
        return;
    }

    if (m_controlPoints.size() >= requiredPointCount()) {
        return;
    }

    m_controlPoints.append(pos);
    emit controlPointsChanged(m_controlPoints);
    rebuildCurve();
}

void BezierWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragEnabled || !m_dragging || m_dragIndex < 0 ||
        m_dragIndex >= m_controlPoints.size()) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    m_controlPoints[m_dragIndex] = eventPosition(event);
    emit controlPointsChanged(m_controlPoints);
    rebuildCurve();
}

void BezierWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        m_dragIndex = -1;
    }
    QWidget::mouseReleaseEvent(event);
}

void BezierWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_curvePoints.size() >= 2) {
        painter.setPen(QPen(QColor(255, 99, 71), 3));
        QPainterPath path(m_curvePoints.first());
        for (int i = 1; i < m_curvePoints.size(); ++i) {
            path.lineTo(m_curvePoints[i]);
        }
        painter.drawPath(path);
    }

    if (m_controlPoints.size() >= 2) {
        QPen controlPen(QColor(30, 144, 255), 1.5, Qt::DashLine);
        painter.setPen(controlPen);
        painter.setBrush(Qt::NoBrush);
        QPolygonF polygon(m_controlPoints);
        painter.drawPolyline(polygon);
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(QColor(65, 105, 225));
    const QPointF labelOffset{kPointRadius + 3.0, -kPointRadius};

    for (int i = 0; i < m_controlPoints.size(); ++i) {
        const QPointF& point = m_controlPoints[i];
        painter.drawEllipse(point, kPointRadius, kPointRadius);
        painter.drawText(point + labelOffset, QStringLiteral("P%1").arg(i));
    }
}

void BezierWidget::rebuildCurve() {
    if (m_controlPoints.size() < requiredPointCount()) {
        m_curvePoints.clear();
        update();
        emit curveUpdated(m_curvePoints);
        return;
    }

    std::vector<plane_geometry::Point2D<double>> control;
    control.reserve(m_controlPoints.size());
    for (const QPointF& point : m_controlPoints) {
        control.push_back({point.x(), point.y()});
    }

    const std::vector<plane_geometry::Point2D<double>> samples =
        plane_geometry::sampleBezier(control, static_cast<std::size_t>(m_sampleCount));

    m_curvePoints.clear();
    m_curvePoints.reserve(static_cast<int>(samples.size()));
    for (const auto& point : samples) {
        m_curvePoints.append(toPointF(point));
    }

    update();
    emit curveUpdated(m_curvePoints);
}

int BezierWidget::requiredPointCount() const {
    return m_degree + 1;
}

int BezierWidget::hitTestPoint(const QPointF& position) const {
    for (int i = 0; i < m_controlPoints.size(); ++i) {
        const QPointF delta = m_controlPoints[i] - position;
        if (QPointF::dotProduct(delta, delta) <= kHitRadius * kHitRadius) {
            return i;
        }
    }
    return -1;
}

QPointF BezierWidget::eventPosition(const QMouseEvent* event) const {
    return event->position();
}
