#include "facade.h"

#include <QMatrix4x4>
#include <memory>

#include "canvas.h"
#include "abstractmodel.h"
#include "light.h"


Facade::Facade()
    : canvas_(std::make_unique<Canvas>()) {}

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
