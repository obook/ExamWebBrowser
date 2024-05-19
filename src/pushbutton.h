#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QObject>
#include <QPushButton>

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

#endif // PUSHBUTTON_H
