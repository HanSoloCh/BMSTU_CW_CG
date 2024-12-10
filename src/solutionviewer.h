#ifndef SOLUTIONVIEWER_H
#define SOLUTIONVIEWER_H

#include <QMainWindow>

#include "memory"
#include "facade.h"
#include "carcasmodel.h"

class SolutionViewer : public QMainWindow {
    Q_OBJECT
public:
    explicit SolutionViewer(QWidget *parent = nullptr);
    explicit SolutionViewer(const QImage &normal_map, QWidget *parent = nullptr);

    void SetModel(const QVector<QPointF> &curve, const axis_t &axis, int slices = 30, const QColor &color = Qt::gray);
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyPressEventWithCtrl(QKeyEvent *event);
    void keyPressEventWithoutCtrl(QKeyEvent *event);

private:
    void SetSentralWidget();
    std::unique_ptr<Facade> canvas_facade_;
};

#endif // SOLUTIONVIEWER_H
