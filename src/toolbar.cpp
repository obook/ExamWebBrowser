/*
 * toolbar.cpp
 *
 * */

#include "toolbar.h"

ToolBar::ToolBar(QWidget* parent, Qt::WindowFlags f)
    : QToolBar(parent) {

}

void ToolBar::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}


ToolBar::~ToolBar() {

}
