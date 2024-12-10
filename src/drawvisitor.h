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
    void Visit(const Triangle &triangle, const std::array<QVector3D, 3> &normals, const std::array<QVector2D, 3> &uv);

protected:

    QVector3D getNormalInPoint(const QVector2D &uv);


    void DrawPoint(Point &point);
    void DrawTriangle(Triangle &pts);
    void DrawTriangle(const std::array<Point, 3> &points,
                      const std::array<QVector3D, 3> &normals,
                      const QColor &color,
                      const std::array<QVector2D, 3> &uv);

    int calculateIntensity(const Point &point, const QVector3D &mormal) const;
private:
    QSize canvas_size_;
    QVector<QVector<double>> z_buffer_;
    const AbstractStrategyProjection *projection_;
    QVector<std::shared_ptr<AbstractLight>> light_;
    QImage normal_map;
};

#endif // DRAWVISITOR_H
