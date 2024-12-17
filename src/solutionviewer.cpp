#include "solutionviewer.h"

#include <QKeyEvent>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <memory>

#include "carcasmodel.h"
#include "facade.h"

SolutionViewer::SolutionViewer(QWidget *parent)
    : QMainWindow(parent), canvas_facade_(std::make_unique<Facade>()) {
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QVBoxLayout *layout = new QVBoxLayout(central_widget);

    AddMaterialsButtons(central_widget, layout);
    AddTexturesButtons(central_widget, layout);

    layout->addWidget(canvas_facade_->GetCanvas());
}

void SolutionViewer::SetSentralWidget() {
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QVBoxLayout *layout = new QVBoxLayout(central_widget);
    layout->addWidget(canvas_facade_->GetCanvas());
}

void SolutionViewer::SetDefaultButton(QButtonGroup *button_group) {
    button_group->blockSignals(true);
    button_group->button(0)->setChecked(true);
    button_group->blockSignals(false);
}

void SolutionViewer::SetModel(const QVector<QPointF> &curve, const axis_t &axis,
                              int slices, const QColor &color) {
    canvas_facade_->AddObject(std::make_shared<CarcasModel>(
        GenerateCarcasModelFromCurve(curve, axis, slices, color)));
    // По-хорошему сделать так, чтобы вращение было в самом коде генерации,
    // но...
    if (axis == Oy) canvas_facade_->Rotate(0, 0, 90);
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

void SolutionViewer::AddMaterialsButtons(QWidget *central_widget,
                                         QVBoxLayout *layout) {
    materials_button_group_ = new QButtonGroup(central_widget);
    QRadioButton *radio_button_no_material =
        new QRadioButton("Без материала", central_widget);
    QRadioButton *radio_button_wood =
        new QRadioButton("Дерево", central_widget);
    QRadioButton *radio_button_rock =
        new QRadioButton("Камень", central_widget);
    QRadioButton *radio_button_metal =
        new QRadioButton("Металл", central_widget);

    radio_button_no_material->setChecked(true);
    materials_button_group_->addButton(radio_button_no_material, 0);
    materials_button_group_->addButton(radio_button_wood, 1);
    materials_button_group_->addButton(radio_button_rock, 2);
    materials_button_group_->addButton(radio_button_metal, 3);

    QHBoxLayout *radio_box_layout = new QHBoxLayout();
    radio_box_layout->addWidget(radio_button_no_material);
    radio_box_layout->addWidget(radio_button_wood);
    radio_box_layout->addWidget(radio_button_rock);
    radio_box_layout->addWidget(radio_button_metal);

    connect(materials_button_group_, &QButtonGroup::idToggled, this,
            [this](int id, bool checked) {
                SetDefaultButton(textures_button_group_);
                if (checked) {
                    canvas_facade_->SetNormalMap(GetNormalMapName(id));
                }
            });

    QLabel *label = new QLabel("Материал:", central_widget);

    layout->addWidget(label);
    layout->addLayout(radio_box_layout);
}

void SolutionViewer::AddTexturesButtons(QWidget *central_widget,
                                        QVBoxLayout *layout) {
    textures_button_group_ = new QButtonGroup(central_widget);
    QRadioButton *radio_button_no_material =
        new QRadioButton("Без текстуры", central_widget);
    QRadioButton *radio_button_wood =
        new QRadioButton("Дерево", central_widget);
    QRadioButton *radio_button_rock =
        new QRadioButton("Камень", central_widget);
    QRadioButton *radio_button_metal =
        new QRadioButton("Металл", central_widget);

    radio_button_no_material->setChecked(true);
    textures_button_group_->addButton(radio_button_no_material, 0);
    textures_button_group_->addButton(radio_button_wood, 1);
    textures_button_group_->addButton(radio_button_rock, 2);
    textures_button_group_->addButton(radio_button_metal, 3);

    QHBoxLayout *radio_box_layout = new QHBoxLayout();
    radio_box_layout->addWidget(radio_button_no_material);
    radio_box_layout->addWidget(radio_button_wood);
    radio_box_layout->addWidget(radio_button_rock);
    radio_box_layout->addWidget(radio_button_metal);

    connect(textures_button_group_, &QButtonGroup::idToggled, this,
            [this](int id, bool checked) {
                SetDefaultButton(materials_button_group_);
                if (checked) {
                    canvas_facade_->SetTexture(GetTextureName(id));
                }
            });

    QLabel *label = new QLabel("Текстура:", central_widget);

    layout->addWidget(label);
    layout->addLayout(radio_box_layout);
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
        case Qt::Key_Minus:
            canvas_facade_->Move(0, 0, 5);
            break;
        case Qt::Key_Equal:
            canvas_facade_->Move(0, 0, -5);
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

QString SolutionViewer::GetNormalMapName(int id) const {
    switch (id) {
        case 1:
            return "wood_map.png";
        case 2:
            return "rock_map.png";
        case 3:
            return "metal_map.png";
        default:
            return ".";
    }
}

QString SolutionViewer::GetTextureName(int id) const {
    switch (id) {
        case 1:
            return "wood.png";
        case 2:
            return "rock.png";
        case 3:
            return "metal.png";
        default:
            return ".";
    }
}
