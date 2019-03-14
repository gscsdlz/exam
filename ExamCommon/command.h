#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

class Command : public QObject
{
    Q_OBJECT
public:
    //指令列表
    enum CommandList {
        CLIENT_MUST_EXIT =  1,          //

        //服务端下行告知initForm的指令 1xx
        CLIENT_ENTER_ORDER_MODE = 101,  //客户端进入配置ID模式
        SERVER_HAS_CONFIRM_ID = 102,    //服务端认可客户端上传的ID
        SERVER_HAS_REJECT_ID = 103,     //服务端拒绝了客户端上传的ID
        CLIENT_CAN_LOGIN = 105,         //客户端现在可以登录了
        CLIENT_LOGIN_ERROR = 106,       //用户名或者密码错误
        CLIENT_LOGIN_SUCCESS = 107,     //登录成功
        CLIENT_HAS_LOGIN = 108,         //

        //服务端下行告知studentMain的指令 2xx
        CURR_EXAM_INFO = 201,           //本次考试的信息 完整的考试信息 JSON编码
        CLIENT_START_EXAM = 203,        //客户端手动开始考试 一般自动启动
        CLIENT_CAN_SUBMIT = 202,        //客户端可以点击交卷了
        CLIENT_STOP_EXAM_FORBID = 204,  //客户端强制停止考试
        CLIENT_UPLOAD_ANSWER = 205,     //客户端上传答案
        CLIENT_CANCEL_FORBID_STOP = 206,//

        //initForm上行通知服务端的指令 3xx
        CLIENT_APPLY_CLIENT_ID = 301,   //客户端想申请编号
        CLIENT_START_LOGIN = 302,       //客户端发起登录请求

        //studentMain上行通知服务端的指令 4xx
        CLIENT_APPLY_EXAM_INFO = 401,   //客户端申请获取题目信息
        CLIENT_SEND_ANSWER = 402,       //上传作答情况

        //通用上行指令
        CLIENT_UPLOAD_STATUS = 501,     //
    };

    explicit Command(QObject *parent = 0);
public slots:
    void parseCommand(QString, int);
    QString stringify(int, QString);
signals:
    void initFormSignals(int, QString);
    void studentMainSignals(int, QString);
    void monitorMainSignals(int, QString, int);
public slots:
};

#endif // COMMAND_H
