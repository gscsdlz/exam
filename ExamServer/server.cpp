#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    idKey = 1;
}

void Server::incomingConnection(qintptr handle)
{
    Client *client = new Client(handle);
    client->createObject();

    //QThread *thread = new QThread;
    //QObject::connect(thread, &QThread::started, client, &Client::createObject);

    QObject::connect(client, &Client::stateChange, [&](QAbstractSocket::SocketState state) {
        emit connectStateChange(state);
    });
    QObject::connect(client, &Client::messageRecv, [&](QString message, int clientId) {
       emit messageRecv(clientId, message);
    });
    //QObject::connect(client, &Client::destroyed, )

    //根据连接顺序自动建立编号 并下发给客户端
    client->setClientId(idKey);
    idKey++;
    client->sendMessage(command.stringify(Command::SERVER_HAS_CONFIRM_ID, QString::number(client->getClientId())));
    socketPool.append(client);

    //QObject::connect(this, SIGNAL(messageSend(int,QString)), client, SLOT(sendMessage(int,QString)));
    //QObject::connect(thread, &QThread::finished, client, &Client::deleteLater);
    //QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    //client->moveToThread(thread);
    //thread->start();
}

void Server::sendMessage(int clientId, QString message)
{
    if (clientId <= 0) {
        for (Client *client : socketPool) {
            client->sendMessage(message);
        }
    } else {
        for (Client *client : socketPool) {
            if (client->getClientId() == clientId) {
                client->sendMessage(message);
            }
        }
    }
}
