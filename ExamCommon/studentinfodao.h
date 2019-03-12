#ifndef STUDENTINFODAO_H
#define STUDENTINFODAO_H

#include "dao.h"

class StudentInfoDao : public DAO
{
public:
    StudentInfoDao();
    QVariant _get(QString);
};

#endif // STUDENTINFODAO_H
