#include "examproblem.h"

ExamProblem::ExamProblem()
{

}

void ExamProblem::setVarInfo(int _id, QString _title, int _type)
{
    id = _id;
    title = _title;
    type = _type;
}

void ExamProblem::appendOptions(QString option)
{
    options.append(option);
}

void ExamProblem::appendAnswer(int ans)
{
    if (answer.empty()) {
        answer.append(ans);
    } else {
        answer[0] = ans;
    }
}

void ExamProblem::appendAnswer(QVector<int> ans)
{
    answer = ans;
}

void ExamProblem::debug()
{
    qDebug() << id << " " << title << " " << type;
    for (int i = 0; i < options.length(); i++) {
        qDebug() << options[i];
    }
}

int ExamProblem::getType()
{
    return type;
}

int ExamProblem::getId()
{
    return id;
}

QString ExamProblem::getTitle()
{
    return title;
}

QVector<QString> ExamProblem::getOptions()
{
    return options;
}

QVector<int> ExamProblem::getAnswer()
{
    return answer;
}
