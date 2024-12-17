#include "abstractmodel.h"

#include "drawvisitor.h"

AbstractModel::AbstractModel(const QColor &color) : color_(color) {}

AbstractModel::~AbstractModel() {}

void AbstractModel::Accept(BaseDrawVisitor *visitor) const {
    visitor->Visit(*this);
}
