﻿#ifndef DATAFILELOAD_H
#define DATAFILELOAD_H

#include <QVector>
#include "../ExamCommon/examinfodao.h"
#include "../ExamCommon/studentinfodao.h"
#include "orm.h"

class DataFileLoad
{
public:
    explicit DataFileLoad();
    QVector<ExamInfoDao> getExamList();
    QVector<ClassInfoDao> getClassList();
    QVector<StudentInfoDao> getStudentInfo();
signals:

};

#endif // DATAFILELOAD_H
