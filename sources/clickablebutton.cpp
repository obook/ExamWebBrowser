/*
 * Nom : clickablebutton.cpp
 * Description : Bouton personnalisé émettant des signaux distincts pour clic gauche et clic droit.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "clickablebutton.h"

ClickableButton::ClickableButton(QWidget *parent)
    : QPushButton(parent) {
}

void ClickableButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        emit rightClicked();
    } else if (event->button() == Qt::LeftButton) {
        emit leftClicked();
    }
    QPushButton::mousePressEvent(event);
}
