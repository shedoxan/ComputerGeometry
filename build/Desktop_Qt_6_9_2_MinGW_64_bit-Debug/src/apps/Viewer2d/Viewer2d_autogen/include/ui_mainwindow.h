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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <drawwidget.h>
#include <precisiondrawwidget.h>
#include <segmentintersectionwidget.h>

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
    QVBoxLayout *verticalLayout;
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
        verticalLayout = new QVBoxLayout(orientationPanel);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        radioButton = new QRadioButton(orientationPanel);
        radioButton->setObjectName("radioButton");

        verticalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(orientationPanel);
        radioButton_2->setObjectName("radioButton_2");

        verticalLayout->addWidget(radioButton_2);

        radioButton_3 = new QRadioButton(orientationPanel);
        radioButton_3->setObjectName("radioButton_3");

        verticalLayout->addWidget(radioButton_3);

        pushButton = new QPushButton(orientationPanel);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);

        textBrowser = new QTextBrowser(orientationPanel);
        textBrowser->setObjectName("textBrowser");
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(textBrowser);


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
        precisionSegmentStartEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "(x1, y1)", nullptr));
        precisionSegmentEndEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "(x2, y2)", nullptr));
        precisionPointEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "(px, py)", nullptr));
        precisionEpsilonEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "1e-30", nullptr));
        precisionCheckButton->setText(QCoreApplication::translate("MainWindow", "Check orientation", nullptr));
        precisionClearButton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(precisionTab), QCoreApplication::translate("MainWindow", "High Precision", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
