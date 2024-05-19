#include "rightclickbutton.h"
#include <QMouseEvent>

QRightClickButton::QRightClickButton(QWidget *parent) :
    QPushButton(parent)
{
}

void QRightClickButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        // OK qDebug() << "QRightClickButton mousePressEvent!";
        emit rightClicked();
    }
    QPushButton::mousePressEvent(event);
}

QRightClickButton::~QRightClickButton() {

}

/*
bool PushButton::mousePressEvent(QObject *obj, QEvent *event)
{
    if (obj == PushButton && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (event->modifiers() == Qt::ShiftModifier) {
            //do what you need
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}


void PushButton::mousePressEvent(QMouseEvent *e)
{
    if (e->modifiers() == Qt::ShiftModifier) {
        //do what you need
        return;
    }
    QPushButton::mousePressEvent(e);
}


*/
