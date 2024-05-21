#include "clickablebutton.h"
#include <QMouseEvent>

ClickableButton::ClickableButton(QWidget *parent) :
    QPushButton(parent)
{
}

void ClickableButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton) {
        emit rightClicked();
    }
    else if (event->button()==Qt::LeftButton) {
        emit leftClicked();
    }
    QPushButton::mousePressEvent(event);
}
