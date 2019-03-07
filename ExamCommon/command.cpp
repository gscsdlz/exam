#include "command.h"

Command::Command(QObject *parent) : QObject(parent)
{

}

void Command::parseCommand(QString cmd, int clientId)
{
    QStringList cmdList = cmd.split("|");
    if (cmdList.length() == 0) {
        return;
    }

    int cmdId = cmdList.at(0).toInt();
    if (cmdId < 200) {
        emit initFormSignals(cmdId, cmdList[1]);
    } else if (cmdId < 300) {
        emit studentMainSignals(cmdId, cmdList[1]);
    } else {
        emit monitorMainSignals(cmdId, cmdList[1], clientId);
    }
}

QString Command::stringify(int cmdId, QString args)
{
    return QString::number(cmdId) + "|" + args;
}
