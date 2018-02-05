#ifndef CANVASLABEL_H
#define CANVASLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPoint>

/*
 * Represents the canvas of the Sprite Editor, custom class required to use the mousePressEvent.
 */
class CanvasLabel : public QLabel
{

Q_OBJECT

public:
    explicit CanvasLabel(const QString& pstrText = "", QWidget* pvntParent = 0);
    ~CanvasLabel();

signals:
    void clicked(CanvasLabel*, QPoint);

protected:
    void mousePressEvent(QMouseEvent* pvntEvent);
    void mouseMoveEvent(QMouseEvent* pvntEvent);

private:
    QPoint gvntPoint;
};

#endif // CANVASLABEL_H
