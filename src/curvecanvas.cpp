#include "curvecanvas.h"

#include <QPainter>
#include <QPainterPath>

#include "curve.h"

CurveCanvas::CurveCanvas(QWidget *parent)
    : QWidget(parent)
    , color_(Qt::gray) {
    setFixedSize(600, 600);
}

QVector<QPointF> CurveCanvas::GetCurvePoints(int steps) const {
    QVector<QPointF> curve_points;
    if (main_points_.size() < 3) {
        return curve_points;
    }

    // Порядок: начало, контрольная точка, конец
    QVector<QPointF> bezier_points = { main_points_[0], main_points_[2], main_points_[1] };
    Curve bezier(bezier_points, steps);
    return bezier.GetPoints();
}

QColor CurveCanvas::GetColor() const noexcept {
    return color_;
}

void CurveCanvas::SetColor(const QColor &color) {
    color_ = color;
    update();
}


void CurveCanvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем ось вращения
    painter.setPen(Qt::gray);
    painter.drawLine(width() / 2, 0, width() / 2, height());
    painter.drawLine(0, height() / 2, width(), height() / 2);

    // Рисуем заданные точки
    painter.setPen(color_);
    for (const auto &point : main_points_) {
        painter.drawEllipse(point, 5, 5);
    }

    // Рисуем кривую
    if (main_points_.size() == 3) {
        QPainterPath path;
        path.moveTo(main_points_[0]);
        path.quadTo(main_points_[2], main_points_[1]);
        painter.drawPath(path);
    }
}


void CurveCanvas::Clean() {
    main_points_.clear();
    update();
}

void CurveCanvas::mousePressEvent(QMouseEvent *event) {
    if (main_points_.size() < 3) {
        main_points_.append(event->pos());
        update();
    }
}
