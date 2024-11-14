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
}

