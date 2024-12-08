#include "curvecanvas.h"

#include <QPainter>
#include <QPainterPath>

#include "curve.h"

CurveCanvas::CurveCanvas(QWidget *parent) : QWidget(parent) {
    setFixedSize(600, 600);
}

QVector<QPointF> CurveCanvas::generateCurvePoints() {
    QVector<QPointF> curvePoints;
    if (mainPoints.size() < 3) {
        return curvePoints; // Недостаточно точек для построения кривой
    }

    QVector<QPointF> bezierPoints = { mainPoints[0], mainPoints[2], mainPoints[1] }; // Порядок: начало, контрольная точка, конец
    Curve bezier(bezierPoints, steps);
    curvePoints = bezier.calculateBezierCurve();

    return curvePoints;
}


void CurveCanvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем ось вращения
    painter.setPen(Qt::gray);
    painter.drawLine(width() / 2, 0, width() / 2, height());

    // Рисуем заданные точки
    painter.setPen(Qt::blue);
    for (const auto &point : mainPoints) {
        // qDebug() << point;
        painter.drawEllipse(point, 5, 5);
    }

    // Рисуем кривую (красная линия)
    if (mainPoints.size() == 3) {
        painter.setPen(Qt::red);
        QPainterPath path;
        path.moveTo(mainPoints[0]);          // Начальная точка
        path.quadTo(mainPoints[2], mainPoints[1]); // Кривая Безье
        painter.drawPath(path);
    }
}

void CurveCanvas::setGeneratedPoints(const QVector<QPointF> &points) {
    generatedPoints = points;
    update();
}

void CurveCanvas::Clean() {
    generatedPoints.clear();
    mainPoints.clear();
    update();
}

void CurveCanvas::mousePressEvent(QMouseEvent *event) {
    if (mainPoints.size() < 3) {
        mainPoints.append(event->pos());
        update();
    }
}
