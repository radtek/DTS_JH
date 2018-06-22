/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GCfgManager.cpp
 * @brief   Configuration Manager
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "GCfgManager.h"

QSqlDatabase DB_LOCAL;
QSqlDatabase DB_REMOTE;

Q_GLOBAL_STATIC(GCfgManager, theInstance);

GCfgManager *GCfgManager::instance()
{
    return theInstance();
}

GCfgManager::GCfgManager()
{
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, "SOFTWARE");
}

GCfgManager::~GCfgManager()
{
}

bool GCfgManager::initialize()
{
    // check settings status
    if (settings.status() != QSettings::NoError)
    {
        qCritical().noquote() << QString("Open regedit failed, error[%1]").arg(settings.status());
        return false;
    }

    // delete invalid
    Q_FOREACH (auto &key, settings.childGroups())
    {
        if (settings.value(QString("%1/%2").arg(key).arg(CONFIG_SYS_APPNAME)).isNull())
        {
            qWarning().noquote() << QString("delete invalid regedit item[%1]").arg(key);
            settings.remove(key);
            settings.sync();
        }
    }
    return true;
}

QMap<QString, QString> GCfgManager::getApplicationList()
{
    QMap<QString, QString> map;
    Q_FOREACH (auto &key, settings.childGroups())
    {
        QString var = settings.value(QString("%1/%2").arg(key).arg(CONFIG_SYS_APPNAME)).toString();
        if (!var.isEmpty())
        {
            map.insert(key, var);
        }
    }
    return map;
}

QString GCfgManager::getVersion()
{
    static const char *major_version = "0";
    static const char *minor_version = "0";
    static const char *datetime = __TIMESTAMP__;

    return QString("%1.%2.%3")
        .arg(major_version)
        .arg(minor_version)
        .arg(QDateTime::fromString(datetime).toString("yyMMdd"));
}

QString GCfgManager::getWorkDir()
{
    return config[CONFIG_SYS_MYAPPID];
}

QString GCfgManager::getWSLocalAddr()
{
    return "127.0.0.1";
}

int GCfgManager::getWSLocalPort()
{
    return 8001;
}

bool GCfgManager::checkApplicationExist(const QString &appID)
{
    return settings.childGroups().contains(appID);
}

bool GCfgManager::createApplication(const QMap<QString, QString> &appConfig, bool update)
{
    QString strAppID = appConfig[CONFIG_SYS_MYAPPID];
    if (strAppID.isEmpty())
    {
        return false;
    }

    if (update != checkApplicationExist(strAppID))
    {
        return false;
    }

    settings.beginGroup(strAppID);
    for (auto it = appConfig.begin(); it != appConfig.end(); ++it)
    {
        settings.setValue(it.key(), it.value());
    }
    settings.endGroup();
    settings.sync();

    return true;
}

bool GCfgManager::deleteApplication(const QString &appID)
{
    settings.remove(appID);
    settings.sync();

    return true;
}

bool GCfgManager::loadApplication(const QString &appID)
{
    if (!checkApplicationExist(appID))
    {
        return false;
    }

    config.clear();

    settings.beginGroup(appID);
    Q_FOREACH (auto &key, settings.childKeys())
    {
        config[key] = settings.value(key).toString();
    }
    settings.endGroup();
    currAppID = appID;

    return true;
}

bool GCfgManager::initConnection()
{
    return true;
}

bool GCfgManager::initSynchData()
{
}
