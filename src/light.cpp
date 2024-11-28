#include "light.h"


AbstractLight::AbstractLight(double intensity)
    : intensity_(intensity) {}

AbstractLight::~AbstractLight() {};

double AbstractLight::GetIntensity() const {
    return intensity_;
}


AmbientLight::AmbientLight(double intensity)
    : AbstractLight(intensity) {}

double AmbientLight::CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const {
    Q_UNUSED(point);
    Q_UNUSED(normal);
    return GetIntensity();
}


PointLight::PointLight(double intensity, const Point &position)
    : AbstractLight(intensity)
    , position_(position) {}

PointLight::PointLight(double intensity, double x, double y, double z)
    : AbstractLight(intensity)
    , position_(x, y, z) {}

double PointLight::CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const {
    QVector3D light_vector = (position_ - point).toVector3D();
    double intensity = GetIntensity() * QVector3D::dotProduct(light_vector, normal) / (normal.length() * light_vector.length());
    return intensity > 0 ? intensity * 255 : 0;
}


DirectionLight::DirectionLight(double intensity, const QVector3D &direction)
    : AbstractLight(intensity)
    , direction_(direction.normalized()) {}


DirectionLight::DirectionLight(double intensity, double x, double y, double z)
    : AbstractLight(intensity)
    , direction_(x, y, z) {
    direction_ = direction_.normalized();
}

double DirectionLight::CalculateIntensityInPoint(const Point &point, const QVector3D &normal) const {
    Q_UNUSED(point);
    double intensity = GetIntensity() * QVector3D::dotProduct(direction_, normal) / (normal.length());
    return intensity > 0 ? intensity * 255 : 0;
}
