#include "facade.h"

#include <memory.h>
#include <QMatrix4x4>

#include "canvas.h"
#include "abstractmodel.h"
#include "light.h"


Facade::Facade(Canvas *canvas)
    : canvas_(canvas) {}

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
