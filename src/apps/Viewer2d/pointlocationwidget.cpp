#include "pointlocationwidget.h"

#include <QLinearGradient>
#include <QtGlobal>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPalette>

#include <algorithm>
#include <cmath>
#include <limits>

namespace {
constexpr double kHitRadius = 6.0;
constexpr int kPointRadius = 5;

QPolygonF buildConvexHull(const QVector<QPointF>& points) {
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
        polygon.append(QPointF(vertex.x, vertex.y));
    }
    return polygon;
}

double distancePointToSegment(const QPointF& p,
                              const QPointF& a,
                              const QPointF& b) {
    const QPointF ab = b - a;
    const double lengthSquared = QPointF::dotProduct(ab, ab);
    if (lengthSquared <= std::numeric_limits<double>::epsilon()) {
        return std::sqrt(QPointF::dotProduct(p - a, p - a));
    }

    const double t = qBound(0.0, QPointF::dotProduct(p - a, ab) / lengthSquared, 1.0);
    const QPointF projection = a + t * ab;
    return std::sqrt(QPointF::dotProduct(p - projection, p - projection));
}
}  // namespace

PointLocationWidget::PointLocationWidget(QWidget* parent)
    : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

QVector<QPointF> PointLocationWidget::rawPoints() const {
    return m_rawPoints;
}

QVector<QPointF> PointLocationWidget::convexHull() const {
    return QVector<QPointF>(m_convexHull.begin(), m_convexHull.end());
}

bool PointLocationWidget::polygonReady() const {
    return m_stage == Stage::PolygonReady && m_convexHull.size() >= 3;
}

bool PointLocationWidget::hasTestPoint() const {
    return m_hasTestPoint;
}

void PointLocationWidget::clearAll() {
    m_rawPoints.clear();
    m_convexHull.clear();
    m_stage = Stage::CollectPolygon;
    m_dragging = false;
    m_dragIndex = -1;
    m_testPointDragging = false;
    m_hasTestPoint = false;
    m_displayState = DisplayState::Unknown;
    update();
    emit polygonPointsChanged(m_rawPoints);
    emit classificationChanged(plane_geometry::PointClassification::Outside,
                               tr("Add points and build the convex hull"));
}

void PointLocationWidget::finalizePolygon() {
    if (m_stage == Stage::PolygonReady) {
        return;
    }
    if (m_rawPoints.size() < 3) {
        emit classificationChanged(plane_geometry::PointClassification::Outside,
                                   tr("Need at least 3 points to build a convex hull"));
        return;
    }

    m_convexHull = buildConvexHull(m_rawPoints);
    if (m_convexHull.size() < 3) {
        emit classificationChanged(plane_geometry::PointClassification::Outside,
                                   tr("Convex hull is degenerate, adjust points"));
        return;
    }

    m_stage = Stage::PolygonReady;
    update();
    emit polygonFinalized(convexHull());
    if (m_hasTestPoint) {
        evaluateTestPoint();
    }
}

void PointLocationWidget::removeTestPoint() {
    if (!m_hasTestPoint) {
        return;
    }
    m_hasTestPoint = false;
    m_testPointDragging = false;
    m_displayState = DisplayState::Unknown;
    update();
    emit classificationChanged(plane_geometry::PointClassification::Outside,
                               tr("Test point removed"));
}

void PointLocationWidget::setDraggingEnabled(bool enabled) {
    if (!enabled) {
        m_dragging = false;
        m_dragIndex = -1;
        m_testPointDragging = false;
    }
    m_dragEnabled = enabled;
}

