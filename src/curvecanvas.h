#ifndef CURVECANVAS_H
#define CURVECANVAS_H

#include <QWidget>
#include <QMouseEvent>


class CurveCanvas : public QWidget {
    Q_OBJECT
public:
    explicit CurveCanvas(QWidget *parent = nullptr);

    // Нужно убрать из public
    QVector<QPointF> generateCurvePoints();
    void setGeneratedPoints(const QVector<QPointF> &points);

public slots:
    void Clean();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QPointF> mainPoints;
    QVector<QPointF> generatedPoints;
    int steps = 50; // Количество точек на кривой
};

#endif // CURVECANVAS_H
