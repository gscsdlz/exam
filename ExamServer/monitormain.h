#ifndef MONITORMAIN_H
#define MONITORMAIN_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class MonitorMain;
}

class MonitorMain : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorMain(QWidget *parent = 0);
    ~MonitorMain();
public slots:
    void handleMessage(int, QString, int);
private:
    Ui::MonitorMain *ui;
};

#endif // MONITORMAIN_H
