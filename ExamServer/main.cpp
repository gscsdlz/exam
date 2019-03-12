﻿#include "monitormain.h"
#include <QApplication>
#include "server.h"
#include "dataconfig.h"
#include "orm.h"
#include "../ExamCommon/command.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server *server = new Server;
    Command cmd;
    DataConfig d;
    MonitorMain w(server);

    d.show();

    QObject::connect(&d, &DataConfig::startExam, &w, &MonitorMain::setExamAndClass);
    QObject::connect(server, &Server::messageRecv, [&](int clientId, QString message) {
        cmd.parseCommand(message, clientId);
    });
    QObject::connect(&cmd, &Command::monitorMainSignals, &w, &MonitorMain::handleMessage);
    return a.exec();
}
