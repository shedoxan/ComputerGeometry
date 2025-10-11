#pragma once

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <type_traits>

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
Orientation classifyPointRelativeToSegment(const Segment2D<Scalar>& segment, const Point2D<Scalar>& point, const Scalar& epsilon = defaultEpsilon<Scalar>());

template <typename Scalar>                                          
SegmentIntersectionResult<Scalar> intersectSegments(const Segment2D<Scalar>& first, const Segment2D<Scalar>& second, const Scalar& epsilon = defaultEpsilon<Scalar>());

} // namespace plane_geometry
