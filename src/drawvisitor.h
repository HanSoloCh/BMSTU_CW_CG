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


protected:
    struct BarycentricCoords {
        double alpha;
        double beta;
        double gamma;
    };

    Point ProjectPoint(const Point &point) const;

    void DrawPoint(const Point &point);
    void DrawTriangle(const Triangle &triangle);
    virtual void DrawTrianglePixel(const QPoint &point,
                                   const Triangle &triangle,
                                   const BarycentricCoords &barycentric_coords);


    std::tuple<int, int, int, int> CalculateBoundingBox(const std::array<Point, 3> &pts) const;
    bool CalculateBarycentricCoords(const std::array<Point, 3> &pts,
                                    int x,
                                    int y,
                                    BarycentricCoords &barycentric_coords) const;
    bool IsInsideTriangle(const BarycentricCoords &barycentric_coords) const;
    bool UpdateZBuffer(int x, int y, double z);

    int CalculateIntensity(const Point &point, const QVector3D &mormal) const;

    template <typename T>
    T InterpolateValue(const std::array<T, 3> values,
                       const BarycentricCoords &barycentric_coords) const;

private:
    QSize canvas_size_;
    QVector<QVector<double>> z_buffer_;
    const AbstractStrategyProjection *projection_;
    QVector<std::shared_ptr<AbstractLight>> light_;
};

template<typename T>
T DrawVisitor::InterpolateValue(const std::array<T, 3> values,
                                const BarycentricCoords &barycentric_coords) const {
    return barycentric_coords.alpha * values[0] +
           barycentric_coords.beta * values[1] +
           barycentric_coords.gamma * values[2];
}

class DrawMappedVisitor : public DrawVisitor {
public:
    DrawMappedVisitor(QPainter *painter,
                      QSize canvas_size,
                      const AbstractStrategyProjection *projection,
                      QVector<std::shared_ptr<AbstractLight>> light,
                      const QImage &normal_map);
    DrawMappedVisitor(const DrawVisitor &draw_visitor,
                      const QImage &normal_map);
    ~DrawMappedVisitor() = default;

    void Visit(const Point &point) override;
    void Visit(const Triangle &triangle) override;
    void Visit(const CarcasModel &carcas_model) override;
protected:
    virtual void DrawTrianglePixel(const QPoint &point,
                                   const Triangle &triangle,
                                   const BarycentricCoords &barycentric_coords) override;
    QVector3D GetNormalInPoint(const QVector2D &uv) const;
private:
    QImage normal_map_;
};


#endif // DRAWVISITOR_H
