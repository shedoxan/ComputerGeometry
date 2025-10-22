#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStringList>

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "convexhullwidget.h"
#include "convexpolygonboolewidget.h"
#include "drawwidget.h"
#include "precisiondrawwidget.h"
#include "segmentintersectionwidget.h"
#include "triangulationwidget.h"
#include "pointlocationwidget.h"
#include "bezierwidget.h"

namespace {
constexpr double kIntersectionTolerance = 1e-4;

PlanePointD toPlanePoint(const QPointF& point) {
    return {point.x(), -point.y()};
}

QPointF toWidgetPoint(const PlanePointD& point) {
    return {point.x, -point.y};
}

plane_geometry::Point2D<double> toGeometryPoint(const QPointF& point) {
    return {point.x(), point.y()};
}

QString formatPoint(const QPointF& point, int precision = 2) {
    return QStringLiteral("(%1, %2)")
        .arg(QString::number(point.x(), 'f', precision))
        .arg(QString::number(point.y(), 'f', precision));
}
}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    // Orientation tab
    connect(ui->widget, &DrawWidget::pointsChanged,
            this, &MainWindow::updatePoints);
    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::onCheckOrientation);

    // Intersection tab
    connect(ui->intersectionWidget, &SegmentIntersectionWidget::segmentsChanged,
            this, &MainWindow::onSegmentsChanged);
    connect(ui->computeIntersectionButton, &QPushButton::clicked,
            this, &MainWindow::onComputeIntersectionClicked);
    connect(ui->liveIntersectionCheckBox, &QCheckBox::toggled,
            this, &MainWindow::onLiveIntersectionToggled);
    connect(ui->clearSegmentsButton, &QPushButton::clicked,
            this, &MainWindow::onClearSegmentsClicked);
    ui->intersectionWidget->setDragEnabled(false);

    // Convex hull tab
    m_hullCanvas = ui->hullCanvas;
    connect(m_hullCanvas, &ConvexHullWidget::pointsChanged,
            this, &MainWindow::onHullPointsChanged);
    connect(ui->buildHullButton, &QPushButton::clicked,
            this, &MainWindow::onBuildHullClicked);
    connect(ui->liveHullCheckBox, &QCheckBox::toggled,
            this, &MainWindow::onLiveHullToggled);
    connect(ui->clearHullButton, &QPushButton::clicked,
            this, &MainWindow::onClearHullClicked);
    m_hullCanvas->setDraggingEnabled(false);

    // High precision tab
    m_precisionCanvas = ui->precisionCanvas;
    connect(ui->precisionCheckButton, &QPushButton::clicked,
            this, &MainWindow::onPrecisionCheckButtonClicked);
    connect(ui->precisionClearButton, &QPushButton::clicked,
            this, &MainWindow::onPrecisionClearButtonClicked);
    if (m_precisionCanvas) {
        m_precisionCanvas->setDragEnabled(true);
        connect(m_precisionCanvas, &PrecisionDrawWidget::canvasPointsChanged,
                this, &MainWindow::onPrecisionCanvasPointsChanged);
    }

    if (ui->precisionEpsilonEdit) {
        const double defaultEpsilon = std::numeric_limits<double>::epsilon();
        ui->precisionEpsilonEdit->setText(QString::number(defaultEpsilon, 'g', 17));
    }

    // Triangulation tab
    m_triangulationCanvas = ui->triangulationCanvas;
    if (m_triangulationCanvas) {
        connect(m_triangulationCanvas, &TriangulationWidget::pointsChanged,
                this, &MainWindow::onTriangulationPointsChanged);
    }
    if (ui->triangulationComputeButton) {
        connect(ui->triangulationComputeButton, &QPushButton::clicked,
                this, &MainWindow::onTriangulationComputeClicked);
    }
    if (ui->triangulationLiveCheckBox) {
        connect(ui->triangulationLiveCheckBox, &QCheckBox::toggled,
                this, &MainWindow::onTriangulationLiveToggled);
    }
    if (ui->triangulationDragCheckBox) {
        ui->triangulationDragCheckBox->setChecked(true);
        connect(ui->triangulationDragCheckBox, &QCheckBox::toggled,
                this, &MainWindow::onTriangulationDragToggled);
    }
    if (ui->triangulationClearButton) {
        connect(ui->triangulationClearButton, &QPushButton::clicked,
                this, &MainWindow::onTriangulationClearClicked);
    }

    // Convex boolean operations tab
    m_booleanCanvas = ui->convexBooleanCanvas;
    if (m_booleanCanvas) {
        connect(m_booleanCanvas, &ConvexPolygonBooleWidget::polygonsChanged,
                this, &MainWindow::onBooleanPolygonsChanged);
        connect(m_booleanCanvas, &ConvexPolygonBooleWidget::phaseChanged,
                this, &MainWindow::onBooleanPhaseChanged);
    }
    if (ui->booleanFinalizeFirstButton) {
        connect(ui->booleanFinalizeFirstButton, &QPushButton::clicked,
                this, &MainWindow::onBooleanFinalizeFirstClicked);
    }
    if (ui->booleanFinalizeSecondButton) {
        connect(ui->booleanFinalizeSecondButton, &QPushButton::clicked,
                this, &MainWindow::onBooleanFinalizeSecondClicked);
    }
    if (ui->booleanComputeButton) {
        connect(ui->booleanComputeButton, &QPushButton::clicked,
                this, &MainWindow::onBooleanComputeClicked);
    }
    if (ui->booleanOperationCombo) {
        connect(ui->booleanOperationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::onBooleanOperationChanged);
    }
    if (ui->booleanLiveCheckBox) {
        connect(ui->booleanLiveCheckBox, &QCheckBox::toggled,
                this, &MainWindow::onBooleanLiveToggled);
    }
    if (ui->booleanClearButton) {
        connect(ui->booleanClearButton, &QPushButton::clicked,
                this, &MainWindow::onBooleanClearClicked);
    }

    // Point localization tab
    m_pointLocationCanvas = ui->pointLocationCanvas;
    if (m_pointLocationCanvas) {
        connect(m_pointLocationCanvas, &PointLocationWidget::polygonPointsChanged,
                this, &MainWindow::onPointLocationPointsChanged);
        connect(m_pointLocationCanvas, &PointLocationWidget::polygonFinalized,
                this, &MainWindow::onPointLocationHullReady);
        connect(m_pointLocationCanvas, &PointLocationWidget::testPointChanged,
                this, &MainWindow::onPointLocationTestPointChanged);
        connect(m_pointLocationCanvas, &PointLocationWidget::classificationChanged,
                this, &MainWindow::onPointLocationSummaryChanged);
    }
    if (ui->pointLocationFinalizeButton) {
        connect(ui->pointLocationFinalizeButton, &QPushButton::clicked,
                this, &MainWindow::onPointLocationFinalizeClicked);
    }
    if (ui->pointLocationClearButton) {
        connect(ui->pointLocationClearButton, &QPushButton::clicked,
                this, &MainWindow::onPointLocationClearClicked);
    }
    if (ui->pointLocationRemoveTestButton) {
        connect(ui->pointLocationRemoveTestButton, &QPushButton::clicked,
                this, &MainWindow::onPointLocationRemoveTestClicked);
    }
    if (ui->pointLocationDragCheckBox) {
        ui->pointLocationDragCheckBox->setChecked(true);
        connect(ui->pointLocationDragCheckBox, &QCheckBox::toggled,
                this, &MainWindow::onPointLocationDragToggled);
    }

    // Bezier curves tab
    m_bezierCanvas = ui->bezierCanvas;
    if (m_bezierCanvas) {
        connect(m_bezierCanvas, &BezierWidget::controlPointsChanged,
                this, &MainWindow::onBezierControlPointsChanged);
        connect(m_bezierCanvas, &BezierWidget::curveUpdated,
                this, &MainWindow::onBezierCurveUpdated);
    }
    if (ui->bezierDegreeCombo) {
        ui->bezierDegreeCombo->setCurrentIndex(m_bezierCanvas ? m_bezierCanvas->degree() - 1 : 0);
        connect(ui->bezierDegreeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::onBezierDegreeChanged);
    }
    if (ui->bezierSamplesSpin && m_bezierCanvas) {
        ui->bezierSamplesSpin->setValue(m_bezierCanvas->sampleCount());
        connect(ui->bezierSamplesSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &MainWindow::onBezierSamplesChanged);
    }
    if (ui->bezierDragCheckBox) {
        ui->bezierDragCheckBox->setChecked(true);
        connect(ui->bezierDragCheckBox, &QCheckBox::toggled,
                this, &MainWindow::onBezierDragToggled);
    }
    if (ui->bezierClearButton) {
        connect(ui->bezierClearButton, &QPushButton::clicked,
                this, &MainWindow::onBezierClearClicked);
    }

    updatePointsSummary();
    updateHullSummary();
    updateIntersectionSummary();
    updateTriangulationSummary();
    updateBooleanSummary();
    updateBooleanControls();
    updatePointLocationSummary(QString());
    updateBezierInfo();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updatePoints(const QVector<QPointF>& points) {
    m_orientationPoints = points;
    ui->pushButton->setEnabled(m_orientationPoints.size() >= 3);

    if (m_orientationPoints.size() < 3) {
        m_hasOrientationResult = false;
        m_lastResultSummary.clear();
    }

    applyOrientationResult(m_hasOrientationResult, m_lastOrientation);
    updatePointsSummary();
}

