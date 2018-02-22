/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      GCfgManager.cpp
 *  \brief
 *      Manager-Config
 *  \since
 *  001     2017/11/01      weiheng     create this file
 */
//!============================================================================

#include "GDtsData.h"


GCfgManager &GCfgManager::Instance()
{
    static GCfgManager inst;
    return inst;
}

GCfgManager::GCfgManager()
{
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, "SOFTWARE");

    QObject::connect(&_SetTimer, &QTimer::timeout, this, &GCfgManager::Slot_QTimer_timeout_SetSync);
}

GCfgManager::~GCfgManager()
{
    _SetTimer.stop();
}

bool GCfgManager::Initialize()
{
    // check settings status
    if (_Settings.status() != QSettings::NoError)
    {
        qCritical().noquote() << QString("打开注册列表失败，错误[%1]").arg(_Settings.status());
        return false;
    }

    // delete invalid
    for (const QString &key : _Settings.childGroups())
    {
        QMutexLocker locker(&_SetMutex);
        if (_Settings.value(QString("%1/%2").arg(key).arg(CONFIG_SYS_APPNAME)).isNull())
        {
            qWarning().noquote() << QString("删除无效注册列表，项[%1]").arg(key);
            _Settings.remove(key);
            _Settings.sync();
        }
    }

    _SetTimer.setInterval(30000);
    _SetTimer.start();

    return true;
}

bool GCfgManager::UnInitialize()
{
    _SetTimer.stop();
    return true;
}

bool GCfgManager::CheckAppID(const QString &strAppID)
{
    return _Settings.childGroups().contains(strAppID);
}

