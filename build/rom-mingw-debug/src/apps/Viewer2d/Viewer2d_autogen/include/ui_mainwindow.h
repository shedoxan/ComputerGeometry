/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <bezierwidget.h>
#include <convexhullwidget.h>
#include <convexpolygonboolewidget.h>
#include <drawwidget.h>
#include <pointlocationwidget.h>
#include <precisiondrawwidget.h>
#include <segmentintersectionwidget.h>
#include <triangulationwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *orientationLayout;
    DrawWidget *widget;
    QWidget *orientationPanel;
    QVBoxLayout *orientationLayoutPanel;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QPushButton *pushButton;
    QTextBrowser *textBrowser;
    QWidget *tab_2;
    QHBoxLayout *intersectionLayout;
    SegmentIntersectionWidget *intersectionWidget;
    QWidget *intersectionPanel;
    QVBoxLayout *verticalLayoutIntersection;
    QRadioButton *intersectionRadioNone;
    QRadioButton *intersectionRadioPoint;
    QRadioButton *intersectionRadioOverlap;
    QPushButton *computeIntersectionButton;
    QCheckBox *liveIntersectionCheckBox;
    QPushButton *clearSegmentsButton;
    QTextBrowser *intersectionTextBrowser;
    QWidget *hullTab;
    QHBoxLayout *hullLayout;
    ConvexHullWidget *hullCanvas;
    QWidget *hullPanel;
    QVBoxLayout *hullPanelLayout;
    QPushButton *buildHullButton;
    QCheckBox *liveHullCheckBox;
    QPushButton *clearHullButton;
    QTextBrowser *hullTextBrowser;
    QWidget *precisionTab;
    QHBoxLayout *precisionMainLayout;
    PrecisionDrawWidget *precisionCanvas;
    QWidget *precisionPanel;
    QVBoxLayout *precisionLayout;
    QLineEdit *precisionSegmentStartEdit;
    QLineEdit *precisionSegmentEndEdit;
    QLineEdit *precisionPointEdit;
    QLineEdit *precisionEpsilonEdit;
    QPushButton *precisionCheckButton;
    QPushButton *precisionClearButton;
    QTextBrowser *precisionResultBrowser;
    QWidget *triangulationTab;
    QHBoxLayout *triangulationLayout;
    TriangulationWidget *triangulationCanvas;
    QWidget *triangulationPanel;
    QVBoxLayout *triangulationPanelLayout;
    QPushButton *triangulationComputeButton;
    QCheckBox *triangulationLiveCheckBox;
    QCheckBox *triangulationDragCheckBox;
    QPushButton *triangulationClearButton;
    QTextBrowser *triangulationSummaryBrowser;
    QWidget *convexBooleanTab;
    QHBoxLayout *convexBooleanLayout;
    ConvexPolygonBooleWidget *convexBooleanCanvas;
    QWidget *convexBooleanPanel;
    QVBoxLayout *convexBooleanPanelLayout;
    QPushButton *booleanFinalizeFirstButton;
    QPushButton *booleanFinalizeSecondButton;
    QComboBox *booleanOperationCombo;
    QPushButton *booleanComputeButton;
    QCheckBox *booleanLiveCheckBox;
    QPushButton *booleanClearButton;
    QTextBrowser *booleanSummaryBrowser;
    QWidget *pointLocationTab;
    QHBoxLayout *pointLocationLayout;
    PointLocationWidget *pointLocationCanvas;
    QWidget *pointLocationPanel;
    QVBoxLayout *pointLocationPanelLayout;
    QPushButton *pointLocationFinalizeButton;
    QPushButton *pointLocationClearButton;
    QPushButton *pointLocationRemoveTestButton;
    QCheckBox *pointLocationDragCheckBox;
    QTextBrowser *pointLocationSummaryBrowser;
    QWidget *bezierTab;
    QHBoxLayout *bezierLayout;
    BezierWidget *bezierCanvas;
    QWidget *bezierPanel;
    QVBoxLayout *bezierPanelLayout;
    QComboBox *bezierDegreeCombo;
    QSpinBox *bezierSamplesSpin;
    QCheckBox *bezierDragCheckBox;
    QPushButton *bezierClearButton;
    QTextBrowser *bezierInfoBrowser;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1100, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setObjectName("centralLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        orientationLayout = new QHBoxLayout(tab);
        orientationLayout->setObjectName("orientationLayout");
        widget = new DrawWidget(tab);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(400, 400));

        orientationLayout->addWidget(widget);

        orientationPanel = new QWidget(tab);
        orientationPanel->setObjectName("orientationPanel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(orientationPanel->sizePolicy().hasHeightForWidth());
        orientationPanel->setSizePolicy(sizePolicy1);
        orientationLayoutPanel = new QVBoxLayout(orientationPanel);
        orientationLayoutPanel->setObjectName("orientationLayoutPanel");
        orientationLayoutPanel->setContentsMargins(0, 0, 0, 0);
        radioButton = new QRadioButton(orientationPanel);
        radioButton->setObjectName("radioButton");

        orientationLayoutPanel->addWidget(radioButton);

        radioButton_2 = new QRadioButton(orientationPanel);
        radioButton_2->setObjectName("radioButton_2");

        orientationLayoutPanel->addWidget(radioButton_2);

        radioButton_3 = new QRadioButton(orientationPanel);
        radioButton_3->setObjectName("radioButton_3");

        orientationLayoutPanel->addWidget(radioButton_3);

        pushButton = new QPushButton(orientationPanel);
        pushButton->setObjectName("pushButton");

        orientationLayoutPanel->addWidget(pushButton);

        textBrowser = new QTextBrowser(orientationPanel);
        textBrowser->setObjectName("textBrowser");
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);

        orientationLayoutPanel->addWidget(textBrowser);


        orientationLayout->addWidget(orientationPanel);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        intersectionLayout = new QHBoxLayout(tab_2);
        intersectionLayout->setObjectName("intersectionLayout");
        intersectionWidget = new SegmentIntersectionWidget(tab_2);
        intersectionWidget->setObjectName("intersectionWidget");
        sizePolicy.setHeightForWidth(intersectionWidget->sizePolicy().hasHeightForWidth());
        intersectionWidget->setSizePolicy(sizePolicy);
        intersectionWidget->setMinimumSize(QSize(400, 400));

        intersectionLayout->addWidget(intersectionWidget);

        intersectionPanel = new QWidget(tab_2);
        intersectionPanel->setObjectName("intersectionPanel");
        sizePolicy1.setHeightForWidth(intersectionPanel->sizePolicy().hasHeightForWidth());
        intersectionPanel->setSizePolicy(sizePolicy1);
        verticalLayoutIntersection = new QVBoxLayout(intersectionPanel);
        verticalLayoutIntersection->setObjectName("verticalLayoutIntersection");
        verticalLayoutIntersection->setContentsMargins(0, 0, 0, 0);
        intersectionRadioNone = new QRadioButton(intersectionPanel);
        intersectionRadioNone->setObjectName("intersectionRadioNone");

        verticalLayoutIntersection->addWidget(intersectionRadioNone);

        intersectionRadioPoint = new QRadioButton(intersectionPanel);
        intersectionRadioPoint->setObjectName("intersectionRadioPoint");

        verticalLayoutIntersection->addWidget(intersectionRadioPoint);

        intersectionRadioOverlap = new QRadioButton(intersectionPanel);
        intersectionRadioOverlap->setObjectName("intersectionRadioOverlap");

        verticalLayoutIntersection->addWidget(intersectionRadioOverlap);

        computeIntersectionButton = new QPushButton(intersectionPanel);
        computeIntersectionButton->setObjectName("computeIntersectionButton");

        verticalLayoutIntersection->addWidget(computeIntersectionButton);

        liveIntersectionCheckBox = new QCheckBox(intersectionPanel);
        liveIntersectionCheckBox->setObjectName("liveIntersectionCheckBox");

        verticalLayoutIntersection->addWidget(liveIntersectionCheckBox);

        clearSegmentsButton = new QPushButton(intersectionPanel);
        clearSegmentsButton->setObjectName("clearSegmentsButton");

        verticalLayoutIntersection->addWidget(clearSegmentsButton);

        intersectionTextBrowser = new QTextBrowser(intersectionPanel);
        intersectionTextBrowser->setObjectName("intersectionTextBrowser");
        sizePolicy1.setHeightForWidth(intersectionTextBrowser->sizePolicy().hasHeightForWidth());
        intersectionTextBrowser->setSizePolicy(sizePolicy1);

        verticalLayoutIntersection->addWidget(intersectionTextBrowser);


        intersectionLayout->addWidget(intersectionPanel);

        tabWidget->addTab(tab_2, QString());
        hullTab = new QWidget();
        hullTab->setObjectName("hullTab");
        hullLayout = new QHBoxLayout(hullTab);
        hullLayout->setObjectName("hullLayout");
        hullCanvas = new ConvexHullWidget(hullTab);
        hullCanvas->setObjectName("hullCanvas");
        sizePolicy.setHeightForWidth(hullCanvas->sizePolicy().hasHeightForWidth());
        hullCanvas->setSizePolicy(sizePolicy);
        hullCanvas->setMinimumSize(QSize(400, 400));

        hullLayout->addWidget(hullCanvas);

        hullPanel = new QWidget(hullTab);
        hullPanel->setObjectName("hullPanel");
        sizePolicy1.setHeightForWidth(hullPanel->sizePolicy().hasHeightForWidth());
        hullPanel->setSizePolicy(sizePolicy1);
        hullPanelLayout = new QVBoxLayout(hullPanel);
        hullPanelLayout->setObjectName("hullPanelLayout");
        hullPanelLayout->setContentsMargins(0, 0, 0, 0);
        buildHullButton = new QPushButton(hullPanel);
        buildHullButton->setObjectName("buildHullButton");

        hullPanelLayout->addWidget(buildHullButton);

        liveHullCheckBox = new QCheckBox(hullPanel);
        liveHullCheckBox->setObjectName("liveHullCheckBox");

        hullPanelLayout->addWidget(liveHullCheckBox);

        clearHullButton = new QPushButton(hullPanel);
        clearHullButton->setObjectName("clearHullButton");

        hullPanelLayout->addWidget(clearHullButton);

        hullTextBrowser = new QTextBrowser(hullPanel);
        hullTextBrowser->setObjectName("hullTextBrowser");
        sizePolicy1.setHeightForWidth(hullTextBrowser->sizePolicy().hasHeightForWidth());
        hullTextBrowser->setSizePolicy(sizePolicy1);

        hullPanelLayout->addWidget(hullTextBrowser);


        hullLayout->addWidget(hullPanel);

        tabWidget->addTab(hullTab, QString());
        precisionTab = new QWidget();
        precisionTab->setObjectName("precisionTab");
        precisionMainLayout = new QHBoxLayout(precisionTab);
        precisionMainLayout->setObjectName("precisionMainLayout");
        precisionCanvas = new PrecisionDrawWidget(precisionTab);
        precisionCanvas->setObjectName("precisionCanvas");
        sizePolicy.setHeightForWidth(precisionCanvas->sizePolicy().hasHeightForWidth());
        precisionCanvas->setSizePolicy(sizePolicy);
        precisionCanvas->setMinimumSize(QSize(400, 400));

        precisionMainLayout->addWidget(precisionCanvas);

        precisionPanel = new QWidget(precisionTab);
        precisionPanel->setObjectName("precisionPanel");
        sizePolicy1.setHeightForWidth(precisionPanel->sizePolicy().hasHeightForWidth());
        precisionPanel->setSizePolicy(sizePolicy1);
        precisionLayout = new QVBoxLayout(precisionPanel);
        precisionLayout->setObjectName("precisionLayout");
        precisionLayout->setContentsMargins(0, 0, 0, 0);
        precisionSegmentStartEdit = new QLineEdit(precisionPanel);
        precisionSegmentStartEdit->setObjectName("precisionSegmentStartEdit");

        precisionLayout->addWidget(precisionSegmentStartEdit);

        precisionSegmentEndEdit = new QLineEdit(precisionPanel);
        precisionSegmentEndEdit->setObjectName("precisionSegmentEndEdit");

        precisionLayout->addWidget(precisionSegmentEndEdit);

        precisionPointEdit = new QLineEdit(precisionPanel);
        precisionPointEdit->setObjectName("precisionPointEdit");

        precisionLayout->addWidget(precisionPointEdit);

        precisionEpsilonEdit = new QLineEdit(precisionPanel);
        precisionEpsilonEdit->setObjectName("precisionEpsilonEdit");

        precisionLayout->addWidget(precisionEpsilonEdit);

        precisionCheckButton = new QPushButton(precisionPanel);
        precisionCheckButton->setObjectName("precisionCheckButton");

        precisionLayout->addWidget(precisionCheckButton);

        precisionClearButton = new QPushButton(precisionPanel);
        precisionClearButton->setObjectName("precisionClearButton");

        precisionLayout->addWidget(precisionClearButton);

        precisionResultBrowser = new QTextBrowser(precisionPanel);
        precisionResultBrowser->setObjectName("precisionResultBrowser");
        sizePolicy1.setHeightForWidth(precisionResultBrowser->sizePolicy().hasHeightForWidth());
        precisionResultBrowser->setSizePolicy(sizePolicy1);

        precisionLayout->addWidget(precisionResultBrowser);


        precisionMainLayout->addWidget(precisionPanel);

        tabWidget->addTab(precisionTab, QString());
        triangulationTab = new QWidget();
        triangulationTab->setObjectName("triangulationTab");
        triangulationLayout = new QHBoxLayout(triangulationTab);
        triangulationLayout->setObjectName("triangulationLayout");
        triangulationCanvas = new TriangulationWidget(triangulationTab);
        triangulationCanvas->setObjectName("triangulationCanvas");
        sizePolicy.setHeightForWidth(triangulationCanvas->sizePolicy().hasHeightForWidth());
        triangulationCanvas->setSizePolicy(sizePolicy);
        triangulationCanvas->setMinimumSize(QSize(400, 400));

        triangulationLayout->addWidget(triangulationCanvas);

        triangulationPanel = new QWidget(triangulationTab);
        triangulationPanel->setObjectName("triangulationPanel");
        sizePolicy1.setHeightForWidth(triangulationPanel->sizePolicy().hasHeightForWidth());
        triangulationPanel->setSizePolicy(sizePolicy1);
        triangulationPanelLayout = new QVBoxLayout(triangulationPanel);
        triangulationPanelLayout->setObjectName("triangulationPanelLayout");
        triangulationPanelLayout->setContentsMargins(0, 0, 0, 0);
        triangulationComputeButton = new QPushButton(triangulationPanel);
        triangulationComputeButton->setObjectName("triangulationComputeButton");

        triangulationPanelLayout->addWidget(triangulationComputeButton);

        triangulationLiveCheckBox = new QCheckBox(triangulationPanel);
        triangulationLiveCheckBox->setObjectName("triangulationLiveCheckBox");

        triangulationPanelLayout->addWidget(triangulationLiveCheckBox);

        triangulationDragCheckBox = new QCheckBox(triangulationPanel);
        triangulationDragCheckBox->setObjectName("triangulationDragCheckBox");
        triangulationDragCheckBox->setChecked(true);

        triangulationPanelLayout->addWidget(triangulationDragCheckBox);

        triangulationClearButton = new QPushButton(triangulationPanel);
        triangulationClearButton->setObjectName("triangulationClearButton");

        triangulationPanelLayout->addWidget(triangulationClearButton);

        triangulationSummaryBrowser = new QTextBrowser(triangulationPanel);
        triangulationSummaryBrowser->setObjectName("triangulationSummaryBrowser");
        sizePolicy1.setHeightForWidth(triangulationSummaryBrowser->sizePolicy().hasHeightForWidth());
        triangulationSummaryBrowser->setSizePolicy(sizePolicy1);

        triangulationPanelLayout->addWidget(triangulationSummaryBrowser);


        triangulationLayout->addWidget(triangulationPanel);

        tabWidget->addTab(triangulationTab, QString());
        convexBooleanTab = new QWidget();
        convexBooleanTab->setObjectName("convexBooleanTab");
        convexBooleanLayout = new QHBoxLayout(convexBooleanTab);
        convexBooleanLayout->setObjectName("convexBooleanLayout");
        convexBooleanCanvas = new ConvexPolygonBooleWidget(convexBooleanTab);
        convexBooleanCanvas->setObjectName("convexBooleanCanvas");
        sizePolicy.setHeightForWidth(convexBooleanCanvas->sizePolicy().hasHeightForWidth());
        convexBooleanCanvas->setSizePolicy(sizePolicy);
        convexBooleanCanvas->setMinimumSize(QSize(400, 400));

        convexBooleanLayout->addWidget(convexBooleanCanvas);

        convexBooleanPanel = new QWidget(convexBooleanTab);
        convexBooleanPanel->setObjectName("convexBooleanPanel");
        sizePolicy1.setHeightForWidth(convexBooleanPanel->sizePolicy().hasHeightForWidth());
        convexBooleanPanel->setSizePolicy(sizePolicy1);
        convexBooleanPanelLayout = new QVBoxLayout(convexBooleanPanel);
        convexBooleanPanelLayout->setObjectName("convexBooleanPanelLayout");
        convexBooleanPanelLayout->setContentsMargins(0, 0, 0, 0);
        booleanFinalizeFirstButton = new QPushButton(convexBooleanPanel);
        booleanFinalizeFirstButton->setObjectName("booleanFinalizeFirstButton");

        convexBooleanPanelLayout->addWidget(booleanFinalizeFirstButton);

        booleanFinalizeSecondButton = new QPushButton(convexBooleanPanel);
        booleanFinalizeSecondButton->setObjectName("booleanFinalizeSecondButton");

        convexBooleanPanelLayout->addWidget(booleanFinalizeSecondButton);

        booleanOperationCombo = new QComboBox(convexBooleanPanel);
        booleanOperationCombo->addItem(QString());
        booleanOperationCombo->addItem(QString());
        booleanOperationCombo->addItem(QString());
        booleanOperationCombo->setObjectName("booleanOperationCombo");

        convexBooleanPanelLayout->addWidget(booleanOperationCombo);

        booleanComputeButton = new QPushButton(convexBooleanPanel);
        booleanComputeButton->setObjectName("booleanComputeButton");

        convexBooleanPanelLayout->addWidget(booleanComputeButton);

        booleanLiveCheckBox = new QCheckBox(convexBooleanPanel);
        booleanLiveCheckBox->setObjectName("booleanLiveCheckBox");

        convexBooleanPanelLayout->addWidget(booleanLiveCheckBox);

        booleanClearButton = new QPushButton(convexBooleanPanel);
        booleanClearButton->setObjectName("booleanClearButton");

        convexBooleanPanelLayout->addWidget(booleanClearButton);

        booleanSummaryBrowser = new QTextBrowser(convexBooleanPanel);
        booleanSummaryBrowser->setObjectName("booleanSummaryBrowser");
        sizePolicy1.setHeightForWidth(booleanSummaryBrowser->sizePolicy().hasHeightForWidth());
        booleanSummaryBrowser->setSizePolicy(sizePolicy1);

        convexBooleanPanelLayout->addWidget(booleanSummaryBrowser);


        convexBooleanLayout->addWidget(convexBooleanPanel);

        tabWidget->addTab(convexBooleanTab, QString());
        pointLocationTab = new QWidget();
        pointLocationTab->setObjectName("pointLocationTab");
        pointLocationLayout = new QHBoxLayout(pointLocationTab);
        pointLocationLayout->setObjectName("pointLocationLayout");
        pointLocationCanvas = new PointLocationWidget(pointLocationTab);
        pointLocationCanvas->setObjectName("pointLocationCanvas");
        sizePolicy.setHeightForWidth(pointLocationCanvas->sizePolicy().hasHeightForWidth());
        pointLocationCanvas->setSizePolicy(sizePolicy);
        pointLocationCanvas->setMinimumSize(QSize(400, 400));

        pointLocationLayout->addWidget(pointLocationCanvas);

        pointLocationPanel = new QWidget(pointLocationTab);
        pointLocationPanel->setObjectName("pointLocationPanel");
        sizePolicy1.setHeightForWidth(pointLocationPanel->sizePolicy().hasHeightForWidth());
        pointLocationPanel->setSizePolicy(sizePolicy1);
        pointLocationPanelLayout = new QVBoxLayout(pointLocationPanel);
        pointLocationPanelLayout->setObjectName("pointLocationPanelLayout");
        pointLocationPanelLayout->setContentsMargins(0, 0, 0, 0);
        pointLocationFinalizeButton = new QPushButton(pointLocationPanel);
        pointLocationFinalizeButton->setObjectName("pointLocationFinalizeButton");

        pointLocationPanelLayout->addWidget(pointLocationFinalizeButton);

        pointLocationClearButton = new QPushButton(pointLocationPanel);
        pointLocationClearButton->setObjectName("pointLocationClearButton");

        pointLocationPanelLayout->addWidget(pointLocationClearButton);

        pointLocationRemoveTestButton = new QPushButton(pointLocationPanel);
        pointLocationRemoveTestButton->setObjectName("pointLocationRemoveTestButton");

        pointLocationPanelLayout->addWidget(pointLocationRemoveTestButton);

        pointLocationDragCheckBox = new QCheckBox(pointLocationPanel);
        pointLocationDragCheckBox->setObjectName("pointLocationDragCheckBox");
        pointLocationDragCheckBox->setChecked(true);

        pointLocationPanelLayout->addWidget(pointLocationDragCheckBox);

        pointLocationSummaryBrowser = new QTextBrowser(pointLocationPanel);
        pointLocationSummaryBrowser->setObjectName("pointLocationSummaryBrowser");
        sizePolicy1.setHeightForWidth(pointLocationSummaryBrowser->sizePolicy().hasHeightForWidth());
        pointLocationSummaryBrowser->setSizePolicy(sizePolicy1);

        pointLocationPanelLayout->addWidget(pointLocationSummaryBrowser);


        pointLocationLayout->addWidget(pointLocationPanel);

        tabWidget->addTab(pointLocationTab, QString());
        bezierTab = new QWidget();
        bezierTab->setObjectName("bezierTab");
        bezierLayout = new QHBoxLayout(bezierTab);
        bezierLayout->setObjectName("bezierLayout");
        bezierCanvas = new BezierWidget(bezierTab);
        bezierCanvas->setObjectName("bezierCanvas");
        sizePolicy.setHeightForWidth(bezierCanvas->sizePolicy().hasHeightForWidth());
        bezierCanvas->setSizePolicy(sizePolicy);
        bezierCanvas->setMinimumSize(QSize(400, 400));

        bezierLayout->addWidget(bezierCanvas);

        bezierPanel = new QWidget(bezierTab);
        bezierPanel->setObjectName("bezierPanel");
        sizePolicy1.setHeightForWidth(bezierPanel->sizePolicy().hasHeightForWidth());
        bezierPanel->setSizePolicy(sizePolicy1);
        bezierPanelLayout = new QVBoxLayout(bezierPanel);
        bezierPanelLayout->setObjectName("bezierPanelLayout");
        bezierPanelLayout->setContentsMargins(0, 0, 0, 0);
        bezierDegreeCombo = new QComboBox(bezierPanel);
        bezierDegreeCombo->addItem(QString());
        bezierDegreeCombo->addItem(QString());
        bezierDegreeCombo->addItem(QString());
        bezierDegreeCombo->setObjectName("bezierDegreeCombo");

        bezierPanelLayout->addWidget(bezierDegreeCombo);

        bezierSamplesSpin = new QSpinBox(bezierPanel);
        bezierSamplesSpin->setObjectName("bezierSamplesSpin");
        bezierSamplesSpin->setMinimum(10);
        bezierSamplesSpin->setMaximum(1000);
        bezierSamplesSpin->setValue(200);

        bezierPanelLayout->addWidget(bezierSamplesSpin);

        bezierDragCheckBox = new QCheckBox(bezierPanel);
        bezierDragCheckBox->setObjectName("bezierDragCheckBox");
        bezierDragCheckBox->setChecked(true);

        bezierPanelLayout->addWidget(bezierDragCheckBox);

        bezierClearButton = new QPushButton(bezierPanel);
        bezierClearButton->setObjectName("bezierClearButton");

        bezierPanelLayout->addWidget(bezierClearButton);

        bezierInfoBrowser = new QTextBrowser(bezierPanel);
        bezierInfoBrowser->setObjectName("bezierInfoBrowser");
        sizePolicy1.setHeightForWidth(bezierInfoBrowser->sizePolicy().hasHeightForWidth());
        bezierInfoBrowser->setSizePolicy(sizePolicy1);

        bezierPanelLayout->addWidget(bezierInfoBrowser);


        bezierLayout->addWidget(bezierPanel);

        tabWidget->addTab(bezierTab, QString());

        centralLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1100, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "Point is left of the segment (1)", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MainWindow", "Point lies on the segment (0)", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MainWindow", "Point is right of the segment (-1)", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Check point position", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Orientation", nullptr));
        intersectionRadioNone->setText(QCoreApplication::translate("MainWindow", "No intersection", nullptr));
        intersectionRadioPoint->setText(QCoreApplication::translate("MainWindow", "Intersection at a point", nullptr));
        intersectionRadioOverlap->setText(QCoreApplication::translate("MainWindow", "Segments overlap", nullptr));
        computeIntersectionButton->setText(QCoreApplication::translate("MainWindow", "Find intersection", nullptr));
        liveIntersectionCheckBox->setText(QCoreApplication::translate("MainWindow", "Live update while dragging", nullptr));
        clearSegmentsButton->setText(QCoreApplication::translate("MainWindow", "Clear segments", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Intersection", nullptr));
        buildHullButton->setText(QCoreApplication::translate("MainWindow", "Build convex hull", nullptr));
        liveHullCheckBox->setText(QCoreApplication::translate("MainWindow", "Live update while dragging", nullptr));
        clearHullButton->setText(QCoreApplication::translate("MainWindow", "Clear hull", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(hullTab), QCoreApplication::translate("MainWindow", "Convex Hull", nullptr));
        precisionSegmentStartEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "(x1, y1)", nullptr));
        precisionSegmentEndEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "(x2, y2)", nullptr));
        precisionPointEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "(px, py)", nullptr));
        precisionEpsilonEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "1e-30", nullptr));
        precisionCheckButton->setText(QCoreApplication::translate("MainWindow", "Check orientation", nullptr));
        precisionClearButton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(precisionTab), QCoreApplication::translate("MainWindow", "High Precision", nullptr));
        triangulationComputeButton->setText(QCoreApplication::translate("MainWindow", "Compute Delaunay triangulation", nullptr));
        triangulationLiveCheckBox->setText(QCoreApplication::translate("MainWindow", "Live recompute while dragging", nullptr));
        triangulationDragCheckBox->setText(QCoreApplication::translate("MainWindow", "Enable point dragging", nullptr));
        triangulationClearButton->setText(QCoreApplication::translate("MainWindow", "Clear points", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(triangulationTab), QCoreApplication::translate("MainWindow", "Triangulation", nullptr));
        booleanFinalizeFirstButton->setText(QCoreApplication::translate("MainWindow", "Lock first polygon", nullptr));
        booleanFinalizeSecondButton->setText(QCoreApplication::translate("MainWindow", "Lock second polygon", nullptr));
        booleanOperationCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Intersection", nullptr));
        booleanOperationCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Union", nullptr));
        booleanOperationCombo->setItemText(2, QCoreApplication::translate("MainWindow", "Difference (A \\ B)", nullptr));

        booleanComputeButton->setText(QCoreApplication::translate("MainWindow", "Compute operation", nullptr));
        booleanLiveCheckBox->setText(QCoreApplication::translate("MainWindow", "Live recompute on drag", nullptr));
        booleanClearButton->setText(QCoreApplication::translate("MainWindow", "Reset polygons", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(convexBooleanTab), QCoreApplication::translate("MainWindow", "Convex Boolean", nullptr));
        pointLocationFinalizeButton->setText(QCoreApplication::translate("MainWindow", "Build convex hull", nullptr));
        pointLocationClearButton->setText(QCoreApplication::translate("MainWindow", "Clear polygon", nullptr));
        pointLocationRemoveTestButton->setText(QCoreApplication::translate("MainWindow", "Remove test point", nullptr));
        pointLocationDragCheckBox->setText(QCoreApplication::translate("MainWindow", "Enable dragging", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(pointLocationTab), QCoreApplication::translate("MainWindow", "Point Localization", nullptr));
        bezierDegreeCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Linear (1st degree)", nullptr));
        bezierDegreeCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Quadratic (2nd degree)", nullptr));
        bezierDegreeCombo->setItemText(2, QCoreApplication::translate("MainWindow", "Cubic (3rd degree)", nullptr));

        bezierDragCheckBox->setText(QCoreApplication::translate("MainWindow", "Enable dragging", nullptr));
        bezierClearButton->setText(QCoreApplication::translate("MainWindow", "Clear control points", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(bezierTab), QCoreApplication::translate("MainWindow", "Bezier Curves", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
