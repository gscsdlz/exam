#ifndef ORM_H
#define ORM_H

#include <QCoreApplication>
#include <QObject>
#include <QVector>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QVariant>
#include <QSqlError>

class ORM : public QObject
{
    Q_OBJECT
public:
    static ORM *getInstance()
    {
        if (!instance) {
            instance = new ORM();
        }
        return instance;
    }
    QVector<QVector<QVariant>> getAll(QString, QVector<QVariant> args = QVector<QVariant>());
    QVector<QVariant> getRow(QString, QVector<QVariant> args = QVector<QVariant>());
    QVariant getOne(QString, QVector<QVariant> args = QVector<QVariant>());
    int execute(QString, QVector<QVariant> args = QVector<QVariant>());
    static QString dbSrc;

protected:
    bool query(QSqlQuery &, QString, QVector<QVariant>);
private:
    explicit ORM(QObject *parent = 0);
    QSqlDatabase db;
    static ORM *instance;
};

#endif // ORM_H
