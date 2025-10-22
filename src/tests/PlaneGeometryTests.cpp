#include <QtTest/QtTest>

#include <vector>

#include "PlaneGeometry/PlaneOperations.h"

namespace {
constexpr double kTolerance = 1e-9;

inline double absValue(double value) {
    return value >= 0.0 ? value : -value;
}

inline bool pointsEqual(const plane_geometry::Point2D<double>& lhs,
                        const plane_geometry::Point2D<double>& rhs,
                        double eps = kTolerance) {
    return std::abs(lhs.x - rhs.x) <= eps && std::abs(lhs.y - rhs.y) <= eps;
}

inline bool lexLess(const plane_geometry::Point2D<double>& lhs,
                    const plane_geometry::Point2D<double>& rhs,
                    double eps = kTolerance) {
    if (std::abs(lhs.x - rhs.x) > eps) {
        return lhs.x < rhs.x;
    }
    if (std::abs(lhs.y - rhs.y) > eps) {
        return lhs.y < rhs.y;
    }
    return false;
}
}  // namespace

class PlaneGeometryTests : public QObject {
    Q_OBJECT

private slots:
    void orientation_left_double();
    void orientation_on_segment_exact();
    void orientation_right_exact();
    void intersection_point_double();
    void intersection_parallel_none();
    void intersection_overlap_exact();
    void convex_hull_square();
    void convex_hull_collinear();
    void delaunay_triangle();
    void delaunay_square();
};

void PlaneGeometryTests::orientation_left_double() {
    plane_geometry::Segment2D<double> segment{{0.0, 0.0}, {10.0, 0.0}};
    plane_geometry::Point2D<double> point{0.0, 5.0};

    const auto orientation = plane_geometry::classifyPointRelativeToSegment(segment, point);

    QCOMPARE(orientation, plane_geometry::Orientation::Left);
}

void PlaneGeometryTests::orientation_on_segment_exact() {
    using Exact = plane_geometry::ExactScalar;
    using PointExact = plane_geometry::Point2D<Exact>;
    using SegmentExact = plane_geometry::Segment2D<Exact>;

    SegmentExact segment{{Exact{"-1000"}, Exact{"50"}}, {Exact{"1000"}, Exact{"50"}}};
    PointExact point{Exact{"0"}, Exact{"50"}};

    const Exact epsilon{"1e-40"};
    const auto orientation =
        plane_geometry::classifyPointRelativeToSegment(segment, point, epsilon);

    QCOMPARE(orientation, plane_geometry::Orientation::OnSegment);
}

void PlaneGeometryTests::orientation_right_exact() {
    using Exact = plane_geometry::ExactScalar;
    using PointExact = plane_geometry::Point2D<Exact>;
    using SegmentExact = plane_geometry::Segment2D<Exact>;

    SegmentExact segment{{Exact{"-5"}, Exact{"-5"}}, {Exact{"5"}, Exact{"5"}}};
    PointExact point{Exact{"10"}, Exact{"-10"}};

    const auto orientation = plane_geometry::classifyPointRelativeToSegment(segment, point);

    QCOMPARE(orientation, plane_geometry::Orientation::Right);
}

void PlaneGeometryTests::intersection_point_double() {
    plane_geometry::Segment2D<double> first{{0.0, 0.0}, {10.0, 0.0}};
    plane_geometry::Segment2D<double> second{{5.0, -5.0}, {5.0, 5.0}};

    const auto result = plane_geometry::intersectSegments(first, second);

    QCOMPARE(result.type, plane_geometry::IntersectionType::Point);
    QVERIFY(absValue(result.point.x - 5.0) < kTolerance);
    QVERIFY(absValue(result.point.y - 0.0) < kTolerance);
}

void PlaneGeometryTests::intersection_parallel_none() {
    plane_geometry::Segment2D<double> first{{0.0, 0.0}, {10.0, 0.0}};
    plane_geometry::Segment2D<double> second{{0.0, 5.0}, {10.0, 5.0}};

    const auto result = plane_geometry::intersectSegments(first, second);

    QCOMPARE(result.type, plane_geometry::IntersectionType::None);
}

void PlaneGeometryTests::intersection_overlap_exact() {
    using Exact = plane_geometry::ExactScalar;
    using SegmentExact = plane_geometry::Segment2D<Exact>;

    SegmentExact first{{Exact{"-10"}, Exact{"0"}}, {Exact{"10"}, Exact{"0"}}};
    SegmentExact second{{Exact{"-5"}, Exact{"0"}}, {Exact{"15"}, Exact{"0"}}};

    const auto result = plane_geometry::intersectSegments(first, second, Exact{"1e-40"});

    QCOMPARE(result.type, plane_geometry::IntersectionType::Overlap);
    QCOMPARE(result.overlap.start.x, Exact{"-5"});
    QCOMPARE(result.overlap.start.y, Exact{"0"});
    QCOMPARE(result.overlap.end.x, Exact{"10"});
    QCOMPARE(result.overlap.end.y, Exact{"0"});
}

