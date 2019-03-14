#ifndef MONITORMAIN_H
#define MONITORMAIN_H

#include <QMap>
#include <QWidget>
#include <QDebug>
#include <QVector>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QJsonObject>
#include <QCryptographicHash>
#include "server.h"
#include "../ExamCommon/command.h"
#include "../ExamCommon/studentinfodao.h"
#include "../ExamCommon/configloader.h"
#include "../ExamCommon/examproblem.h"
#include "../ExamCommon/clientstatus.h"
#include "../ExamCommon/answerinfo.h"
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
    void dispatcherProblems(int);
    void handleStatus(int, int);
    void collectAnswer(int, QString);
    void checkAnswer();
protected:
    void sendCmd(QString);
    QVector<int> getSelectedClient();
public slots:
    void setExamAndClass(int, int);
    void handleMessage(int, QString, int);
    void addClient(int);
    void handleLostConnect(int);

private slots:
    void on_orderMode_clicked();
    void on_startLogin_clicked();
    void on_exitExam_clicked();
    void on_unlockExamStop_clicked();
    void on_startExam_clicked();
    void on_selectAll_clicked();
    void on_unSelect_clicked();    
    void on_checkAnswer_clicked();
    void on_closeClient_clicked();

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
    QMap<int, int> clientRow;
    QMap<QString, bool> loginVis;
    DataFileLoad load;
};

#endif // MONITORMAIN_H
