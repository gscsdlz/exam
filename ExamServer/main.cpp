#include "monitormain.h"
#include <QApplication>
#include "server.h"
#include "dataconfig.h"
#include "orm.h"
#include "../ExamCommon/configloader.h"
#include "../ExamCommon/command.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Command cmd;
    MonitorMain w;
    DataConfig d;

    Server *server = new Server;
    ConfigLoader *config = new ConfigLoader();
    QString addr = config->getConfig("network/addr");
    QString port = config->getConfig("network/port");

    d.show();

    QObject::connect(server, &Server::messageRecv, [&](int clientId, QString message) {
        cmd.parseCommand(message, clientId);
    });
    QObject::connect(&cmd, &Command::monitorMainSignals, &w, &MonitorMain::handleMessage);

    server->listen(QHostAddress(addr), port.toInt());
    return a.exec();
}
