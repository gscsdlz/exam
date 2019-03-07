#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

/**
 * 被封装好的TCPSocket对象 负责最底层的数据传输
 * @brief The Client class
 */
class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject *parent = 0);
    Client(qintptr, QObject *parent = 0);
    ~Client();
    void connectToServer(QString, quint16);
    void sendMessage(QString);
    QAbstractSocket::SocketState getState();
    void setClientId(int);
    int getClientId();
signals:
    void messageRecv(QString, int);
    void stateChange(QAbstractSocket::SocketState);
    void clientIdChange(int);
public slots:
    void createObject();
    void readMessage();
private:
    QString host;
    quint16 port;
    QByteArray buffer;
    int borderCount;
    int clientId;
    QTcpSocket *client;
    qintptr socketDescriptor;
};

#endif // CLIENT_H
