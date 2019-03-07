#ifndef STUDENTMAIN_H
#define STUDENTMAIN_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QStandardItemModel>
#include <QRadioButton>
#include <QCheckBox>
#include <QTimer>
#include "../ExamCommon/client.h"
#include "../ExamCommon/command.h"
#include "../ExamCommon/examproblem.h"
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui {
class StudentMain;
}

class StudentMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentMain(QWidget *parent = 0, Client *client = 0);
    ~StudentMain();
    void parseExamJson(QString);
    void nextProblem();
    bool checkSelect();
public slots:
    void startExam();
    void stopExam();
    void handleCmd(int, QString);
    void initExam();
    void saveRadioResult(bool);
    void saveCheckBoxResult(bool);
private slots:
    void on_nextProblem_clicked();
    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::StudentMain *ui;
    Client *client;         //底层TCP对象
    Command command;        //底层命令对象
    uint startTime;
    uint endTime;

    QTimer *beginExamT;   //考试开始倒计时
    QTimer *showSubmitT;  //可以交卷倒计时
    QTimer *stopExamT;    //停止考试倒计时

    QVector<ExamProblem> problemList;   //题目详细信息
    QVector<QListWidgetItem *> listItem;    //题目列表对象
    QVector<QRadioButton *> radioOptions;   //单选按钮组
    QVector<QCheckBox *> checkOptions;      //多选按钮组
    int currIdx = 0;                        //当前正在作答的题目
};

#endif // STUDENTMAIN_H