void MainWindow::onCheckOrientation() {
    if (m_orientationPoints.size() < 3) {
        ui->textBrowser->append(tr("Add two points for the segment and one point to test."));
        return;
    }

    const PlaneSegmentD segment{
        toPlanePoint(m_orientationPoints[0]),
        toPlanePoint(m_orientationPoints[1])
    };
    const PlanePointD testPoint = toPlanePoint(m_orientationPoints[2]);

    const auto orientation = plane_geometry::classifyPointRelativeToSegment(segment, testPoint);

    m_hasOrientationResult = true;
    m_lastOrientation = orientation;

    const int orientationValue = static_cast<int>(orientation);
    const QString description = orientationDescription(orientation);
    m_lastResultSummary = QStringLiteral("Result: %1 (%2)").arg(orientationValue).arg(description);

    applyOrientationResult(true, orientation);
    updatePointsSummary();
}

void MainWindow::applyOrientationResult(bool hasResult, plane_geometry::Orientation orientation) {
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setChecked(true);
    ui->radioButton_3->setChecked(true);

    if (!hasResult) {
        return;
    }

    switch (orientation) {
    case plane_geometry::Orientation::Left:
        ui->radioButton->setChecked(true);
        break;
    case plane_geometry::Orientation::OnSegment:
        ui->radioButton_2->setChecked(true);
        break;
    case plane_geometry::Orientation::Right:
        ui->radioButton_3->setChecked(true);
        break;
    }
}

QString MainWindow::orientationDescription(plane_geometry::Orientation orientation) const {
    switch (orientation) {
    case plane_geometry::Orientation::Left:
        return tr("point is left of the segment");
    case plane_geometry::Orientation::OnSegment:
        return tr("point lies on the segment");
    case plane_geometry::Orientation::Right:
        return tr("point is right of the segment");
    }
    return tr("unknown orientation");
}

