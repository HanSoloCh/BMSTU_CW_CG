#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "facade.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void setValidatorsToIntputEdits();
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_setForming_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<CanvasFacade> canvasFacade;
};
#endif // MAINWINDOW_H
