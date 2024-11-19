#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QColor>

class BaseDrawVisitor;

class AbstractModel
{
public:
    explicit AbstractModel(const QColor &color = Qt::white);
    virtual ~AbstractModel() = 0;

    QColor GetColor() const {
        return color_;
    }

    virtual void Accept(BaseDrawVisitor &visitor) const = 0;
private:
    QColor color_;
};

#endif // ABSTRACTMODEL_H
