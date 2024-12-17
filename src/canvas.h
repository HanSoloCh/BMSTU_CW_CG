#ifndef CANVAS_H
#define CANVAS_H

#include <QMatrix4x4>
#include <QPainter>
#include <QWidget>
#include <memory>

#include "abstractmodel.h"
#include "light.h"

// Можно было бы полиморфизмом
typedef enum {
    def,
    mapping,
    textuting,
} draw_type_t;

class Canvas : public QWidget {
    Q_OBJECT
   public:
    explicit Canvas(QWidget *parent = nullptr);

    void AddModel(const std::shared_ptr<AbstractModel> carcas_model);
    void AddLight(const std::shared_ptr<AbstractLight> light);

    void Transform(const QMatrix4x4 &transform_matrix);

    void SetDefault();
    void SetNormalMap(const QImage &normal_map);
    void SetTexuture(const QImage &texture);

   protected:
    void paintEvent(QPaintEvent *event) override;

   private:
    QVector<std::shared_ptr<AbstractModel>> scene_objects_;
    QVector<std::shared_ptr<AbstractLight>> light_;

    // Можно сделать целый класс из этого
    draw_type_t draw_type_;
    QImage image_;
};

#endif  // CANVAS_H
