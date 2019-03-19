#ifndef DATAFILELOAD_H
#define DATAFILELOAD_H

#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QProcess>
#include "../ExamCommon/examinfodao.h"
#include "../ExamCommon/studentinfodao.h"
#include "../ExamCommon/classinfodao.h"
#include "../ExamCommon/examproblem.h"
#include "../ExamCommon/answerinfo.h"
#include "../ExamCommon/orm.h"

class DataFileLoad : public QObject
{
    Q_OBJECT
public:
    explicit DataFileLoad(QObject *parent = 0);
    QVector<ExamInfoDao> getExamList();
    QVector<ClassInfoDao> getClassList();
    QVector<StudentInfoDao> getStudentInfo(int classId);
    QVector<ExamProblem> getAllProblems(int examId);
    QVector<AnswerInfo> getAllAnswer(int, int);
    ExamInfoDao getExamInfo(int);
    int getProblemAnswer(int);

    bool updateExamInfo(int, QString, QString, QString);
    bool updateClassInfo(int, QString);
    bool saveAnswerInfo(int, int, QString);
    bool updateStudentInfo(int, QString, QString, int);
    void exportFile(int, int);
public slots:
    void checkAnswer(int, int);
signals:
    void completeCheck(int);
private:
    ORM *db;
};

#endif // DATAFILELOAD_H
