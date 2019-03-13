#ifndef DATAFILELOAD_H
#define DATAFILELOAD_H

#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include "../ExamCommon/examinfodao.h"
#include "../ExamCommon/studentinfodao.h"
#include "../ExamCommon/classinfodao.h"
#include "../ExamCommon/examproblem.h"
#include "../ExamCommon/answerinfo.h"
#include "orm.h"

class DataFileLoad
{
public:
    explicit DataFileLoad();
    QVector<ExamInfoDao> getExamList();
    QVector<ClassInfoDao> getClassList();
    QVector<StudentInfoDao> getStudentInfo(int classId);
    QVector<ExamProblem> getAllProblems(int examId);
    QVector<AnswerInfo> getAllAnswer(int);
    ExamInfoDao getExamInfo(int);

    bool updateExamInfo(int, QString, QString, QString);
    bool updateClassInfo(int, QString);
    bool saveAnswerInfo(int, int, QString);
};

#endif // DATAFILELOAD_H
