#include "canvas.h"

#include <QPainter>
#include <memory>

#include "point.h"
#include "triangle.h"
#include "carcasmodel.h"

#include "strategy.h"
#include "drawvisitor.h"


Canvas::Canvas(QWidget *parent)
    : QWidget(parent) {}

void Canvas::AddModel(const std::shared_ptr<AbstractModel> &carcas_model) {
    carcas_models_.push_back(carcas_model);
}


void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing);

    Point A(-0.5, 1, 8);
    Point B(-2.5, 1, 8);
    Point C(-2.5, -1, 8);
    Point D(-0.5, -1, 8);

    Point E(-0.5, 1, 6);
    Point F(-2.5, 1, 6);
    Point G(-2.5, -1, 6);
    Point H(-0.5, -1, 6);

    std::shared_ptr<Triangle> t1 = std::make_shared<Triangle>(QVector<Point>{A, B, C}, Qt::red);
    std::shared_ptr<Triangle> t2 = std::make_shared<Triangle>(QVector<Point>{A, C, D}, Qt::red);
    std::shared_ptr<Triangle> t3 = std::make_shared<Triangle>(QVector<Point>{E, A, D}, Qt::green);
    std::shared_ptr<Triangle> t4 = std::make_shared<Triangle>(QVector<Point>{E, D, H}, Qt::green);
    std::shared_ptr<Triangle> t5 = std::make_shared<Triangle>(QVector<Point>{F, E, H}, Qt::cyan);
    std::shared_ptr<Triangle> t6 = std::make_shared<Triangle>(QVector<Point>{F, H, G}, Qt::cyan);
    std::shared_ptr<Triangle> t7 = std::make_shared<Triangle>(QVector<Point>{B, F, G}, Qt::yellow);
    std::shared_ptr<Triangle> t8 = std::make_shared<Triangle>(QVector<Point>{B, F, C}, Qt::yellow);
    std::shared_ptr<Triangle> t9 = std::make_shared<Triangle>(QVector<Point>{E, F, B}, Qt::white);
    std::shared_ptr<Triangle> t10 = std::make_shared<Triangle>(QVector<Point>{E, B, A}, Qt::white);
    std::shared_ptr<Triangle> t11 = std::make_shared<Triangle>(QVector<Point>{C, G, H}, Qt::blue);
    std::shared_ptr<Triangle> t12 = std::make_shared<Triangle>(QVector<Point>{C, H, D}, Qt::blue);


    // AddModel(t1);
    // AddModel(t12);
    // AddModel(t2);
    // AddModel(t4);
    // AddModel(t6);
    // AddModel(t5);
    // AddModel(t11);
    // AddModel(t7);
    // AddModel(t10);
    // AddModel(t8);
    // AddModel(t9);
    // AddModel(t3);

    // AddModel(std::make_shared<Triangle>(A, B, C, Qt::green));

    AddModel(std::make_shared<CarcasModel>(GenerateShape(2, 10, 10, Qt::green, Point(0, 0, 8))));

    AbstractStrategyProjection *strategy = new PerspectivStrategyProjection();

    // painter.drawLine(strategy->ProjectPoint({0, 0, 3}, {1000, 1000}), strategy->ProjectPoint({10, 10, 3}, {1000, 1000}));
    DrawVisitor visitor(&painter, size(), strategy);


    for (auto &object : carcas_models_) {
        object->Accept(visitor);
    }
    delete strategy;
}
