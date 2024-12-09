#include "solutionviewer.h"

#include <memory>
#include <QVBoxLayout>
#include <QKeyEvent>

#include "carcasmodel.h"
#include "facade.h"

SolutionViewer::SolutionViewer(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    canvas_facade_ = std::make_unique<Facade>();

    QVBoxLayout *layout = new QVBoxLayout(central_widget);
    layout->addWidget(canvas_facade_->GetCanvas());
}

void SolutionViewer::SetModel(const QVector<QPointF> &curve, const axis_t &axis, int slices, const QColor &color) {
    canvas_facade_->AddObject(std::make_shared<CarcasModel>(GenerateCarcasModelFromCurve(curve, axis, slices, color)));
    // По-хорошему сделать так, чтобы вращение было в самом коде генерации, но...
    if (axis == Oy)
        canvas_facade_->Rotate(0, 0, 90);
}

void SolutionViewer::keyPressEventWithoutCtrl(QKeyEvent *event) {
    int pressedButton = event->key();
    switch (pressedButton) {
    case Qt::Key_D:
        canvas_facade_->Move(5, 0, 0);
        break;
    case Qt::Key_A:
        canvas_facade_->Move(-5, 0, 0);
        break;
    case Qt::Key_W:
        canvas_facade_->Move(0, -5, 0);
        break;
    case Qt::Key_S:
        canvas_facade_->Move(0, 5, 0);
        break;
    }
}

void SolutionViewer::keyPressEventWithCtrl(QKeyEvent *event) {
    int pressedButton = event->key();
    switch (pressedButton) {
    case Qt::Key_D:
        canvas_facade_->Rotate(0, 1, 0);
        break;
    case Qt::Key_A:
        canvas_facade_->Rotate(0, -1, 0);
        break;
    case Qt::Key_S:
        canvas_facade_->Rotate(-1, 0, 0);
        break;
    case Qt::Key_W:
        canvas_facade_->Rotate(1, 0, 0);
        break;
    case Qt::Key_E:
        canvas_facade_->Rotate(0, 0, -1);
        break;
    case Qt::Key_Q:
        canvas_facade_->Rotate(0, 0, 1);
        break;
    }

}

void SolutionViewer::keyPressEvent(QKeyEvent *event) {
    bool ctrlPressed = event->modifiers() & Qt::ControlModifier;
    if (ctrlPressed)
        keyPressEventWithCtrl(event);
    else
        keyPressEventWithoutCtrl(event);
}
