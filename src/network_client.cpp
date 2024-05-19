#include "network_client.h"



network_client::network_client(QObject *parent)
    : QObject{parent}
{
    qDebug() << "network_client:network_client";
}

void network_client::Start(QString IpServer) {

}

void network_client::Connect(QString ServerIp, int Port)
{
    qDebug() << "network_client:Connect";

    socket = new QTcpSocket(this);
    socket->connectToHost(ServerIp, Port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        // send
        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();

        qDebug() << socket->readAll();

        socket->close();
    }
    else
    {
        qDebug() << "Not connected!";
    }

    // sent

    // got

    // closed
}
