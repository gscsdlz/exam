#include <QApplication>
#include <QDebug>
#include "settingdialog.h"
#include "initform.h"
#include "../ExamCommon/configloader.h"
#include "../ExamCommon/client.h"
#include "../ExamCommon/command.h"
#include "studentmain.h"


int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);

    //初始化基本窗体
    Client *tcpClient = new Client;
    Command *cmd = new Command;

    SettingDialog settingDialog(NULL, tcpClient);
    InitForm initForm(NULL, tcpClient);
    StudentMain studentMain(NULL, tcpClient);
    //studentMain.show();
    initForm.show();

    //载入配置文件
    ConfigLoader *config = new ConfigLoader();
    QString addr = config->getConfig("network/addr");
    QString port = config->getConfig("network/port");
    QString clientId = config->getConfig("client/id", "1");

    tcpClient->setClientId(clientId.toInt());

    //设置关键信号槽 断网即出现设置窗体
    QObject::connect(tcpClient, &Client::stateChange, [&](QAbstractSocket::SocketState state){
        if (state == QAbstractSocket::UnconnectedState) {
            settingDialog.show();
        }
        initForm.networkStateChange(state);
    });
    QObject::connect(&initForm, SIGNAL(openSettingDialog()), &settingDialog, SLOT(show()));
    QObject::connect(&initForm, SIGNAL(openStudentMain()), &studentMain, SLOT(initExam()));

    QObject::connect(tcpClient, SIGNAL(messageRecv(QString, int)), cmd, SLOT(parseCommand(QString, int)));

    QObject::connect(cmd, SIGNAL(initFormSignals(int,QString)), &initForm, SLOT(handleCmd(int, QString)));
    QObject::connect(cmd, SIGNAL(studentMainSignals(int,QString)), &studentMain, SLOT(handleCmd(int, QString)));


    //尝试发起连接
    tcpClient->connectToServer(addr, port.toInt());

    return a.exec();
}
