#include "pushbutton.h"
#include <QMouseEvent>

PushButton::PushButton(QWidget* parent, Qt::WindowFlags f)
    : QPushButton(parent) {
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
*/

void PushButton::mousePressEvent(QMouseEvent *e)
{
    if (e->modifiers() == Qt::ShiftModifier) {
        //do what you need
        return;
    }
    QPushButton::mousePressEvent(e);
}

PushButton::~PushButton() {

}
