#include "convexpolygonboolewidget.h"

#include <QBrush>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPalette>

namespace {
constexpr double kHitRadius = 6.0;
constexpr int kPointRadius = 5;

QPointF toPointF(const plane_geometry::Point2D<double>& point) {
    return {point.x, point.y};
}
}  // namespace

ConvexPolygonBooleWidget::ConvexPolygonBooleWidget(QWidget* parent)
    : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

QVector<QPointF> ConvexPolygonBooleWidget::firstRawPoints() const {
    return m_firstRawPoints;
}

QVector<QPointF> ConvexPolygonBooleWidget::secondRawPoints() const {
    return m_secondRawPoints;
}

QVector<QPointF> ConvexPolygonBooleWidget::firstConvexHull() const {
    return QVector<QPointF>(m_firstHull.begin(), m_firstHull.end());
}

QVector<QPointF> ConvexPolygonBooleWidget::secondConvexHull() const {
    return QVector<QPointF>(m_secondHull.begin(), m_secondHull.end());
}

QVector<QPointF> ConvexPolygonBooleWidget::resultPolygon() const {
    if (m_resultPolygons.isEmpty()) {
        return {};
    }
    const QPolygonF& first = m_resultPolygons.front();
    return QVector<QPointF>(first.begin(), first.end());
}

ConvexPolygonBooleWidget::Phase ConvexPolygonBooleWidget::phase() const {
    return m_phase;
}

void ConvexPolygonBooleWidget::clearAll() {
    m_firstRawPoints.clear();
    m_secondRawPoints.clear();
    m_firstHull.clear();
    m_secondHull.clear();
    m_resultPolygons.clear();
    m_resultHoles.clear();
    m_firstLocked = false;
    m_secondLocked = false;
    m_phase = Phase::FirstPolygonInput;
    m_dragging = false;
    m_dragHit = {};
    update();
    emit phaseChanged(m_phase);
    emit polygonsChanged();
}

void ConvexPolygonBooleWidget::finalizeFirstPolygon() {
    if (m_firstLocked) {
        return;
    }
    m_firstHull = buildHullFromPoints(m_firstRawPoints);
    m_firstLocked = true;
    m_phase = Phase::SecondPolygonInput;
    clearResultPolygon();
    update();
    emit phaseChanged(m_phase);
    emit polygonsChanged();
}

void ConvexPolygonBooleWidget::finalizeSecondPolygon() {
    if (m_secondLocked) {
        return;
    }
    m_secondHull = buildHullFromPoints(m_secondRawPoints);
    m_secondLocked = true;
    m_phase = Phase::ResultReady;
    clearResultPolygon();
    update();
    emit phaseChanged(m_phase);
    emit polygonsChanged();
}

void ConvexPolygonBooleWidget::setResultPolygon(const QVector<QPointF>& polygon) {
    QVector<QPolygonF> outers;
    if (!polygon.isEmpty()) {
        QPolygonF outer;
        outer.reserve(polygon.size());
        for (const auto& point : polygon) {
            outer.append(point);
        }
        outers.push_back(std::move(outer));
    }
    setResultPolygons(outers);
}

void ConvexPolygonBooleWidget::setResultPolygons(const QVector<QPolygonF>& outers,
                                                 const QVector<QPolygonF>& holes) {
    m_resultPolygons = outers;
    m_resultHoles = holes;
    update();
}

void ConvexPolygonBooleWidget::clearResultPolygon() {
    if (m_resultPolygons.isEmpty() && m_resultHoles.isEmpty()) {
        return;
    }
    m_resultPolygons.clear();
    m_resultHoles.clear();
    update();
}

void ConvexPolygonBooleWidget::setDraggingEnabled(bool enabled) {
    if (!enabled) {
        m_dragging = false;
        m_dragHit = {};
    }
    m_dragEnabled = enabled;
}

void ConvexPolygonBooleWidget::mousePressEvent(QMouseEvent* event) {
    const QPointF pos = eventPosition(event);

    if (event->button() == Qt::RightButton) {
        const HitInfo hit = hitTest(pos);
        if (hit.valid) {
            removePoint(hit);
            clearResultPolygon();
            emit polygonsChanged();
        }
        return;
    }

    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    const HitInfo hit = hitTest(pos);
    if (hit.valid && m_dragEnabled) {
        m_dragging = true;
        m_dragHit = hit;
        return;
    }

    appendPoint(pos);
}

void ConvexPolygonBooleWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragEnabled || !m_dragging || !m_dragHit.valid) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    updatePoint(m_dragHit, eventPosition(event));
}

void ConvexPolygonBooleWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        m_dragHit = {};
    }
    QWidget::mouseReleaseEvent(event);
}

void ConvexPolygonBooleWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_resultPolygons.isEmpty()) {
        QColor fillColor(255, 215, 0, 100);
        QPainterPath fillPath;
        fillPath.setFillRule(Qt::OddEvenFill);
        for (const auto& poly : m_resultPolygons) {
            fillPath.addPolygon(poly);
        }
        for (const auto& hole : m_resultHoles) {
            fillPath.addPolygon(hole);
        }
        painter.fillPath(fillPath, fillColor);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(255, 165, 0), 2.5));
        for (const auto& poly : m_resultPolygons) {
            painter.drawPolygon(poly);
        }
        if (!m_resultHoles.isEmpty()) {
            QPen holePen(QColor(255, 165, 0), 1.5, Qt::DashLine);
            painter.setPen(holePen);
            for (const auto& hole : m_resultHoles) {
                painter.drawPolygon(hole);
            }
        }
    }

    painter.setBrush(Qt::NoBrush);

    if (m_firstHull.size() >= 2) {
        painter.setPen(QPen(QColor(0, 120, 215), 2));
        painter.drawPolygon(m_firstHull);
    }

    if (m_secondHull.size() >= 2) {
        painter.setPen(QPen(QColor(0, 168, 107), 2));
        painter.drawPolygon(m_secondHull);
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(QColor(220, 20, 60));
    const QPointF labelOffset{kPointRadius + 3.0, -kPointRadius};

    for (int i = 0; i < m_firstRawPoints.size(); ++i) {
        const QPointF& point = m_firstRawPoints[i];
        painter.drawEllipse(point, kPointRadius, kPointRadius);
        painter.drawText(point + labelOffset, QStringLiteral("A%1").arg(i + 1));
    }

    painter.setBrush(QColor(34, 139, 34));
    for (int i = 0; i < m_secondRawPoints.size(); ++i) {
        const QPointF& point = m_secondRawPoints[i];
        painter.drawEllipse(point, kPointRadius, kPointRadius);
        painter.drawText(point + labelOffset, QStringLiteral("B%1").arg(i + 1));
    }
}

ConvexPolygonBooleWidget::HitInfo ConvexPolygonBooleWidget::hitTest(const QPointF& position) const {
    const auto checkPoints = [&](const QVector<QPointF>& points, bool first) -> HitInfo {
        for (int i = 0; i < points.size(); ++i) {
            const QPointF delta = points[i] - position;
            if (QPointF::dotProduct(delta, delta) <= kHitRadius * kHitRadius) {
                HitInfo info;
                info.valid = true;
                info.firstPolygon = first;
                info.index = i;
                return info;
            }
        }
        return {};
    };

    HitInfo hit = checkPoints(m_firstRawPoints, true);
    if (hit.valid) {
        return hit;
    }
    return checkPoints(m_secondRawPoints, false);
}

void ConvexPolygonBooleWidget::appendPoint(const QPointF& position) {
    switch (m_phase) {
    case Phase::FirstPolygonInput:
        if (m_firstLocked) {
            return;
        }
        m_firstRawPoints.append(position);
        clearResultPolygon();
        break;
    case Phase::SecondPolygonInput:
        if (m_secondLocked) {
            return;
        }
        m_secondRawPoints.append(position);
        clearResultPolygon();
        break;
    case Phase::ResultReady:
        return;
    }

    update();
    emit polygonsChanged();
}

void ConvexPolygonBooleWidget::removePoint(const HitInfo& hitInfo) {
    if (!hitInfo.valid) {
        return;
    }

    if (hitInfo.firstPolygon) {
        if (hitInfo.index >= 0 && hitInfo.index < m_firstRawPoints.size()) {
            m_firstRawPoints.removeAt(hitInfo.index);
            if (m_firstLocked) {
                rebuildFirstHull();
            }
        }
    } else {
        if (hitInfo.index >= 0 && hitInfo.index < m_secondRawPoints.size()) {
            m_secondRawPoints.removeAt(hitInfo.index);
            if (m_secondLocked) {
                rebuildSecondHull();
            }
        }
    }
    update();
}

void ConvexPolygonBooleWidget::updatePoint(const HitInfo& hitInfo, const QPointF& position) {
    if (!hitInfo.valid) {
        return;
    }

    if (hitInfo.firstPolygon) {
        if (hitInfo.index >= 0 && hitInfo.index < m_firstRawPoints.size()) {
            m_firstRawPoints[hitInfo.index] = position;
            if (m_firstLocked) {
                rebuildFirstHull();
            }
        }
    } else {
        if (hitInfo.index >= 0 && hitInfo.index < m_secondRawPoints.size()) {
            m_secondRawPoints[hitInfo.index] = position;
            if (m_secondLocked) {
                rebuildSecondHull();
            }
        }
    }

    clearResultPolygon();
    update();
    emit polygonsChanged();
}

QPointF ConvexPolygonBooleWidget::eventPosition(const QMouseEvent* event) const {
    return event->position();
}

void ConvexPolygonBooleWidget::rebuildFirstHull() {
    m_firstHull = buildHullFromPoints(m_firstRawPoints);
    emit polygonsChanged();
}

void ConvexPolygonBooleWidget::rebuildSecondHull() {
    m_secondHull = buildHullFromPoints(m_secondRawPoints);
    emit polygonsChanged();
}

QPolygonF ConvexPolygonBooleWidget::buildHullFromPoints(const QVector<QPointF>& points) {
    if (points.size() < 3) {
        return {};
    }

    std::vector<plane_geometry::Point2D<double>> geometryPoints;
    geometryPoints.reserve(points.size());
    for (const QPointF& point : points) {
        geometryPoints.push_back({point.x(), point.y()});
    }

    const std::vector<plane_geometry::Point2D<double>> hull =
        plane_geometry::computeConvexHull(geometryPoints);

    QPolygonF polygon;
    polygon.reserve(static_cast<int>(hull.size()));
    for (const auto& vertex : hull) {
        polygon.append(toPointF(vertex));
    }
    return polygon;
}
