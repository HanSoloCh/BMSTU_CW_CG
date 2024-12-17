#ifndef FACADE_H
#define FACADE_H

#include <memory>

#include "abstractmodel.h"
#include "canvas.h"
#include "light.h"

class Facade {
   public:
    Facade();
    explicit Facade(const QImage &normal_map);
    ~Facade() = default;

    void AddObject(std::shared_ptr<AbstractModel> object);
    void AddLight(std::shared_ptr<AbstractLight> light);

    void Move(double x, double y, double z);
    void Rotate(double x, double y, double z);

    void SetNormalMap(const QString &file_name);
    void SetTexture(const QString &file_name);

    Canvas *GetCanvas() const;

   private:
    void AddBaseLight();
    std::unique_ptr<Canvas> canvas_;
};

#endif  // FACADE_H