void PointLocationWidget::mousePressEvent(QMouseEvent* event) {
    const QPointF pos = eventPosition(event);

    if (event->button() == Qt::RightButton) {
        if (m_stage == Stage::PolygonReady && m_hasTestPoint &&
            distancePointToSegment(pos, m_testPoint, m_testPoint) <= kHitRadius) {
            removeTestPoint();
            return;
        }

        const int index = hitTestPoint(pos);
        if (index >= 0) {
            m_rawPoints.removeAt(index);
            emit polygonPointsChanged(m_rawPoints);
            if (m_stage == Stage::PolygonReady) {
                rebuildHull();
                if (m_hasTestPoint) {
                    evaluateTestPoint();
                }
            }
            update();
        }
        return;
    }

    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    if (m_stage == Stage::CollectPolygon) {
        const int index = hitTestPoint(pos);
        if (index >= 0 && m_dragEnabled) {
            m_dragging = true;
            m_dragIndex = index;
            return;
        }
        appendPoint(pos);
        return;
    }

    // Stage::PolygonReady
    if (m_dragEnabled) {
        const int index = hitTestPoint(pos);
        if (index >= 0) {
            m_dragging = true;
            m_dragIndex = index;
            return;
        }

        const double distanceToTest = m_hasTestPoint
            ? std::sqrt(QPointF::dotProduct(pos - m_testPoint, pos - m_testPoint))
            : std::numeric_limits<double>::max();
        if (m_hasTestPoint && distanceToTest <= kHitRadius) {
            m_testPointDragging = true;
            return;
        }
    }

    m_testPoint = pos;
    m_hasTestPoint = true;
    emit testPointChanged(m_testPoint);
    evaluateTestPoint();
    update();
}

void PointLocationWidget::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragEnabled && m_dragging && m_dragIndex >= 0) {
        updatePoint(m_dragIndex, eventPosition(event));
        return;
    }

    if (m_dragEnabled && m_testPointDragging && m_hasTestPoint) {
        m_testPoint = eventPosition(event);
        emit testPointChanged(m_testPoint);
        evaluateTestPoint();
        update();
        return;
    }

    QWidget::mouseMoveEvent(event);
}

void PointLocationWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        m_dragIndex = -1;
        m_testPointDragging = false;
    }
    QWidget::mouseReleaseEvent(event);
}

void PointLocationWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_convexHull.size() >= 2) {
        painter.setPen(QPen(QColor(0, 120, 215), 2));
        painter.setBrush(QColor(0, 120, 215, 40));
        painter.drawPolygon(m_convexHull);
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::red);
    const QPointF labelOffset{kPointRadius + 3.0, -kPointRadius};

    for (int i = 0; i < m_rawPoints.size(); ++i) {
        const QPointF& point = m_rawPoints[i];
        painter.drawEllipse(point, kPointRadius, kPointRadius);
        painter.drawText(point + labelOffset, QString::number(i + 1));
    }

    if (m_hasTestPoint) {
        QColor color = Qt::darkGray;
        switch (m_displayState) {
        case DisplayState::Inside:
            color = QColor(50, 205, 50);
            break;
        case DisplayState::Outside:
            color = QColor(220, 20, 60);
            break;
        case DisplayState::OnBoundary:
            color = QColor(255, 165, 0);
            break;
        case DisplayState::NearBoundary:
            color = QColor(30, 144, 255);
            break;
        case DisplayState::Unknown:
            color = Qt::darkGray;
            break;
        }
        painter.setBrush(color);
        painter.setPen(QPen(color.darker(), 2));
        painter.drawEllipse(m_testPoint, kPointRadius + 1, kPointRadius + 1);
    }
}

void PointLocationWidget::appendPoint(const QPointF& position) {
    if (m_stage != Stage::CollectPolygon) {
        return;
    }

    m_rawPoints.append(position);
    emit polygonPointsChanged(m_rawPoints);
    update();
}

void PointLocationWidget::updatePoint(int index, const QPointF& position) {
    if (index < 0 || index >= m_rawPoints.size()) {
        return;
    }

    m_rawPoints[index] = position;
    emit polygonPointsChanged(m_rawPoints);

    if (m_stage == Stage::PolygonReady) {
        rebuildHull();
        if (m_hasTestPoint) {
            evaluateTestPoint();
        }
    }

    update();
}

int PointLocationWidget::hitTestPoint(const QPointF& position) const {
    for (int i = 0; i < m_rawPoints.size(); ++i) {
        const QPointF delta = m_rawPoints[i] - position;
        if (QPointF::dotProduct(delta, delta) <= kHitRadius * kHitRadius) {
            return i;
        }
    }
    return -1;
}

