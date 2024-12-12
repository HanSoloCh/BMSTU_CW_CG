#include "canvas.h"

#include <QPainter>
#include <memory>

#include "strategy.h"
#include "drawvisitor.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent) {
    setFixedSize(1000, 1000);
}

Canvas::Canvas(const QImage &normal_map, QWidget *parent)
    : QWidget(parent)
    , normal_map_(normal_map) {
    setFixedSize(1000, 1000);
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


void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    AbstractStrategyProjection *strategy = new NewStrategy();

    std::unique_ptr<BaseDrawVisitor> visitor;
    if (normal_map_.isNull())
        visitor = std::make_unique<DrawVisitor>(&painter, size(), strategy, light_);
    else
        visitor = std::make_unique<DrawMappedVisitor>(&painter,
                                                      size(),
                                                      strategy,
                                                      light_,
                                                      normal_map_);
    // auto start = clock();
    for (const auto &object : scene_objects_) {
        object->Accept(visitor.get());
    }
    // auto stop = clock();
    // qDebug() << stop - start;
    delete strategy;
}
