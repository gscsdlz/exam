#ifndef DATAFILELOAD_H
#define DATAFILELOAD_H

#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include "../ExamCommon/examinfodao.h"
#include "../ExamCommon/studentinfodao.h"
#include "../ExamCommon/classinfodao.h"
#include "../ExamCommon/examproblem.h"
#include "orm.h"

class DataFileLoad
{
public:
    explicit DataFileLoad();
    QVector<ExamInfoDao> getExamList();
    QVector<ClassInfoDao> getClassList();
    QVector<StudentInfoDao> getStudentInfo(int classId);
    QVector<ExamProblem> getAllProblems(int examId);
    ExamInfoDao getExamInfo(int);

};

#endif // DATAFILELOAD_H
