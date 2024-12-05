#include "canvas.h"

#include <QPainter>
#include <memory>

#include "carcasmodel.h"
#include "strategy.h"
#include "drawvisitor.h"


Canvas::Canvas(QWidget *parent)
    : QWidget(parent) {
    AddLight(std::make_shared<DirectionLight>(1, QVector3D(1, 1, -1)));

    AddModel(std::make_shared<CarcasModel>(GenerateShape(2, 100, 100, Qt::green, Point(0, 0, 8))));
}

void Canvas::AddModel(const std::shared_ptr<AbstractModel> carcas_model) {
    carcas_models_.push_back(carcas_model);
}

void Canvas::AddLight(const std::shared_ptr<AbstractLight> light) {
    light_.push_back(light);
}


void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    AbstractStrategyProjection *strategy = new PerspectivStrategyProjection();

    DrawVisitor visitor(&painter, size(), strategy, light_);

    auto start = clock();
    for (const auto &object : carcas_models_) {
        object->Accept(visitor);
    }
    auto stop = clock();
    qDebug() << stop - start;
    delete strategy;
}
