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
    void addObject(std::shared_ptr<AbstractObject> newObject);

    void move(double xMove, double yMove, double zMove);
    void rotate(double xAngle, double yAngle, double zAngle);
    void scale(double xScale, double yScale, double zScale);

    void deleteAll();
protected:
    void addAxes();

    void saveMove(double xMove, double yMove, double zMove);
    void saveRotate(double xAngle, double yAngle, double zAngle);
    void saveScale(double xScale, double yScale, double zScale);

    void paintEvent(QPaintEvent *event) override;

private:
    QVector<std::shared_ptr<AbstractObject>> objectsArray;

    struct {
        struct {
            double xMove = 0;
            double yMove = 0;
            double zMove = 0;
        } MoveTransform;
        struct {
            double xAngle = 0;
            double yAngle = 0;
            double zAngle = 0;
        } RotateTransform;
        struct {
            double xScale = 1;
            double yScale = 1;
            double zScale = 1;
        } ScaleTransform;
    } Transformation;
};

#endif // CANVAS_H
