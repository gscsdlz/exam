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
    Server *server = new Server;
    Command cmd;
    DataConfig d;
    MonitorMain w(server);

    ConfigLoader *config = new ConfigLoader();
    QString addr = config->getConfig("network/addr");
    QString port = config->getConfig("network/port");

    d.show();

    QObject::connect(&d, &DataConfig::startExam, [&](int e, int c) {
        w.setExamAndClass(e, c);
        server->listen(QHostAddress(addr), port.toInt());
    });

    QObject::connect(server, &Server::messageRecv, [&](int clientId, QString message) {
        cmd.parseCommand(message, clientId);
    });
    QObject::connect(&cmd, &Command::monitorMainSignals, &w, &MonitorMain::handleMessage);
    return a.exec();
}
