#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

#include "BezierCurve.h"


class CurveCanvas : public QWidget {
    Q_OBJECT

public:
    explicit CurveCanvas(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(600, 600);
        axisCenter = QPointF(width() / 2, height() / 2); // Центр оси вращения
    }

    QVector<QPointF> generateCurvePoints();

    void setGeneratedPoints(const QVector<QPointF> &points) {
        generatedPoints = points;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override {
        if (mainPoints.size() < 3) {
            mainPoints.append(event->pos());
            update();
        }
    }

private:
    QPointF axisCenter;
    QVector<QPointF> mainPoints;
    QVector<QPointF> generatedPoints;
    int steps = 50; // Количество точек на кривой
};

#endif // CANVAS_H
