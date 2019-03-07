#ifndef INITFORM_H
#define INITFORM_H

#include <QWidget>
#include <QMessageBox>
#include <QCursor>
#include <QMouseEvent>
#include "../ExamCommon/command.h"
#include "../ExamCommon/client.h"

namespace Ui {
class InitForm;
}

class InitForm : public QWidget
{
    Q_OBJECT

public:
    explicit InitForm(QWidget *parent = 0, Client *client = 0);
    ~InitForm();
    void networkStateChange(QAbstractSocket::SocketState);
    void orderModeOn();
    void orderModeOff(QString);
    void loginOn();
    bool eventFilter(QObject *, QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
public slots:
    void showID(int);
    void handleCmd(int, QString);
private slots:
    void on_login_clicked();
signals:
    void openSettingDialog();
    void openStudentMain();
private:
    QPoint pLast;
    bool m_bPressed;
    Ui::InitForm *ui;
    Client *client;
    Command command;
};

#endif // INITFORM_H
