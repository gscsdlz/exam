#ifndef LOCALCACHE_H
#define LOCALCACHE_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include "../ExamCommon/orm.h"
#include "../ExamCommon/answerinfo.h"

class LocalCache : public QThread
{
public:
    LocalCache();
    QString getExam();
    bool setExam(QString);
    void clearCache();
    QVector<AnswerInfo> getAnswers();
    //bool setAnswers(QVector<AnswerInfo>);
public slots:
    void setAnswer(int, int);
protected:
    void run();
private:
    ORM *db;
    bool stop;
    int type;
    QMutex m;
    int _proId;
    int _ans;
    QString _str;
};

#endif // LOCALCACHE_H
