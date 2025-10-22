#include "PlaneOperations.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <map>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

namespace plane_geometry {
namespace detail {

constexpr double Epsilon = 1e-9;

template <typename T>
inline auto absValue(const T& value) {
    using std::abs;
    using boost::multiprecision::abs;
    return abs(value);
}

template <typename Scalar>
inline Scalar sqrtValue(const Scalar& value) {
    using std::sqrt;
    using boost::multiprecision::sqrt;
    return sqrt(value);
}

template <typename Scalar>
Point2D<Scalar> subtract(const Point2D<Scalar>& lhs, const Point2D<Scalar>& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename Scalar>
Scalar cross(const Point2D<Scalar>& a, const Point2D<Scalar>& b) {
    return a.x * b.y - a.y * b.x;
}

template <typename Scalar>
Scalar dot(const Point2D<Scalar>& a, const Point2D<Scalar>& b) {
    return a.x * b.x + a.y * b.y;
}

template <typename Scalar>
Scalar squaredLength(const Point2D<Scalar>& v) {
    return dot(v, v);
}

template <typename Scalar>
bool nearlyZero(const Scalar& value, const Scalar& tolerance) {
    return absValue<Scalar>(value) <= tolerance;
}

template <typename Scalar>
Scalar crossTolerance(const Scalar& epsilon,
                      const Scalar& lhsSquaredLength,
                      const Scalar& rhsSquaredLength) {
    return epsilon * (lhsSquaredLength + rhsSquaredLength + Scalar{1});
}

template <typename Scalar>
Scalar dotTolerance(const Scalar& epsilon,
                    const Scalar& lhsSquaredLength,
                    const Scalar& rhsSquaredLength,
                    const Scalar& dotValue) {
    return epsilon * (lhsSquaredLength + rhsSquaredLength + absValue<Scalar>(dotValue) + Scalar{1});
}

template <typename Scalar>
inline bool pointsEqual(const Point2D<Scalar>& lhs,
                        const Point2D<Scalar>& rhs,
                        const Scalar& eps = defaultEpsilon<Scalar>()) {
    return absValue(lhs.x - rhs.x) <= eps && absValue(lhs.y - rhs.y) <= eps;
}

template <typename Scalar>
inline bool lexLess(const Point2D<Scalar>& lhs,
                    const Point2D<Scalar>& rhs,
                    Scalar eps = Epsilon) {
    if (absValue(lhs.x - rhs.x) > eps) {
        return lhs.x < rhs.x;
    }
    if (absValue(lhs.y - rhs.y) > eps) {
        return lhs.y < rhs.y;
    }
    return false;
}

template <typename Scalar>
inline Scalar orientationDet(const Point2D<Scalar>& a,
                             const Point2D<Scalar>& b,
                             const Point2D<Scalar>& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename Scalar>
inline Scalar signedArea(const Polygon<Scalar>& polygon) {
    Scalar area = Scalar{};
    const std::size_t n = polygon.size();
    for (std::size_t i = 0; i < n; ++i) {
        const auto& current = polygon[i];
        const auto& next = polygon[(i + 1) % n];
        area += current.x * next.y - next.x * current.y;
    }
    return area * Scalar{0.5};
}

template <typename Scalar>
Polygon<Scalar> removeDuplicateVertices(const Polygon<Scalar>& polygon,
                                        const Scalar& eps) {
    Polygon<Scalar> result;
    result.reserve(polygon.size());
    for (const auto& point : polygon) {
        if (result.empty() || !pointsEqual(point, result.back(), eps)) {
            result.push_back(point);
        }
    }
    if (result.size() >= 2 && pointsEqual(result.front(), result.back(), eps)) {
        result.pop_back();
    }
    return result;
}

template <typename Scalar>
Polygon<Scalar> removeColinearSpikes(const Polygon<Scalar>& polygon,
                                     const Scalar& eps) {
    if (polygon.size() <= 2) {
        return polygon;
    }

    Polygon<Scalar> cleaned;
    cleaned.reserve(polygon.size());
    const std::size_t n = polygon.size();

    for (std::size_t i = 0; i < n; ++i) {
        const auto& prev = polygon[(i + n - 1) % n];
        const auto& current = polygon[i];
        const auto& next = polygon[(i + 1) % n];
        const Scalar orient = orientationDet(prev, current, next);
        if (absValue(orient) <= eps) {
            const auto v1 = subtract(current, prev);
            const auto v2 = subtract(next, current);
            if (dot(v1, v2) > Scalar{}) {
                continue;
            }
        }
        if (cleaned.empty() || !pointsEqual(cleaned.back(), current, eps)) {
            cleaned.push_back(current);
        }
    }

    if (cleaned.size() >= 2 && pointsEqual(cleaned.front(), cleaned.back(), eps)) {
        cleaned.pop_back();
    }

    return cleaned;
}

template <typename Scalar>
Polygon<Scalar> cleanupPolygon(Polygon<Scalar> polygon, const Scalar& eps) {
    polygon = removeDuplicateVertices(polygon, eps);
    if (polygon.size() < 3) {
        return polygon;
    }

    polygon = removeColinearSpikes(polygon, eps);
    if (polygon.size() < 3) {
        return {};
    }

    Scalar area = signedArea(polygon);
    if (absValue(area) <= eps) {
        return {};
    }
    if (area < Scalar{}) {
        std::reverse(polygon.begin(), polygon.end());
    }

    polygon = removeDuplicateVertices(polygon, eps);
    if (polygon.size() < 3) {
        return {};
    }

    return polygon;
}

template <typename Scalar>
Polygon<Scalar> normalizePolygon(const Polygon<Scalar>& polygon, const Scalar& eps) {
    Polygon<Scalar> normalized = removeDuplicateVertices(polygon, eps);
    if (normalized.size() < 3) {
        return normalized;
    }
    if (signedArea(normalized) < Scalar{}) {
        std::reverse(normalized.begin(), normalized.end());
    }
    return normalized;
}

template <typename Scalar>
bool polygonContainsPoint(const Polygon<Scalar>& polygon,
                          const Point2D<Scalar>& point,
                          const Scalar& eps) {
    const std::size_t n = polygon.size();
    if (n < 3) {
        return false;
    }
    for (std::size_t i = 0; i < n; ++i) {
        if (orientationDet(polygon[i], polygon[(i + 1) % n], point) < -eps) {
            return false;
        }
    }
    return true;
}

template <typename Scalar>
bool polygonContainsPolygon(const Polygon<Scalar>& outer,
                            const Polygon<Scalar>& inner,
                            const Scalar& eps) {
    if (inner.empty()) {
        return true;
    }
    for (const auto& point : inner) {
        if (!polygonContainsPoint(outer, point, eps)) {
            return false;
        }
    }
    return true;
}

template <typename Scalar>
bool polygonsDoNotOverlap(const Polygon<Scalar>& A,
                          const Polygon<Scalar>& B,
                          const Scalar& eps) {
    for (const auto& point : A) {
        if (polygonContainsPoint(B, point, eps)) {
            return false;
        }
    }
    for (const auto& point : B) {
        if (polygonContainsPoint(A, point, eps)) {
            return false;
        }
    }

    const std::size_t nA = A.size();
    const std::size_t nB = B.size();
    for (std::size_t i = 0; i < nA; ++i) {
        const Segment2D<Scalar> edgeA{A[i], A[(i + 1) % nA]};
        for (std::size_t j = 0; j < nB; ++j) {
            const Segment2D<Scalar> edgeB{B[j], B[(j + 1) % nB]};
            const auto result = intersectSegments(edgeA, edgeB, eps);
            if (result.type != IntersectionType::None) {
                return false;
            }
        }
    }
    return true;
}

template <typename Scalar>
bool pointOnSegment(const Point2D<Scalar>& point,
                    const Point2D<Scalar>& start,
                    const Point2D<Scalar>& end,
                    const Scalar& eps) {
    const auto segment = subtract(end, start);
    const Scalar lengthSquared = squaredLength(segment);
    if (lengthSquared <= eps * eps) {
        return pointsEqual(point, start, eps);
    }
    const auto toPoint = subtract(point, start);
    const Scalar crossValue = cross(segment, toPoint);
    if (absValue(crossValue) > eps * (lengthSquared + Scalar{1})) {
        return false;
    }
    const Scalar projection = dot(toPoint, segment);
    if (projection < -eps || projection > lengthSquared + eps) {
        return false;
    }
    return true;
}

template <typename Scalar>
inline bool isInsideHalfPlane(const Point2D<Scalar>& edgeStart,
                              const Point2D<Scalar>& edgeEnd,
                              const Point2D<Scalar>& point,
                              bool keepInside,
                              const Scalar& eps) {
    const Scalar orient = orientationDet(edgeStart, edgeEnd, point);
    if (keepInside) {
        return orient >= -eps;
    }
    return orient <= eps;
}

template <typename Scalar>
bool computeIntersectionPoint(const Point2D<Scalar>& p0,
                              const Point2D<Scalar>& p1,
                              const Point2D<Scalar>& q0,
                              const Point2D<Scalar>& q1,
                              const Scalar& eps,
                              Point2D<Scalar>& intersection) {
    const auto r = subtract(p1, p0);
    const auto s = subtract(q1, q0);
    const Scalar denom = cross(r, s);
    if (absValue(denom) <= eps) {
        return false;
    }
    const auto qp = subtract(q0, p0);
    const Scalar t = cross(qp, s) / denom;
    if (t < -eps || t > Scalar{1} + eps) {
        return false;
    }
    const Scalar u = cross(qp, r) / denom;
    if (u < -eps || u > Scalar{1} + eps) {
        return false;
    }
    intersection = {p0.x + t * r.x, p0.y + t * r.y};
    return true;
}

template <typename Scalar>
void appendUniquePoint(std::vector<Point2D<Scalar>>& container,
                       const Point2D<Scalar>& point,
                       const Scalar& eps) {
    for (const auto& existing : container) {
        if (pointsEqual(existing, point, eps)) {
            return;
        }
    }
    container.push_back(point);
}

template <typename Scalar>
Polygon<Scalar> convexHullFromPoints(std::vector<Point2D<Scalar>> points,
                                     const Scalar& eps) {
    if (points.size() <= 1) {
        return points;
    }
    std::sort(points.begin(), points.end(),
              [&](const auto& lhs, const auto& rhs) {
                  if (absValue(lhs.x - rhs.x) > eps) {
                      return lhs.x < rhs.x;
                  }
                  if (absValue(lhs.y - rhs.y) > eps) {
                      return lhs.y < rhs.y;
                  }
                  return false;
              });
    points.erase(std::unique(points.begin(), points.end(),
                             [&](const auto& lhs, const auto& rhs) {
                                 return pointsEqual(lhs, rhs, eps);
                             }),
                 points.end());
    if (points.size() <= 2) {
        return points;
    }
    std::vector<Point2D<Scalar>> lower;
    lower.reserve(points.size());
    for (const auto& point : points) {
        while (lower.size() >= 2 && orientationDet(lower[lower.size() - 2], lower.back(), point) <= eps) {
            lower.pop_back();
        }
        lower.push_back(point);
    }
    std::vector<Point2D<Scalar>> upper;
    upper.reserve(points.size());
    for (auto it = points.rbegin(); it != points.rend(); ++it) {
        while (upper.size() >= 2 && orientationDet(upper[upper.size() - 2], upper.back(), *it) <= eps) {
            upper.pop_back();
        }
        upper.push_back(*it);
    }
    lower.pop_back();
    upper.pop_back();
    std::vector<Point2D<Scalar>> hull;
    hull.reserve(lower.size() + upper.size());
    hull.insert(hull.end(), lower.begin(), lower.end());
    hull.insert(hull.end(), upper.begin(), upper.end());
    return cleanupPolygon(hull, eps);
}

template <typename Scalar>
struct IntersectionInfo {
    Point2D<Scalar> point;
    std::size_t edgeIndexA{};
    Scalar parameterA{};
    std::size_t edgeIndexB{};
    Scalar parameterB{};
};

template <typename Scalar>
std::vector<IntersectionInfo<Scalar>> collectIntersections(const Polygon<Scalar>& A,
                                                           const Polygon<Scalar>& B,
                                                           const Scalar& eps) {
    std::vector<IntersectionInfo<Scalar>> intersections;
    const std::size_t nA = A.size();
    const std::size_t nB = B.size();

    for (std::size_t i = 0; i < nA; ++i) {
        const Segment2D<Scalar> edgeA{A[i], A[(i + 1) % nA]};
        for (std::size_t j = 0; j < nB; ++j) {
            const Segment2D<Scalar> edgeB{B[j], B[(j + 1) % nB]};
            const auto result = intersectSegments(edgeA, edgeB, eps);
            if (result.type == IntersectionType::None) {
                continue;
            }
            auto addPoint = [&](const Point2D<Scalar>& point) {
                for (const auto& existing : intersections) {
                    if (pointsEqual(existing.point, point, eps)) {
                        return;
                    }
                }
                IntersectionInfo<Scalar> info;
                info.point = point;
                info.edgeIndexA = i;
                info.edgeIndexB = j;
                const auto computeParam = [&](const Segment2D<Scalar>& edge) {
                    const auto edgeVec = subtract(edge.end, edge.start);
                    const Scalar lengthSquared = squaredLength(edgeVec);
                    if (lengthSquared <= eps * eps) {
                        return Scalar{};
                    }
                    const auto toPoint = subtract(point, edge.start);
                    return dot(toPoint, edgeVec) / lengthSquared;
                };
                info.parameterA = computeParam(edgeA);
                info.parameterB = computeParam(edgeB);
                if (info.parameterA < -eps || info.parameterA > Scalar{1} + eps ||
                    info.parameterB < -eps || info.parameterB > Scalar{1} + eps) {
                    return;
                }
                intersections.push_back(info);
            };

            if (result.type == IntersectionType::Point) {
                addPoint(result.point);
            } else if (result.type == IntersectionType::Overlap) {
                addPoint(result.overlap.start);
                addPoint(result.overlap.end);
            }
        }
    }

    return intersections;
}

template <typename Scalar>
std::vector<Point2D<Scalar>> collectIntersectionPoints(const Polygon<Scalar>& A,
                                                       const Polygon<Scalar>& B,
                                                       const Scalar& eps) {
    std::vector<Point2D<Scalar>> points;
    const auto info = collectIntersections(A, B, eps);
    for (const auto& item : info) {
        appendUniquePoint(points, item.point, eps);
    }
    return points;
}

template <typename Scalar>
struct EdgeKey {
    Point2D<Scalar> u;
    Point2D<Scalar> v;

    bool operator<(const EdgeKey& other) const {
        if (lexLess(u, other.u)) {
            return true;
        }
        if (lexLess(other.u, u)) {
            return false;
        }
        return lexLess(v, other.v);
    }
};

template <typename Scalar>
EdgeKey<Scalar> makeEdge(const Point2D<Scalar>& a, const Point2D<Scalar>& b) {
    EdgeKey<Scalar> edge{a, b};
    if (lexLess(edge.v, edge.u)) {
        std::swap(edge.u, edge.v);
    }
    return edge;
}

template <typename Scalar>
std::array<Point2D<Scalar>, 3> makeSuperTriangle(const std::vector<Point2D<Scalar>>& points) {
    Scalar minX = points.front().x;
    Scalar maxX = points.front().x;
    Scalar minY = points.front().y;
    Scalar maxY = points.front().y;

    for (const auto& point : points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }

    const Scalar dx = maxX - minX;
    const Scalar dy = maxY - minY;
    const Scalar delta = std::max(dx, dy);
    const Scalar midX = (minX + maxX) / Scalar{2};
    const Scalar midY = (minY + maxY) / Scalar{2};
    const Scalar radius = std::max(delta, Scalar{1}) * Scalar{20};

    return {Point2D<Scalar>{midX - radius, midY - radius},
            Point2D<Scalar>{midX, midY + radius},
            Point2D<Scalar>{midX + radius, midY - radius}};
}

template <typename Scalar>
bool isPointInsideCircumcircle(const Triangle2D<Scalar>& triangle,
                               const Point2D<Scalar>& point) {
    const Scalar ax = triangle.a.x - point.x;
    const Scalar ay = triangle.a.y - point.y;
    const Scalar bx = triangle.b.x - point.x;
    const Scalar by = triangle.b.y - point.y;
    const Scalar cx = triangle.c.x - point.x;
    const Scalar cy = triangle.c.y - point.y;

    const Scalar det = (ax * ax + ay * ay) * (bx * cy - cx * by)
                     - (bx * bx + by * by) * (ax * cy - cx * ay)
                     + (cx * cx + cy * cy) * (ax * by - bx * ay);

    const Scalar orient = orientationDet(triangle.a, triangle.b, triangle.c);
    if (orient > Scalar{}) {
        return det > Scalar{Epsilon};
    }
    return det < -Scalar{Epsilon};
}

}  // namespace detail

namespace {

template <typename Scalar>
Point2D<Scalar> polygonCentroid(const Polygon<Scalar>& polygon) {
    Point2D<Scalar> centroid{};
    if (polygon.empty()) {
        return centroid;
    }
    const Scalar count = static_cast<Scalar>(polygon.size());
    for (const auto& point : polygon) {
        centroid.x += point.x;
        centroid.y += point.y;
    }
    centroid.x /= count;
    centroid.y /= count;
    return centroid;
}

template <typename Scalar>
Point2D<Scalar> polygonCentroidFromBoundary(const std::vector<Point2D<Scalar>>& points) {
    Point2D<Scalar> centroid{};
    if (points.empty()) {
        return centroid;
    }
    const Scalar count = static_cast<Scalar>(points.size());
    for (const auto& point : points) {
        centroid.x += point.x;
        centroid.y += point.y;
    }
    centroid.x /= count;
    centroid.y /= count;
    return centroid;
}

}  // namespace

template <typename Scalar>
Orientation classifyPointRelativeToSegment(const Segment2D<Scalar>& segment,
                                           const Point2D<Scalar>& point,
                                           const Scalar& epsilon) {
    const auto ab = detail::subtract(segment.end, segment.start);
    const auto ap = detail::subtract(point, segment.start);
    const Scalar abLengthSquared = detail::squaredLength(ab);
    if (abLengthSquared <= epsilon * epsilon) {
        const Scalar distanceSquared = detail::squaredLength(ap);
        return distanceSquared <= epsilon * epsilon ? Orientation::OnSegment : Orientation::Right;
    }

    const Scalar crossValue = detail::cross(ab, ap);
    const Scalar crossAllowed = detail::crossTolerance(epsilon, abLengthSquared, detail::squaredLength(ap));
    if (detail::absValue(crossValue) <= crossAllowed) {
        const Scalar dotValue = detail::dot(ab, ap);
        const Scalar dotAllowed = detail::dotTolerance(epsilon, abLengthSquared, detail::squaredLength(ap), dotValue);
        if (dotValue >= -dotAllowed && dotValue <= abLengthSquared + dotAllowed) {
            return Orientation::OnSegment;
        }
        return dotValue < Scalar{} ? Orientation::Right : Orientation::Left;
    }

    return crossValue > Scalar{} ? Orientation::Left : Orientation::Right;
}

template <typename Scalar>
SegmentIntersectionResult<Scalar> intersectSegments(const Segment2D<Scalar>& first,
                                                    const Segment2D<Scalar>& second,
                                                    const Scalar& epsilon) {
    SegmentIntersectionResult<Scalar> result{};

    const auto p = first.start;
    const auto r = detail::subtract(first.end, first.start);
    const auto q = second.start;
    const auto s = detail::subtract(second.end, second.start);
    const auto qp = detail::subtract(q, p);

    const Scalar rLengthSquared = detail::squaredLength(r);
    const Scalar sLengthSquared = detail::squaredLength(s);
    const Scalar qpLengthSquared = detail::squaredLength(qp);

    const Scalar parallelTolerance = detail::crossTolerance(epsilon, rLengthSquared, sLengthSquared);
    const Scalar qpxrTolerance = detail::crossTolerance(epsilon, rLengthSquared, qpLengthSquared);

    const Scalar rxs = detail::cross(r, s);
    const Scalar qpxr = detail::cross(qp, r);

    const Scalar epsilonSquared = epsilon * epsilon;

    if (detail::nearlyZero(rxs, parallelTolerance) && detail::nearlyZero(qpxr, qpxrTolerance)) {
        if (rLengthSquared <= epsilonSquared && sLengthSquared <= epsilonSquared) {
            if (detail::squaredLength(detail::subtract(first.start, second.start)) <= epsilonSquared) {
                result.type = IntersectionType::Point;
                result.point = first.start;
            }
            return result;
        }

        if (rLengthSquared <= epsilonSquared) {
            const Orientation orientation = classifyPointRelativeToSegment(second, first.start, epsilon);
            if (orientation == Orientation::OnSegment) {
                result.type = IntersectionType::Point;
                result.point = first.start;
            }
            return result;
        }

        if (sLengthSquared <= epsilonSquared) {
            const Orientation orientation = classifyPointRelativeToSegment(first, second.start, epsilon);
            if (orientation == Orientation::OnSegment) {
                result.type = IntersectionType::Point;
                result.point = second.start;
            }
            return result;
        }

        const Scalar rDotr = rLengthSquared;
        const Scalar t0 = detail::dot(qp, r) / rDotr;
        const Scalar t1 = t0 + detail::dot(s, r) / rDotr;

        const Scalar tMin = std::min(t0, t1);
        const Scalar tMax = std::max(t0, t1);

        const Scalar parameterTolerance =
            epsilon * (detail::sqrtValue(rLengthSquared) + detail::sqrtValue(sLengthSquared) + Scalar{1});

        if (tMin > Scalar{1} + parameterTolerance || tMax < Scalar{} - parameterTolerance) {
            return result;
        }

        const Scalar tClampedStart = std::max(Scalar{}, tMin);
        const Scalar tClampedEnd = std::min(Scalar{1}, tMax);

        if (detail::absValue(tClampedStart - tClampedEnd) <= parameterTolerance) {
            result.type = IntersectionType::Point;
            result.point = {p.x + tClampedStart * r.x, p.y + tClampedStart * r.y};
            return result;
        }

        result.type = IntersectionType::Overlap;
        result.overlap.start = {p.x + tClampedStart * r.x, p.y + tClampedStart * r.y};
        result.overlap.end = {p.x + tClampedEnd * r.x, p.y + tClampedEnd * r.y};
        return result;
    }

    if (detail::nearlyZero(rxs, parallelTolerance)) {
        return result;
    }

    const Scalar t = detail::cross(qp, s) / rxs;
    const Scalar u = detail::cross(qp, r) / rxs;
    const Scalar parameterTolerance =
        epsilon * (detail::sqrtValue(rLengthSquared) + detail::sqrtValue(sLengthSquared) + Scalar{1});

    if (t >= -parameterTolerance && t <= Scalar{1} + parameterTolerance &&
        u >= -parameterTolerance && u <= Scalar{1} + parameterTolerance) {
        result.type = IntersectionType::Point;
        result.point = {p.x + t * r.x, p.y + t * r.y};
    }

    return result;
}

template <typename Scalar>
std::vector<Point2D<Scalar>> computeConvexHull(const std::vector<Point2D<Scalar>>& points) {
    return detail::convexHullFromPoints(points, defaultEpsilon<Scalar>());
}

template <typename Scalar>
std::vector<Triangle2D<Scalar>> delaunayTriangulation(const std::vector<Point2D<Scalar>>& points) {
    if (points.size() < 3) {
        return {};
    }

    std::vector<Point2D<Scalar>> uniquePoints = points;
    std::sort(uniquePoints.begin(), uniquePoints.end(), [](const auto& lhs, const auto& rhs) {
        if (detail::lexLess(lhs, rhs)) {
            return true;
        }
        if (detail::lexLess(rhs, lhs)) {
            return false;
        }
        return false;
    });
    uniquePoints.erase(std::unique(uniquePoints.begin(), uniquePoints.end(), [](const auto& lhs, const auto& rhs) {
                       return detail::pointsEqual(lhs, rhs);
                   }),
                   uniquePoints.end());

    if (uniquePoints.size() < 3) {
        return {};
    }

    std::mt19937 generator{std::random_device{}()};
    std::shuffle(uniquePoints.begin(), uniquePoints.end(), generator);

    const auto superTriangle = detail::makeSuperTriangle(uniquePoints);

    std::vector<Triangle2D<Scalar>> triangulation;
    triangulation.push_back({superTriangle[0], superTriangle[1], superTriangle[2]});

    for (const auto& point : uniquePoints) {
        std::vector<Triangle2D<Scalar>> badTriangles;
        std::vector<Triangle2D<Scalar>> goodTriangles;
        badTriangles.reserve(triangulation.size());
        goodTriangles.reserve(triangulation.size());

        for (const auto& triangle : triangulation) {
            if (detail::isPointInsideCircumcircle(triangle, point)) {
                badTriangles.push_back(triangle);
            } else {
                goodTriangles.push_back(triangle);
            }
        }

        std::map<detail::EdgeKey<Scalar>, int> edgeCounter;
        for (const auto& triangle : badTriangles) {
            edgeCounter[detail::makeEdge(triangle.a, triangle.b)]++;
            edgeCounter[detail::makeEdge(triangle.b, triangle.c)]++;
            edgeCounter[detail::makeEdge(triangle.c, triangle.a)]++;
        }

        triangulation = std::move(goodTriangles);

        for (const auto& [edge, count] : edgeCounter) {
            if (count != 1) {
                continue;
            }
            Triangle2D<Scalar> newTriangle{edge.u, edge.v, point};
            if (detail::orientationDet(newTriangle.a, newTriangle.b, newTriangle.c) < Scalar{}) {
                std::swap(newTriangle.b, newTriangle.c);
            }
            triangulation.push_back(newTriangle);
        }
    }

    const auto isSuperVertex = [&](const Point2D<Scalar>& vertex) {
        return detail::pointsEqual(vertex, superTriangle[0]) ||
               detail::pointsEqual(vertex, superTriangle[1]) ||
               detail::pointsEqual(vertex, superTriangle[2]);
    };

    triangulation.erase(std::remove_if(triangulation.begin(), triangulation.end(),
                                       [&](const Triangle2D<Scalar>& triangle) {
                                           return isSuperVertex(triangle.a) ||
                                                  isSuperVertex(triangle.b) ||
                                                  isSuperVertex(triangle.c);
                                       }),
                        triangulation.end());

    return triangulation;
}

template <typename Scalar>
Polygon<Scalar> intersectConvexPolygons(const Polygon<Scalar>& polyA,
                                        const Polygon<Scalar>& polyB,
                                        const Scalar& epsilon) {
    Polygon<Scalar> normalizedA = detail::normalizePolygon(polyA, epsilon);
    Polygon<Scalar> normalizedB = detail::normalizePolygon(polyB, epsilon);

    if (normalizedA.empty() || normalizedB.empty()) {
        return {};
    }

    std::vector<Point2D<Scalar>> candidates;
    candidates.reserve(normalizedA.size() + normalizedB.size());

    for (const auto& point : normalizedA) {
        if (detail::polygonContainsPoint(normalizedB, point, epsilon)) {
            detail::appendUniquePoint(candidates, point, epsilon);
        }
    }
    for (const auto& point : normalizedB) {
        if (detail::polygonContainsPoint(normalizedA, point, epsilon)) {
            detail::appendUniquePoint(candidates, point, epsilon);
        }
    }

    const auto intersections = detail::collectIntersections(normalizedA, normalizedB, epsilon);
    for (const auto& info : intersections) {
        detail::appendUniquePoint(candidates, info.point, epsilon);
    }

    if (candidates.size() < 3) {
        return {};
    }

    auto hull = detail::convexHullFromPoints(std::move(candidates), epsilon);
    return detail::cleanupPolygon(hull, epsilon);
}

template <typename Scalar>
std::vector<Polygon<Scalar>> buildLoopsFromSegments(
    const std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>>& segments,
    const Scalar& epsilon)
{
    if (segments.empty()) return {};

    std::vector<Point2D<Scalar>> uniquePoints;
    uniquePoints.reserve(segments.size() * 2);
    std::vector<std::vector<std::size_t>> adjacency;

    struct SegmentRecord {
        std::size_t start{};
        std::size_t end{};
        double angle{};
        bool used{false};
    };

    constexpr double TwoPi = 6.283185307179586476925286766559005768394;
    constexpr double AngleEps = 1e-12;

    auto toDouble = [](const Scalar& v) -> double { return static_cast<double>(v); };

    std::vector<SegmentRecord> records;
    records.reserve(segments.size());

    auto indexOf = [&](const Point2D<Scalar>& p) -> std::size_t {
        for (std::size_t i = 0; i < uniquePoints.size(); ++i)
            if (detail::pointsEqual(uniquePoints[i], p, epsilon)) return i;
        uniquePoints.push_back(p);
        adjacency.resize(uniquePoints.size());
        return uniquePoints.size() - 1;
    };

    for (const auto& seg : segments) {
        if (detail::pointsEqual(seg.first, seg.second, epsilon)) continue;
        const std::size_t s = indexOf(seg.first);
        const std::size_t e = indexOf(seg.second);
        const auto dir = detail::subtract(uniquePoints[e], uniquePoints[s]);
        double angle = std::atan2(toDouble(dir.y), toDouble(dir.x));
        if (angle < 0.0) angle += TwoPi;
        records.push_back({s, e, angle, false});
        adjacency[s].push_back(records.size() - 1);
    }

    if (records.empty()) return {};

    for (auto& neigh : adjacency) {
        std::sort(neigh.begin(), neigh.end(),
                  [&](std::size_t L, std::size_t R){ return records[L].angle < records[R].angle; });
    }

    std::vector<Polygon<Scalar>> loops;

    for (std::size_t i = 0; i < records.size(); ++i) {
        if (records[i].used) continue;

        Polygon<Scalar> loop;
        std::vector<std::size_t> path;
        std::size_t cur = i;
        double prevAngle = records[cur].angle;
        bool closed = false;

        while (true) {
            auto& rec = records[cur];
            rec.used = true;
            path.push_back(cur);

            const auto& sP = uniquePoints[rec.start];
            const auto& eP = uniquePoints[rec.end];

            if (loop.empty() || !detail::pointsEqual(loop.back(), sP, epsilon)) loop.push_back(sP);
            if (!detail::pointsEqual(loop.back(), eP, epsilon)) loop.push_back(eP);

            if (detail::pointsEqual(eP, loop.front(), epsilon)) { closed = true; break; }

            const auto& out = adjacency[rec.end];
            std::size_t next = std::numeric_limits<std::size_t>::max();
            double bestDelta = std::numeric_limits<double>::infinity();

            for (const auto cand : out) {
                if (records[cand].used) continue;
                double delta = records[cand].angle - prevAngle;
                while (delta <= AngleEps) delta += TwoPi;
                if (delta < bestDelta) { bestDelta = delta; next = cand; }
            }

            if (next == std::numeric_limits<std::size_t>::max()) break;

            prevAngle = records[next].angle;
            cur = next;
        }

        if (!closed) {
            for (const auto idx : path) records[idx].used = false;
            continue;
        }

        if (loop.size() >= 3) {
            loop = detail::cleanupPolygon(loop, epsilon);
            if (!loop.empty()) loops.push_back(std::move(loop));
        }
    }

    return loops;
}

template <typename Scalar>
Polygon<Scalar> buildPolygonFromSegments(const std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>>& segments,
                                         const Scalar& epsilon) {
    auto loops = buildLoopsFromSegments(segments, epsilon);
    if (loops.empty()) return {};

    Polygon<Scalar> best = loops.front();
    Scalar bestArea = detail::absValue(detail::signedArea(best));
    for (const auto& L : loops) {
        const Scalar a = detail::absValue(detail::signedArea(L));
        if (a > bestArea) { bestArea = a; best = L; }
    }
    return detail::cleanupPolygon(best, epsilon);
}

template <typename Scalar>
BooleanResult<Scalar> unionConvexPolygons(const Polygon<Scalar>& polyA,
                                    const Polygon<Scalar>& polyB,
                                    const Scalar& epsilon) {
    BooleanResult<Scalar> R;
    Polygon<Scalar> normalizedA = detail::normalizePolygon(polyA, epsilon);
    Polygon<Scalar> normalizedB = detail::normalizePolygon(polyB, epsilon);

    if (normalizedA.empty() && normalizedB.empty()) return R;
    if (normalizedA.empty()) {
        R.outers.push_back(detail::cleanupPolygon(normalizedB, epsilon));
        return R;
    }
    if (normalizedB.empty()) {
        R.outers.push_back(detail::cleanupPolygon(normalizedA, epsilon));
        return R;
    }
    if (detail::polygonContainsPolygon(normalizedA, normalizedB, epsilon)) {
        R.outers.push_back(normalizedA);
        return R;
    }
    if (detail::polygonContainsPolygon(normalizedB, normalizedA, epsilon)) {
        R.outers.push_back(normalizedB);
        return R;
    }
    if (detail::polygonsDoNotOverlap(normalizedA, normalizedB, epsilon)) {
        R.outers.push_back(normalizedA);
        R.outers.push_back(normalizedB);
        return R;
    }

    const auto intersections = detail::collectIntersections(normalizedA, normalizedB, epsilon);

    auto buildSegments = [&](const Polygon<Scalar>& polygon, bool useFirst) {
        std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> segments;
        if (polygon.empty()) {
            return segments;
        }
        std::vector<std::vector<std::pair<Scalar, Point2D<Scalar>>>> edgePoints(polygon.size());
        for (std::size_t i = 0; i < polygon.size(); ++i) {
            edgePoints[i].push_back({Scalar{}, polygon[i]});
            edgePoints[i].push_back({Scalar{1}, polygon[(i + 1) % polygon.size()]});
        }
        for (const auto& info : intersections) {
            std::size_t edgeIndex = useFirst ? info.edgeIndexA : info.edgeIndexB;
            if (edgeIndex >= polygon.size()) {
                continue;
            }
            Scalar param = useFirst ? info.parameterA : info.parameterB;
            if (param < Scalar{}) {
                param = Scalar{};
            }
            if (param > Scalar{1}) {
                param = Scalar{1};
            }
            edgePoints[edgeIndex].push_back({param, info.point});
        }
        for (auto& edge : edgePoints) {
            std::sort(edge.begin(), edge.end(), [](const auto& lhs, const auto& rhs) {
                return lhs.first < rhs.first;
            });
            std::vector<std::pair<Scalar, Point2D<Scalar>>> unique;
            for (const auto& entry : edge) {
                if (!unique.empty() && detail::pointsEqual(unique.back().second, entry.second, epsilon)) {
                    continue;
                }
                unique.push_back(entry);
            }
            for (std::size_t i = 0; i + 1 < unique.size(); ++i) {
                const auto& start = unique[i].second;
                const auto& end = unique[i + 1].second;
                if (!detail::pointsEqual(start, end, epsilon)) {
                    segments.emplace_back(start, end);
                }
            }
        }
        return segments;
    };

    auto collectSegments = [&](const Polygon<Scalar>& first,
                               const Polygon<Scalar>& second,
                               bool useFirst,
                               bool keepOutside) {
        std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> segments = buildSegments(first, useFirst);
        std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> output;
        for (const auto& segment : segments) {
            Point2D<Scalar> midpoint{(segment.first.x + segment.second.x) / Scalar{2},
                                      (segment.first.y + segment.second.y) / Scalar{2}};
            const bool inside = detail::polygonContainsPoint(second, midpoint, epsilon);
            if ((keepOutside && !inside) || (!keepOutside && inside)) {
                output.push_back(segment);
            }
        }
        return output;
    };

    std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> boundarySegments;
    auto outsideSegmentsA = collectSegments(normalizedA, normalizedB, true, true);
    auto outsideSegmentsB = collectSegments(normalizedB, normalizedA, false, true);
    boundarySegments.insert(boundarySegments.end(), outsideSegmentsA.begin(), outsideSegmentsA.end());
    boundarySegments.insert(boundarySegments.end(), outsideSegmentsB.begin(), outsideSegmentsB.end());

    std::map<detail::EdgeKey<Scalar>, int> cnt;
    for (const auto& s : boundarySegments) {
        cnt[detail::makeEdge(s.first, s.second)]++;
    }
    std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> filtered;
    filtered.reserve(boundarySegments.size());
    for (const auto& s : boundarySegments) {
        if (cnt[detail::makeEdge(s.first, s.second)] == 1) {
            filtered.push_back(s);
        }
    }
    boundarySegments.swap(filtered);

    auto loops = buildLoopsFromSegments(boundarySegments, epsilon);
    for (auto& L : loops) {
        if (detail::signedArea(L) > Scalar{}) R.outers.push_back(std::move(L)); // только внешние
    }
    return R;
}

template <typename Scalar>
BooleanResult<Scalar> differenceConvexPolygons(const Polygon<Scalar>& polyA,
                                         const Polygon<Scalar>& polyB,
                                         const Scalar& epsilon) {
    BooleanResult<Scalar> R;
    Polygon<Scalar> normalizedA = detail::normalizePolygon(polyA, epsilon);
    Polygon<Scalar> normalizedB = detail::normalizePolygon(polyB, epsilon);

    if (normalizedA.empty()) {
        return R;
    }
    if (normalizedB.empty()) {
        R.outers.push_back(detail::cleanupPolygon(normalizedA, epsilon));
        return R;
    }
    if (detail::polygonContainsPolygon(normalizedB, normalizedA, epsilon)) {
        return R;
    }
    if (detail::polygonContainsPolygon(normalizedA, normalizedB, epsilon)) {
        R.outers.push_back(normalizedA);
        auto Bh = normalizedB; std::reverse(Bh.begin(), Bh.end()); // CW для дыр
        R.holes.push_back(std::move(Bh));
        return R;
    }
    if (detail::polygonsDoNotOverlap(normalizedA, normalizedB, epsilon)) {
        R.outers.push_back(detail::cleanupPolygon(normalizedA, epsilon));
        return R;
    }

    const auto intersections = detail::collectIntersections(normalizedA, normalizedB, epsilon);

    auto buildSegments = [&](const Polygon<Scalar>& polygon, bool useFirst) {
        std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> segments;
        if (polygon.empty()) {
            return segments;
        }
        std::vector<std::vector<std::pair<Scalar, Point2D<Scalar>>>> edgePoints(polygon.size());
        for (std::size_t i = 0; i < polygon.size(); ++i) {
            edgePoints[i].push_back({Scalar{}, polygon[i]});
            edgePoints[i].push_back({Scalar{1}, polygon[(i + 1) % polygon.size()]});
        }
        for (const auto& info : intersections) {
            std::size_t edgeIndex = useFirst ? info.edgeIndexA : info.edgeIndexB;
            if (edgeIndex >= polygon.size()) {
                continue;
            }
            Scalar param = useFirst ? info.parameterA : info.parameterB;
            if (param < Scalar{}) {
                param = Scalar{};
            }
            if (param > Scalar{1}) {
                param = Scalar{1};
            }
            edgePoints[edgeIndex].push_back({param, info.point});
        }
        for (auto& edge : edgePoints) {
            std::sort(edge.begin(), edge.end(), [](const auto& lhs, const auto& rhs) {
                return lhs.first < rhs.first;
            });
            std::vector<std::pair<Scalar, Point2D<Scalar>>> unique;
            for (const auto& entry : edge) {
                if (!unique.empty() && detail::pointsEqual(unique.back().second, entry.second, epsilon)) {
                    continue;
                }
                unique.push_back(entry);
            }
            for (std::size_t i = 0; i + 1 < unique.size(); ++i) {
                const auto& start = unique[i].second;
                const auto& end = unique[i + 1].second;
                if (!detail::pointsEqual(start, end, epsilon)) {
                    segments.emplace_back(start, end);
                }
            }
        }
        return segments;
    };

    std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> boundarySegments;

    auto segmentsA = buildSegments(normalizedA, true);
    for (const auto& segment : segmentsA) {
        Point2D<Scalar> midpoint{(segment.first.x + segment.second.x) / Scalar{2},
                                  (segment.first.y + segment.second.y) / Scalar{2}};
        if (!detail::polygonContainsPoint(normalizedB, midpoint, epsilon)) {
            boundarySegments.push_back(segment);
        }
    }

    auto segmentsB = buildSegments(normalizedB, false);
    for (const auto& segment : segmentsB) {
        Point2D<Scalar> midpoint{(segment.first.x + segment.second.x) / Scalar{2},
                                  (segment.first.y + segment.second.y) / Scalar{2}};
        if (detail::polygonContainsPoint(normalizedA, midpoint, epsilon)) {
            boundarySegments.emplace_back(segment.second, segment.first);
        }
    }

    std::map<detail::EdgeKey<Scalar>, int> cnt;
    for (const auto& s : boundarySegments) {
        cnt[detail::makeEdge(s.first, s.second)]++;
    }
    std::vector<std::pair<Point2D<Scalar>, Point2D<Scalar>>> filtered;
    filtered.reserve(boundarySegments.size());
    for (const auto& s : boundarySegments) {
        if (cnt[detail::makeEdge(s.first, s.second)] == 1) {
            filtered.push_back(s);
        }
    }
    boundarySegments.swap(filtered);
    
    auto loops = buildLoopsFromSegments(boundarySegments, epsilon);

    // Разнести петли на внешние (CCW) и дырки (CW)
    std::vector<Polygon<Scalar>> holes;
    for (auto& L : loops) {
        (detail::signedArea(L) > Scalar{}) ? R.outers.push_back(std::move(L))
                                           : holes.push_back(std::move(L));
    }

    // (опционально) привязать дырки к внешним по точке
    for (auto& H : holes) {
        for (auto& O : R.outers) {
            if (detail::polygonContainsPoint(O, H.front(), epsilon)) {
                R.holes.push_back(std::move(H));
                break;
            }
        }
    }

    return R;
}

template <typename Scalar>
PointClassification locatePointInConvexPolygon(const Polygon<Scalar>& polygon,
                                               const Point2D<Scalar>& point,
                                               const Scalar& epsilon) {
    const std::size_t vertexCount = polygon.size();
    if (vertexCount < 3) {
        throw std::invalid_argument("locatePointInConvexPolygon requires at least three vertices");
    }

    int orientationSign = 0;
    for (std::size_t i = 0; i < vertexCount; ++i) {
        const auto& current = polygon[i];
        const auto& next = polygon[(i + 1) % vertexCount];

        if (detail::pointOnSegment(point, current, next, epsilon)) {
            return PointClassification::OnBoundary;
        }

        const auto edge = detail::subtract(next, current);
        const auto toPoint = detail::subtract(point, current);
        const Scalar crossValue = detail::cross(edge, toPoint);
        const Scalar tolerance = detail::crossTolerance(epsilon,
                                                        detail::squaredLength(edge),
                                                        detail::squaredLength(toPoint));
        if (detail::absValue(crossValue) <= tolerance) {
            continue;
        }
        const int currentSign = crossValue > Scalar{} ? 1 : -1;
        if (orientationSign == 0) {
            orientationSign = currentSign;
        } else if (orientationSign != currentSign) {
            return PointClassification::Outside;
        }
    }

    return PointClassification::Inside;
}

template <typename Scalar>
PointClassification locatePointInPolygon(const Polygon<Scalar>& polygon,
                                         const Point2D<Scalar>& point,
                                         const Scalar& epsilon) {
    if (polygon.size() < 3) {
        throw std::invalid_argument("locatePointInPolygon requires at least three vertices");
    }

    for (std::size_t i = 0; i < polygon.size(); ++i) {
        if (detail::pointOnSegment(point, polygon[i], polygon[(i + 1) % polygon.size()], epsilon)) {
            return PointClassification::OnBoundary;
        }
    }

    int windingNumber = 0;
    for (std::size_t i = 0; i < polygon.size(); ++i) {
        const auto& current = polygon[i];
        const auto& next = polygon[(i + 1) % polygon.size()];
        const bool upward = current.y <= point.y && next.y > point.y;
        const bool downward = current.y > point.y && next.y <= point.y;
        if (!upward && !downward) {
            continue;
        }
        const auto edge = detail::subtract(next, current);
        const auto toPoint = detail::subtract(point, current);
        const Scalar crossValue = detail::cross(edge, toPoint);
        const Scalar tolerance = detail::crossTolerance(epsilon,
                                                        detail::squaredLength(edge),
                                                        detail::squaredLength(toPoint));
        if (upward && crossValue > tolerance) {
            ++windingNumber;
        } else if (downward && crossValue < -tolerance) {
            --windingNumber;
        }
    }

    return windingNumber == 0 ? PointClassification::Outside : PointClassification::Inside;
}

template <typename Scalar>
Point2D<Scalar> evaluateBezier(const std::vector<Point2D<Scalar>>& controlPoints,
                               const Scalar& t) {
    if (controlPoints.empty()) {
        throw std::invalid_argument("evaluateBezier requires control points");
    }
    if (t < Scalar{} || t > Scalar{1}) {
        throw std::invalid_argument("Bezier parameter t must lie in [0, 1]");
    }

    std::vector<Point2D<Scalar>> temp = controlPoints;
    const Scalar oneMinusT = Scalar{1} - t;

    for (std::size_t level = temp.size(); level > 1; --level) {
        for (std::size_t i = 0; i < level - 1; ++i) {
            temp[i].x = oneMinusT * temp[i].x + t * temp[i + 1].x;
            temp[i].y = oneMinusT * temp[i].y + t * temp[i + 1].y;
        }
    }

    return temp.front();
}

template <typename Scalar>
Point2D<Scalar> evaluateBezierLinear(const Point2D<Scalar>& p0,
                                     const Point2D<Scalar>& p1,
                                     const Scalar& t) {
    if (t < Scalar{} || t > Scalar{1}) {
        throw std::invalid_argument("Bezier parameter t must lie in [0, 1]");
    }
    const Scalar oneMinusT = Scalar{1} - t;
    return {oneMinusT * p0.x + t * p1.x,
            oneMinusT * p0.y + t * p1.y};
}

template <typename Scalar>
Point2D<Scalar> evaluateBezierQuadratic(const Point2D<Scalar>& p0,
                                        const Point2D<Scalar>& p1,
                                        const Point2D<Scalar>& p2,
                                        const Scalar& t) {
    if (t < Scalar{} || t > Scalar{1}) {
        throw std::invalid_argument("Bezier parameter t must lie in [0, 1]");
    }
    const Scalar oneMinusT = Scalar{1} - t;
    const Scalar oneMinusTSquared = oneMinusT * oneMinusT;
    const Scalar tSquared = t * t;
    const Scalar two = Scalar{2};
    return {oneMinusTSquared * p0.x + two * oneMinusT * t * p1.x + tSquared * p2.x,
            oneMinusTSquared * p0.y + two * oneMinusT * t * p1.y + tSquared * p2.y};
}

template <typename Scalar>
Point2D<Scalar> evaluateBezierCubic(const Point2D<Scalar>& p0,
                                    const Point2D<Scalar>& p1,
                                    const Point2D<Scalar>& p2,
                                    const Point2D<Scalar>& p3,
                                    const Scalar& t) {
    if (t < Scalar{} || t > Scalar{1}) {
        throw std::invalid_argument("Bezier parameter t must lie in [0, 1]");
    }
    const Scalar oneMinusT = Scalar{1} - t;
    const Scalar oneMinusTSquared = oneMinusT * oneMinusT;
    const Scalar oneMinusTCubed = oneMinusTSquared * oneMinusT;
    const Scalar tSquared = t * t;
    const Scalar tCubed = tSquared * t;
    const Scalar three = Scalar{3};
    return {oneMinusTCubed * p0.x +
                three * oneMinusTSquared * t * p1.x +
                three * oneMinusT * tSquared * p2.x +
                tCubed * p3.x,
            oneMinusTCubed * p0.y +
                three * oneMinusTSquared * t * p1.y +
                three * oneMinusT * tSquared * p2.y +
                tCubed * p3.y};
}

template <typename Scalar>
std::vector<Point2D<Scalar>> sampleBezier(const std::vector<Point2D<Scalar>>& controlPoints,
                                          std::size_t sampleCount) {
    if (sampleCount == 0) {
        throw std::invalid_argument("sampleCount must be greater than zero");
    }
    if (controlPoints.empty()) {
        throw std::invalid_argument("sampleBezier requires control points");
    }

    std::vector<Point2D<Scalar>> samples;
    samples.reserve(sampleCount);

    if (sampleCount == 1) {
        samples.push_back(evaluateBezier(controlPoints, Scalar{}));
        return samples;
    }

    for (std::size_t i = 0; i < sampleCount; ++i) {
        const Scalar t = static_cast<Scalar>(i) / static_cast<Scalar>(sampleCount - 1);
        samples.push_back(evaluateBezier(controlPoints, t));
    }

    return samples;
}

// Explicit instantiations for double and ExactScalar

#ifndef PLANE_GEOMETRY_SKIP_EXPLICIT_INSTANTIATIONS
template Orientation classifyPointRelativeToSegment<double>(const Segment2D<double>&,
                                                            const Point2D<double>&,
                                                            const double&);
template Orientation classifyPointRelativeToSegment<ExactScalar>(const Segment2D<ExactScalar>&,
                                                                const Point2D<ExactScalar>&,
                                                                const ExactScalar&);

template SegmentIntersectionResult<double> intersectSegments<double>(const Segment2D<double>&,
                                                                      const Segment2D<double>&,
                                                                      const double&);
template SegmentIntersectionResult<ExactScalar> intersectSegments<ExactScalar>(const Segment2D<ExactScalar>&,
                                                                              const Segment2D<ExactScalar>&,
                                                                              const ExactScalar&);

template std::vector<Point2D<double>> computeConvexHull<double>(const std::vector<Point2D<double>>&);
template std::vector<Point2D<ExactScalar>> computeConvexHull<ExactScalar>(const std::vector<Point2D<ExactScalar>>&);

template std::vector<Triangle2D<double>> delaunayTriangulation<double>(const std::vector<Point2D<double>>&);
template std::vector<Triangle2D<ExactScalar>> delaunayTriangulation<ExactScalar>(const std::vector<Point2D<ExactScalar>>&);

template Polygon<double> intersectConvexPolygons<double>(const Polygon<double>&,
                                                          const Polygon<double>&,
                                                          const double&);
template Polygon<ExactScalar> intersectConvexPolygons<ExactScalar>(const Polygon<ExactScalar>&,
                                                                   const Polygon<ExactScalar>&,
                                                                   const ExactScalar&);

template BooleanResult<double> unionConvexPolygons<double>(const Polygon<double>&,
                                                      const Polygon<double>&,
                                                      const double&);
template BooleanResult<ExactScalar> unionConvexPolygons<ExactScalar>(const Polygon<ExactScalar>&,
                                                               const Polygon<ExactScalar>&,
                                                               const ExactScalar&);

template BooleanResult<double> differenceConvexPolygons<double>(const Polygon<double>&,
                                                           const Polygon<double>&,
                                                           const double&);
template BooleanResult<ExactScalar> differenceConvexPolygons<ExactScalar>(const Polygon<ExactScalar>&,
                                                                    const Polygon<ExactScalar>&,
                                                                    const ExactScalar&);

template PointClassification locatePointInConvexPolygon<double>(const Polygon<double>&,
                                                                const Point2D<double>&,
                                                                const double&);
template PointClassification locatePointInConvexPolygon<ExactScalar>(const Polygon<ExactScalar>&,
                                                                    const Point2D<ExactScalar>&,
                                                                    const ExactScalar&);

template PointClassification locatePointInPolygon<double>(const Polygon<double>&,
                                                          const Point2D<double>&,
                                                          const double&);
template PointClassification locatePointInPolygon<ExactScalar>(const Polygon<ExactScalar>&,
                                                              const Point2D<ExactScalar>&,
                                                              const ExactScalar&);

template Point2D<double> evaluateBezier<double>(const std::vector<Point2D<double>>&,
                                                const double&);
template Point2D<ExactScalar> evaluateBezier<ExactScalar>(const std::vector<Point2D<ExactScalar>>&,
                                                          const ExactScalar&);

template Point2D<double> evaluateBezierLinear<double>(const Point2D<double>&,
                                                      const Point2D<double>&,
                                                      const double&);
template Point2D<ExactScalar> evaluateBezierLinear<ExactScalar>(const Point2D<ExactScalar>&,
                                                                const Point2D<ExactScalar>&,
                                                                const ExactScalar&);

template Point2D<double> evaluateBezierQuadratic<double>(const Point2D<double>&,
                                                         const Point2D<double>&,
                                                         const Point2D<double>&,
                                                         const double&);
template Point2D<ExactScalar> evaluateBezierQuadratic<ExactScalar>(const Point2D<ExactScalar>&,
                                                                   const Point2D<ExactScalar>&,
                                                                   const Point2D<ExactScalar>&,
                                                                   const ExactScalar&);

template Point2D<double> evaluateBezierCubic<double>(const Point2D<double>&,
                                                     const Point2D<double>&,
                                                     const Point2D<double>&,
                                                     const Point2D<double>&,
                                                     const double&);
template Point2D<ExactScalar> evaluateBezierCubic<ExactScalar>(const Point2D<ExactScalar>&,
                                                               const Point2D<ExactScalar>&,
                                                               const Point2D<ExactScalar>&,
                                                               const Point2D<ExactScalar>&,
                                                               const ExactScalar&);

template std::vector<Point2D<double>> sampleBezier<double>(const std::vector<Point2D<double>>&,
                                                           std::size_t);
template std::vector<Point2D<ExactScalar>> sampleBezier<ExactScalar>(const std::vector<Point2D<ExactScalar>>&,
                                                                    std::size_t);
#endif  // PLANE_GEOMETRY_SKIP_EXPLICIT_INSTANTIATIONS

}  // namespace plane_geometry
