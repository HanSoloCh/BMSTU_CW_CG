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
    AmbientLight(double intensity);

    double CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const override;
};

class PointLight: public AbstractLight {
    PointLight(double intensity, const Point &position);
    PointLight(double intensity, double x, double y, double z);

    double CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const override;

private:
    Point position_;
};

class DirectionLight: public AbstractLight {
    DirectionLight(double intensity, const QVector3D &direction);
    DirectionLight(double intensity, double x, double y, double z);

    double CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const override;
private:
    QVector3D direction_;
};

#endif // LIGHT_H
