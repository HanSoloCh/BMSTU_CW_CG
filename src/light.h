#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

#include "point.h"

class AbstractLight {
public:
    AbstractLight(double intensity);
    virtual ~AbstractLight() = 0;

    double GetIntensity() const;

    virtual double CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const = 0;
private:
    double intensity_;
};


class AmbientLight: public AbstractLight {
public:
    AmbientLight(double intensity);

    double CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const override;
};

// В работе не используется, можно удалить
// (DrawVisitor не правильно высчитывает освещенность, если есть такой истчник)
class PointLight: public AbstractLight {
public:
    PointLight(double intensity, const Point &position);
    PointLight(double intensity, double x, double y, double z);

    double CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const override;

private:
    Point position_;
};

class DirectionLight: public AbstractLight {
public:
    DirectionLight(double intensity, const QVector3D &direction);
    DirectionLight(double intensity, double x, double y, double z);

    double CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const override;
private:
    QVector3D direction_;
};

#endif // LIGHT_H
