#ifndef EXAMPROBLEM_H
#define EXAMPROBLEM_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDebug>

class ExamProblem
{
public:
    explicit ExamProblem();
    void setVarInfo(int, QString, int);
    void appendOptions(QString);
    void setAnswer(int);

    void debug();
    int getType();
    int getId();
    QString getTitle();
    QVector<QString> getOptions();
    int getAnswer();
private:
    int id;
    QString title;
    int type;
    QVector<QString> options;
    int answer;
};

#endif // EXAMPROBLEM_H
