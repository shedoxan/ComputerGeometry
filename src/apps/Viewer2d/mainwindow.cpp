#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>
#include <QStringList>
#include <limits>

#include "segmentintersectionwidget.h"
#include "drawwidget.h"
#include "precisiondrawwidget.h"


constexpr double kIntersectionTolerance = 1e-4;

PlanePointD toPlanePoint(const QPointF& point) {
    return {point.x(), -point.y()};
}

QPointF toWidgetPoint(const PlanePointD& point) {
    return {point.x, -point.y};
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    ui->radioButton->setEnabled(true);
    ui->radioButton_2->setEnabled(true);
    ui->radioButton_3->setEnabled(true);
    ui->pushButton->setEnabled(false);

    connect(ui->widget, &DrawWidget::pointsChanged, this, &MainWindow::updatePoints);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onCheckOrientation);

    ui->computeIntersectionButton->setEnabled(false);
    ui->liveIntersectionCheckBox->setChecked(false);
    ui->intersectionRadioNone->setEnabled(true);
    ui->intersectionRadioPoint->setEnabled(true);
    ui->intersectionRadioOverlap->setEnabled(true);
    ui->intersectionRadioNone->setChecked(true);

    connect(ui->intersectionWidget, &SegmentIntersectionWidget::segmentsChanged,
            this, &MainWindow::onSegmentsChanged);
    connect(ui->computeIntersectionButton, &QPushButton::clicked,
            this, &MainWindow::onComputeIntersectionClicked);
    connect(ui->liveIntersectionCheckBox, &QCheckBox::toggled,
            this, &MainWindow::onLiveIntersectionToggled);
    connect(ui->clearSegmentsButton, &QPushButton::clicked,
            this, &MainWindow::onClearSegmentsClicked);
    ui->intersectionWidget->setDragEnabled(false);

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

    updatePointsSummary();
    updateIntersectionSummary();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePoints(const QVector<QPointF>& points)
{
    m_points = points;
    ui->pushButton->setEnabled(m_points.size() >= 3);

    if (m_points.size() < 3) {
        m_hasOrientationResult = false;
        m_lastResultSummary.clear();
    }

    applyOrientationResult(m_hasOrientationResult, m_lastOrientation);
    updatePointsSummary();
}

void MainWindow::onCheckOrientation()
{
    if (m_points.size() < 3) {
        ui->textBrowser->append("Add two points for the segment and one point to test.");
        return;
    }

    const PlaneSegmentD segment{
        toPlanePoint(m_points[0]),
        toPlanePoint(m_points[1])
    };
    const PlanePointD testPoint = toPlanePoint(m_points[2]);

    const auto orientation = plane_geometry::classifyPointRelativeToSegment(segment, testPoint);

    m_hasOrientationResult = true;
    m_lastOrientation = orientation;

    const int orientationValue = static_cast<int>(orientation);
    const QString description = orientationDescription(orientation);
    m_lastResultSummary = QString("Result: %1 (%2)").arg(orientationValue).arg(description);

    applyOrientationResult(true, orientation);
    updatePointsSummary();
}

