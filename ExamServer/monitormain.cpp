﻿#include "monitormain.h"
#include "ui_monitormain.h"

MonitorMain::MonitorMain(Server *s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorMain)
{
    server = s;
    ui->setupUi(this);

    QObject::connect(server, &Server::newClientIn, this, &MonitorMain::addClient);

    model = new QStandardItemModel(this);
    model->setColumnCount(6);
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("客户端编号"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("姓名"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("学号"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("班级"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("性别"));
    model->setHeaderData(5, Qt::Horizontal, QStringLiteral("考试状态"));

    ui->clientTable->setModel(model);
}

MonitorMain::~MonitorMain()
{
    delete ui;
}

void MonitorMain::handleMessage(int cmdId, QString message, int clientId)
{
    switch (cmdId)
    {
    case Command::CLIENT_APPLY_CLIENT_ID:
        changeClientId(clientId, message.toInt());
        break;
    case Command::CLIENT_START_LOGIN:
        checkLogin(clientId, message);
        break;
    case Command::CLIENT_APPLY_EXAM_INFO:
        dispatcherProblems(clientId);
        break;
    case Command::CLIENT_SEND_ANSWER:
        collectAnswer(clientId, message);
        break;
    case Command::CLIENT_UPLOAD_STATUS:
        handleStatus(clientId, message.toInt());
        break;
    }
}

void MonitorMain::addClient(int clientId)
{
    QStandardItem *item = new QStandardItem(QString::number(clientId));
    item->setCheckable(true);
    int row = model->rowCount();
    model->setItem(row, 0, item);
    clientRow[clientId] = row;

    for (int i = 1; i < 6; i++) {
        QStandardItem *item = new QStandardItem();
        model->setItem(row, i, item);
    }
}

void MonitorMain::on_orderMode_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_ENTER_ORDER_MODE, "1"));
}

void MonitorMain::on_exitExam_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_STOP_EXAM_FORBID, "1"));
}

void MonitorMain::on_startLogin_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_CAN_LOGIN, "1"));
}

void MonitorMain::on_unlockLogin_clicked()
{
    QVector<int> res = getSelectedClient();
    if(res.length() == 1 && res[0] == -1) {
        loginVis.clear();
    } else {
        for(int id : res) {
            int row = clientRow[id];
            loginVis[model->item(row, 2)->text()] = false;
        }
    }
}

void MonitorMain::on_unlockExamStop_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_CANCEL_FORBID_STOP, "1"));
}

void MonitorMain::checkLogin(int clientId, QString str)
{
    QStringList arr = str.split(",");
    QString username = arr.at(0);
    qDebug() << arr;
    QString password = QCryptographicHash::hash(arr.at(1).toUtf8(), QCryptographicHash::Md5).toHex();
    bool correct = false;
    for (StudentInfoDao item : studentList) {
        qDebug() << item._get("student_id") << " " << item._get("password") << " " << username << " " << password;
        if (item._get("student_id").toString() == username && item._get("password").toString() == password) {
            if (loginVis[item._get("student_id").toString()]) {
                server->sendMessage(clientId, cmd.stringify(Command::CLIENT_HAS_LOGIN, "1"));
                return;
            }
            int row = clientRow[clientId];
            model->item(row, 1)->setText(item._get("student_name").toString());
            model->item(row, 2)->setText(item._get("student_id").toString());
            model->item(row, 3)->setText(item._get("class_name").toString());
            model->item(row, 4)->setText(item._get("str_sex").toString());

            loginVis[item._get("student_id").toString()] = true;
            correct = true;
        }
    }
    if (!correct) {
        server->sendMessage(clientId, cmd.stringify(Command::CLIENT_LOGIN_ERROR, "1"));
    } else {
        server->sendMessage(clientId, cmd.stringify(Command::CLIENT_LOGIN_SUCCESS, "1"));
    }
}

