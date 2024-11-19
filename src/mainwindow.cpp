#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "canvas.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // canvas = new Canvas(1, 1, 1, this);
    // ui->canvasLayout->setLayout(new QVBoxLayout);
    // ui->canvasLayout->layout()->addWidget(canvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}
