#include "examinfodao.h"

ExamInfoDao::ExamInfoDao()
{

}

void ExamInfoDao::_set(QString key, QVariant val)
{
    if (key == "startTime" || key == "endTime") {
        DAO::_set(key, QDateTime::fromString(startTime, "yyyy-MM-dd HH:mm:ss").toTime_t());
    } else {
        DAO::_set(key, val);
    }
}

