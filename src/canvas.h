#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <memory>

#include "abstractobject.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    void addObject(AbstractObject *newObject);
    void rotate(double xAngle, double yAngle, double zAngle);
protected:
    void addAxes();
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<std::shared_ptr<AbstractObject>> objectsArray;
};

#endif // CANVAS_H
