#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "facade.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    canvas_facade_ = new Facade();

    ui->canvasLayout->setLayout(new QVBoxLayout);
    ui->canvasLayout->layout()->addWidget(canvas_facade.GetCanvas());
}

MainWindow::~MainWindow()
{
    delete ui;
}
