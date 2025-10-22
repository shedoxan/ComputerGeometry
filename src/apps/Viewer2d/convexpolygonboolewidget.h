#ifndef CONVEXPOLYGONBOOLEWIDGET_H
#define CONVEXPOLYGONBOOLEWIDGET_H

#include <QPointF>
#include <QPolygonF>
#include <QVector>
#include <QWidget>

#include "PlaneGeometry/PlaneOperations.h"

class QMouseEvent;
class QPaintEvent;

class ConvexPolygonBooleWidget : public QWidget {
    Q_OBJECT

public:
    enum class Phase {
        FirstPolygonInput,
        SecondPolygonInput,
        ResultReady
    };

    explicit ConvexPolygonBooleWidget(QWidget* parent = nullptr);

    QVector<QPointF> firstRawPoints() const;
    QVector<QPointF> secondRawPoints() const;
    QVector<QPointF> firstConvexHull() const;
    QVector<QPointF> secondConvexHull() const;
    QVector<QPointF> resultPolygon() const;

    Phase phase() const;

    void clearAll();
    void finalizeFirstPolygon();
    void finalizeSecondPolygon();
    void setResultPolygon(const QVector<QPointF>& polygon);
    void setResultPolygons(const QVector<QPolygonF>& outers,
                           const QVector<QPolygonF>& holes = {});
    void clearResultPolygon();
    void setDraggingEnabled(bool enabled);

signals:
    void polygonsChanged();
    void phaseChanged(ConvexPolygonBooleWidget::Phase phase);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    struct HitInfo {
        bool valid = false;
        bool firstPolygon = true;
        int index = -1;
    };

    HitInfo hitTest(const QPointF& position) const;
    void appendPoint(const QPointF& position);
    void removePoint(const HitInfo& hitInfo);
    void updatePoint(const HitInfo& hitInfo, const QPointF& position);
    QPointF eventPosition(const QMouseEvent* event) const;

    void rebuildFirstHull();
    void rebuildSecondHull();
    static QPolygonF buildHullFromPoints(const QVector<QPointF>& points);

    QVector<QPointF> m_firstRawPoints;
    QVector<QPointF> m_secondRawPoints;

    QPolygonF m_firstHull;
    QPolygonF m_secondHull;
    QVector<QPolygonF> m_resultPolygons;
    QVector<QPolygonF> m_resultHoles;

    Phase m_phase = Phase::FirstPolygonInput;
    bool m_firstLocked = false;
    bool m_secondLocked = false;

    bool m_dragEnabled = true;
    bool m_dragging = false;
    HitInfo m_dragHit;
};

#endif
