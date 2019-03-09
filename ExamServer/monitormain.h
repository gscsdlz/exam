#ifndef MONITORMAIN_H
#define MONITORMAIN_H

#include <QWidget>
#include <QDebug>
#include <QVector>
#include <QStandardItemModel>
#include <QCryptographicHash>
#include "server.h"
#include "../ExamCommon/command.h"
#include "../ExamCommon/studentinfodao.h"
#include "datafileload.h"

namespace Ui {
class MonitorMain;
}

class MonitorMain : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorMain(Server *s, QWidget *parent = 0);
    ~MonitorMain();
    void changeClientId(int, int);
    void checkLogin(int, QString);
    void setExamAndClass(int, int);
public slots:
    void handleMessage(int, QString, int);
    void addClient(int);
private slots:
    void on_orderMode_clicked();
    void on_startLogin_clicked();

signals:
    void sendCtlMsg(int, QString);
private:
    Ui::MonitorMain *ui;
    QStandardItemModel *model;
    Command cmd;
    Server *server;
    int examId;
    int classId;
    QVector<StudentInfoDao> studentList;
};

#endif // MONITORMAIN_H
