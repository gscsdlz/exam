#include "datafileload.h"

DataFileLoad::DataFileLoad()
{

}

QVector<ExamInfoDao> DataFileLoad::getExamList()
{
    ORM *db = ORM::getInstance();

    QVector<ExamInfoDao> result;

    QVector<QVector<QVariant>> res = db->getAll("SELECT * FROM exam_info");
    for (QVector<QVariant> row : res) {
        ExamInfoDao dao;
    }

    return result;
}

QVector<StudentInfoDao> DataFileLoad::getStudentInfo()
{
    ORM *db = ORM::getInstance();
    QVector<StudentInfoDao> result;

    QVector<QVector<QVariant>> res = db->getAll("");

    for (QVector<QVariant> row :res) {

    }

    return result;
}

