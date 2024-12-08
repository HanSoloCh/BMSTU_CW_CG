#include "canvas.h"

QVector<QPointF> CurveCanvas::generateCurvePoints() {
    QVector<QPointF> curvePoints;
    if (mainPoints.size() < 3) {
        return curvePoints; // Недостаточно точек для построения кривой
    }

    QVector<QPointF> bezierPoints = { mainPoints[0], mainPoints[2], mainPoints[1] }; // Порядок: начало, контрольная точка, конец
    BezierCurve bezier(bezierPoints, steps);
    curvePoints = bezier.calculateBezierCurve();

    return curvePoints;
}



void CurveCanvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем ось вращения
    painter.setPen(Qt::gray);
    painter.drawLine(axisCenter.x(), 0, axisCenter.x(), height());

    // Рисуем заданные точки
    painter.setPen(Qt::blue);
    for (const auto &point : mainPoints) {
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

    // Рисуем сгенерированные точки на кривой (зеленые точки)
    painter.setPen(Qt::green);
    for (const auto &point : generatedPoints) {
        painter.drawEllipse(point, 3, 3);
    }
}
