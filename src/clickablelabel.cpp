#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    //connect(this, SIGNAL(myLabelClicked()), this, SLOT(slotLabelClicked()));
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

ClickableLabel::~ClickableLabel() {

}
