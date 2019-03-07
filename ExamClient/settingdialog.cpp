#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent, Client *c) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    client = c;
    ui->setupUi(this);
    ui->addr->setText(config.getConfig("network/addr"));
    ui->port->setText(config.getConfig("network/port"));
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(reconnect()));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::reconnect()
{
    QString addr = ui->addr->text();
    int port = ui->port->text().toInt();
    config.setConfig("network/addr", addr);
    config.setConfig("network/port", ui->port->text());
    client->connectToServer(addr, port);
    this->hide();
}
