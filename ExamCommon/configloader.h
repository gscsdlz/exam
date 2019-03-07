#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

class ConfigLoader : public QObject
{
    Q_OBJECT
public:
    explicit ConfigLoader(QObject *parent = 0);
    QString getConfig(QString, QString = "");
    void setConfig(QString, QString);
signals:

public slots:

private:
    QSettings *config;
};

#endif // CONFIGLOADER_H
