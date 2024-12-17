#ifndef CURVECANVAS_H
#define CURVECANVAS_H

#include <QColor>
#include <QMouseEvent>
#include <QWidget>

#include "carcasmodel.h"

class CurveCanvas : public QWidget {
    Q_OBJECT
   public:
    explicit CurveCanvas(QWidget *parent = nullptr);

    QVector<QPointF> GetCurvePoints(const axis_t axis = Ox, int steps = 50);
    QColor GetColor() const noexcept;

   public slots:
    void SetColor(const QColor &color);
    void Clean();

   protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

   private:
    void SortPoints();

    QVector<QPointF> main_points_;
    QVector<QPointF> control_points_;
    QColor color_;
};

#endif  // CURVECANVAS_H
