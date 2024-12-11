#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QVBoxLayout>

#include "solutionviewer.h"
#include "curvecanvas.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    void AddButtons(QWidget *central_widget, QVBoxLayout *layout);
    void AddAxisButtons(QWidget *central_widget, QVBoxLayout *layout);
    void AddMaterialsButtons(QWidget *central_widget, QVBoxLayout *layout);

private slots:
    void onGenerateButtonClicked();


private:
    QString GetFileName() const;

    CurveCanvas *curve_canvas_;
    QButtonGroup *axis_button_group_;
    QButtonGroup *materials_button_group_;
};
#endif // MAINWINDOW_H
