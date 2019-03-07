#include "initform.h"
#include "ui_initform.h"

InitForm::InitForm(QWidget *parent, Client *c) :
    QWidget(parent),
    ui(new Ui::InitForm)
{
    client = c;
    QObject::connect(client, SIGNAL(clientIdChange(int)), this, SLOT(showID(int)));
    ui->setupUi(this);

    ui->login->hide();
    ui->password->hide();
    ui->username->hide();

    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint);
    setFixedSize(this->width(),this->height());

    ui->setting->installEventFilter(this);
    ui->close->installEventFilter(this);
}

bool InitForm::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->setting) {
        if (event->type() == QEvent::MouseButtonPress) {
            emit openSettingDialog();
        }
    } else if (watched == ui->close) {
        if (event->type() == QEvent::MouseButtonPress) {
            this->close();
        }
    }
    return QWidget::eventFilter(watched, event);
}

InitForm::~InitForm()
{
    delete ui;
}

void InitForm::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
    if(Qt::LeftButton == event->button())
    {
       pLast = event->globalPos();
       event->ignore();
    }
    m_bPressed = true;
}

void InitForm::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && m_bPressed)
    {
       QPoint ptemp = event->globalPos();
       move(ptemp - pLast + pos());
       pLast = ptemp;
    }
}

void InitForm::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
    event->ignore();

    m_bPressed = false;
}

void InitForm::networkStateChange(QAbstractSocket::SocketState state)
{
    QString txt;
    switch (state)
    {
    case QAbstractSocket::UnconnectedState:
        txt = QStringLiteral("连接已断开!");
        break;
    case QAbstractSocket::HostLookupState:
        txt = QStringLiteral("DNS解析中...");
        break;
    case QAbstractSocket::ConnectingState:
        txt = QStringLiteral("正在连接中...");
        break;
    case QAbstractSocket::ConnectedState:
        txt = QStringLiteral("已连接! 等待服务器下发指令");
        break;
    default:
        txt = QStringLiteral("...");
    }
    ui->status->setText(txt);
}

void InitForm::showID(int clientId)
{
    if (clientId == -1) {
        ui->id->setText(QStringLiteral("-"));
    } else {
        ui->id->setText(QString::number(clientId));
    }
}

void InitForm::handleCmd(int cmdId, QString arg)
{
    switch (cmdId) {
    case Command::CLIENT_ENTER_ORDER_MODE:
        ui->status->setText(QStringLiteral("请输入新的ID"));
        orderModeOn();
        break;
    case Command::SERVER_HAS_REJECT_ID:
        ui->status->setText(QStringLiteral("ID重复请重试"));
        break;
    case Command::SERVER_HAS_CONFIRM_ID:
        ui->status->setText(QStringLiteral("已使用最新的ID，进入考前准备状态"));
        orderModeOff(arg);
        break;
    case Command::CLIENT_CAN_LOGIN:
        ui->status->setText(QStringLiteral("请开始登录"));
        loginOn();
        break;
    case Command::CLIENT_LOGIN_ERROR:
        ui->status->setText(QStringLiteral("用户名或者密码错误，请重试"));
        ui->password->clear();
        break;
    case Command::CLIENT_LOGIN_SUCCESS:
        //登录成功进入考试 移交控制权
        hide();
        emit openStudentMain();
        break;
    default:
        break;
    }
}

//客户端进入编号配置模式
void InitForm::orderModeOn()
{
    showID(-1);
    ui->login->setText(QStringLiteral("提交编号"));
    ui->login->show();
    ui->password->hide();
    ui->username->setText("");
    ui->username->show();
}

void InitForm::orderModeOff(QString arg)
{
    ui->login->hide();
    ui->password->hide();
    ui->username->hide();
    ui->id->setText(arg);
}

void InitForm::loginOn()
{
    ui->login->setText(QStringLiteral("登录"));
    ui->login->show();
    ui->password->show();
    ui->username->show();
    ui->username->setText("");
    ui->password->setText("");
}

void InitForm::on_login_clicked()
{
    if (ui->login->text() == QStringLiteral("提交编号")) {
        QString newId = ui->username->text();

        client->sendMessage(command.stringify(Command::CLIENT_APPLY_CLIENT_ID, newId));
        ui->status->setText(QStringLiteral("申请客户端ID命令已发出!"));
    } else {
        QString username = ui->username->text();
        QString password = ui->password->text();
        if (username.length() == 0) {
            ui->status->setText(QStringLiteral("用户名不能为空"));
        } else if (password.length() == 0) {
            ui->status->setText(QStringLiteral("密码不能为空"));
        } else {
            client->sendMessage(command.stringify(Command::CLIENT_START_LOGIN, username + "," + password));
            ui->status->setText(QStringLiteral("已发起登录"));
        }
    }
}
