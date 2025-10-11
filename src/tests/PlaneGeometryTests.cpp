#include <QtTest/QtTest>

#include <QtCore/QVector>

#include "PlaneGeometry/PlaneOperations.h"

namespace {
constexpr double kTolerance = 1e-9;

double absValue(double value) {
    return value >= 0.0 ? value : -value;
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
};

void PlaneGeometryTests::orientation_left_double() { 
    // Сегмент (0,0)-(10,0) и точка (0,5) в double. Ожидаем Orientation::Lef
    plane_geometry::Segment2D<double> segment{{0.0, 0.0}, {10.0, 0.0}};
    plane_geometry::Point2D<double> point{0.0, 5.0};

    const auto orientation = plane_geometry::classifyPointRelativeToSegment(segment, point);

    QCOMPARE(orientation, plane_geometry::Orientation::Left);
}

void PlaneGeometryTests::orientation_on_segment_exact() { 
    // Сегмент из точных чисел ExactScalar (от (-1000,50) до (1000,50)) и точка (0,50). 
    // Используем epsilon=1e-40, убеждаемся, что точка считается лежащей на отрезке (Orientation::OnSegment).
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
    // Диагональный сегмент через (-5,-5) и (5,5) с точкой (10,-10) в ExactScalar.
    // Проверка, что результат — Orientation::Right.
    using Exact = plane_geometry::ExactScalar;
    using PointExact = plane_geometry::Point2D<Exact>;
    using SegmentExact = plane_geometry::Segment2D<Exact>;

    SegmentExact segment{{Exact{"-5"}, Exact{"-5"}}, {Exact{"5"}, Exact{"5"}}};
    PointExact point{Exact{"10"}, Exact{"-10"}};

    const auto orientation = plane_geometry::classifyPointRelativeToSegment(segment, point);

    QCOMPARE(orientation, plane_geometry::Orientation::Right);
}

void PlaneGeometryTests::intersection_point_double() {
    // Пересечение двух отрезков в точке (5,0) в double. Первый отрезок горизонтальный (0,0)-(10,0), второй вертикальный (5,-5)-(5,5).
    // Проверка, что тип пересечения — Point, и координаты точки пересечения (5,0) с точностью до 1e-9.
    // Используем absValue для сравнения с учётом погрешности.
    plane_geometry::Segment2D<double> first{{0.0, 0.0}, {10.0, 0.0}};
    plane_geometry::Segment2D<double> second{{5.0, -5.0}, {5.0, 5.0}};

    const auto result = plane_geometry::intersectSegments(first, second);

    QCOMPARE(result.type, plane_geometry::IntersectionType::Point);
    QVERIFY(absValue(result.point.x - 5.0) < kTolerance);
    QVERIFY(absValue(result.point.y - 0.0) < kTolerance);
}

void PlaneGeometryTests::intersection_parallel_none() {
    // Параллельные горизонтальные отрезки (0,0)-(10,0) и (0,5)-(10,5) в double.
    // Функция должна вернуть IntersectionType::None.
    plane_geometry::Segment2D<double> first{{0.0, 0.0}, {10.0, 0.0}};
    plane_geometry::Segment2D<double> second{{0.0, 5.0}, {10.0, 5.0}};

    const auto result = plane_geometry::intersectSegments(first, second);

    QCOMPARE(result.type, plane_geometry::IntersectionType::None);
}

void PlaneGeometryTests::intersection_overlap_exact() {
    // Пересечение точных сегментов [-10,0]–[10,0] и [-5,0]–[15,0] с epsilon=1e-40.
    // Ждём IntersectionType::Overlap и проверяем, что общая часть [(-5,0), (10,0)].
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

QTEST_MAIN(PlaneGeometryTests)
#include "PlaneGeometryTests.moc"
