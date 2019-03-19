#include "datafileload.h"

DataFileLoad::DataFileLoad(QObject *parent)
    :QObject(parent)
{
    db = ORM::getInstance();
}

QVector<ExamInfoDao> DataFileLoad::getExamList()
{
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
        p.setAnswer(row.at(3).toInt());
        result.append(p);
    }
    return result;
}

ExamInfoDao DataFileLoad::getExamInfo(int examId)
{
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
    QVector<QVariant> args;
    args.push_back(name);
    args.push_back(id);

    int row = db->execute("UPDATE class_info SET class_name = ? WHERE id = ?", args);
    return row > 0;
}

bool DataFileLoad::updateStudentInfo(int id, QString name, QString stuId, int sex)
{
    QVector<QVariant> args;

    args.append(name);
    args.append(stuId);
    args.append(sex);
    args.append(id);

    int row = db->execute("UPDATE student_info SET student_name = ?, student_id = ?, sex = ? WHERE id = ?", args);
    return row > 0;
}

bool DataFileLoad::saveAnswerInfo(int studentId, int examId, QString ansStr)
{
    QVector<QVariant> args;
    args.append(studentId);
    args.append(examId);

    int row = db->getOne("SELECT COUNT(*) as num FROM answer_result WHERE student_id = ? AND exam_id = ?", args).toInt();
    if (row == 0) {
        args.append(ansStr);
        int insertId = db->execute("INSERT INTO answer_result (student_id, exam_id, ans_str, score) VALUES (?,?,?, 0)", args);
        return insertId > 0;
    }
    return false;
}

QVector<AnswerInfo> DataFileLoad::getAllAnswer(int examId, int classId)
{
    QVector<QVariant> args;
    args.push_back(classId);
    args.push_back(examId);

    QVector<AnswerInfo> result;
    auto res = db->getAll("SELECT answer_result.* FROM answer_result LEFT JOIN class_info ON (class_info.id = answer_result.student_id) WHERE class_info.id = ? AND exam_id = ?", args);

    for (auto row : res) {
        AnswerInfo info;
        info._set("id", row.at(0));
        info._set("student_id", row.at(1));
        info._set("exam_id", row.at(2));
        info._set("ans_str", row.at(3));
        result.append(info);
    }
    return result;
}

int DataFileLoad::getProblemAnswer(int pid)
{
    QVariant ans = db->getOne("SELECT answer FROM problem_info WHERE id = ?", QVector<QVariant>(1, pid));
    return ans.toInt();
}

void DataFileLoad::checkAnswer(int examId, int classId)
{
    QVector<AnswerInfo> result = getAllAnswer(examId, classId);
    int complete = 0;
    for (AnswerInfo info : result) {
        QJsonDocument dom = QJsonDocument::fromJson(info._get("ans_str").toString().toLatin1());
        QJsonArray arr = dom.array();

        int score = 0;

        for (int i = 0; i < arr.count(); i++) {
            QJsonObject obj = arr.at(i).toObject();
            int proId = obj.take("pro_id").toInt();
            int userAns = obj.take("answer").toInt();
            int correctAns = getProblemAnswer(proId);
            if (userAns == correctAns) {
               score++;
            }
        }
        QVector<QVariant> args;
        args.append(score);
        args.append(info._get("id"));
        db->execute("UPDATE answer_result SET score = ? WHERE id = ?", args);
        emit completeCheck(int(complete * 1.0 / result.length()));
    }
    emit completeCheck(101);
    exportFile(examId, classId);
}

void DataFileLoad::exportFile(int examId, int classId)
{
    QString path = QCoreApplication::applicationDirPath();
    path += "/data";

    QFile file(path + "/out.csv");

    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
         return;
    }
    QTextStream out(&file);

    QVector<QVariant> args;
    args.append(examId);
    args.append(classId);

    auto result = db->getAll("SELECT score, student_info.*, class_name FROM answer_result "
               "LEFT JOIN student_info ON (answer_result.student_id = student_info.id) "
               "LEFT JOIN class_info ON (class_info.id = student_info.class_id) "
               "WHERE exam_id = ? AND class_id = ?", args);

    out.setCodec("UTF-8");
    QChar head = 0xfeff;
    out << head;

    out << QStringLiteral("班级,学号,姓名,性别,分数\n");
    for (auto row : result) {
        out << row.at(8).toInt() << ","
            << row.at(3).toString() << ","
            << row.at(2).toString() << ","
            << (row.at(5).toInt() == 1 ? QStringLiteral("女") : QStringLiteral("男")) << ","
            << row.at(0).toInt() << "\n";
    }
    file.close();
    path.replace("/", "\\");
    QProcess::startDetached("explorer " + path);
}
