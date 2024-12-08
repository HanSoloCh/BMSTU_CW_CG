#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "canvas.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        auto *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        curveCanvas = new CurveCanvas(this);
        auto *generateButton = new QPushButton("Generate Points", this);

        auto *layout = new QVBoxLayout(centralWidget);
        layout->addWidget(curveCanvas);
        layout->addWidget(generateButton);

        connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGeneratePoints);
    }

private slots:
    void onGeneratePoints() {
        QVector<QPointF> points = curveCanvas->generateCurvePoints();
        for (const auto &point : points) {
            qDebug() << "Point:" << point;
        }
        curveCanvas->setGeneratedPoints(points);
    }

private:
    CurveCanvas *curveCanvas;
};

#endif // MAINWINDOW_H
