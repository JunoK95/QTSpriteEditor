#include "canvaslabel.h"
#include <QDebug>

/*
 * Represents the canvas of the Sprite Editor, custom class required to use the mousePressEvent.
 */
CanvasLabel::CanvasLabel(const QString& pstrText, QWidget* pvntParent) :
    QLabel(pvntParent)
{
    setText(pstrText);
}

CanvasLabel::~CanvasLabel()
{
}
// Allows the x,y position of the mouse to be retrieved after a mouse press. pvntEvent represents the event being used.
void CanvasLabel::mousePressEvent(QMouseEvent* pvntEvent)
{
    emit clicked(this, pvntEvent->pos());
}
// Allows mouse movement to be tracked, and x,y position retrieved. pvntEvent represents the event being used.
void CanvasLabel::mouseMoveEvent(QMouseEvent* pvntEvent)
{
    if(pvntEvent->buttons() & Qt::LeftButton)
    {
        emit clicked(this, pvntEvent->pos());
    }
}
