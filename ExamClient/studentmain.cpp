#include "studentmain.h"
#include "ui_studentmain.h"

StudentMain::StudentMain(QWidget *parent, Client *c) :
    QMainWindow(parent),
    ui(new Ui::StudentMain)
{
    client = c;
    ui->setupUi(this);
    ui->submit->hide();
    ui->nextProblem->hide();
    ui->listWidget->hide();
    ui->title->hide();
    ui->mask->hide();

    //定时器小队
    beginExamT = new QTimer(this);
    showSubmitT = new QTimer(this);
    stopExamT = new QTimer(this);

    QObject::connect(beginExamT, SIGNAL(timeout()), this, SLOT(startExam()));
    QObject::connect(showSubmitT, &QTimer::timeout, [&](){
       showSubmitT->stop();
       ui->submit->show();
    });
    QObject::connect(stopExamT, SIGNAL(timeout()), this, SLOT(stopExam()));

    //加入组 方便管理
    radioOptions.append(ui->o1);
    radioOptions.append(ui->o2);
    radioOptions.append(ui->o3);
    radioOptions.append(ui->o4);

    checkOptions.append(ui->c1);
    checkOptions.append(ui->c2);
    checkOptions.append(ui->c3);
    checkOptions.append(ui->c4);

    //绑定信号槽 处理点击事件并保存答案
    for (QRadioButton *item : radioOptions) {
        item->hide();
        QObject::connect(item, SIGNAL(clicked(bool)), this, SLOT(saveRadioResult(bool)));
    }
    for (QCheckBox *item : checkOptions) {
        item->hide();
        QObject::connect(item, SIGNAL(toggled(bool)), this, SLOT(saveCheckBoxResult(bool)));
    }

    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint);
    setFixedSize(this->width(),this->height());

    parseExamJson("eyJleGFtX25hbWUiOiLogIPor5XlkI3np7AiLCJ0ZXN0IjpmYWxzZSwic3RhcnRfdGltZSI6IjIwMTktMDMtMDIgMTg6MDU6MDAiLCJlbmRfdGltZSI6IjIwMTktMDMtMDIgMTg6MDY6MDAiLCJwcm9ibGVtcyI6W3sicHJvX2lkIjoxLCJ0aXRsZSI6IuS7peS4i+ivtOazleato+ehrueahOaYryIsInR5cGUiOjEsIm9wdGlvbnMiOlsiQysr5piv5LiA56eN6ISa5pys6K+t6KiAIiwiUEhQ5piv5LiA56eN57yW6K+R5oCn6K+t6KiAIiwi5rGH57yW6K+t6KiA5ZKM5py65Zmo6K+t6KiA5LiA5LiA5a+55bqUIiwi6L+Z5Liq6YCJ6aG55piv5Zyo57yW5LiN5LiL5Y675LqGIl19LHsicHJvX2lkIjoyLCJ0aXRsZSI6Iua1i+ivleagh+mimDIiLCJ0eXBlIjoyLCJvcHRpb25zIjpbIkEiLCJCIiwiQyIsIkQiXX0seyJwcm9faWQiOjQsInRpdGxlIjoi5rWL6K+V5qCH6aKYMyIsInR5cGUiOjIsIm9wdGlvbnMiOlsiQSIsIkIiLCJDIiwiRCJdfV19");
}

StudentMain::~StudentMain()
{
    delete ui;
}

void StudentMain::handleCmd(int cmdId, QString arg)
{
    switch (cmdId)
    {
    case Command::CURR_EXAM_INFO:
        parseExamJson(arg);
        break;
    case Command::CLIENT_START_EXAM:
        startExam();
        break;
    case Command::CLIENT_CAN_SUBMIT:
        ui->submit->show();
        break;

    }
}

void StudentMain::initExam()
{
    if (this->isVisible()) {
        return;
    }
    this->show();
    client->sendMessage(command.stringify(Command::CLIENT_APPLY_EXAM_INFO, "0"));
}

