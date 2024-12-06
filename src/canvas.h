#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>

#include <memory>
#include <QMatrix4x4>

#include "abstractmodel.h"
#include "light.h"


class Canvas : public QWidget {
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void AddModel(const std::shared_ptr<AbstractModel> carcas_model);
    void AddLight(const std::shared_ptr<AbstractLight> light);

    void Transform(const QMatrix4x4 &transform_matrix);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<std::shared_ptr<AbstractModel>> scene_objects_;
    QVector<std::shared_ptr<AbstractLight>> light_;
};

#endif // CANVAS_H
