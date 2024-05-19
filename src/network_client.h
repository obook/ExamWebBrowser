#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>

class network_client : public QObject
{
    Q_OBJECT
public:
    explicit network_client(QObject *parent = nullptr);
    void Start(QString IpServer);
    void Connect(QString ServerIp, int Port);
private:
    QTcpSocket *socket;
signals:
};

#endif // NETWORK_CLIENT_H
