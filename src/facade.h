#ifndef FACADE_H
#define FACADE_H

#include <memory>

#include "canvas.h"

#include "abstractmodel.h"

class Facade
{
public:
    Facade(std::unique_ptr<Canvas> canvas);
    Facade(Canvas *canvas);

    void AddObject(std::shared_ptr<AbstractModel> object);

    Canvas *GetCanvas() const;

private:
    std::unique_ptr<Canvas> canvas_;
};



#endif // FACADE_H
