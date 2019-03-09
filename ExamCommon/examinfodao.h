#ifndef EXAMINFODAO_H
#define EXAMINFODAO_H

#include <QString>
#include <QTime>
#include <QDebug>
#include "dao.h"

class ExamInfoDao : public DAO
{
public:
    explicit ExamInfoDao();
    QVariant _get(QString);
};

#endif // EXAMINFODAO_H
