#include "curvecanvas.h"

#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>

#include "curve.h"

CurveCanvas::CurveCanvas(QWidget *parent)
    : QWidget(parent)
    , color_(Qt::gray) {
    setFixedSize(800, 800);
}

QVector<QPointF> CurveCanvas::GetCurvePoints(const axis_t axis, int steps) {
    QVector<QPointF> curve_points;
    if (main_points_.size() < 2) {
        return curve_points;
    }

    if (axis == Ox && main_points_[0].x() < main_points_[1].x()) {
        std::swap(main_points_[0], main_points_[1]);
    } else if (axis == Oy && main_points_[0].y() > main_points_[1].y()) {
        std::swap(main_points_[0], main_points_[1]);
    }

    Curve bezier(main_points_, control_points_, steps);
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
    int centerX = width() / 2;
    int centerY = height() / 2;
    painter.drawLine(centerX, 0, centerX, height());
    painter.drawLine(0, centerY, width(), centerY);

    // Рисуем стрелки на концах осей
    QPolygonF arrowX, arrowY;

    // Стрелка на оси X
    arrowX << QPointF(width() - 10, centerY - 5)
           << QPointF(width() - 10, centerY + 5)
           << QPointF(width(), centerY);
    painter.drawPolygon(arrowX);

    // Стрелка на оси Y
    arrowY << QPointF(centerX - 5, 10)
           << QPointF(centerX + 5, 10)
           << QPointF(centerX, 0);
    painter.drawPolygon(arrowY);

    // Подписи осей
    painter.setFont(QFont("Arial", 10));
    painter.drawText(width() - 20, centerY - 10, "X");
    painter.drawText(centerX + 10, 20, "Y");

    // Рисуем заданные точки
    painter.setPen(color_);
    for (const auto &point : main_points_) {
        painter.drawEllipse(point, 5, 5);
    }

    for (const auto &point : control_points_) {
        painter.drawEllipse(point, 5, 5);
    }

    if (main_points_.size() == 2) {
        SortPoints();
        Curve bezier(main_points_, control_points_);
        QVector<QPointF> curve_points = bezier.GetPoints();

        QPainterPath path;
        path.moveTo(curve_points.first());
        for (int i = 1; i < curve_points.size(); ++i) {
            path.lineTo(curve_points[i]);
        }
        painter.setPen(color_);
        painter.drawPath(path);
    }
}

void CurveCanvas::SortPoints() {
    double d_x = main_points_[0].x() - main_points_[1].x();
    double d_y = main_points_[0].y() - main_points_[1].y();
    if (fabs(d_x) > fabs(d_y))
        std::sort(control_points_.begin(), control_points_.end(), [](const QPointF &p1, const QPointF &p2) {
            return p1.x() < p2.x();
        });
    else
        std::sort(control_points_.begin(), control_points_.end(), [](const QPointF &p1, const QPointF &p2) {
            return p1.y() < p2.y();
        });
}


void CurveCanvas::Clean() {
    main_points_.clear();
    control_points_.clear();
    update();
}

void CurveCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (main_points_.size() >= 2) {
            QMessageBox::warning(this, "Ошибка", "Начальная и конечная точки уже заданы.");
        } else {
            main_points_.append(event->pos());
        }
    } else if (event->button() == Qt::RightButton) {
        if (main_points_.size() < 2) {
            QMessageBox::warning(this, "Ошибка", "Сначала задайте начальную и конечную точки (ЛКМ).");
        } else {
            control_points_.append(event->pos());
        }
    }
    update();
}
