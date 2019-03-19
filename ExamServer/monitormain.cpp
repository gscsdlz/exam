#include "monitormain.h"
#include "ui_monitormain.h"

MonitorMain::MonitorMain(Server *s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorMain)
{
    server = s;
    ui->setupUi(this);

    setFixedWidth(1000);
    setFixedHeight(650);


    model = new QStandardItemModel(this);
    model->setColumnCount(6);
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("客户端编号"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("姓名"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("学号"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("班级"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("性别"));
    model->setHeaderData(5, Qt::Horizontal, QStringLiteral("状态"));
    ui->clientTable->setModel(model);
    ui->clientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->progressBar->hide();
    ui->progressBar->setValue(0);

    load.moveToThread(&checkThread);
    QObject::connect(this, &MonitorMain::startCheck, &load, &DataFileLoad::checkAnswer);
    QObject::connect(&load, &DataFileLoad::completeCheck, this, &MonitorMain::refreshProgress);
    checkThread.start();

    QObject::connect(server, &Server::newClientIn, this, &MonitorMain::addClient);
    QObject::connect(model, &QStandardItemModel::dataChanged, this, &MonitorMain::updateStudentInfo);
}

MonitorMain::~MonitorMain()
{
    checkThread.quit();
    checkThread.wait();
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
        handleStatus(clientId, QByteArray::fromBase64(message.toUtf8()));
        break;
    }
}

void MonitorMain::addClient(int clientId)
{
    QStandardItem *item = new QStandardItem(QString::number(clientId));
    item->setCheckable(true);
    int row = model->rowCount();
    clientRow[clientId] = row;
    model->setItem(row, 0, item);

    for (int i = 1; i < 6; i++) {
        QStandardItem *item = new QStandardItem();
        model->setItem(row, i, item);
    }
    handleStatus(clientId, QStringLiteral("连接成功"));
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

void MonitorMain::on_unlockExamStop_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_CANCEL_FORBID_STOP, "1"));
}

