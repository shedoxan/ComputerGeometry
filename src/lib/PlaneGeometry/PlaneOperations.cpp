#include "PlaneOperations.h"

#include <boost/multiprecision/cpp_dec_float.hpp>

#include <algorithm>
#include <cmath>

namespace plane_geometry {
namespace detail {

template <typename T>
inline auto absValue(const T& x) {
    using std::abs;
    using boost::multiprecision::abs; 
    return abs(x);                    
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
Scalar crossTolerance(const Scalar& epsilon, const Scalar& lhsSquaredLength, const Scalar& rhsSquaredLength) {
    return epsilon * (lhsSquaredLength + rhsSquaredLength + Scalar{1});
}

template <typename Scalar>
Scalar dotTolerance(const Scalar& epsilon, const Scalar& lhsSquaredLength, const Scalar& rhsSquaredLength, const Scalar& dotValue) {
    return epsilon * (lhsSquaredLength + rhsSquaredLength + absValue<Scalar>(dotValue) + Scalar{1});
}

} // namespace detail

template <typename Scalar>
Orientation classifyPointRelativeToSegment(const Segment2D<Scalar>& segment, const Point2D<Scalar>& point, const Scalar& epsilon) {
    const Point2D<Scalar> ab = detail::subtract(segment.end, segment.start);
    const Point2D<Scalar> ac = detail::subtract(point, segment.start);

    const Scalar abLengthSquared = detail::squaredLength(ab);
    const Scalar acLengthSquared = detail::squaredLength(ac);

    const Scalar epsilonSquared = epsilon * epsilon;

    if (abLengthSquared <= epsilonSquared) {
        const Scalar distanceSquared = acLengthSquared;
        return distanceSquared <= epsilonSquared ? Orientation::OnSegment : Orientation::Right;
    }

    const Scalar crossValue = detail::cross(ab, ac);
    const Scalar allowedCrossError = detail::crossTolerance(epsilon, abLengthSquared, acLengthSquared);

    if (detail::absValue<Scalar>(crossValue) <= allowedCrossError) {
        const Scalar dotValue = detail::dot(ab, ac);
        const Scalar allowedDotError = detail::dotTolerance(epsilon, abLengthSquared, acLengthSquared, dotValue);

        if (dotValue >= -allowedDotError && dotValue <= abLengthSquared + allowedDotError) {
            return Orientation::OnSegment;
        }

        return dotValue < Scalar{} ? Orientation::Right : Orientation::Left;
    }

    return crossValue > Scalar{} ? Orientation::Left : Orientation::Right;
}

template<typename Scalar>
SegmentIntersectionResult<Scalar> intersectSegments(const Segment2D<Scalar>& first, const Segment2D<Scalar>& second, const Scalar& epsilon) {
    SegmentIntersectionResult<Scalar> result{};

    const Point2D<Scalar> p = first.start;
    const Point2D<Scalar> r = detail::subtract(first.end, first.start);
    const Point2D<Scalar> q = second.start;
    const Point2D<Scalar> s = detail::subtract(second.end, second.start);
    const Point2D<Scalar> qp = detail::subtract(q, p);

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
            const Scalar distanceSquared = detail::squaredLength(detail::subtract(first.start, second.start));
            if (distanceSquared <= epsilonSquared) {
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

        const Scalar parameterTolerance = epsilon * (detail::sqrtValue(rLengthSquared) + detail::sqrtValue(sLengthSquared) + Scalar{1});

        if (tMin > Scalar{1} + parameterTolerance || tMax < Scalar{} - parameterTolerance) {
            return result;
        }

        const Scalar tClampedStart = std::max(Scalar{}, tMin);
        const Scalar tClampedEnd = std::min(Scalar{1}, tMax);

        if (detail::absValue<Scalar>(tClampedStart - tClampedEnd) <= parameterTolerance) {
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
    const Scalar parameterTolerance = epsilon * (detail::sqrtValue(rLengthSquared) + detail::sqrtValue(sLengthSquared) + Scalar{1});

    if (t >= -parameterTolerance && t <= Scalar{1} + parameterTolerance &&
        u >= -parameterTolerance && u <= Scalar{1} + parameterTolerance) {
        result.type = IntersectionType::Point;
        result.point = {p.x + t * r.x, p.y + t * r.y};
    }

    return result;
}

template Orientation classifyPointRelativeToSegment<ExactScalar>(const Segment2D<ExactScalar>&, const Point2D<ExactScalar>&, const ExactScalar&);

template SegmentIntersectionResult<ExactScalar> intersectSegments<ExactScalar>(const Segment2D<ExactScalar>&, const Segment2D<ExactScalar>&, const ExactScalar&);

template Orientation classifyPointRelativeToSegment<double>(const Segment2D<double>&, const Point2D<double>&, const double&);

template SegmentIntersectionResult<double> intersectSegments<double>(const Segment2D<double>&, const Segment2D<double>&, const double&);

} // namespace plane_geometry