void PointLocationWidget::rebuildHull() {
    if (m_rawPoints.size() < 3) {
        m_convexHull.clear();
        m_stage = Stage::CollectPolygon;
        m_hasTestPoint = false;
        m_displayState = DisplayState::Unknown;
        emit classificationChanged(plane_geometry::PointClassification::Outside,
                                   tr("Polygon lost convex hull, add more points"));
        return;
    }

    m_convexHull = buildConvexHull(m_rawPoints);
    if (m_convexHull.size() < 3) {
        m_stage = Stage::CollectPolygon;
        emit classificationChanged(plane_geometry::PointClassification::Outside,
                                   tr("Convex hull is degenerate, adjust points"));
        return;
    }

    m_stage = Stage::PolygonReady;
    emit polygonFinalized(convexHull());
}

QPointF PointLocationWidget::eventPosition(const QMouseEvent* event) const {
    return event->position();
}

void PointLocationWidget::evaluateTestPoint() {
    if (!polygonReady() || !m_hasTestPoint) {
        m_displayState = DisplayState::Unknown;
        emit classificationChanged(plane_geometry::PointClassification::Outside,
                                   tr("Define polygon and test point"));
        return;
    }

    std::vector<plane_geometry::Point2D<double>> polygon;
    polygon.reserve(m_convexHull.size());
    for (const QPointF& point : m_convexHull) {
        polygon.push_back({point.x(), point.y()});
    }

    const plane_geometry::Point2D<double> testPoint{m_testPoint.x(), m_testPoint.y()};
    const auto classification = plane_geometry::locatePointInConvexPolygon(
        polygon, testPoint);

    const double delta = computeDelta();
    const double distance = distanceToPolygonBoundary(m_testPoint);

    if (classification == plane_geometry::PointClassification::OnBoundary) {
        m_displayState = DisplayState::OnBoundary;
    } else if (distance < delta) {
        m_displayState = DisplayState::NearBoundary;
    } else if (classification == plane_geometry::PointClassification::Inside) {
        m_displayState = DisplayState::Inside;
    } else {
        m_displayState = DisplayState::Outside;
    }

    m_lastClassification = classification;
    emit testPointChanged(m_testPoint);
    emit classificationChanged(classification,
                               describeClassification(classification, m_displayState,
                                                      distance, delta));
}

double PointLocationWidget::computeDelta() const {
    if (m_convexHull.size() < 2) {
        return 0.0;
    }

    double minimal = std::numeric_limits<double>::max();
    for (int i = 0; i < m_convexHull.size(); ++i) {
        const QPointF& a = m_convexHull[i];
        const QPointF& b = m_convexHull[(i + 1) % m_convexHull.size()];
        const double length = std::sqrt(QPointF::dotProduct(b - a, b - a));
        minimal = std::min(minimal, length);
    }

    if (minimal <= std::numeric_limits<double>::epsilon()) {
        return 0.0;
    }
    return minimal / 10.0;
}

double PointLocationWidget::distanceToPolygonBoundary(const QPointF& position) const {
    if (m_convexHull.size() < 2) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double minDistance = std::numeric_limits<double>::max();
    for (int i = 0; i < m_convexHull.size(); ++i) {
        const QPointF& a = m_convexHull[i];
        const QPointF& b = m_convexHull[(i + 1) % m_convexHull.size()];
        minDistance = std::min(minDistance, distancePointToSegment(position, a, b));
    }
    return minDistance;
}
QString PointLocationWidget::describeClassification(plane_geometry::PointClassification classification,
                                                   DisplayState state,
                                                   double distance,
                                                   double delta) {
    QString base;
    switch (classification) {
    case plane_geometry::PointClassification::Inside:
        base = tr("Point is inside the polygon");
        break;
    case plane_geometry::PointClassification::Outside:
        base = tr("Point is outside the polygon");
        break;
    case plane_geometry::PointClassification::OnBoundary:
        base = tr("Point lies on the boundary");
        break;
    }

    if (state == DisplayState::NearBoundary) {
        base += tr(" (near boundary, distance %1, delta %2)")
                    .arg(QString::number(distance, 'f', 3))
                    .arg(QString::number(delta, 'f', 3));
    }

    return base;
}
