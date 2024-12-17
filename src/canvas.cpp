#include "canvas.h"

#include <QPainter>
#include <memory>

#include "strategy.h"
#include "drawvisitor.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , draw_type_(def) {
    setFixedSize(800, 800);
}

void Canvas::AddModel(const std::shared_ptr<AbstractModel> carcas_model) {
    scene_objects_.push_back(carcas_model);
}

void Canvas::AddLight(const std::shared_ptr<AbstractLight> light) {
    light_.push_back(light);
}

void Canvas::Transform(const QMatrix4x4 &transform_matrix) {
    for (auto objects : scene_objects_) {
        objects->Transform(transform_matrix);
    }
}

void Canvas::SetDefault() {
    draw_type_ = def;
}

void Canvas::SetNormalMap(const QImage &normal_map) {
    draw_type_ = mapping;
    image_ = normal_map;
}

void Canvas::SetTexuture(const QImage &texture) {
    draw_type_ = textuting;
    image_ = texture;
}


void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    AbstractStrategyProjection *strategy = new NewStrategy();

    std::unique_ptr<BaseDrawVisitor> visitor;
    // Можно было бы полиморфизмом
    if (draw_type_ == def)
        visitor = std::make_unique<DrawVisitor>(&painter, size(), strategy, light_);
    else if (draw_type_ == mapping)
        visitor = std::make_unique<DrawMappedVisitor>(&painter,
                                                      size(),
                                                      strategy,
                                                      light_,
                                                      image_);
    else
        visitor = std::make_unique<DrawTextureVisitor>(&painter,
                                                       size(),
                                                       strategy,
                                                       light_,
                                                       image_);

    for (const auto &object : scene_objects_) {
        object->Accept(visitor.get());
    }
    delete strategy;
}
