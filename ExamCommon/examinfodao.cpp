#include "examinfodao.h"

ExamInfoDao::ExamInfoDao()
{

}

QVariant ExamInfoDao::_get(QString key)
{
    if (key == "str_start_time" || key == "str_end_time") {
        QDateTime T = QDateTime::fromMSecsSinceEpoch(DAO::_get(key.mid(4)).toLongLong());

        return T.toString("yyyy-MM-dd HH:mm:ss");
    }
    return DAO::_get(key);
}
