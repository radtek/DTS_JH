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

#include <QMutex>
#include <QTimer>
#include <QSettings>

typedef QMap<QString, QString>  QStringMap;

#define CONFIG_SYS_MYAPPID                          "MYAPPID"
#define CONFIG_SYS_APPNAME                          "APPNAME"
#define CONFIG_SYS_LWSADDR                          "LWSADDR"
#define CONFIG_SYS_LWSPORT                          "LWSPORT"
#define CONFIG_SYS_LDBADDR                          "LDBADDR"
#define CONFIG_SYS_LDBPORT                          "LDBPORT"
#define CONFIG_SYS_LDBUSER                          "LDBUSER"
#define CONFIG_SYS_LDBPASS                          "LDBPASS"
#define CONFIG_SYS_LDBNAME                          "LDBNAME"
#define CONFIG_SYS_RWSADDR                          "RWSADDR"
#define CONFIG_SYS_RWSPORT                          "RWSPORT"
#define CONFIG_SYS_RDBADDR                          "RDBADDR"
#define CONFIG_SYS_RDBPORT                          "RDBPORT"
#define CONFIG_SYS_RDBUSER                          "RDBUSER"
#define CONFIG_SYS_RDBPASS                          "RDBPASS"
#define CONFIG_SYS_RDBNAME                          "RDBNAME"
#define CONFIG_RUN_LG_ENAB                          "LG_ENAB"
#define CONFIG_RUN_LG_PATH                          "LG_PATH"
#define CONFIG_RUN_LG_NAME                          "LG_NAME"
#define CONFIG_RUN_LG_MCNT                          "LG_MCNT"
#define CONFIG_RUN_LG_MSIZ                          "LG_MSIZ"
#define CONFIG_RUN_LG_DLVL                          "LG_DLVL"
#define CONFIG_RUN_LG_PATT                          "LG_PATT"
#define CONFIG_RUN_TM_UPLD                          "TM_UPLD"
#define CONFIG_RUN_TM_FUPD                          "TM_FUPD"
#define CONFIG_RUN_TM_DNLD                          "TM_DNLD"
#define CONFIG_RUN_TM_FDND                          "TM_FDND"
#define CONFIG_SYN_DOWNSQL                          "DOWNSQL"
#define CONFIG_SYN_UPLDSQL                          "UPLDSQL"

#define DEFAULT_CONFIG_SYS_MYAPPID      ""
#define DEFAULT_CONFIG_SYS_APPNAME      ""
#define DEFAULT_CONFIG_SYS_LWSADDR      "127.0.0.1"
#define DEFAULT_CONFIG_SYS_LWSPORT      8002
#define DEFAULT_CONFIG_SYS_LDBADDR      "127.0.0.1"
#define DEFAULT_CONFIG_SYS_LDBPORT      1433
#define DEFAULT_CONFIG_SYS_LDBUSER      "sa"
#define DEFAULT_CONFIG_SYS_LDBPASS      "123456"
#define DEFAULT_CONFIG_SYS_LDBNAME      "ChangKeMes"
#define DEFAULT_CONFIG_SYS_RWSADDR      "127.0.0.1"
#define DEFAULT_CONFIG_SYS_RWSPORT      8001
#define DEFAULT_CONFIG_SYS_RDBADDR      "127.0.0.1"
#define DEFAULT_CONFIG_SYS_RDBPORT      1433
#define DEFAULT_CONFIG_SYS_RDBUSER      "sa"
#define DEFAULT_CONFIG_SYS_RDBPASS      "123456"
#define DEFAULT_CONFIG_SYS_RDBNAME      "ChangKeMes"
#define DEFAULT_CONFIG_RUN_TM_UPLD      600
#define DEFAULT_CONFIG_MAX_RUN_TM_UPLD  86400
#define DEFAULT_CONFIG_MIN_RUN_TM_UPLD  120
#define DEFAULT_CONFIG_RUN_TM_FUPD      10
#define DEFAULT_CONFIG_RUN_TM_DNLD      300
#define DEFAULT_CONFIG_MAX_RUN_TM_DNLD  86400
#define DEFAULT_CONFIG_MIN_RUN_TM_DNLD  60
#define DEFAULT_CONFIG_RUN_TM_FDND      10
#define DEFAULT_CONFIG_SYN_DOWNSQL      ""
#define DEFAULT_CONFIG_SYN_UPLDSQL      ""

#define QSQL_DB_STRING_LOCAL        "LOCAL_HOST"
#define QSQL_DB_STRING_REMOTE       "REMOTE_HOST"
#define QSQL_DB_STRING_UPLD         "Center_Upload"
#define QSQL_DB_STRING_DNLD         "Workshop_Download"
#define QSQL_DB_STRING_DNWO         "Workshop_WorkOrder"

class GCfgManager : public QObject
{
    Q_OBJECT

public:
    static GCfgManager &Instance();

private:
    GCfgManager();
    virtual ~GCfgManager();

public:
    bool Initialize();
    bool UnInitialize();

public:
    bool CheckAppID(const QString &strAppID);
    bool SetAppID(const QString &strAppID);

    void GetAppList(QStringMap &appList);
    void SetConfig(const QVariantMap &config);
    bool CreateApp(const QVariantMap &config);
    void DeleteApp(const QString &strAppID);

private:
    void openDBConnect();

public Q_SLOTS:
    void Slot_QTimer_timeout_SetSync();

public:
    QVariantMap _Config;

private:
    QSettings   _Settings;
    QMutex      _SetMutex;
    QTimer      _SetTimer;
};

#define qCfgManager     (&GCfgManager::Instance())

#endif
