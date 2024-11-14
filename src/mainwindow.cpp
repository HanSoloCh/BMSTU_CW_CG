#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QIntValidator>
#include <QKeyEvent>


#include "facade.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvasFacade = std::make_unique<CanvasFacade>();
    ui->canvasLayout->setLayout(new QVBoxLayout);
    ui->canvasLayout->layout()->addWidget(canvasFacade->getCanvas());

    setValidatorsToIntputEdits();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setValidatorsToIntputEdits()
{
    ui->x1InputEdit->setValidator(new QIntValidator);
    ui->x2InputEdit->setValidator(new QIntValidator);
    ui->y1InputEdit->setValidator(new QIntValidator);
    ui->y2InputEdit->setValidator(new QIntValidator);
    ui->z1InputEdit->setValidator(new QIntValidator);
    ui->z2InputEdit->setValidator(new QIntValidator);
}

void MainWindow::on_setForming_clicked()
{
    canvasFacade->addLine(ui->x1InputEdit->text().toDouble(),
                          ui->y1InputEdit->text().toDouble(),
                          ui->z1InputEdit->text().toDouble(),
                          ui->x2InputEdit->text().toDouble(),
                          ui->y2InputEdit->text().toDouble(),
                          ui->z2InputEdit->text().toDouble(),
                          Qt::white);
    Point start{ui->x1InputEdit->text().toDouble(), ui->y1InputEdit->text().toDouble(), ui->z1InputEdit->text().toDouble()};
    Point end{ui->x2InputEdit->text().toDouble(), ui->y2InputEdit->text().toDouble(), ui->z2InputEdit->text().toDouble()};
    canvasFacade->line = Line(qMakePair(start, end), Qt::black);

    canvasFacade->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_D:
        canvasFacade->rotate(0, 0, 1);
        break;
    case Qt::Key_A:
        canvasFacade->rotate(0, 0, -1);
        break;
    case Qt::Key_S:
        canvasFacade->rotate(1, 0, 0);
        break;
    case Qt::Key_W:
        canvasFacade->rotate(-1, 0, 0);
        break;
    case Qt::Key_E:
        canvasFacade->rotate(0, 1, 0);
        break;
    case Qt::Key_Q:
        canvasFacade->rotate(0, -1, 0);
        break;
    }
}


void MainWindow::on_drawButton_clicked()
{
    int steps = 60;
    Line *line = &(canvasFacade->line);
    std::vector<std::vector<QPointF>> surfacePoints;
    float angleStep = 360.0f / steps;

    // Генерация точек поверхности вращения
    for (int i = 0; i <= steps; ++i) {
        float angle = i * angleStep;
        QVector3D rotatedStart = rotatePoint(line->start(), angle, axis);
        QVector3D rotatedEnd = rotatePoint(line->end(), angle, axis);

        surfacePoints.push_back({ projectTo2D(rotatedStart), projectTo2D(rotatedEnd) });
    }

    // Теперь соединяем эти точки в треугольники для создания полигональной сетки
    for (int i = 0; i < surfacePoints.size() - 1; ++i) {
        for (int j = 0; j < surfacePoints[i].size() - 1; ++j) {
            QPointF p1 = surfacePoints[i][j];
            QPointF p2 = surfacePoints[i + 1][j];
            QPointF p3 = surfacePoints[i + 1][j + 1];
            QPointF p4 = surfacePoints[i][j + 1];

            // Создайте два треугольника
            addTriangle(p1, p2, p3);
            addTriangle(p1, p3, p4);
        }
    }
}

}

