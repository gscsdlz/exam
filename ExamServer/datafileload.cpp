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

    QVector<QVector<QVariant>> res = db->getAll("SELECT student_info.*, class_name FROM student_info LEFT JOIN class_info ON (student_info.class_id = class_info.id) WHERE class_id = ?", args);

    for (QVector<QVariant> row :res) {
        StudentInfoDao dao;
        dao._set("id", row.at(0));
        dao._set("student_name", row.at(1));
        dao._set("student_id", row.at(2));
        dao._set("class_id", row.at(3));
        dao._set("sex", row.at(4));
        dao._set("idcard", row.at(5));
        dao._set("password", row.at(6));
        dao._set("class_name", row.at(7));
        result.append(dao);
    }

    return result;
}

QVector<ExamProblem> DataFileLoad::getAllProblems(int examId)
{
    ORM *db = ORM::getInstance();
    QVector<QVariant> args;
    args.push_back(examId);
    auto res = db->getAll("SELECT * FROM problem_info WHERE exam_id = ?", args);
    QVector<ExamProblem> result;
    for(auto row : res) {
        ExamProblem p;
        p.setVarInfo(row.at(0).toInt(), row.at(1).toString(), row.at(5).toInt());
        QJsonDocument data = QJsonDocument::fromJson(row.at(2).toString().toLatin1());
        QJsonArray options = data.array();
        for (int i = 0; i < options.size(); i++) {
            p.appendOptions(options.at(i).toString());
        }
        data = QJsonDocument::fromJson(row.at(3).toString().toLatin1());
        QJsonArray answers = data.array();
        QVector<int> ans;
        for (int i = 0; i < answers.size(); i++) {
            ans.push_back(answers.at(i).toInt());
        }
        p.appendAnswer(ans);
        p.debug();
        result.append(p);
    }
    return result;
}

ExamInfoDao DataFileLoad::getExamInfo(int examId)
{
    ORM *db = ORM::getInstance();
    QVector<QVariant> args;
    args.push_back(examId);
    auto res = db->getRow("SELECT * FROM exam_info WHERE id = ?", args);
    ExamInfoDao dao;
    dao._set("exam_id", res.at(0));
    dao._set("exam_name", res.at(1));
    dao._set("start_time", res.at(2));
    dao._set("end_time", res.at(3));
    return dao;
}

bool DataFileLoad::updateExamInfo(int id, QString name, QString st, QString et)
{
    ORM *db = ORM::getInstance();
    QVector<QVariant> args;
    args.push_back(name);
    args.push_back(st);
    args.push_back(et);
    args.push_back(id);
    int row = db->execute("UPDATE exam_info SET exam_name = ?, start_time = ?, end_time = ? WHERE id = ?", args);
    return row > 0;
}

bool DataFileLoad::updateClassInfo(int id, QString name)
{
    ORM *db = ORM::getInstance();
    QVector<QVariant> args;
    args.push_back(name);
    args.push_back(id);

    int row = db->execute("UPDATE class_info SET class_name = ? WHERE id = ?", args);
    return row > 0;
}

bool DataFileLoad::saveAnswerInfo(int studentId, int examId, QString ansStr)
{
    ORM *db = ORM::getInstance();
    QVector<QVariant> args;
    args.append(studentId);
    args.append(examId);
    args.append(ansStr);

    int insertId = db->execute("INSERT INTO answer_result (student_id, exam_id, ans_str) VALUES (?,?,?)", args);

    return insertId > 0;
}
