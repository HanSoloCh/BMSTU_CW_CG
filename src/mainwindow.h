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

    QString GetColor() const;
    QString GetAxis() const;

protected:
    void AddButtons(QWidget *central_widget, QVBoxLayout *layout);
    void AddAxisButtons(QWidget *central_widget, QVBoxLayout *layout);

private slots:
    void onGenerateButtonClicked();


private:
    SolutionViewer *solution_viewer_;
    CurveCanvas *curve_canvas_;
    QButtonGroup *radio_button_group_;
    QColor model_color_;
};
#endif // MAINWINDOW_H
