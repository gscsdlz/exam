#ifndef DAO_H
#define DAO_H

#include <QVariant>
#include <QMap>

class DAO
{
public:
    DAO();
    QVariant _get(QString);
    void _set(QString, QVariant);
private:
    QMap<QString, QVariant> map;
};

#endif // DAO_H