void PlaneGeometryTests::convex_hull_square() {
    std::vector<plane_geometry::Point2D<double>> points{
        {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {0.5, 0.5}};

    const auto hull = plane_geometry::computeConvexHull(points);

    QCOMPARE(static_cast<int>(hull.size()), 4);
    QCOMPARE(hull[0].x, 0.0);
    QCOMPARE(hull[0].y, 0.0);
    QCOMPARE(hull[1].x, 1.0);
    QCOMPARE(hull[1].y, 0.0);
    QCOMPARE(hull[2].x, 1.0);
    QCOMPARE(hull[2].y, 1.0);
    QCOMPARE(hull[3].x, 0.0);
    QCOMPARE(hull[3].y, 1.0);
}

void PlaneGeometryTests::convex_hull_collinear() {
    std::vector<plane_geometry::Point2D<double>> points{
        {-1.0, 0.0}, {0.0, 0.0}, {1.0, 0.0}, {0.5, 0.0}, {-0.25, 0.0}};

    const auto hull = plane_geometry::computeConvexHull(points);

    QCOMPARE(static_cast<int>(hull.size()), 2);
    QCOMPARE(hull.front().x, -1.0);
    QCOMPARE(hull.front().y, 0.0);
    QCOMPARE(hull.back().x, 1.0);
    QCOMPARE(hull.back().y, 0.0);
}

void PlaneGeometryTests::delaunay_triangle() {
    std::vector<plane_geometry::Point2D<double>> points{
        {0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};

    const auto triangulation = plane_geometry::delaunayTriangulation(points);

    QCOMPARE(static_cast<int>(triangulation.size()), 1);

    auto vertices = std::array<plane_geometry::Point2D<double>, 3>{
        triangulation[0].a, triangulation[0].b, triangulation[0].c};
    std::sort(vertices.begin(), vertices.end(), [](const auto& lhs, const auto& rhs) { return lexLess(lhs, rhs); });

    std::array<plane_geometry::Point2D<double>, 3> expected{
        plane_geometry::Point2D<double>{0.0, 0.0},
        plane_geometry::Point2D<double>{0.0, 1.0},
        plane_geometry::Point2D<double>{1.0, 0.0}};
    std::sort(expected.begin(), expected.end(), [](const auto& lhs, const auto& rhs) { return lexLess(lhs, rhs); });

    for (std::size_t i = 0; i < expected.size(); ++i) {
        QVERIFY(pointsEqual(vertices[i], expected[i]));
    }
}

void PlaneGeometryTests::delaunay_square() {
    std::vector<plane_geometry::Point2D<double>> points{
        {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};

    const auto triangulation = plane_geometry::delaunayTriangulation(points);

    QCOMPARE(static_cast<int>(triangulation.size()), 2);

    std::vector<std::array<plane_geometry::Point2D<double>, 3>> triangles;
    triangles.reserve(triangulation.size());
    for (const auto& triangle : triangulation) {
        std::array<plane_geometry::Point2D<double>, 3> verts{triangle.a, triangle.b, triangle.c};
        std::sort(verts.begin(), verts.end(), [](const auto& lhs, const auto& rhs) { return lexLess(lhs, rhs); });
        triangles.push_back(verts);
    }
    std::sort(triangles.begin(), triangles.end(), [](const auto& lhs, const auto& rhs) {
        if (lexLess(lhs[0], rhs[0])) {
            return true;
        }
        if (lexLess(rhs[0], lhs[0])) {
            return false;
        }
        if (lexLess(lhs[1], rhs[1])) {
            return true;
        }
        if (lexLess(rhs[1], lhs[1])) {
            return false;
        }
        return lexLess(lhs[2], rhs[2]);
    });

    const std::vector<std::array<plane_geometry::Point2D<double>, 3>> expected = {
        {plane_geometry::Point2D<double>{0.0, 0.0},
         plane_geometry::Point2D<double>{0.0, 1.0},
         plane_geometry::Point2D<double>{1.0, 1.0}},
        {plane_geometry::Point2D<double>{0.0, 0.0},
         plane_geometry::Point2D<double>{1.0, 0.0},
         plane_geometry::Point2D<double>{1.0, 1.0}},
    };

    for (std::size_t i = 0; i < expected.size(); ++i) {
        for (std::size_t j = 0; j < expected[i].size(); ++j) {
            QVERIFY(pointsEqual(triangles[i][j], expected[i][j]));
        }
    }
}

QTEST_MAIN(PlaneGeometryTests)
#include "PlaneGeometryTests.moc"
