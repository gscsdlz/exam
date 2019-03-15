#ifndef LOCALCACHE_H
#define LOCALCACHE_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include "../ExamCommon/orm.h"
#include "../ExamCommon/answerinfo.h"

class LocalCache : public QObject
{
    Q_OBJECT
public:
    LocalCache(QObject * = 0);
    QString getExam();
    bool setExam(QString);
    void clearCache();
    QVector<AnswerInfo> getAnswers();
    //bool setAnswers(QVector<AnswerInfo>);
public slots:
    void setAnswer(int, int);
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
