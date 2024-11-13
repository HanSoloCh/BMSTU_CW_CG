#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include <QObject>
#include <QPainter>

class AbstractObject : public QObject
{
    Q_OBJECT
public:
    explicit AbstractObject(QObject *parent = nullptr);
    virtual ~AbstractObject() = 0;

    virtual void draw(QPainter &painter) = 0;
};

#endif // ABSTRACTOBJECT_H
