#include "dataconfig.h"
#include "ui_dataconfig.h"

DataConfig::DataConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataConfig)
{
    ui->setupUi(this);
}

DataConfig::~DataConfig()
{
    delete ui;
}
