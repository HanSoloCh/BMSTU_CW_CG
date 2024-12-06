#include "facade.h"

#include <QMatrix4x4>
#include <memory>

#include "canvas.h"
#include "abstractmodel.h"
#include "light.h"
#include "carcasmodel.h"


Facade::Facade()
    : canvas_(std::make_unique<Canvas>()) {
    AddLight(std::make_shared<DirectionLight>(1, QVector3D(1, 1, -1)));
    AddObject(std::make_shared<CarcasModel>(GenerateShape(2, 3, 3, Qt::green, Point(0, 0, 8))));
}

void Facade::AddObject(std::shared_ptr<AbstractModel> object) {
    canvas_->AddModel(object);
    canvas_->update();
}

void Facade::AddLight(std::shared_ptr<AbstractLight> light) {
    canvas_->AddLight(light);
    canvas_->update();
}

void Facade::Move(double x, double y, double z) {
    QMatrix4x4 translation_matrix;
    translation_matrix.translate(x, y, z);
}


Canvas *Facade::GetCanvas() const {
    return canvas_.get();
}
