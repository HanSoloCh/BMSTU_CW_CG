#ifndef DRAWVISITOR_H
#define DRAWVISITOR_H

#include <QPainter>

#include "point.h"
#include "triangle.h"
#include "carcasmodel.h"
#include "strategy.h"

class BaseDrawVisitor {
public:
    BaseDrawVisitor(QPainter *painter);
    virtual ~BaseDrawVisitor() = 0;

    void Visit(const AbstractModel &abstractModel) {
        Q_UNUSED(abstractModel);
    };

    void SetColor(const QColor &color) {
        color_ = painter_->pen().color();
        painter_->setPen(color);
    }

    void ResetColor() {
        painter_->setPen(color_);
    }

    virtual void Visit(const Point &point) = 0;
    virtual void Visit(const Triangle &triangle) = 0;
    virtual void Visit(const CarcasModel &carcas_mode) = 0;

protected:
    QPainter * const painter_;

private:
    QColor color_;
};

class DrawVisitor : public BaseDrawVisitor {
public:
    DrawVisitor(QPainter *painter, QSize canvas_size, const AbstractStrategyProjection *projection);
    ~DrawVisitor() = default;

    void Visit(const Point &point) override;
    void Visit(const Triangle &triangle) override;
    void Visit(const CarcasModel &carcas_model) override;

    static QVector<double> Interpolate(double x0, double y0, double x1, double y1);
protected:
    void DrawPoint(Point &point);
    void DrawTriangle(const Point &p0, const Point &p1, const Point &p2);

private:
    QSize canvas_size_;
    QVector<QVector<double>> z_buffer_;
    const AbstractStrategyProjection *projection_;
};

#endif // DRAWVISITOR_H
