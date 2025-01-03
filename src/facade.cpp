#include "facade.h"

#include <QMatrix4x4>
#include <memory>

#include "abstractmodel.h"
#include "canvas.h"
#include "light.h"

Facade::Facade() : canvas_(std::make_unique<Canvas>()) { AddBaseLight(); }

void Facade::AddBaseLight() {
    AddLight(std::make_shared<DirectionLight>(1, QVector3D(0, 0, -1)));
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
    canvas_->Transform(translation_matrix);
    canvas_->update();
}

void Facade::Rotate(double x, double y, double z) {
    QMatrix4x4 rotation_matrix;
    rotation_matrix.rotate(x, 1, 0, 0);
    rotation_matrix.rotate(y, 0, 1, 0);
    rotation_matrix.rotate(z, 0, 0, 1);
    canvas_->Transform(rotation_matrix);
    canvas_->update();
}

void Facade::SetNormalMap(const QString &file_name) {
    QImage image(file_name);
    if (image.isNull())
        canvas_->SetDefault();
    else
        canvas_->SetNormalMap(QImage(file_name));
    canvas_->update();
}

void Facade::SetTexture(const QString &file_name) {
    QImage image(file_name);
    if (image.isNull())
        canvas_->SetDefault();
    else
        canvas_->SetTexuture(QImage(file_name));
    canvas_->update();
}

Canvas *Facade::GetCanvas() const { return canvas_.get(); }
