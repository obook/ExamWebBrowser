/*
 * Nom : toolbar.cpp
 * Description : Barre d'outils personnalisée avec signal de clic et gestion des styles focus.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent, Qt::WindowFlags f)
    : QToolBar(parent) {
}

void ToolBar::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}

void ToolBar::setFocusOnStyle() {
    QString styleSheet(
        "QToolBar "
        "{background-color: rgba(50,255,50,128); "
        "border-radius: 1px;} "
        "QToolButton "
        "{max-width: 48px; "
        "max-height: 49px; "
        "border: 0px;}");
    setStyleSheet(styleSheet);
}

void ToolBar::setFocusOffStyle() {
    QString styleSheet(
        "QToolBar "
        "{background-color: rgba(255,50,50,128); "
        "border-radius: 1px;} "
        "QToolButton "
        "{max-width: 48px; "
        "max-height: 49px; "
        "border: 0px;}");
    setStyleSheet(styleSheet);
}

ToolBar::~ToolBar() {
}
