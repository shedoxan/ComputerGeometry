#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QPointF>
#include <QVector>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;

class DrawWidget : public QWidget {
    Q_OBJECT

    public:
        explicit DrawWidget(QWidget* parent = nullptr);

        QVector<QPointF> points() const;
        void clearPoints();

    signals:
        void pointsChanged(const QVector<QPointF>& points);

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void paintEvent(QPaintEvent* event) override;

    private:
        QVector<QPointF> m_points;  
};

#endif
