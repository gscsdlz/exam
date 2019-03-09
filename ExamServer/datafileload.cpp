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
        dao._set("id", row[0]);
        dao._set("exam_name", row[1]);
        dao._set("start_time", row[2]);
        dao._set("end_time", row[3]);
        result.push_back(dao);
    }

    return result;
}

QVector<ClassInfoDao> DataFileLoad::getClassList()
{
    ORM *db = ORM::getInstance();
    QVector<ClassInfoDao> result;

    QVector<QVector<QVariant>> res = db->getAll("SELECT class_info.* FROM class_info");

    for (QVector<QVariant> row :res) {
        ClassInfoDao dao;
        dao._set("id", row[0]);
        dao._set("class_name", row[1]);
        QVector<QVariant> args;
        args.push_back(row[0]);
        dao._set("number", db->getOne("SELECT COUNT(*) FROM student_info WHERE class_id = ?", args));
        result.push_back(dao);
    }

    return result;
}

QVector<StudentInfoDao> DataFileLoad::getStudentInfo(int classId)
{
    ORM *db = ORM::getInstance();
    QVector<StudentInfoDao> result;
    QVector<QVariant> args;
    args.push_back(classId);

    QVector<QVector<QVariant>> res = db->getAll("SELECT * FROM student_info WHERE class_id = ?", args);

    for (QVector<QVariant> row :res) {
        StudentInfoDao dao;
        result.append(dao);
    }

    return result;
}