void MainWindow::applyOrientationResult(bool hasResult, plane_geometry::Orientation orientation)
{
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

QString MainWindow::orientationDescription(plane_geometry::Orientation orientation) const
{
    switch (orientation) {
    case plane_geometry::Orientation::Left:
        return "point is left of the segment";
    case plane_geometry::Orientation::OnSegment:
        return "point lies on the segment";
    case plane_geometry::Orientation::Right:
        return "point is right of the segment";
    }
    return "unknown orientation";
}

void MainWindow::updatePointsSummary()
{
    QStringList lines;
    const QStringList labels = {"A", "B", "C"};

    for (int i = 0; i < m_points.size(); ++i) {
        const QString label = i < labels.size() ? labels[i] : QString("C%1").arg(i + 1);
        lines << QString("%1 = (%2, %3)")
                    .arg(label)
                    .arg(QString::number(m_points[i].x(), 'f', 2))
                    .arg(QString::number(m_points[i].y(), 'f', 2));
    }

    if (m_points.isEmpty()) {
        lines << "Click two segment endpoints and then the test point.";
    } else if (m_points.size() == 1) {
        lines << "Add one more point to complete the segment.";
    } else if (m_points.size() == 2) {
        lines << "Click the test point to check its position.";
    } else { // m_points.size() >= 3
        if (m_hasOrientationResult && !m_lastResultSummary.isEmpty()) {
            lines << m_lastResultSummary;
        } else {
            lines << "Press \"Check point position\" to evaluate the third point.";
        }
    }

    ui->textBrowser->setPlainText(lines.join('\n'));
}

void MainWindow::onSegmentsChanged(const QVector<QPointF>& points)
{
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

void MainWindow::onComputeIntersectionClicked()
{
    if (!intersectionSegmentsReady()) {
        m_hasIntersectionResult = false;
        m_lastIntersectionSummary = "Add four points (A, B, C, D) to define two segments.";
        updateIntersectionSummary();
        return;
    }

    applyIntersectionResult();
}

void MainWindow::onLiveIntersectionToggled(bool checked)
{
    m_liveIntersection = checked;
    ui->intersectionWidget->setDragEnabled(checked);
    if (m_liveIntersection && intersectionSegmentsReady()) {
        applyIntersectionResult();
    }
}

void MainWindow::onClearSegmentsClicked()
{
    m_segmentPoints.clear();
    m_hasIntersectionResult = false;
    m_lastIntersectionSummary.clear();
    ui->intersectionWidget->clearSegments();
    ui->intersectionRadioNone->setChecked(true);
    ui->intersectionRadioPoint->setChecked(false);
    ui->intersectionRadioOverlap->setChecked(false);
    updateIntersectionSummary();
}

void MainWindow::updateIntersectionSummary()
{
    QStringList lines;
    const QStringList labels = {"A", "B", "C", "D"};

    for (int i = 0; i < m_segmentPoints.size(); ++i) {
        const QString label = i < labels.size() ? labels[i] : QString("P%1").arg(i + 1);
        lines << QString("%1 = (%2, %3)")
                    .arg(label)
                    .arg(QString::number(m_segmentPoints[i].x(), 'f', 2))
                    .arg(QString::number(m_segmentPoints[i].y(), 'f', 2));
    }

    if (m_segmentPoints.isEmpty()) {
        lines << "Click to set A-B, then C-D. Right-click clears all points.";
    } else if (m_segmentPoints.size() < 4) {
        lines << "Add points until both segments are defined (A-B and C-D).";
    }

    if (!m_lastIntersectionSummary.isEmpty()) {
        lines << m_lastIntersectionSummary;
    } else if (intersectionSegmentsReady() && !m_liveIntersection) {
        lines << "Press \"Find intersection\" to compute.";
    }

    ui->intersectionTextBrowser->setPlainText(lines.join('\n'));
}

void MainWindow::applyIntersectionResult()
{
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
        m_lastIntersectionSummary = "Segments do not intersect.";
        break;
    case plane_geometry::IntersectionType::Point: {
        const QPointF point = toWidgetPoint(m_lastIntersection.point);
        ui->intersectionWidget->setIntersectionPoint(point);
        ui->intersectionRadioPoint->setChecked(true);
        m_lastIntersectionSummary = QString("Intersection at (%1, %2)")
                                        .arg(QString::number(point.x(), 'f', 2))
                                        .arg(QString::number(point.y(), 'f', 2));
        break;
    }
    case plane_geometry::IntersectionType::Overlap: {
        const QPointF start = toWidgetPoint(m_lastIntersection.overlap.start);
        const QPointF end = toWidgetPoint(m_lastIntersection.overlap.end);
        ui->intersectionWidget->setIntersectionOverlap(start, end);
        ui->intersectionRadioOverlap->setChecked(true);
        m_lastIntersectionSummary = QString("Segments overlap from (%1, %2) to (%3, %4)")
                                        .arg(QString::number(start.x(), 'f', 2))
                                        .arg(QString::number(start.y(), 'f', 2))
                                        .arg(QString::number(end.x(), 'f', 2))
                                        .arg(QString::number(end.y(), 'f', 2));
        break;
    }
    }

    updateIntersectionSummary();
}

bool MainWindow::intersectionSegmentsReady() const
{
    return m_segmentPoints.size() >= 4;
}

SegmentIntersectionResultD MainWindow::evaluateIntersection() const
{
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

void MainWindow::onPrecisionCheckButtonClicked()
{
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

void MainWindow::onPrecisionClearButtonClicked()
{
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
    const QVector<plane_geometry::Point2D<plane_geometry::ExactScalar>>& points)
{
    const auto formatPoint = [](const plane_geometry::Point2D<plane_geometry::ExactScalar>& point) {
        return QStringLiteral("(%1, %2)")
            .arg(precision_converter::formatScalar(point.x))
            .arg(precision_converter::formatScalar(point.y));
    };

    if (ui->precisionSegmentStartEdit) {
        if (points.size() >= 1) {
            ui->precisionSegmentStartEdit->setText(formatPoint(points[0]));
        } else {
            ui->precisionSegmentStartEdit->clear();
        }
    }

    if (ui->precisionSegmentEndEdit) {
        if (points.size() >= 2) {
            ui->precisionSegmentEndEdit->setText(formatPoint(points[1]));
        } else {
            ui->precisionSegmentEndEdit->clear();
        }
    }

    if (ui->precisionPointEdit) {
        if (points.size() >= 3) {
            ui->precisionPointEdit->setText(formatPoint(points[2]));
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
