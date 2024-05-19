#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <QObject>
#include <QString>

class network_client : public QObject
{
    Q_OBJECT
public:
    explicit network_client(QObject *parent = nullptr);
    void Start(QString IpServer);
signals:
};

#endif // NETWORK_CLIENT_H