void MonitorMain::checkLogin(int clientId, QString str)
{
    QStringList arr = str.split(",");
    QString username = arr.at(0);
    QString password = QCryptographicHash::hash(arr.at(1).toUtf8(), QCryptographicHash::Md5).toHex();
    bool correct = false;
    for (StudentInfoDao item : studentList) {
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
            model->item(row, 5)->setText(QStringLiteral("已登录"));

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
    studentList = load.getStudentInfo(classId);
    ui->status->setText(QStringLiteral("载入数据成功，等待客户端连接"));

    ConfigLoader *config = new ConfigLoader();
    QString addr = config->getConfig("network/addr");
    QString port = config->getConfig("network/port");

    server->listen(QHostAddress(addr), port.toInt());
    this->show();
}

void MonitorMain::handleLostConnect(int clientId)
{
    int row = clientRow[clientId];
    if (model->rowCount() <= row) {
        return;
    }
    if (loginVis[model->item(row, 2)->text()] == false) {
        model->removeRow(row);
    } else {
        model->item(row, 5)->setText(QStringLiteral("已掉线"));
    }
}

void MonitorMain::changeClientId(int oldId, int newId)
{
    if (server->changeClientId(newId, oldId)) {

        int row = clientRow[newId];
        if (model->item(row, 5)->text() == QStringLiteral("已掉线") && newId == model->item(row, 0)->text().toInt()){
            model->item(row, 5)->setText(QStringLiteral("已恢复"));
            model->removeRow(clientRow[oldId]);
            server->sendMessage(newId, cmd.stringify(Command::CLIENT_LOGIN_SUCCESS, "1"));
        } else {
            clientRow[newId] = clientRow[oldId];
            model->item(clientRow[oldId], 0)->setText(QString::number(newId));
            server->sendMessage(newId, cmd.stringify(Command::SERVER_HAS_CONFIRM_ID, QString::number(newId)));
        }
    } else {
        server->sendMessage(oldId, cmd.stringify(Command::SERVER_HAS_REJECT_ID, "1"));
    }
}

void MonitorMain::dispatcherProblems(int clientId)
{
    QVector<ExamProblem> result =  load.getAllProblems(this->examId);
    ExamInfoDao exam = load.getExamInfo(this->examId);

    QJsonObject mainObj;
    mainObj.insert("exam_name", exam._get("exam_name").toString());
    mainObj.insert("test", false);
    mainObj.insert("start_time", exam._get("start_time").toString());
    mainObj.insert("end_time", exam._get("end_time").toString());

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
    model->item(clientRow[clientId], 5)->setText(QStringLiteral("已下发试卷"));
}

QVector<int> MonitorMain::getSelectedClient()
{
    QVector<int> ids;
    for (int i = 0; i < model->rowCount(); i++) {
        QStandardItem *item = model->item(i, 0);
        if (item->checkState() == Qt::Checked && model->item(i, 5)->text() != QStringLiteral("已掉线")) {
            handleStatus(item->text().toInt(), QStringLiteral("已下发指令"));
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

void MonitorMain::handleStatus(int clientId, QString mess)
{
    int row = clientRow[clientId];
    QStandardItem *item = model->item(row, 5);
    item->setText(mess);
}

void MonitorMain::collectAnswer(int clientId, QString str)
{
    int row = clientRow[clientId];
    QString studentId = model->item(row, 2)->text();

    int id = 0;
    for(StudentInfoDao s : studentList) {
      if (s._get("student_id").toString() == studentId) {
          id = s._get("id").toInt();
          break;
      }
    }
    if (id != 0) {
        if (load.saveAnswerInfo(id, examId, QByteArray::fromBase64(str.toUtf8()))) {
            model->item(row, 5)->setText(QStringLiteral("已收到答案"));
        }
    } else {

    }
}

void MonitorMain::on_startExam_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_START_EXAM, "1"));
}

void MonitorMain::on_selectAll_clicked()
{
    for(int i = 0; i < model->rowCount(); i++) {
        model->item(i, 0)->setCheckState(Qt::Checked);
    }
}

void MonitorMain::on_unSelect_clicked()
{
    for(int i = 0; i < model->rowCount(); i++) {
        model->item(i, 0)->setCheckState(Qt::Unchecked);
    }
}

void MonitorMain::on_checkAnswer_clicked()
{
    ui->progressBar->show();
    emit startCheck(examId, classId);
}

void MonitorMain::on_closeClient_clicked()
{
    sendCmd(cmd.stringify(Command::CLIENT_MUST_EXIT, "1"));
    clientRow.clear();
    loginVis.clear();
    for(int i = 0; i < model->rowCount(); i++) {
        model->removeRow(i);
    }
}

void MonitorMain::refreshProgress(int p)
{
    ui->progressBar->setValue(p);
    if (p >= 101) {
        ui->progressBar->setValue(100);
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("计算完成，开始输出文件！"), QMessageBox::Ok);
        ui->progressBar->hide();
    }
}

void MonitorMain::updateStudentInfo(QModelIndex idx1, QModelIndex, QVector<int> t)
{
    int row = idx1.row();
    if (model->item(row, 1) == NULL || model->item(row, 1)->text() == "") {
        return;
    }
    if (idx1.column() == 1 || idx1.column() == 2 || idx1.column() == 4) {
        int id = 0;
        for(StudentInfoDao s : studentList) {
            if (s._get("student_id").toString() == model->item(row, 2)->text()) {
                id = s._get("id").toInt();
                break;
            }
        }
        if (id == 0) {
            return;
        }
        load.updateStudentInfo(
            id,
            model->item(row, 1)->text(),
            model->item(row, 2)->text(),
            (model->item(row, 4)->text() == QStringLiteral("男"))? 0 : 1
        );
    }
}
