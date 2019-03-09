#include "monitormain.h"
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
        break;
    case Command::CLIENT_SEND_ANSWER:
        break;
    }
}

void MonitorMain::addClient(int clientId)
{
    QStandardItem *item = new QStandardItem(QString::number(clientId));
    item->setCheckable(true);
    int row = model->rowCount();
    model->setItem(row, 0, item);
}

void MonitorMain::on_orderMode_clicked()
{
    server->sendMessage(0, cmd.stringify(Command::CLIENT_ENTER_ORDER_MODE, "1"));
}

void MonitorMain::checkLogin(int clientId, QString str)
{
    QStringList arr = str.split(",");
    QString username = arr.at(0);
    QString password = QCryptographicHash::hash(arr.at(1).toLatin1(), QCryptographicHash::Md5).toHex();

    bool correct = false;
    for (StudentInfoDao item : studentList) {
        if (item._get("student_id").toString() == username && item._get("password").toString() == password) {
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
    this->show();
}

void MonitorMain::changeClientId(int oldId, int newId)
{
    if (server->changeClientId(newId, oldId)) {
        server->sendMessage(newId, cmd.stringify(Command::SERVER_HAS_CONFIRM_ID, QString::number(newId)));
    } else {
        server->sendMessage(oldId, cmd.stringify(Command::SERVER_HAS_REJECT_ID, "0"));
    }
}

void MonitorMain::on_startLogin_clicked()
{
    server->sendMessage(0, cmd.stringify(Command::CLIENT_CAN_LOGIN, "1"));
}
