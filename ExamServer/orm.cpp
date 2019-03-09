#include "orm.h"

ORM *ORM::instance = NULL;

ORM::ORM(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QCoreApplication::applicationDirPath();
    db.setDatabaseName(path + "/exam.db");
    if (!db.open()) {
        QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("找不到数据库文件"));
    }
}

QVector<QVector<QVariant>> ORM::getAll(QString sql, QVector<QVariant> args)
{
    QVector<QVector<QVariant>> res;
    QSqlQuery q;
    if (query(q, sql, args)) {
        while (q.next()) {
            QVector<QVariant> item;
            int idx = 0;
            while (q.value(idx).isValid()) {
                item.push_back(q.value(idx++));
            }
            res.push_back(item);
        }
    }
    return res;
}

QVector<QVariant> ORM::getRow(QString sql, QVector<QVariant> args)
{
    QSqlQuery q;
    QVector<QVariant> res;
    if (query(q, sql, args)) {
        while (q.next()) {
            int idx = 0;
            while (q.value(idx).isValid()) {
                res.push_back(q.value(idx++));
            }
            break;
        }
    }
    return res;
}

QVariant ORM::getOne(QString sql, QVector<QVariant> args)
{
    QVector<QVariant> res = getRow(sql, args);
    return res[0];
}

int ORM::execute(QString sql, QVector<QVariant> args)
{
    QSqlQuery q;
    if (query(q, sql, args)) {
        if (sql.startsWith("INSERT") || sql.startsWith("insert")) {
            return q.lastInsertId().toInt();
        } else {
            return q.numRowsAffected();
        }
    } else {
       return 0;
    }
}

bool ORM::query(QSqlQuery &query, QString sql, QVector<QVariant> args)
{
    query.prepare(sql);

    for (int i = 0; i < args.length(); i++) {
        query.bindValue(i, args[i]);
    }
    return query.exec();
}