bool GCfgManager::SetAppID(const QString &strAppID)
{
    _Config[CONFIG_SYS_MYAPPID] = strAppID;

    do
    {
        QMutexLocker locker(&_SetMutex);

        _Settings.beginGroup(strAppID);
        _Config[CONFIG_SYS_APPNAME] = _Settings.value(CONFIG_SYS_APPNAME, DEFAULT_CONFIG_SYS_APPNAME);
        _Config[CONFIG_SYS_LWSADDR] = _Settings.value(CONFIG_SYS_LWSADDR, DEFAULT_CONFIG_SYS_LWSADDR);
        _Config[CONFIG_SYS_LWSPORT] = _Settings.value(CONFIG_SYS_LWSPORT, DEFAULT_CONFIG_SYS_LWSPORT);
        _Config[CONFIG_SYS_LDBADDR] = _Settings.value(CONFIG_SYS_LDBADDR, DEFAULT_CONFIG_SYS_LDBADDR);
        _Config[CONFIG_SYS_LDBPORT] = _Settings.value(CONFIG_SYS_LDBPORT, DEFAULT_CONFIG_SYS_LDBPORT);
        _Config[CONFIG_SYS_LDBUSER] = _Settings.value(CONFIG_SYS_LDBUSER, DEFAULT_CONFIG_SYS_LDBUSER);
        _Config[CONFIG_SYS_LDBPASS] = _Settings.value(CONFIG_SYS_LDBPASS, DEFAULT_CONFIG_SYS_LDBPASS);
        _Config[CONFIG_SYS_LDBNAME] = _Settings.value(CONFIG_SYS_LDBNAME, DEFAULT_CONFIG_SYS_LDBNAME);
        _Config[CONFIG_SYS_RWSADDR] = _Settings.value(CONFIG_SYS_RWSADDR, DEFAULT_CONFIG_SYS_RWSADDR);
        _Config[CONFIG_SYS_RWSPORT] = _Settings.value(CONFIG_SYS_RWSPORT, DEFAULT_CONFIG_SYS_RWSPORT);
        _Config[CONFIG_SYS_RDBADDR] = _Settings.value(CONFIG_SYS_RDBADDR, DEFAULT_CONFIG_SYS_RDBADDR);
        _Config[CONFIG_SYS_RDBPORT] = _Settings.value(CONFIG_SYS_RDBPORT, DEFAULT_CONFIG_SYS_RDBPORT);
        _Config[CONFIG_SYS_RDBUSER] = _Settings.value(CONFIG_SYS_RDBUSER, DEFAULT_CONFIG_SYS_RDBUSER);
        _Config[CONFIG_SYS_RDBPASS] = _Settings.value(CONFIG_SYS_RDBPASS, DEFAULT_CONFIG_SYS_RDBPASS);
        _Config[CONFIG_SYS_RDBNAME] = _Settings.value(CONFIG_SYS_RDBNAME, DEFAULT_CONFIG_SYS_RDBNAME);
        _Config[CONFIG_RUN_LG_ENAB] = _Settings.value(CONFIG_RUN_LG_ENAB, DEFAULT_CONFIG_LG_ENAB);
        _Config[CONFIG_RUN_LG_PATH] = _Settings.value(CONFIG_RUN_LG_PATH, DEFAULT_CONFIG_LG_PATH);
        _Config[CONFIG_RUN_LG_NAME] = _Settings.value(CONFIG_RUN_LG_NAME, DEFAULT_CONFIG_LG_NAME);
        _Config[CONFIG_RUN_LG_MCNT] = _Settings.value(CONFIG_RUN_LG_MCNT, DEFAULT_CONFIG_LG_MCNT);
        _Config[CONFIG_RUN_LG_MSIZ] = _Settings.value(CONFIG_RUN_LG_MSIZ, DEFAULT_CONFIG_LG_MSIZ);
        _Config[CONFIG_RUN_LG_DLVL] = _Settings.value(CONFIG_RUN_LG_DLVL, DEFAULT_CONFIG_LG_DLVL);
        _Config[CONFIG_RUN_TM_UPLD] = _Settings.value(CONFIG_RUN_TM_UPLD, DEFAULT_CONFIG_RUN_TM_UPLD);
        _Config[CONFIG_RUN_TM_FUPD] = _Settings.value(CONFIG_RUN_TM_FUPD, DEFAULT_CONFIG_RUN_TM_FUPD);
        _Config[CONFIG_RUN_TM_DNLD] = _Settings.value(CONFIG_RUN_TM_DNLD, DEFAULT_CONFIG_RUN_TM_DNLD);
        _Config[CONFIG_RUN_TM_FDND] = _Settings.value(CONFIG_RUN_TM_FDND, DEFAULT_CONFIG_RUN_TM_FDND);
        _Config[CONFIG_SYN_DOWNSQL] = _Settings.value(CONFIG_SYN_DOWNSQL, DEFAULT_CONFIG_SYN_DOWNSQL);
        _Config[CONFIG_SYN_UPLDSQL] = _Settings.value(CONFIG_SYN_UPLDSQL, DEFAULT_CONFIG_SYN_UPLDSQL);
        _Settings.endGroup();
    }
    while (0);

    _Config[CONFIG_RUN_TM_UPLD] = CheckValueRange(_Config[CONFIG_RUN_TM_UPLD].toInt(), DEFAULT_CONFIG_MAX_RUN_TM_UPLD, DEFAULT_CONFIG_MIN_RUN_TM_UPLD);
    _Config[CONFIG_RUN_TM_DNLD] = CheckValueRange(_Config[CONFIG_RUN_TM_DNLD].toInt(), DEFAULT_CONFIG_MAX_RUN_TM_DNLD, DEFAULT_CONFIG_MIN_RUN_TM_DNLD);

    openDBConnect();

    return true;
}

void GCfgManager::GetAppList(QStringMap &appList)
{
    appList.clear();

    for (const QString &key : _Settings.childGroups())
    {
        QString var = _Settings.value(QString("%1/%2").arg(key).arg(CONFIG_SYS_APPNAME)).toString();
        if (!var.isEmpty())
        {
            appList.insert(key, var);
        }
    }
}

void GCfgManager::SetConfig(const QVariantMap &config)
{
    for (auto it = config.begin(); it != config.end(); ++it)
    {
        _Config[it.key()] = it.value();
    }
}

