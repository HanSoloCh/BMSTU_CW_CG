#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>

#include <memory>

#include "abstractmodel.h"


class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void AddModel(const std::shared_ptr<AbstractModel> &carcas_model);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<std::shared_ptr<AbstractModel>> carcas_models_;
};

#endif // CANVAS_H
