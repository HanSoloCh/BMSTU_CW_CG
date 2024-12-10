#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QColorDialog>
#include <QLabel>

#include "solutionviewer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QVBoxLayout *layout = new QVBoxLayout(central_widget);

    AddAxisButtons(central_widget, layout);
    AddMaterialsButtons(central_widget, layout);

    curve_canvas_ = new CurveCanvas(central_widget);
    layout->addWidget(curve_canvas_);

    AddButtons(central_widget, layout);
}

void MainWindow::AddButtons(QWidget *central_widget, QVBoxLayout *layout) {
    QPushButton *select_color = new QPushButton("Задать цвет", central_widget);
    connect(select_color, &QPushButton::clicked, this, [this](){
        QColor color = QColorDialog::getColor(Qt::blue, this, "Выберите цвет");
        if (color.isValid()) {
            curve_canvas_->SetColor(color);
        }
    });

    QPushButton *clean_canvas = new QPushButton("Очистить", central_widget);
    connect(clean_canvas, &QPushButton::clicked, curve_canvas_, &CurveCanvas::Clean);

    QPushButton *generate_button = new QPushButton("Сгенерировать тело вращения", central_widget);
    connect(generate_button, &QPushButton::clicked, this, &MainWindow::onGenerateButtonClicked);

    QHBoxLayout *buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(select_color);
    buttons_layout->addWidget(clean_canvas);

    layout->addLayout(buttons_layout);
    layout->addWidget(generate_button);
}

void MainWindow::AddAxisButtons(QWidget *central_widget, QVBoxLayout *layout) {
    axis_button_group_ = new QButtonGroup(central_widget);
    QRadioButton *radio_button_ox = new QRadioButton("Ось OX", central_widget);
    QRadioButton *radio_button_oy = new QRadioButton("Ось OY", central_widget);

    radio_button_ox->setChecked(true);

    // Создаем группу радиокнопок
    axis_button_group_->addButton(radio_button_ox, 1);
    axis_button_group_->addButton(radio_button_oy, 2);

    QHBoxLayout *radio_box_layout = new QHBoxLayout();

    radio_box_layout->addWidget(radio_button_ox);
    radio_box_layout->addWidget(radio_button_oy);

    layout->addLayout(radio_box_layout);
}

void MainWindow::AddMaterialsButtons(QWidget *central_widget, QVBoxLayout *layout) {
    materials_button_group_ = new QButtonGroup(central_widget);
    QRadioButton *radio_button_no_material = new QRadioButton("Без материала", central_widget);
    QRadioButton *radio_button_wood = new QRadioButton("Дерево", central_widget);
    QRadioButton *radio_button_rock = new QRadioButton("Камень", central_widget);
    QRadioButton *radio_button_metal = new QRadioButton("Металл", central_widget);

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

    QLabel *label = new QLabel("Материал:", central_widget);

    layout->addWidget(label);
    layout->addLayout(radio_box_layout);
}


void MainWindow::onGenerateButtonClicked() {
    QVector<QPointF> curve_points = curve_canvas_->GetCurvePoints();
    if (curve_points.size() < 2) {
        QMessageBox::warning(this, "Ошибка", "Необходимо задать хотя бы две точки.");
        return;
    }

    // Показать окно тела вращения
    QString texture_name = GetFileName();
    SolutionViewer *solution_viewer = new SolutionViewer(QImage(texture_name), this);
    for (auto &point : curve_points) {
        point -= QPointF(curve_canvas_->width() / 2, curve_canvas_->height() / 2);
        point.setY(-point.y());
    }

    solution_viewer->SetModel(curve_points, static_cast<axis_t>(axis_button_group_->checkedId()), 30, curve_canvas_->GetColor());
    solution_viewer->show();
}

QString MainWindow::GetFileName() const {
    switch (materials_button_group_->checkedId()) {
    case 0:
        return "";
    case 1:
        return "wood.png";
    case 2:
        return "rock.png";
    case 3:
        return "metal.png";
    default:
        return "";
    }
}