void MainWindow::updatePointsSummary() {
    QStringList lines;
    const QStringList labels = {QStringLiteral("A"), QStringLiteral("B"), QStringLiteral("C")};

    for (int i = 0; i < m_orientationPoints.size(); ++i) {
        const QString label = i < labels.size() ? labels[i] : QStringLiteral("P%1").arg(i + 1);
        lines << QStringLiteral("%1 = %2")
                     .arg(label)
                     .arg(formatPoint(m_orientationPoints[i]));
    }

    if (m_orientationPoints.isEmpty()) {
        lines << tr("Click two segment endpoints and then the test point.");
    } else if (m_orientationPoints.size() == 1) {
        lines << tr("Add one more point to complete the segment.");
    } else if (m_orientationPoints.size() == 2) {
        lines << tr("Click the test point to check its position.");
    } else {
        if (m_hasOrientationResult && !m_lastResultSummary.isEmpty()) {
            lines << m_lastResultSummary;
        } else {
            lines << tr("Press \"Check point position\" to evaluate the third point.");
        }
    }

    ui->textBrowser->setPlainText(lines.join('\n'));
}

void MainWindow::onHullPointsChanged(const QVector<QPointF>& points) {
    m_hullInputPoints = points;

    if (points.isEmpty()) {
        clearConvexHull();
        return;
    }

    if (m_liveConvexHull) {
        rebuildConvexHull();
    } else {
        clearConvexHull();
        updateHullSummary();
    }
}

void MainWindow::onBuildHullClicked() {
    rebuildConvexHull();
}

void MainWindow::onLiveHullToggled(bool checked) {
    m_liveConvexHull = checked;
    if (m_hullCanvas) {
        m_hullCanvas->setDraggingEnabled(checked);
    }

    if (m_liveConvexHull) {
        rebuildConvexHull();
    }
}

void MainWindow::onClearHullClicked() {
    clearConvexHull();
}

void MainWindow::updateHullSummary() {
    QStringList lines;

    if (m_hullInputPoints.isEmpty()) {
        lines << tr("No points available. Click on the canvas to add points.");
    } else {
        lines << tr("Input points (%1 total):").arg(m_hullInputPoints.size());
        for (int i = 0; i < m_hullInputPoints.size(); ++i) {
            lines << QStringLiteral("%1: %2")
                        .arg(i + 1)
                        .arg(formatPoint(m_hullInputPoints[i]));
        }
    }

    if (m_hasConvexHull && !m_convexHullPoints.isEmpty()) {
        lines << QString();
        lines << tr("Hull vertices (%1 total):").arg(m_convexHullPoints.size());
        for (int i = 0; i < m_convexHullPoints.size(); ++i) {
            lines << QStringLiteral("%1: %2")
                        .arg(i + 1)
                        .arg(formatPoint(m_convexHullPoints[i]));
        }
    } else if (!m_hullInputPoints.isEmpty()) {
        lines << QString();
        lines << tr("Press \"Build convex hull\" or enable live update.");
    }

    ui->hullTextBrowser->setPlainText(lines.join('\n'));
}

void MainWindow::rebuildConvexHull() {
    if (m_hullInputPoints.isEmpty()) {
        clearConvexHull();
        return;
    }

    std::vector<plane_geometry::Point2D<double>> input;
    input.reserve(m_hullInputPoints.size());
    for (const auto& point : m_hullInputPoints) {
        input.push_back(toGeometryPoint(point));
    }

    const auto hull = plane_geometry::computeConvexHull(input);

    m_convexHullPoints.clear();
    m_convexHullPoints.reserve(static_cast<int>(hull.size()));
    for (const auto& point : hull) {
        m_convexHullPoints.append(QPointF(point.x, point.y));
    }

    m_hasConvexHull = !m_convexHullPoints.isEmpty();
    if (m_hullCanvas) {
        m_hullCanvas->setConvexHull(m_convexHullPoints);
    }
    updateHullSummary();
}

void MainWindow::clearConvexHull() {
    m_convexHullPoints.clear();
    m_hasConvexHull = false;
    if (m_hullCanvas) {
        m_hullCanvas->clearConvexHull();
    }
    updateHullSummary();
}

void MainWindow::updateIntersectionSummary() {
    QStringList lines;
    const QStringList labels = {QStringLiteral("A"), QStringLiteral("B"), QStringLiteral("C"), QStringLiteral("D")};

    for (int i = 0; i < m_segmentPoints.size(); ++i) {
        const QString label = i < labels.size() ? labels[i] : QStringLiteral("P%1").arg(i + 1);
        lines << QStringLiteral("%1 = %2")
                    .arg(label)
                    .arg(formatPoint(m_segmentPoints[i]));
    }

    if (m_segmentPoints.isEmpty()) {
        lines << tr("Click to set A-B, then C-D. Right-click clears all points.");
    } else if (m_segmentPoints.size() < 4) {
        lines << tr("Add points until both segments are defined (A-B and C-D).");
    }

    if (!m_lastIntersectionSummary.isEmpty()) {
        lines << m_lastIntersectionSummary;
    } else if (intersectionSegmentsReady() && !m_liveIntersection) {
        lines << tr("Press \"Find intersection\" to compute.");
    }

    ui->intersectionTextBrowser->setPlainText(lines.join('\n'));
}

void MainWindow::applyIntersectionResult() {
    if (!intersectionSegmentsReady()) {
        return;
    }

    m_lastIntersection = evaluateIntersection();
    m_hasIntersectionResult = true;
    ui->intersectionRadioNone->setChecked(false);
    ui->intersectionRadioPoint->setChecked(false);
    ui->intersectionRadioOverlap->setChecked(false);

    switch (m_lastIntersection.type) {
    case plane_geometry::IntersectionType::None:
        ui->intersectionWidget->setIntersectionNone();
        ui->intersectionRadioNone->setChecked(true);
        m_lastIntersectionSummary = tr("Segments do not intersect.");
        break;
    case plane_geometry::IntersectionType::Point: {
        const QPointF point = toWidgetPoint(m_lastIntersection.point);
        ui->intersectionWidget->setIntersectionPoint(point);
        ui->intersectionRadioPoint->setChecked(true);
        m_lastIntersectionSummary = QStringLiteral("Intersection at (%1, %2)")
                                        .arg(QString::number(point.x(), 'f', 2))
                                        .arg(QString::number(point.y(), 'f', 2));
        break;
    }
    case plane_geometry::IntersectionType::Overlap: {
        const QPointF start = toWidgetPoint(m_lastIntersection.overlap.start);
        const QPointF end = toWidgetPoint(m_lastIntersection.overlap.end);
        ui->intersectionWidget->setIntersectionOverlap(start, end);
        ui->intersectionRadioOverlap->setChecked(true);
        m_lastIntersectionSummary = QStringLiteral("Segments overlap from (%1, %2) to (%3, %4)")
                                        .arg(QString::number(start.x(), 'f', 2))
                                        .arg(QString::number(start.y(), 'f', 2))
                                        .arg(QString::number(end.x(), 'f', 2))
                                        .arg(QString::number(end.y(), 'f', 2));
        break;
    }
    }

    updateIntersectionSummary();
}

