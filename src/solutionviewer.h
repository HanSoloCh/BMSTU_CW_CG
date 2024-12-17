#ifndef SOLUTIONVIEWER_H
#define SOLUTIONVIEWER_H

#include <QButtonGroup>
#include <QMainWindow>
#include <QVBoxLayout>

#include "carcasmodel.h"
#include "facade.h"
#include "memory"

class SolutionViewer : public QMainWindow {
    Q_OBJECT
   public:
    explicit SolutionViewer(QWidget *parent = nullptr);
    explicit SolutionViewer(const QImage &normal_map,
                            QWidget *parent = nullptr);

    void SetModel(const QVector<QPointF> &curve, const axis_t &axis,
                  int slices = 30, const QColor &color = Qt::gray);

   protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyPressEventWithCtrl(QKeyEvent *event);
    void keyPressEventWithoutCtrl(QKeyEvent *event);

    void AddMaterialsButtons(QWidget *central_widget, QVBoxLayout *layout);
    void AddTexturesButtons(QWidget *central_widget, QVBoxLayout *layout);

    QString GetNormalMapName(int id) const;
    QString GetTextureName(int id) const;

   private:
    void SetSentralWidget();
    void SetDefaultButton(QButtonGroup *button_group);

    std::unique_ptr<Facade> canvas_facade_;

    QButtonGroup *materials_button_group_;
    QButtonGroup *textures_button_group_;
};

#endif  // SOLUTIONVIEWER_H
