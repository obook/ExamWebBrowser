/*
 * Nom : toolbar.h
 * Description : Barre d'outils personnalisée avec signal de clic et gestion des styles focus.
 * Auteur : O. Booklage
 * Date : 2024
 */

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QWidget>
#include <Qt>

class ToolBar : public QToolBar {
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ToolBar();

    void setFocusOnStyle();
    void setFocusOffStyle();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event);

    using QToolBar::QToolBar;
};

#endif /* TOOLBAR_H */
