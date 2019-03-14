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
    if (100 <= cmdId && cmdId < 200) {
        emit initFormSignals(cmdId, cmdList[1]);
    } else if (200 <= cmdId && cmdId < 300) {
        emit studentMainSignals(cmdId, cmdList[1]);
    } else if (cmdId >= 300){
        emit monitorMainSignals(cmdId, cmdList[1], clientId);
    } else if (cmdId < 100) {
        emit initFormSignals(cmdId, cmdList[1]);
        emit studentMainSignals(cmdId, cmdList[1]);
    }
}

QString Command::stringify(int cmdId, QString args)
{
    return QString::number(cmdId) + "|" + args;
}
