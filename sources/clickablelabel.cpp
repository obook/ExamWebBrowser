/*
 * Nom : clickablelabel.cpp
 * Description : Label personnalisé émettant un signal clicked() lors d'un clic souris.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent) {
}

void ClickableLabel::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}

ClickableLabel::~ClickableLabel() {
}
