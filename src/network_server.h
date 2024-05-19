#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <QObject>

class network_server : public QObject
{
    Q_OBJECT
public:
    explicit network_server(QObject *parent = nullptr);

signals:
};

#endif // NETWORK_SERVER_H
