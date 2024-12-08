#ifndef FACADE_H
#define FACADE_H

#include <memory>

#include "canvas.h"
#include "abstractmodel.h"
#include "light.h"

class Facade {
public:
    Facade();
    ~Facade() = default;

    void AddObject(std::shared_ptr<AbstractModel> object);
    void AddLight(std::shared_ptr<AbstractLight> light);

    void Move(double x, double y, double z);
    void Rotate(double x, double y, double z);

    Canvas *GetCanvas() const;

private:
    std::unique_ptr<Canvas> canvas_;
};


#endif // FACADE_H
