#ifndef DRAWVISITOR_H
#define DRAWVISITOR_H

#include <QPainter>
#include <memory>

#include "light.h"
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
    DrawVisitor(QPainter *painter,
                QSize canvas_size,
                const AbstractStrategyProjection *projection,
                QVector<std::shared_ptr<AbstractLight>> light);
    ~DrawVisitor() = default;

    void Visit(const Point &point) override;
    void Visit(const Triangle &triangle) override;
    void Visit(const CarcasModel &carcas_model) override;
    void Visit(const Triangle &triangle, const std::array<QVector3D, 3> &normals);

    // static QVector<double> Interpolate(double x0, double y0, double x1, double y1);
protected:
    void DrawPoint(Point &point);
    void DrawTriangle(const std::array<Point, 3> &points);
    void DrawTriangle(const std::array<Point, 3> &points, const std::array<QVector3D, 3> &normals, const QColor &color);

    int calculateIntensity(const Point &point, const QVector3D &mormal) const;
private:
    QSize canvas_size_;
    QVector<QVector<double>> z_buffer_;
    const AbstractStrategyProjection *projection_;
    QVector<std::shared_ptr<AbstractLight>> light_;
};

#endif // DRAWVISITOR_H
