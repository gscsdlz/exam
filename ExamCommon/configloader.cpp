#include "configloader.h"

ConfigLoader::ConfigLoader(QObject *parent) : QObject(parent)
{
    QString path = QCoreApplication::applicationDirPath();
    config = new QSettings(path + "/config.ini", QSettings::IniFormat);
}

QString ConfigLoader::getConfig(QString key, QString def)
{
    return config->value(key, def).toString();
}

void ConfigLoader::setConfig(QString key, QString val)
{
    config->setValue(key, val);
}
