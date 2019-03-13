#include "dataconfig.h"
#include "ui_dataconfig.h"

DataConfig::DataConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataConfig)
{
    examId = classId = 0;

    ui->setupUi(this);
    examLists = load.getExamList();
    classLists = load.getClassList();

    examModel = new QStandardItemModel(this);
    examModel->setColumnCount(5);
    examModel->setHeaderData(0, Qt::Horizontal, QStringLiteral("编号"));
    examModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("考试名称"));
    examModel->setHeaderData(2, Qt::Horizontal, QStringLiteral("开始时间"));
    examModel->setHeaderData(3, Qt::Horizontal, QStringLiteral("结束时间"));
    examModel->setHeaderData(4, Qt::Horizontal, QStringLiteral("状态"));


    classModel = new QStandardItemModel(this);
    classModel->setColumnCount(4);
    classModel->setHeaderData(0, Qt::Horizontal, QStringLiteral("编号"));
    classModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("班级名"));
    classModel->setHeaderData(2, Qt::Horizontal, QStringLiteral("人数"));
    classModel->setHeaderData(3, Qt::Horizontal, QStringLiteral("状态"));


    for (int i = 0; i < examLists.length(); i++) {

        generateExamItem(examLists[i]._get("id").toString(), i, 0);
        generateExamItem(examLists[i]._get("exam_name").toString(), i, 1, true);
        generateExamItem(examLists[i]._get("start_time").toString(), i, 2, true);
        generateExamItem(examLists[i]._get("end_time").toString(), i, 3, true);

        QStandardItem *item = new QStandardItem(QIcon(":r/init.png"), "");
        item->setEditable(false);
        item->setSelectable(false);
        examModel->setItem(i, 4, item);
    }

    for (int i = 0; i < classLists.length(); i++) {

        generateClassItem(classLists[i]._get("id").toString(), i, 0);
        generateClassItem(classLists[i]._get("class_name").toString(), i, 1, true);
        generateClassItem(classLists[i]._get("number").toString(), i, 2);
        QStandardItem *item = new QStandardItem(QIcon(":r/init.png"), "");
        item->setEditable(false);
        item->setSelectable(false);
        classModel->setItem(i, 3, item);
    }

    QObject::connect(ui->examTable, &QTableView::clicked, this, &DataConfig::setExamSelected);
    QObject::connect(ui->classTable, &QTableView::clicked, this, &DataConfig::setClassSelected);
    QObject::connect(classModel, &QStandardItemModel::dataChanged, this, &DataConfig::updateClassInfo);
    QObject::connect(examModel, &QStandardItemModel::dataChanged, this, &DataConfig::updateExamInfo);

    ui->examTable->setModel(examModel);
//    ui->examTable->setColumnWidth(0, 40);
//    ui->examTable->setColumnWidth(1, 341);
//    ui->examTable->setColumnWidth(2, 130);
//    ui->examTable->setColumnWidth(3, 130);
//    ui->examTable->setColumnWidth(4, 90);
    ui->examTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->classTable->setModel(classModel);
    ui->classTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setFixedWidth(800);
    setFixedHeight(500);
}

DataConfig::~DataConfig()
{
    delete ui;
}

void DataConfig::generateExamItem(QString str, int row, int col, bool edit)
{
    QStandardItem *item = new QStandardItem(str);
    item->setEditable(edit);
    item->setSelectable(false);
    examModel->setItem(row, col, item);
}

void DataConfig::generateClassItem(QString str, int row, int col, bool edit)
{
    QStandardItem *item = new QStandardItem(str);
    item->setEditable(edit);
    item->setSelectable(false);
    classModel->setItem(row, col, item);
}

void DataConfig::setExamSelected(QModelIndex idx)
{
    for (int i = 0; i < examModel->rowCount(); i++) {
        examModel->item(i, examModel->columnCount() - 1)->setIcon(QIcon(":r/init.png"));
        examModel->item(i, examModel->columnCount() - 1)->setText("");
    }
    examModel->item(idx.row(), examModel->columnCount() - 1)->setIcon(QIcon(":r/ok.png"));
    examModel->item(idx.row(), examModel->columnCount() - 1)->setText(QStringLiteral(" 已选择"));
    examId = examModel->item(idx.row())->text().toInt();
}

void DataConfig::setClassSelected(QModelIndex idx)
{
    for (int i = 0; i < classModel->rowCount(); i++) {
        classModel->item(i, classModel->columnCount() - 1)->setIcon(QIcon(":r/init.png"));
        classModel->item(i, classModel->columnCount() - 1)->setText("");
    }
    classModel->item(idx.row(), classModel->columnCount() - 1)->setIcon(QIcon(":r/ok.png"));
    classModel->item(idx.row(), classModel->columnCount() - 1)->setText(QStringLiteral(" 已选择"));
    classId = classModel->item(idx.row())->text().toInt();
}

void DataConfig::on_start_clicked()
{
    if (examId == 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请选择考试场次"));
    } else if (classId == 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请选择参加考试的班级"));
    } else {
       emit startExam(examId, classId);
        this->close();
    }
}

void DataConfig::on_exit_clicked()
{
    this->close();
}

void DataConfig::updateExamInfo(QModelIndex idx1, QModelIndex, QVector<int>)
{
    if (idx1.column() >= 1 && idx1.column() <= 3) {
        int row = idx1.row();

        load.updateExamInfo(
            examModel->item(row, 0)->text().toInt(),
            examModel->item(row, 1)->text(),
            examModel->item(row, 2)->text(),
            examModel->item(row, 3)->text()
        );
    }
}

void DataConfig::updateClassInfo(QModelIndex idx1, QModelIndex, QVector<int>)
{
    if (idx1.column() == 1) {
        int row = idx1.row();

        load.updateClassInfo(
            classModel->item(row, 0)->text().toInt(),
            classModel->item(row, 1)->text()
        );
    }
}
