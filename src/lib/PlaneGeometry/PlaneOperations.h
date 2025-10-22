#pragma once

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/fwd.hpp>
#include <type_traits>
#include <vector>

namespace plane_geometry {

using ExactScalar = boost::multiprecision::cpp_dec_float_50;

template <typename Scalar>
inline Scalar defaultEpsilon() {
    if constexpr (std::is_floating_point_v<Scalar>) {
        return Scalar{1e-9};
    } else {
        return Scalar{"1e-9"};
    }
}

template <typename Scalar>
struct Point2D {
    Scalar x{};
    Scalar y{};
};

template <typename Scalar>
struct Segment2D {
    Point2D<Scalar> start;
    Point2D<Scalar> end;
};

enum class Orientation : int {
    Right = -1,
    OnSegment = 0,
    Left = 1
};

enum class IntersectionType : int {
    None = 0,
    Point = 1,
    Overlap = 2
};

template <typename Scalar>
struct SegmentIntersectionResult {
    IntersectionType type{IntersectionType::None};
    Point2D<Scalar> point{};
    Segment2D<Scalar> overlap{};
};

template <typename Scalar>
struct Triangle2D {
    Point2D<Scalar> a;
    Point2D<Scalar> b;
    Point2D<Scalar> c;
};

template <typename Scalar>
Orientation classifyPointRelativeToSegment(const Segment2D<Scalar>& segment,
                                           const Point2D<Scalar>& point,
                                           const Scalar& epsilon = defaultEpsilon<Scalar>());

template <typename Scalar>
SegmentIntersectionResult<Scalar> intersectSegments(const Segment2D<Scalar>& first,
                                                    const Segment2D<Scalar>& second,
                                                    const Scalar& epsilon = defaultEpsilon<Scalar>());
template <typename Scalar>
std::vector<Point2D<Scalar>> computeConvexHull(const std::vector<Point2D<Scalar>>& points);


template <typename Scalar>
std::vector<Triangle2D<Scalar>> delaunayTriangulation(const std::vector<Point2D<Scalar>>& points);

template <typename Scalar> 
using Polygon = std::vector<Point2D<Scalar>>;

template <typename Scalar>
struct BooleanResult {
    std::vector<Polygon<Scalar>> outers; // внешние контуры (CCW)
    std::vector<Polygon<Scalar>> holes;  // дырки (CW)
    void clear() { outers.clear(); holes.clear(); }
    bool empty() const { return outers.empty() && holes.empty(); }
};

template <typename Scalar> 
Polygon<Scalar> intersectConvexPolygons(const Polygon<Scalar>& polyA, 
                                        const Polygon<Scalar>& polyB, 
                                        const Scalar& epsilon = defaultEpsilon<Scalar>());

template <typename Scalar> 
BooleanResult<Scalar> unionConvexPolygons(const Polygon<Scalar>& polyA, 
                                           const Polygon<Scalar>& polyB, 
                                           const Scalar& epsilon = defaultEpsilon<Scalar>());

template <typename Scalar> 
BooleanResult<Scalar> differenceConvexPolygons(const Polygon<Scalar>& polyA, 
                                         const Polygon<Scalar>& polyB, 
                                         const Scalar& epsilon = defaultEpsilon<Scalar>());

enum class PointClassification : int {
    Outside = -1,
    OnBoundary = 0,
    Inside = 1
};

template <typename Scalar>
PointClassification locatePointInConvexPolygon(const Polygon<Scalar>& polygon,
                                               const Point2D<Scalar>& point,
                                               const Scalar& epsilon = defaultEpsilon<Scalar>());

template <typename Scalar>
PointClassification locatePointInPolygon(const Polygon<Scalar>& polygon,
                                         const Point2D<Scalar>& point,
                                         const Scalar& epsilon = defaultEpsilon<Scalar>());

template <typename Scalar>
Point2D<Scalar> evaluateBezier(const std::vector<Point2D<Scalar>>& controlPoints,
                               const Scalar& t);

template <typename Scalar>
Point2D<Scalar> evaluateBezierLinear(const Point2D<Scalar>& p0,
                                     const Point2D<Scalar>& p1,
                                     const Scalar& t);

template <typename Scalar>
Point2D<Scalar> evaluateBezierQuadratic(const Point2D<Scalar>& p0,
                                        const Point2D<Scalar>& p1,
                                        const Point2D<Scalar>& p2,
                                        const Scalar& t);

template <typename Scalar>
Point2D<Scalar> evaluateBezierCubic(const Point2D<Scalar>& p0,
                                    const Point2D<Scalar>& p1,
                                    const Point2D<Scalar>& p2,
                                    const Point2D<Scalar>& p3,
                                    const Scalar& t);

template <typename Scalar>
std::vector<Point2D<Scalar>> sampleBezier(const std::vector<Point2D<Scalar>>& controlPoints,
                                          std::size_t sampleCount);

}  // namespace plane_geometry
