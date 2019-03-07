#ifndef ORM_H
#define ORM_H

#include <QCoreApplication>
#include <QObject>
#include <QVector>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QVariant>

class ORM : public QObject
{
    Q_OBJECT
public:
    static ORM *getInstance()
    {
        if (!instance) {
            instance = new ORM;
        }
        return instance;
    }
    QVector<QVector<QVariant>> getAll(QString, QVector<QString> args = QVector<QString>());
    QVector<QVariant> getRow(QString, QVector<QString> args = QVector<QString>());
    int execute(QString, QVector<QString> args = QVector<QString>());

protected:
    bool query(QSqlQuery &, QString, QVector<QString>);
private:
    explicit ORM(QObject *parent = 0);
    QSqlDatabase db;
    static ORM *instance;
};

#endif // ORM_H
