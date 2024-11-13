#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QIntValidator>

#include "canvas.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->canvasLayout->setLayout(new QVBoxLayout);
    ui->canvasLayout->layout()->addWidget(new Canvas(this));
    ui->x1InputEdit->setValidator(new QIntValidator);
    ui->x2InputEdit->setValidator(new QIntValidator);
    ui->y1InputEdit->setValidator(new QIntValidator);
    ui->y2InputEdit->setValidator(new QIntValidator);
}

MainWindow::~MainWindow()
{
    delete ui;
}
