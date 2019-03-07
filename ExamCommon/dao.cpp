#include "dao.h"

DAO::DAO()
{

}

QVariant DAO::_get(QString key)
{
    return map[key];
}

void DAO::_set(QString key, QVariant val)
{
    map[key] = val;
}
