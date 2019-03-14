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
        if (state == QAbstractSocket::UnconnectedState) {
            QVector<int> mustDel;
            for (auto it = socketPool.begin(); it != socketPool.end(); it++) {
                Client *c = it.value();
                if (c != NULL && c->getState() == QAbstractSocket::UnconnectedState) {
                    emit lostConnect(c->getClientId());
                    mustDel.append(it.key());
                    c->deleteLater();
                }
            }
            for (int k :mustDel) {
                socketPool.remove(k);
            }
        }
    });
    QObject::connect(client, &Client::messageRecv, [&](QString message, int clientId) {
       emit messageRecv(clientId, message.trimmed());
    });
    //QObject::connect(client, &Client::destroyed, )

    //根据连接顺序自动建立编号 并下发给客户端
    client->setClientId(idKey);
    client->sendMessage(command.stringify(Command::SERVER_HAS_CONFIRM_ID, QString::number(client->getClientId())));
    socketPool[idKey] = client;
    emit newClientIn(idKey);

    idKey++;
    //QObject::connect(this, SIGNAL(messageSend(int,QString)), client, SLOT(sendMessage(int,QString)));
    //QObject::connect(thread, &QThread::finished, client, &Client::deleteLater);
    //QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    //client->moveToThread(thread);
    //thread->start();
    emit newConnection();
}

void Server::sendMessage(int clientId, QString message)
{
    if (clientId <= 0) {
        for (Client *client : socketPool) {
            if (client != NULL) {
                client->sendMessage(message);
            }
        }
    } else {
       socketPool[clientId]->sendMessage(message);
    }
}

bool Server::changeClientId(int newId, int oldId)
{
    if (socketPool[newId] != NULL) {
        return false;
    } else {
        socketPool[newId] = socketPool[oldId];
        socketPool.remove(oldId);
        socketPool[newId]->setClientId(newId);

        if (idKey <= newId) {
            idKey = newId + 1;
            //避免下一次客户端连接的时候占用这个位置
        }
        return true;
    }
}
