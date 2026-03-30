/*
 * Nom : clickablelabel.h
 * Description : Label personnalisé émettant un signal clicked() lors d'un clic souris.
 * Auteur : O. Booklage
 * Date : 2024
 */

#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event);

    using QLabel::QLabel;
};

#endif /* CLICKABLELABEL_H */