bool MainWindow::intersectionSegmentsReady() const {
    return m_segmentPoints.size() >= 4;
}

SegmentIntersectionResultD MainWindow::evaluateIntersection() const {
    if (!intersectionSegmentsReady()) {
        return {};
    }

    const PlaneSegmentD first{
        toPlanePoint(m_segmentPoints[0]),
        toPlanePoint(m_segmentPoints[1])
    };
    const PlaneSegmentD second{
        toPlanePoint(m_segmentPoints[2]),
        toPlanePoint(m_segmentPoints[3])
    };

    return plane_geometry::intersectSegments(first, second, kIntersectionTolerance);
}

void MainWindow::onSegmentsChanged(const QVector<QPointF>& points) {
    m_segmentPoints = points;
    ui->computeIntersectionButton->setEnabled(intersectionSegmentsReady());

    if (!intersectionSegmentsReady()) {
        m_hasIntersectionResult = false;
        m_lastIntersectionSummary.clear();
        ui->intersectionWidget->setIntersectionNone();
        ui->intersectionRadioNone->setChecked(true);
        ui->intersectionRadioPoint->setChecked(false);
        ui->intersectionRadioOverlap->setChecked(false);
    }

    if (m_liveIntersection && intersectionSegmentsReady()) {
        applyIntersectionResult();
    } else {
        updateIntersectionSummary();
    }
}

void MainWindow::onComputeIntersectionClicked() {
    if (!intersectionSegmentsReady()) {
        m_hasIntersectionResult = false;
        m_lastIntersectionSummary = tr("Add four points (A, B, C, D) to define two segments.");
        updateIntersectionSummary();
        return;
    }

    applyIntersectionResult();
}

void MainWindow::onLiveIntersectionToggled(bool checked) {
    m_liveIntersection = checked;
    ui->intersectionWidget->setDragEnabled(checked);
    if (m_liveIntersection && intersectionSegmentsReady()) {
        applyIntersectionResult();
    }
}

void MainWindow::onClearSegmentsClicked() {
    m_segmentPoints.clear();
    m_hasIntersectionResult = false;
    m_lastIntersectionSummary.clear();
    ui->intersectionWidget->clearSegments();
    ui->intersectionRadioNone->setChecked(true);
    ui->intersectionRadioPoint->setChecked(false);
    ui->intersectionRadioOverlap->setChecked(false);
    updateIntersectionSummary();
}

void MainWindow::onPrecisionCheckButtonClicked() {
    if (!ui->precisionSegmentStartEdit || !ui->precisionSegmentEndEdit ||
        !ui->precisionPointEdit) {
        return;
    }

    QString errorMessage;
    const auto segmentOpt = precision_converter::parseSegment(
        ui->precisionSegmentStartEdit->text(),
        ui->precisionSegmentEndEdit->text(),
        &errorMessage);
    if (!segmentOpt) {
        QMessageBox::warning(this, tr("High precision input"), errorMessage);
        return;
    }

    const auto pointOpt =
        precision_converter::parsePoint(ui->precisionPointEdit->text(), &errorMessage);
    if (!pointOpt) {
        QMessageBox::warning(this, tr("High precision input"), errorMessage);
        return;
    }

    plane_geometry::ExactScalar epsilon =
        plane_geometry::defaultEpsilon<plane_geometry::ExactScalar>();
    const QString epsilonText = ui->precisionEpsilonEdit
                                    ? ui->precisionEpsilonEdit->text().trimmed()
                                    : QString();
    if (!epsilonText.isEmpty()) {
        const auto epsilonOpt = precision_converter::parseScalar(epsilonText, &errorMessage);
        if (!epsilonOpt) {
            QMessageBox::warning(this, tr("High precision input"), errorMessage);
            return;
        }
        epsilon = *epsilonOpt;
    }

    const auto orientation = plane_geometry::classifyPointRelativeToSegment(
        *segmentOpt, *pointOpt, epsilon);

    if (m_precisionCanvas) {
        m_precisionCanvas->setSegment(*segmentOpt);
        m_precisionCanvas->setPoint(*pointOpt);
    }

    QStringList lines;
    lines << QStringLiteral("Result: %1 (%2)")
                 .arg(static_cast<int>(orientation))
                 .arg(orientationDescription(orientation));
    lines << QStringLiteral("Segment start: %1, %2")
                 .arg(precision_converter::formatScalar(segmentOpt->start.x))
                 .arg(precision_converter::formatScalar(segmentOpt->start.y));
    lines << QStringLiteral("Segment end: %1, %2")
                 .arg(precision_converter::formatScalar(segmentOpt->end.x))
                 .arg(precision_converter::formatScalar(segmentOpt->end.y));
    lines << QStringLiteral("Point: %1, %2")
                 .arg(precision_converter::formatScalar(pointOpt->x))
                 .arg(precision_converter::formatScalar(pointOpt->y));
    lines << QStringLiteral("Epsilon: %1").arg(precision_converter::formatScalar(epsilon));

    if (ui->precisionResultBrowser) {
        ui->precisionResultBrowser->setPlainText(lines.join('\n'));
    }
}

