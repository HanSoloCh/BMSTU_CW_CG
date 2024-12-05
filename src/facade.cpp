#include "facade.h"

Facade::Facade(Canvas *canvas)
    : canvas_(canvas) {}

void Facade::AddObject(std::shared_ptr<AbstractModel> object) {
    canvas_->AddModel(object);
    canvas_->update();
}


Canvas *Facade::GetCanvas() const {
    return canvas_.get();
}