//解析服务端发送过来的JSON对象
void StudentMain::parseExamJson(QString str)
{
    QByteArray res = QByteArray::fromBase64(str.toLatin1());

    QJsonDocument data = QJsonDocument::fromJson(res, nullptr);
    QJsonObject obj = data.object();

    QString examName = obj.take("exam_name").toString();
    if (obj.take("text").toBool()) {
        examName += QStringLiteral("-模拟测试");
    }
    ui->examName->setText(examName);
    QString t1 = obj.take("start_time").toString();
    QString t2 = obj.take("end_time").toString();
    ui->examTime->setText(t1 + " ~ " + t2);


    startTime = QDateTime::fromString(t1, "yyyy-MM-dd HH:mm:ss").toTime_t();
    endTime = QDateTime::fromString(t2, "yyyy-MM-dd HH:mm:ss").toTime_t();
    //设置关联的倒计时

    uint currTs = QDateTime::currentDateTime().toTime_t();
    qDebug() << startTime << " " << endTime << " " << currTs;
    beginExamT->start((startTime - currTs) * 1000);
    stopExamT->start((endTime - currTs) * 1000);
    //解析题目数据
    QJsonArray problems = obj.take("problems").toArray();
    for (int i = 0; i < problems.size(); i++) {
        QJsonObject problemObj = problems.at(i).toObject();
        ExamProblem p;
        p.setVarInfo(problemObj.take("pro_id").toInt(), problemObj.take("title").toString(), problemObj.take("type").toInt());

        QJsonArray options = problemObj.take("options").toArray();
        for (int j = 0; j < options.size(); j++) {
            p.appendOptions(options.at(j).toString());
        }
        problemList.append(p);

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::number(i + 1));
        item->setIcon(QIcon(":/init.png"));
        listItem.append(item);
        ui->listWidget->addItem(item);
    }
}

//开始考试 初始化界面
void StudentMain::startExam()
{
    beginExamT->stop();

    ui->wait->hide();
    ui->nextProblem->show();
    ui->listWidget->show();
    currIdx = 0;
    ui->title->show();
    ui->mask->show();
    nextProblem();
}

//切换到下一题
void StudentMain::nextProblem()
{
    ExamProblem p = problemList[currIdx];
    ui->title->setText(p.getTitle());
    QVector<QString> options = p.getOptions();
    QVector<int> answer = p.getAnswer();
    if (p.getType() == 1) {
        //单选
        for (int i = 0; i < 4; i++) {
            checkOptions[i]->hide();
            radioOptions[i]->setText(options[i]);
            radioOptions[i]->show();
        }
        if (!answer.empty()) {
            radioOptions[answer[0]]->setChecked(true);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            radioOptions[i]->hide();
            checkOptions[i]->setText(options[i]);
            checkOptions[i]->show();
            checkOptions[i]->setChecked(false);
        }
        for (int i = 0; i < answer.length(); i++) {
            checkOptions[answer[i]]->setChecked(true);
        }
    }
}

//点击按钮下一题
void StudentMain::on_nextProblem_clicked()
{
    if (checkSelect()) {
        currIdx++;
        if (currIdx >= problemList.size()) {
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("已经是最后一题了"), QMessageBox::Ok);
            currIdx = problemList.size() - 1;
            return;
        }
        nextProblem();
    }
}

//保存单选的答案
void StudentMain::saveRadioResult(bool)
{
    for (int i = 0; i < 4; i++) {
        if (radioOptions[i]->isChecked()) {
            problemList[currIdx].appendAnswer(i);
            listItem[currIdx]->setIcon(QIcon(":/ok.png"));
            break;
        }
    }
}

//保存多选的答案
void StudentMain::saveCheckBoxResult(bool)
{
    QVector<int> res;
    for (int i = 0; i < 4; i++) {
        if (checkOptions[i]->isChecked()) {
            res.append(i);
        }
    }
    if (!res.empty()) {
        listItem[currIdx]->setIcon(QIcon(":/ok.png"));
    } else {
        listItem[currIdx]->setIcon(QIcon(":/init.png"));
    }
    problemList[currIdx].appendAnswer(res);
}

//点击左侧题目列表框
void StudentMain::on_listWidget_clicked(const QModelIndex &index)
{
    if (checkSelect()) {
        currIdx = index.row();
        nextProblem();
    }
}

//所有切换的题目操作发生之前检查是否有未完成的题目
bool StudentMain::checkSelect()
{
    QVector<int> ans = problemList[currIdx].getAnswer();
    if (ans.empty()) {
        if (QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("当前题目还没作答 确认离开吗？"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void StudentMain::stopExam()
{
    stopExamT->stop();
    qDebug() << "stop";
}