void MainWindow::onPrecisionClearButtonClicked() {
    if (ui->precisionSegmentStartEdit) {
        ui->precisionSegmentStartEdit->clear();
    }
    if (ui->precisionSegmentEndEdit) {
        ui->precisionSegmentEndEdit->clear();
    }
    if (ui->precisionPointEdit) {
        ui->precisionPointEdit->clear();
    }
    if (ui->precisionEpsilonEdit) {
        const double defaultEpsilon = std::numeric_limits<double>::epsilon();
        ui->precisionEpsilonEdit->setText(QString::number(defaultEpsilon, 'g', 17));
    }
    if (ui->precisionResultBrowser) {
        ui->precisionResultBrowser->clear();
    }
    if (m_precisionCanvas) {
        m_precisionCanvas->clear();
    }
}

void MainWindow::onPrecisionCanvasPointsChanged(
    const QVector<plane_geometry::Point2D<plane_geometry::ExactScalar>>& points) {
    const auto formatExactPoint = [](const plane_geometry::Point2D<plane_geometry::ExactScalar>& point) {
        return QStringLiteral("(%1, %2)")
            .arg(precision_converter::formatScalar(point.x))
            .arg(precision_converter::formatScalar(point.y));
    };

    if (ui->precisionSegmentStartEdit) {
        if (points.size() >= 1) {
            ui->precisionSegmentStartEdit->setText(formatExactPoint(points[0]));
        } else {
            ui->precisionSegmentStartEdit->clear();
        }
    }

    if (ui->precisionSegmentEndEdit) {
        if (points.size() >= 2) {
            ui->precisionSegmentEndEdit->setText(formatExactPoint(points[1]));
        } else {
            ui->precisionSegmentEndEdit->clear();
        }
    }

    if (ui->precisionPointEdit) {
        if (points.size() >= 3) {
            ui->precisionPointEdit->setText(formatExactPoint(points[2]));
        } else {
            ui->precisionPointEdit->clear();
        }
    }

    if (points.size() >= 3) {
        onPrecisionCheckButtonClicked();
    } else if (ui->precisionResultBrowser) {
        ui->precisionResultBrowser->clear();
    }
}
void MainWindow::onTriangulationPointsChanged(const QVector<QPointF>& /*points*/) {
    if (!m_triangulationCanvas) {
        return;
    }
    m_triangulationTriangles.clear();
    if (m_triangulationLive) {
        computeDelaunayTriangulation();
    } else {
        m_triangulationCanvas->clearTriangles();
        updateTriangulationSummary();
    }
}

void MainWindow::onTriangulationComputeClicked() {
    computeDelaunayTriangulation();
}

void MainWindow::onTriangulationLiveToggled(bool checked) {
    m_triangulationLive = checked;
    if (m_triangulationLive) {
        computeDelaunayTriangulation();
    }
}

void MainWindow::onTriangulationDragToggled(bool checked) {
    if (m_triangulationCanvas) {
        m_triangulationCanvas->setDraggingEnabled(checked);
    }
}

void MainWindow::onTriangulationClearClicked() {
    if (m_triangulationCanvas) {
        m_triangulationCanvas->clearPoints();
        m_triangulationCanvas->clearTriangles();
    }
    m_triangulationTriangles.clear();
    updateTriangulationSummary();
}

void MainWindow::updateTriangulationSummary() {
    QStringList lines;
    const int pointCount = m_triangulationCanvas ? m_triangulationCanvas->points().size() : 0;
    const int triangleCount = m_triangulationTriangles.size();

    lines << tr("Points: %1").arg(pointCount);
    lines << tr("Triangles: %1").arg(triangleCount);

    if (pointCount == 0) {
        lines << tr("Click on the canvas to add points.");
    } else if (pointCount < 3) {
        lines << tr("Need at least three points for triangulation.");
    } else if (!m_triangulationLive && triangleCount == 0) {
        lines << tr("Press \"Compute\" to build the triangulation.");
    }

    m_triangulationSummary = lines.join('\n');
    if (ui->triangulationSummaryBrowser) {
        ui->triangulationSummaryBrowser->setPlainText(m_triangulationSummary);
    }
}

void MainWindow::computeDelaunayTriangulation() {
    if (!m_triangulationCanvas) {
        return;
    }

    const QVector<QPointF> points = m_triangulationCanvas->points();
    if (points.size() < 3) {
        m_triangulationTriangles.clear();
        m_triangulationCanvas->clearTriangles();
        updateTriangulationSummary();
        return;
    }

    const auto geometryPoints = toGeometryPoints(points);
    const std::vector<plane_geometry::Triangle2D<double>> triangulation =
        plane_geometry::delaunayTriangulation(geometryPoints);

    QVector<QPolygonF> triangles;
    triangles.reserve(static_cast<int>(triangulation.size()));
    for (const auto& triangle : triangulation) {
        QPolygonF polygon;
        polygon << QPointF(triangle.a.x, triangle.a.y)
                << QPointF(triangle.b.x, triangle.b.y)
                << QPointF(triangle.c.x, triangle.c.y);
        triangles.append(polygon);
    }

    m_triangulationTriangles = triangles;
    m_triangulationCanvas->setTriangles(m_triangulationTriangles);
    updateTriangulationSummary();
}

void MainWindow::onBooleanPolygonsChanged() {
    if (!m_booleanCanvas) {
        return;
    }

    m_booleanOuters.clear();
    m_booleanHoles.clear();
    m_booleanCanvas->clearResultPolygon();

    if (m_booleanLive &&
        m_booleanCanvas->phase() == ConvexPolygonBooleWidget::Phase::ResultReady) {
        recomputeBooleanOperation();
    } else {
        updateBooleanSummary();
    }

    updateBooleanControls();
}