void MonitorMain::setExamAndClass(int e, int c)
{
    examId = e;
    classId = c;
    DataFileLoad load;
    studentList = load.getStudentInfo(classId);
    ui->status->setText(QStringLiteral("载入数据成功，等待客户端连接"));

    ConfigLoader *config = new ConfigLoader();
    QString addr = config->getConfig("network/addr");
    QString port = config->getConfig("network/port");

    server->listen(QHostAddress(addr), port.toInt());
    this->show();
}

void MonitorMain::changeClientId(int oldId, int newId)
{
    if (server->changeClientId(newId, oldId)) {
        server->sendMessage(newId, cmd.stringify(Command::SERVER_HAS_CONFIRM_ID, QString::number(newId)));
    } else {
        server->sendMessage(oldId, cmd.stringify(Command::SERVER_HAS_REJECT_ID, "1"));
    }
}

void MonitorMain::dispatcherProblems(int clientId)
{
    DataFileLoad load;
    QVector<ExamProblem> result =  load.getAllProblems(this->examId);
    ExamInfoDao exam = load.getExamInfo(this->examId);

    QJsonObject mainObj;
    mainObj.insert("exam_name", exam._get("exam_name").toString());
    mainObj.insert("test", false);
    mainObj.insert("start_time", exam._get("str_start_time").toString());
    mainObj.insert("end_time", exam._get("str_end_time").toString());

    QJsonArray problems;
    for (int i = 0; i < result.length(); i++) {
        QJsonObject p;
        p.insert("pro_id",result[i].getId());
        p.insert("title", result[i].getTitle());
        p.insert("type", result[i].getType());
        QJsonArray ops;
        QVector<QString> options = result[i].getOptions();
        for (int j = 0; j < options.length(); j++) {
            ops.append(options[j]);
        }
        p.insert("options", ops);
        problems.append(p);
    }
    mainObj.insert("problems", problems);

    QJsonDocument j;
    j.setObject(mainObj);
    QByteArray res = j.toJson().toBase64();
    server->sendMessage(clientId, cmd.stringify(Command::CURR_EXAM_INFO, res));
}

QVector<int> MonitorMain::getSelectedClient()
{
    QVector<int> ids;
    for (int i = 0; i < model->rowCount(); i++) {
        QStandardItem *item = model->item(i, 0);
        if (item->checkState() == Qt::Checked) {
            ids.push_back(item->text().toInt());
        }
    }
    if (ids.length() == model->rowCount()) {
        ids.clear();
        ids.append(-1);
    }
    return ids;
}

void MonitorMain::sendCmd(QString message)
{
    QVector<int> ids = this->getSelectedClient();
    if (ids.length() == 1 && ids[0] == -1) {
        server->sendMessage(0, message);
    } else {
        for (int id : ids) {
            server->sendMessage(id, message);
        }
    }
    ui->status->setText(QStringLiteral("下发指令成功，等待客户端响应"));
}

void MonitorMain::handleStatus(int clientId, int statusType)
{
    int row = clientRow[clientId];
    QStandardItem *item = model->item(row, 5);
    QString t;
    switch (statusType)
    {
    case ClientStatus::CLIENT_FIRST_CONNECT:
        break;
    }
    item->setText(t);
}

void MonitorMain::collectAnswer(int clientId, QString str)
{
    qDebug() << str;
//    QJsonDocument dom = QJsonDocument::fromJson(str.toLatin1());
//    QJsonArray arr = dom.array();

//    QVector<QVector<int>> result;
//    for (int i = 0; i < arr.count(); i++) {
//        QJsonObject obj = arr.at(i).toObject();
//        QVector<int> t;

//        t.push_back(obj.take("pro_id").toInt());
//        QJsonArray ans = obj.take("answers").toArray();
//        for (int j = 0; j < ans.size(); j++) {
//            t.push_back(ans.at(i).toInt());
//        }
//        result.push_back(t);
//    }


}

void MonitorMain::on_startExam_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_START_EXAM, "1"));
}
