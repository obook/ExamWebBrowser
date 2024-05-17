#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QWidget>
#include <Qt>

class ToolBar : public QToolBar {
    Q_OBJECT

public:
    explicit ToolBar(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ToolBar();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);

    using QToolBar::QToolBar;
};

#endif // TOOLBAR_H
