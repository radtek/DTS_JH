/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GCfgManager.h
 * @brief   配置文件
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_MAIN_GCFGMANAGER_H
#define DTS_MAIN_GCFGMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QtWidgets>

typedef QMap<QString, QString> QStringMap;

#define CONFIG_SYS_MYAPPID "MYAPPID"
#define CONFIG_SYS_APPNAME "APPNAME"
#define CONFIG_SYS_LWSADDR "LWSADDR"
#define CONFIG_SYS_LWSPORT "LWSPORT"
#define CONFIG_SYS_LDBADDR "LDBADDR"
#define CONFIG_SYS_LDBPORT "LDBPORT"
#define CONFIG_SYS_LDBUSER "LDBUSER"
#define CONFIG_SYS_LDBPASS "LDBPASS"
#define CONFIG_SYS_LDBNAME "LDBNAME"
#define CONFIG_SYS_RWSADDR "RWSADDR"
#define CONFIG_SYS_RWSPORT "RWSPORT"
#define CONFIG_SYS_RDBADDR "RDBADDR"
#define CONFIG_SYS_RDBPORT "RDBPORT"
#define CONFIG_SYS_RDBUSER "RDBUSER"
#define CONFIG_SYS_RDBPASS "RDBPASS"
#define CONFIG_SYS_RDBNAME "RDBNAME"
#define CONFIG_RUN_LG_ENAB "LG_ENAB"
#define CONFIG_RUN_LG_PATH "LG_PATH"
#define CONFIG_RUN_LG_NAME "LG_NAME"
#define CONFIG_RUN_LG_MCNT "LG_MCNT"
#define CONFIG_RUN_LG_MSIZ "LG_MSIZ"
#define CONFIG_RUN_LG_DLVL "LG_DLVL"
#define CONFIG_RUN_LG_PATT "LG_PATT"
#define CONFIG_RUN_TM_UPLD "TM_UPLD"
#define CONFIG_RUN_TM_FUPD "TM_FUPD"
#define CONFIG_RUN_TM_DNLD "TM_DNLD"
#define CONFIG_RUN_TM_FDND "TM_FDND"
#define CONFIG_SYN_DOWNSQL "DOWNSQL"
#define CONFIG_SYN_UPLDSQL "UPLDSQL"

#define QSQL_DB_STRING_LOCAL "LOCAL_HOST"
#define QSQL_DB_STRING_REMOTE "REMOTE_HOST"

#include <QtCore>

class GCfgManager
{
public:
    static GCfgManager *instance();

public:
    GCfgManager();
    virtual ~GCfgManager();

public:
    bool initialize();
    bool checkApplicationExist(const QString &appID);
    bool createApplication(const QMap<QString, QString> &appConfig, bool update = false);
    bool deleteApplication(const QString &appID);
    bool loadApplication(const QString &appID);
    bool initConnection();
    bool initSynchData();
    QMap<QString, QString> getApplicationList();

public:
    QString getVersion();
    QString getWorkDir();
    QString getWSLocalAddr();
    int getWSLocalPort();

private:
    QSettings settings;
    QString currAppID;
    QMap<QString, QString> config;
};

#define qCfgManager (GCfgManager::instance())

extern QSqlDatabase DB_LOCAL;
extern QSqlDatabase DB_REMOTE;

#endif
