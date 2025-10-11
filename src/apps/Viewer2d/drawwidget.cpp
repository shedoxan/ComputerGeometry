#include "drawwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPalette>

DrawWidget::DrawWidget(QWidget* parent) : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

QVector<QPointF> DrawWidget::points() const {
    return m_points;
}

void DrawWidget::clearPoints() {
    if (m_points.isEmpty()) {
        return;
    }

    m_points.clear();
    update();
    emit pointsChanged(m_points);
}

void DrawWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    if (m_points.size() == 3) {
        m_points.clear();
    }

    const QPointF clickedPoint = event->position();

    m_points.append(clickedPoint);
    update();
    emit pointsChanged(m_points);
}

void DrawWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_points.size() >= 2) {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(m_points[0], m_points[1]);
    }

    const int radius = 5;
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::red);

    const QStringList labels = {"A", "B", "C"};
    const QPointF offset{radius + 3, -radius};

    for (int i = 0; i < m_points.size(); ++i) {
        const QPointF& point = m_points[i];

        painter.drawEllipse(point, radius, radius);

        const QString label = labels[i];

        painter.drawText(point + offset, label);
    }    
}
