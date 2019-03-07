#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "../ExamCommon/client.h"
#include "../ExamCommon/configloader.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0, Client *client = 0);
    ~SettingDialog();
public slots:
    void reconnect();
private:
    Ui::SettingDialog *ui;
    Client *client;
    ConfigLoader config;

};

#endif // SETTINGDIALOG_H