bool GCfgManager::CreateApp(const QVariantMap &config)
{
    QString strAppID = config[CONFIG_SYS_MYAPPID].toString();
    if (strAppID.isEmpty())
    {
        return false;
    }
    if (CheckAppID(strAppID))
    {
        return false;
    }

    _Settings.beginGroup(strAppID);
    for (auto it = config.begin(); it != config.end(); ++it)
    {
        _Settings.setValue(it.key(), it.value());
    }
    _Settings.endGroup();
    _Settings.sync();

    return true;
}

void GCfgManager::DeleteApp(const QString &strAppID)
{
    _Settings.remove(strAppID);
    _Settings.sync();
}

namespace
{
const QStringList StrList_Local_SqlConnect = { QSQL_DB_STRING_LOCAL, QSQL_DB_STRING_DNLD, QSQL_DB_STRING_DNWO };
const QStringList StrList_Remote_SqlConnect = { QSQL_DB_STRING_REMOTE, QSQL_DB_STRING_UPLD};
}

void GCfgManager::openDBConnect()
{
    QString dsn;
    QSqlDatabase dbSQL;
    QBool bOpenResult = false;

    // Local
    do
    {
        dsn = QString("Driver={sql server};SERVER=%1;PORT=%2;UID=%3;PWD=%4;DATABASE=%5;")
              .arg(_Config[CONFIG_SYS_LDBADDR].toString())
              .arg(_Config[CONFIG_SYS_LDBPORT].toString())
              .arg(_Config[CONFIG_SYS_LDBUSER].toString())
              .arg(_Config[CONFIG_SYS_LDBPASS].toString())
              .arg(_Config[CONFIG_SYS_LDBNAME].toString());

        for (auto const &str : StrList_Local_SqlConnect)
        {
            dbSQL = QSqlDatabase::addDatabase("QODBC", str);
            dbSQL.setDatabaseName(dsn);
            //dbSQL.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
            qDebug().noquote() << QString("创建数据库连接[%1],[%2]").arg(str).arg(dsn);
            bOpenResult = dbSQL.open();
            if (bOpenResult)
            {
                qInfo().noquote() << QString("数据库连接成功！[%1]").arg(str);
            }
            else
            {
                qWarning().noquote() << QString("数据库连接失败！[%1]").arg(str);
            }
        }
    }
    while (0);

    // Remote
    do
    {
        dsn = QString("Driver={sql server};SERVER=%1;PORT=%2;UID=%3;PWD=%4;DATABASE=%5;")
              .arg(_Config[CONFIG_SYS_RDBADDR].toString())
              .arg(_Config[CONFIG_SYS_RDBPORT].toString())
              .arg(_Config[CONFIG_SYS_RDBUSER].toString())
              .arg(_Config[CONFIG_SYS_RDBPASS].toString())
              .arg(_Config[CONFIG_SYS_RDBNAME].toString());

        for (const QString &str : StrList_Remote_SqlConnect)
        {
            dbSQL = QSqlDatabase::addDatabase("QODBC", str);
            dbSQL.setDatabaseName(dsn);
            //dbSQL.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
            qDebug().noquote() << QString("创建数据库连接[%1],[%2]").arg(str).arg(dsn);
            bOpenResult = dbSQL.open();
            if (bOpenResult)
            {
                qInfo().noquote() << QString("数据库连接成功！[%1]").arg(str);
            }
            else
            {
                qWarning().noquote() << QString("数据库连接失败！[%1]").arg(str);
            }
        }
    }
    while (0);
}

void GCfgManager::Slot_QTimer_timeout_SetSync()
{
    QString strAppID = _Config[CONFIG_SYS_MYAPPID].toString();

    _Settings.beginGroup(strAppID);
    for (auto it = _Config.begin(); it != _Config.end(); ++it)
    {
        _Settings.setValue(it.key(), it.value());
    }
    _Settings.endGroup();
    _Settings.sync();
}
