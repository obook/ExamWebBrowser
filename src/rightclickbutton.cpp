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
