#ifndef CLICKABLEBUTTON_H
#define CLICKABLEBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class ClickableButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ClickableButton(QWidget *parent = 0);

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void rightClicked();
    void leftClicked();
protected:
    using QPushButton::QPushButton;
};

#endif // CLICKABLEBUTTON_H