void MainWindow::onBooleanFinalizeFirstClicked() {
    if (!m_booleanCanvas) {
        return;
    }
    if (m_booleanCanvas->firstRawPoints().size() < 3) {
        QMessageBox::information(this, tr("Convex Boolean"),
                                 tr("Add at least three points for the first polygon."));
        return;
    }

    m_booleanCanvas->finalizeFirstPolygon();
    m_booleanOuters.clear();
    m_booleanHoles.clear();
    updateBooleanControls();
    updateBooleanSummary();
}

void MainWindow::onBooleanFinalizeSecondClicked() {
    if (!m_booleanCanvas) {
        return;
    }
    if (m_booleanCanvas->secondRawPoints().size() < 3) {
        QMessageBox::information(this, tr("Convex Boolean"),
                                 tr("Add at least three points for the second polygon."));
        return;
    }

    m_booleanCanvas->finalizeSecondPolygon();
    if (m_booleanLive) {
        recomputeBooleanOperation();
    } else {
        updateBooleanSummary();
    }
    updateBooleanControls();
}

void MainWindow::onBooleanComputeClicked() {
    recomputeBooleanOperation();
}

void MainWindow::onBooleanOperationChanged(int /*index*/) {
    if (m_booleanLive) {
        recomputeBooleanOperation();
    } else {
        updateBooleanSummary();
    }
}

void MainWindow::onBooleanLiveToggled(bool checked) {
    m_booleanLive = checked;
    if (m_booleanLive && m_booleanCanvas &&
        m_booleanCanvas->phase() == ConvexPolygonBooleWidget::Phase::ResultReady) {
        recomputeBooleanOperation();
    }
}

void MainWindow::onBooleanClearClicked() {
    m_booleanLive = false;
    if (ui->booleanLiveCheckBox) {
        ui->booleanLiveCheckBox->setChecked(false);
    }
    m_booleanOuters.clear();
    m_booleanHoles.clear();
    m_booleanStatusMessage.clear();
    if (m_booleanCanvas) {
        m_booleanCanvas->clearAll();
    }
    updateBooleanControls();
    updateBooleanSummary();
}

void MainWindow::onBooleanPhaseChanged(ConvexPolygonBooleWidget::Phase /*phase*/) {
    updateBooleanControls();
    if (m_booleanLive && m_booleanCanvas &&
        m_booleanCanvas->phase() == ConvexPolygonBooleWidget::Phase::ResultReady) {
        recomputeBooleanOperation();
    } else {
        updateBooleanSummary();
    }
}

void MainWindow::updateBooleanSummary() {
    QStringList lines;

    if (!m_booleanCanvas) {
        lines << tr("Boolean widget is not available.");
    } else {
        const int firstRaw = m_booleanCanvas->firstRawPoints().size();
        const int secondRaw = m_booleanCanvas->secondRawPoints().size();
        const int firstHull = m_booleanCanvas->firstConvexHull().size();
        const int secondHull = m_booleanCanvas->secondConvexHull().size();

        lines << tr("First polygon points: %1").arg(firstRaw);
        lines << tr("Second polygon points: %1").arg(secondRaw);
        lines << tr("First hull vertices: %1").arg(firstHull);
        lines << tr("Second hull vertices: %1").arg(secondHull);

        switch (m_booleanCanvas->phase()) {
        case ConvexPolygonBooleWidget::Phase::FirstPolygonInput:
            lines << tr("Stage: editing first polygon");
            break;
        case ConvexPolygonBooleWidget::Phase::SecondPolygonInput:
            lines << tr("Stage: editing second polygon");
            break;
        case ConvexPolygonBooleWidget::Phase::ResultReady:
            lines << tr("Stage: result ready");
            break;
        }
    }

    const auto totalVertices = [](const QVector<QPolygonF>& polygons) {
        int sum = 0;
        for (const auto& poly : polygons) {
            sum += poly.size();
        }
        return sum;
    };

    bool statusAppended = false;
    if (!m_booleanOuters.isEmpty()) {
        lines << QString();
        lines << tr("Result components: %1").arg(m_booleanOuters.size());
        lines << tr("Result vertices: %1").arg(totalVertices(m_booleanOuters));
        if (!m_booleanHoles.isEmpty()) {
            lines << tr("Contained holes: %1").arg(m_booleanHoles.size());
        }
    } else if (m_booleanCanvas &&
               m_booleanCanvas->phase() == ConvexPolygonBooleWidget::Phase::ResultReady) {
        lines << QString();
        if (!m_booleanStatusMessage.isEmpty()) {
            lines << m_booleanStatusMessage;
            statusAppended = true;
        } else {
            lines << tr("Result polygon is empty.");
        }
    }

    if (!m_booleanStatusMessage.isEmpty() && !statusAppended) {
        if (lines.isEmpty() || !lines.back().isEmpty()) {
            lines << QString();
        }
        lines << m_booleanStatusMessage;
    }

    m_booleanSummary = lines.join('\n');
    if (ui->booleanSummaryBrowser) {
        ui->booleanSummaryBrowser->setPlainText(m_booleanSummary);
    }
}

