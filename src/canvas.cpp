#include "canvas.h"

#include <QPainter>
#include <memory>

#include "strategy.h"
#include "drawvisitor.h"


Canvas::Canvas(QWidget *parent)
    : QWidget(parent) {}

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
    qDebug() << "JUI";
    QPainter painter(this);

    AbstractStrategyProjection *strategy = new PerspectivStrategyProjection();

    DrawVisitor visitor(&painter, size(), strategy, light_);

    auto start = clock();
    for (const auto &object : scene_objects_) {
        object->Accept(visitor);
    }
    auto stop = clock();
    qDebug() << stop - start;
    delete strategy;
}
