#ifndef CURVECANVAS_H
#define CURVECANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QColor>

class CurveCanvas : public QWidget {
    Q_OBJECT
public:
    explicit CurveCanvas(QWidget *parent = nullptr);

    QVector<QPointF> GetCurvePoints(int steps = 50) const;
    QColor GetColor() const noexcept;

public slots:
    void SetColor(const QColor &color);
    void Clean();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QPointF> main_points_;
    QColor color_;
};

#endif // CURVECANVAS_H
