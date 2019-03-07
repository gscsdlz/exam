#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QVector>
#include "../ExamCommon/client.h"
#include "../ExamCommon/command.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void incomingConnection(qintptr handle);
protected:
    QThread *getNewThread();
signals:
    void messageRecv(int, QString);
    void connectStateChange(QAbstractSocket::SocketState);

public slots:
    void sendMessage(int, QString);
private:
    QVector<Client *> socketPool;
    Command command;
    int idKey;
};

#endif // SERVER_H
