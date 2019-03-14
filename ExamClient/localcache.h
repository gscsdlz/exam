#ifndef LOCALCACHE_H
#define LOCALCACHE_H

#include "../ExamCommon/orm.h"
#include "../ExamCommon/answerinfo.h"

class LocalCache
{
public:
    LocalCache();
    QString getExam();
    bool setExam(QString);
    void clearCache();
    QVector<AnswerInfo> getAnswers();
    //bool setAnswers(QVector<AnswerInfo>);
    bool setAnswer(int, QVector<int>);
private:
    ORM *db;
};

#endif // LOCALCACHE_H
