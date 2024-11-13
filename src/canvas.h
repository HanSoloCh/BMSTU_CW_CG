#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "abstractobject.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    QPointF projectTo2D(float x, float y, float z);
    void printAxes();

private:
    QVector<AbstractObject *> objectsArray;
};

#endif // CANVAS_H
