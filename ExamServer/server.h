#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QMap>
#include "../ExamCommon/client.h"
#include "../ExamCommon/command.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void incomingConnection(qintptr handle);
    bool changeClientId(int, int);
protected:
    QThread *getNewThread();
signals:
    void messageRecv(int, QString);
    void lostConnect(int);
    void newClientIn(int);
public slots:
    void sendMessage(int, QString);
private:
    QMap<int, Client *> socketPool;
    Command command;
    int idKey;
};

#endif // SERVER_H
