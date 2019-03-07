#include "monitormain.h"
#include "ui_monitormain.h"

MonitorMain::MonitorMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorMain)
{
    ui->setupUi(this);
}

MonitorMain::~MonitorMain()
{
    delete ui;
}

void MonitorMain::handleMessage(int cmdId, QString message, int clientId)
{
    qDebug() << cmdId << " " << message << " " << clientId;
}
