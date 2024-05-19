#ifndef RIGHTCLICKBUTTON_H
#define RIGHTCLICKBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class QRightClickButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QRightClickButton(QWidget *parent = 0);
    ~QRightClickButton();

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void rightClicked();
protected:
    using QPushButton::QPushButton;
};

/*

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~PushButton();

protected:
    void mousePressEvent(QMouseEvent *e);

    using QPushButton::QPushButton;
};
*/

#endif // RIGHTCLICKBUTTON_H
