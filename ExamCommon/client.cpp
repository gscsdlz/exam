#include "client.h"
#include <QTcpSocket>

Client::Client(QObject *parent) : QObject(parent)
{
    client = new QTcpSocket;
    QObject::connect(client, SIGNAL(readyRead()), this, SLOT(readMessage()));
    QObject::connect(client, &QTcpSocket::stateChanged, [&](QAbstractSocket::SocketState state){
        emit this->stateChange(state);
    });
}

Client::Client(qintptr socketDescriptor, QObject *parent) : QObject(parent)
{
    this->socketDescriptor = socketDescriptor;
    clientId = 0;
}

Client::~Client()
{
    client->disconnectFromHost();
}

void Client::connectToServer(QString hostArg = "", quint16 portArg = 0)
{
    if (hostArg != "" && portArg != 0) {
        host = hostArg;
        port = portArg;
    }
    if (client->state() == QAbstractSocket::UnconnectedState) {
        client->connectToHost(host, port);
    }
}

void Client::readMessage()
{
    QByteArray res = client->readAll();
    /*for (int i = 0; i < res.length(); i++) {
        if (res[i] == '\r') {
            borderCount++;
        } else {
            borderCount = 0;
        }
        buffer.append(res[i]);
        if (borderCount == 3) {
            QString message = buffer.length() - 3;
            borderCount = 0;
            buffer.clear();
            emit messageRecv(message);
        }
    }*/
    emit messageRecv(res, clientId);
}

void Client::sendMessage(QString message)
{
    client->write(message.toLatin1() + "\r\n");
}

QAbstractSocket::SocketState Client::getState()
{
    return client->state();
}

int Client::getClientId() {
    return this->clientId;
}

void Client::setClientId(int id)
{
    clientId = id;
    emit clientIdChange(id);
}

void Client::createObject()
{
    client = new QTcpSocket;
    if (!client->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "error";
    }
    clientId = 0;
    QObject::connect(client, SIGNAL(readyRead()), this, SLOT(readMessage()));
    QObject::connect(client, &QTcpSocket::stateChanged, [&](QAbstractSocket::SocketState state){
        emit this->stateChange(state);
    });
}
