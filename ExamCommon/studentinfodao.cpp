#include "studentinfodao.h"

StudentInfoDao::StudentInfoDao()
{

}

QVariant StudentInfoDao::_get(QString key)
{
    if (key == "str_sex") {
        if (DAO::_get("sex").toInt() == 0) {
            return QStringLiteral("男");
        } else {
            return QStringLiteral("女");
        }
    }
    return DAO::_get(key);
}
