#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QWidget>
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include "datafileload.h"
#include "../ExamCommon/examinfodao.h"
#include "../ExamCommon/classinfodao.h"

namespace Ui {
class DataConfig;
}

class DataConfig : public QWidget
{
    Q_OBJECT

public:
    explicit DataConfig(QWidget *parent = 0);
    ~DataConfig();
protected:
    void generateExamItem(QString, int, int, bool = false);
    void generateClassItem(QString, int, int, bool = false);
public slots:
    void setExamSelected(QModelIndex);
    void setClassSelected(QModelIndex);
    void updateExamInfo(QModelIndex, QModelIndex, QVector<int>);
    void updateClassInfo(QModelIndex, QModelIndex, QVector<int>);
private slots:
    void on_start_clicked();
    void on_exit_clicked();
signals:
    void startExam(int, int);
private:
    Ui::DataConfig *ui;
    DataFileLoad load;
    QVector<ExamInfoDao> examLists;
    QVector<ClassInfoDao> classLists;
    QStandardItemModel *examModel;
    QStandardItemModel *classModel;
    int examId;
    int classId;
};

#endif // DATACONFIG_H