void MainWindow::recomputeBooleanOperation() {
    if (!m_booleanCanvas) {
        return;
    }

    m_booleanStatusMessage.clear();
    m_booleanOuters.clear();
    m_booleanHoles.clear();

    const QVector<QPointF> firstHull = m_booleanCanvas->firstConvexHull();
    const QVector<QPointF> secondHull = m_booleanCanvas->secondConvexHull();
    if (firstHull.size() < 3 || secondHull.size() < 3) {
        m_booleanCanvas->clearResultPolygon();
        m_booleanStatusMessage = tr("Add at least three vertices to each polygon.");
        updateBooleanSummary();
        return;
    }

    const auto operation = currentBooleanOperation();
    const auto firstGeometry = toGeometryPoints(firstHull);
    const auto secondGeometry = toGeometryPoints(secondHull);
    const auto intersectionPolygon = plane_geometry::intersectConvexPolygons(firstGeometry, secondGeometry);

    const auto containsPolygon = [](const std::vector<plane_geometry::Point2D<double>>& outer,
                                    const std::vector<plane_geometry::Point2D<double>>& inner) {
        if (outer.size() < 3 || inner.empty()) {
            return false;
        }
        for (const auto& point : inner) {
            const auto classification = plane_geometry::locatePointInConvexPolygon(outer, point);
            if (classification == plane_geometry::PointClassification::Outside) {
                return false;
            }
        }
        return true;
    };

    const bool firstContainsSecond = containsPolygon(firstGeometry, secondGeometry);
    const bool secondContainsFirst = containsPolygon(secondGeometry, firstGeometry);
    const bool polygonsDisjoint = intersectionPolygon.empty();

    plane_geometry::BooleanResult<double> booleanResult;

    switch (operation) {
    case BooleanOperation::Intersection:
        if (!intersectionPolygon.empty()) {
            booleanResult.outers.push_back(intersectionPolygon);
        }
        break;
    case BooleanOperation::Union:
        booleanResult = plane_geometry::unionConvexPolygons(firstGeometry, secondGeometry);
        break;
    case BooleanOperation::Difference:
        booleanResult = plane_geometry::differenceConvexPolygons(firstGeometry, secondGeometry);
        break;
    }

    const auto toQPolygon = [](const plane_geometry::Polygon<double>& polygon) {
        QPolygonF result;
        result.reserve(static_cast<int>(polygon.size()));
        for (const auto& vertex : polygon) {
            result.append(QPointF(vertex.x, vertex.y));
        }
        return result;
    };

    QStringList statusHints;

    if (booleanResult.empty()) {
        m_booleanCanvas->clearResultPolygon();

        switch (operation) {
        case BooleanOperation::Intersection:
            statusHints << (polygonsDisjoint ? tr("Polygons do not intersect.")
                                             : tr("Intersection is empty."));
            break;
        case BooleanOperation::Union:
            statusHints << tr("Union result is empty.");
            break;
        case BooleanOperation::Difference:
            if (secondContainsFirst) {
                statusHints << tr("Second polygon completely covers the first; difference is empty.");
            } else if (polygonsDisjoint) {
                statusHints << tr("Polygons do not overlap; difference equals the first polygon, but no boundary produced.");
            } else {
                statusHints << tr("Difference is empty.");
            }
            break;
        }
    } else {
        m_booleanOuters.reserve(booleanResult.outers.size());
        for (const auto& outer : booleanResult.outers) {
            m_booleanOuters.append(toQPolygon(outer));
        }
        m_booleanHoles.reserve(booleanResult.holes.size());
        for (const auto& hole : booleanResult.holes) {
            m_booleanHoles.append(toQPolygon(hole));
        }

        m_booleanCanvas->setResultPolygons(m_booleanOuters, m_booleanHoles);

        if (booleanResult.outers.size() > 1) {
            statusHints << tr("Result contains %1 disjoint regions.")
                              .arg(booleanResult.outers.size());
        }
        if (!booleanResult.holes.empty()) {
            statusHints << tr("Result includes %1 hole(s).")
                              .arg(booleanResult.holes.size());
        }

        if (operation == BooleanOperation::Union) {
            if (firstContainsSecond) {
                statusHints << tr("Second polygon lies entirely inside the first; union equals the first polygon.");
            } else if (secondContainsFirst) {
                statusHints << tr("First polygon lies entirely inside the second; union equals the second polygon.");
            } else if (polygonsDisjoint && booleanResult.outers.size() > 1) {
                statusHints << tr("Polygons are disjoint; union shows both components.");
            }
        } else if (operation == BooleanOperation::Difference) {
            if (polygonsDisjoint) {
                statusHints << tr("Polygons do not overlap; the difference equals the first polygon.");
            } else if (secondContainsFirst) {
                // Already handled by empty case, but keep for completeness.
                statusHints << tr("Subtracted polygon covers the first.");
            } else if (firstContainsSecond && !booleanResult.holes.empty()) {
                statusHints << tr("Second polygon lies inside the first; interior hole created.");
            }
        }
    }

    m_booleanStatusMessage = statusHints.join(QStringLiteral(" "));
    updateBooleanSummary();
}

MainWindow::BooleanOperation MainWindow::currentBooleanOperation() const {
    const int index = ui->booleanOperationCombo ? ui->booleanOperationCombo->currentIndex() : 0;
    switch (index) {
    case 1:
        return BooleanOperation::Union;
    case 2:
        return BooleanOperation::Difference;
    default:
        return BooleanOperation::Intersection;
    }
}

void MainWindow::updateBooleanControls() {
    const auto setEnabled = [](QWidget* widget, bool enabled) {
        if (widget) {
            widget->setEnabled(enabled);
        }
    };

    if (!m_booleanCanvas) {
        setEnabled(ui->booleanFinalizeFirstButton, false);
        setEnabled(ui->booleanFinalizeSecondButton, false);
        setEnabled(ui->booleanComputeButton, false);
        setEnabled(ui->booleanOperationCombo, false);
        setEnabled(ui->booleanLiveCheckBox, false);
        return;
    }

    const auto phase = m_booleanCanvas->phase();
    const int firstCount = m_booleanCanvas->firstRawPoints().size();
    const int secondCount = m_booleanCanvas->secondRawPoints().size();

    setEnabled(ui->booleanFinalizeFirstButton,
               phase == ConvexPolygonBooleWidget::Phase::FirstPolygonInput && firstCount >= 3);
    setEnabled(ui->booleanFinalizeSecondButton,
               phase == ConvexPolygonBooleWidget::Phase::SecondPolygonInput && secondCount >= 3);

    const bool resultReady = phase == ConvexPolygonBooleWidget::Phase::ResultReady;
    setEnabled(ui->booleanComputeButton, resultReady);
    setEnabled(ui->booleanOperationCombo, resultReady);
    setEnabled(ui->booleanLiveCheckBox, resultReady);
}

