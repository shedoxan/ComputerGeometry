#include "precisiondrawwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStringList>

namespace {
constexpr double kHitRadius = 6.0;
constexpr int kPointRadius = 5;

QPointF toWidgetPoint(const plane_geometry::Point2D<plane_geometry::ExactScalar>& point) {
    return {point.x.convert_to<double>(), -point.y.convert_to<double>()};
}

plane_geometry::Point2D<plane_geometry::ExactScalar> toPlanePoint(const QPointF& point) {
    return {plane_geometry::ExactScalar(point.x()),
            plane_geometry::ExactScalar(-point.y())};
}

QString pointLabel(int index) {
    static const QStringList labels{"A", "B", "C"};
    return index < labels.size() ? labels[index] : QStringLiteral("P%1").arg(index + 1);
}
}  // namespace

PrecisionDrawWidget::PrecisionDrawWidget(QWidget* parent) : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void PrecisionDrawWidget::setSegment(
    const plane_geometry::Segment2D<plane_geometry::ExactScalar>& segment) {
    m_segment = segment;
    m_hasSegment = true;

    const QPointF start = toWidgetPoint(segment.start);
    const QPointF end = toWidgetPoint(segment.end);

    if (m_points.size() >= 1) {
        m_points[0] = start;
    } else {
        m_points.append(start);
    }

    if (m_points.size() >= 2) {
        m_points[1] = end;
    } else {
        m_points.append(end);
    }

    updateExactStateFromPoints();
    update();
}

void PrecisionDrawWidget::setPoint(
    const plane_geometry::Point2D<plane_geometry::ExactScalar>& point) {
    m_point = point;
    m_hasPoint = true;

    const QPointF widgetPoint = toWidgetPoint(point);
    if (m_points.size() >= 3) {
        m_points[2] = widgetPoint;
    } else {
        while (m_points.size() < 2) {
            m_points.append(QPointF{});
        }
        m_points.append(widgetPoint);
    }

    updateExactStateFromPoints();
    update();
}

void PrecisionDrawWidget::clear() {
    if (m_points.isEmpty()) {
        return;
    }

    m_points.clear();
    m_dragging = false;
    m_dragIndex = -1;
    m_hasSegment = false;
    m_hasPoint = false;
    updateExactStateFromPoints();
    update();
    emitPointsChanged();
}

void PrecisionDrawWidget::setDragEnabled(bool enabled) {
    if (!enabled) {
        m_dragging = false;
        m_dragIndex = -1;
    }
    m_dragEnabled = enabled;
}

void PrecisionDrawWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_points.size() >= 2) {
        painter.setPen(QPen(Qt::black, 2));
        painter.drawLine(m_points[0], m_points[1]);
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::red);

    for (int i = 0; i < m_points.size(); ++i) {
        painter.drawEllipse(m_points[i], kPointRadius, kPointRadius);
        painter.setPen(Qt::black);
        painter.drawText(m_points[i] + QPointF(kPointRadius + 3, -kPointRadius),
                         pointLabel(i));
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(Qt::red);
    }
}

void PrecisionDrawWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        clear();
        return;
    }

    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    const QPointF pos = event->position();
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

    if (m_points.size() == 3) {
        m_points.clear();
        m_hasSegment = false;
        m_hasPoint = false;
    }

    m_points.append(pos);
    updateExactStateFromPoints();
    emitPointsChanged();
    update();
}

void PrecisionDrawWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragEnabled || !m_dragging || m_dragIndex < 0 || m_dragIndex >= m_points.size()) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    m_points[m_dragIndex] = event->position();
    updateExactStateFromPoints();
    emitPointsChanged();
    update();
}

void PrecisionDrawWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        m_dragIndex = -1;
    }

    QWidget::mouseReleaseEvent(event);
}

void PrecisionDrawWidget::updateExactStateFromPoints() {
    m_hasSegment = m_points.size() >= 2;
    m_hasPoint = m_points.size() >= 3;

    if (m_hasSegment) {
        m_segment.start = toPlanePoint(m_points[0]);
        m_segment.end = toPlanePoint(m_points[1]);
    }
    if (m_hasPoint) {
        m_point = toPlanePoint(m_points[2]);
    }
}

void PrecisionDrawWidget::emitPointsChanged() {
    QVector<plane_geometry::Point2D<plane_geometry::ExactScalar>> planePoints;
    planePoints.reserve(m_points.size());
    for (const auto& widgetPoint : m_points) {
        planePoints.append(toPlanePoint(widgetPoint));
    }
    emit canvasPointsChanged(planePoints);
}

int PrecisionDrawWidget::findPointIndex(const QPointF& position) const {
    for (int i = 0; i < m_points.size(); ++i) {
        const QPointF delta = m_points[i] - position;
        if (QPointF::dotProduct(delta, delta) <= kHitRadius * kHitRadius) {
            return i;
        }
    }
    return -1;
}
