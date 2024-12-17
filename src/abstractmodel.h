#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QColor>
#include <QMatrix4x4>

class BaseDrawVisitor;

class AbstractModel {
   public:
    explicit AbstractModel(const QColor &color = Qt::gray);
    virtual ~AbstractModel() = 0;

    QColor GetColor() const { return color_; }

    virtual void Accept(BaseDrawVisitor *visitor) const = 0;
    // По идее можно сделать visitor-ом
    virtual void Transform(const QMatrix4x4 &transform_matrix) = 0;

   private:
    QColor color_;
};

#endif  // ABSTRACTMODEL_H