QVector<QPointF> MainWindow::toQPointVector(const std::vector<plane_geometry::Point2D<double>>& polygon) {
    QVector<QPointF> result;
    result.reserve(static_cast<int>(polygon.size()));
    for (const auto& point : polygon) {
        result.append(QPointF(point.x, point.y));
    }
    return result;
}

std::vector<plane_geometry::Point2D<double>> MainWindow::toGeometryPoints(const QVector<QPointF>& points) {
    std::vector<plane_geometry::Point2D<double>> result;
    result.reserve(points.size());
    for (const QPointF& point : points) {
        result.push_back({point.x(), point.y()});
    }
    return result;
}

void MainWindow::onPointLocationPointsChanged(const QVector<QPointF>& /*points*/) {
    updatePointLocationSummary(m_pointLocationStatusText);
}

void MainWindow::onPointLocationHullReady(const QVector<QPointF>& /*hull*/) {
    m_pointLocationStatusText = tr("Convex hull ready. Add a test point.");
    updatePointLocationSummary(m_pointLocationStatusText);
}

void MainWindow::onPointLocationTestPointChanged(const QPointF& point) {
    m_pointLocationHasTestPoint = true;
    m_pointLocationTestPoint = point;
    updatePointLocationSummary(m_pointLocationStatusText);
}

void MainWindow::onPointLocationSummaryChanged(plane_geometry::PointClassification /*classification*/,
                                               const QString& description) {
    m_pointLocationStatusText = description;
    updatePointLocationSummary(m_pointLocationStatusText);
}

void MainWindow::onPointLocationFinalizeClicked() {
    if (m_pointLocationCanvas) {
        m_pointLocationCanvas->finalizePolygon();
    }
}

void MainWindow::onPointLocationClearClicked() {
    if (m_pointLocationCanvas) {
        m_pointLocationCanvas->clearAll();
    }
    m_pointLocationHasTestPoint = false;
    m_pointLocationStatusText = tr("Add points and build the convex hull.");
    updatePointLocationSummary(m_pointLocationStatusText);
}

void MainWindow::onPointLocationRemoveTestClicked() {
    if (m_pointLocationCanvas) {
        m_pointLocationCanvas->removeTestPoint();
    }
    m_pointLocationHasTestPoint = false;
    updatePointLocationSummary(m_pointLocationStatusText);
}

void MainWindow::onPointLocationDragToggled(bool checked) {
    if (m_pointLocationCanvas) {
        m_pointLocationCanvas->setDraggingEnabled(checked);
    }
}

void MainWindow::updatePointLocationSummary(const QString& description) {
    if (!description.isEmpty()) {
        m_pointLocationStatusText = description;
    } else if (m_pointLocationStatusText.isEmpty()) {
        m_pointLocationStatusText = tr("Add points and build the convex hull.");
    }

    QStringList lines;
    const int pointCount = m_pointLocationCanvas ? m_pointLocationCanvas->rawPoints().size() : 0;
    const bool polygonReady = m_pointLocationCanvas && m_pointLocationCanvas->polygonReady();

    lines << tr("Polygon points: %1").arg(pointCount);
    lines << tr("Hull ready: %1").arg(polygonReady ? tr("yes") : tr("no"));
    lines << tr("Test point: %1").arg(m_pointLocationHasTestPoint ? tr("present") : tr("not set"));

    if (m_pointLocationHasTestPoint) {
        lines << tr("Test point coordinates: %1").arg(formatPoint(m_pointLocationTestPoint));
    }

    if (!m_pointLocationStatusText.isEmpty()) {
        lines << QString();
        lines << m_pointLocationStatusText;
    }

    m_pointLocationSummary = lines.join('\n');
    if (ui->pointLocationSummaryBrowser) {
        ui->pointLocationSummaryBrowser->setPlainText(m_pointLocationSummary);
    }
}

void MainWindow::onBezierControlPointsChanged(const QVector<QPointF>& /*points*/) {
    updateBezierInfo();
}

void MainWindow::onBezierCurveUpdated(const QVector<QPointF>& curve) {
    m_bezierCurvePoints = curve;
    updateBezierInfo();
}

void MainWindow::onBezierDegreeChanged(int index) {
    if (m_bezierCanvas) {
        m_bezierCanvas->setDegree(index + 1);
        updateBezierInfo();
    }
}

void MainWindow::onBezierSamplesChanged(int value) {
    if (m_bezierCanvas) {
        m_bezierCanvas->setSampleCount(value);
        updateBezierInfo();
    }
}

void MainWindow::onBezierDragToggled(bool checked) {
    if (m_bezierCanvas) {
        m_bezierCanvas->setDraggingEnabled(checked);
    }
}

void MainWindow::onBezierClearClicked() {
    if (m_bezierCanvas) {
        m_bezierCanvas->clearControlPoints();
    }
    m_bezierCurvePoints.clear();
    updateBezierInfo();
}

void MainWindow::updateBezierInfo() {
    QStringList lines;
    if (!m_bezierCanvas) {
        lines << tr("Bezier widget is not available.");
    } else {
        const int degree = m_bezierCanvas->degree();
        const int required = degree + 1;
        const int controlCount = m_bezierCanvas->controlPoints().size();
        lines << tr("Degree: %1").arg(degree);
        lines << tr("Control points: %1 / %2").arg(controlCount).arg(required);
        lines << tr("Sample count: %1").arg(m_bezierCanvas->sampleCount());
        if (controlCount < required) {
            lines << QString();
            lines << tr("Add %1 more control point(s).").arg(required - controlCount);
        } else {
            lines << tr("Curve samples generated: %1").arg(m_bezierCurvePoints.size());
        }
    }

    m_bezierInfo = lines.join('\n');
    if (ui->bezierInfoBrowser) {
        ui->bezierInfoBrowser->setPlainText(m_bezierInfo);
    }
}
