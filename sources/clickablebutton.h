/*
 * Nom : clickablebutton.h
 * Description : Bouton personnalisé émettant des signaux distincts pour clic gauche et clic droit.
 * Auteur : O. Booklage
 * Date : 2024
 */

#ifndef CLICKABLEBUTTON_H
#define CLICKABLEBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class ClickableButton : public QPushButton {
    Q_OBJECT

public:
    explicit ClickableButton(QWidget *parent = nullptr);

signals:
    void rightClicked();
    void leftClicked();

protected:
    void mousePressEvent(QMouseEvent *event);

    using QPushButton::QPushButton;
};

#endif /* CLICKABLEBUTTON_H */
