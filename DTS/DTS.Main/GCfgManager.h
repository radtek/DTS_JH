/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GCfgManager.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_MAIN_GCFGMANAGER_H
#define DTS_MAIN_GCFGMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QtWidgets>
#include <QtXml>

#define CONFIG_SYS_APPID "app_id"
#define CONFIG_SYS_APPNAME "app_name"

#define CONFIG_WS_LOCAL_ADDR "ws_local_addr"
#define CONFIG_WS_LOCAL_PORT "ws_local_port"
#define CONFIG_WS_REMOTE_URL "ws_remote_url"

#define CONFIG_DB_LOCAL_ADDR "db_local_addr"
#define CONFIG_DB_LOCAL_PORT "db_local_port"
#define CONFIG_DB_LOCAL_USER "db_local_user"
#define CONFIG_DB_LOCAL_PASS "db_local_pass"
#define CONFIG_DB_LOCAL_NAME "db_local_name"

#define CONFIG_DB_REMOTE_ADDR "db_remote_addr"
#define CONFIG_DB_REMOTE_PORT "db_remote_port"
#define CONFIG_DB_REMOTE_USER "db_remote_user"
#define CONFIG_DB_REMOTE_PASS "db_remote_pass"
#define CONFIG_DB_REMOTE_NAME "db_remote_name"

#define CONFIG_RUN_DOWNLOAD "db_download"
#define CONFIG_RUN_UPLOAD "db_upload"
#define CONFIG_RUN_UPLOAD_TIMESPAN "tm_upload"
#define CONFIG_RUN_DOWNLOAD_TIMESPAN "tm_download"
#define CONFIG_RUN_GASWEIGHT_STORE "tm_gasweight_store"

#define QSQL_DB_STRING_LOCAL "LOCAL_HOST"
#define QSQL_DB_STRING_REMOTE "REMOTE_HOST"

#define DEFAULT_SQLSYNC_PATH "SqlSync.xml"

#define TABLE_WORKORDER "MES_WorkOrder"
#define TABLE_GASDATA "MES_Process_RefData"

class CSynchData;
typedef QSharedPointer<CSynchData> CSynchDataPtr;

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
    bool importSynchData(const QString &filename);
    bool exportSynchData(const QString &filename);
    QMap<QString, QString> getApplicationList();

public:
    void setConfig(const QMap<QString, QString> &currConfig);
    const QMap<QString, QString> &getConfig() const;
    QStringList getSynchDataKeys();
    CSynchDataPtr getSynchData(const QString &id);

    QString getVersion();
    QString getWorkDir();
    QString getAppName();
    QString getWSLocalAddr();
    int getWSLocalPort();
    QString getWSRemoteUrl();
    QStringList getDownload();
    QStringList getUpload();
    int getDownloadTimeSpan();
    int getUploadTimeSpan();
    int getGasWeightInitTime();

private:
    QSettings settings;
    QString currAppID;
    QMap<QString, QString> config;
    QMap<QString, CSynchDataPtr> synch;
};

#define qCfgManager (GCfgManager::instance())

extern QSqlDatabase DB_LOCAL;
extern QSqlDatabase DB_REMOTE;

#endif
