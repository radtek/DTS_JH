///*!
// * *****************************************************************************
// * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
// *
// * @file    GSqlManager.h
// * @brief   数据库公用
// * @version 1.0
// *
// * -----------------------------------------------------------------------------
// * @history
// *  <Date>    | <Author>       | <Description>
// * 2018/03/01 | WeiHeng        | Create this file
// * *****************************************************************************
// */

//#ifndef DTS_GSQLMANAGER_H
//#define DTS_GSQLMANAGER_H

//#include "GDtsDef.h"

//#define DEFAULT_SQLSYNC_PATH     "SqlSync.Configuration.xml"

//class TSqlSync
//{
//public:
//    QString id;

//    QString db_remote;
//    QString db_local;

//    QString table_local;
//    QString table_remote;

//    QStringList column_local;
//    QStringList column_remote;

//    QString SelectTable(bool local = true);
//    QString DeleteTable(bool local = true);
//    QString InsertTable(bool local = true);
//    QString InsertTableNoUID(bool local = true);
//};

//typedef QSharedPointer<TSqlSync> LPTSqlSync;

//class GSqlManager : public QObject
//{
//    Q_OBJECT

//public:
//    static GSqlManager &Instance();

//private:
//    GSqlManager();
//    virtual ~GSqlManager();

//public:
//    GERROR Initialize();
//    GERROR UnInitialize();

//    bool SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantMap &dbParam);
//    bool SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantList &dbParam);

//private:
//    GERROR readSqlSync(const QString &filename, QMap<QString, LPTSqlSync> &tmpSync);

//public:
//    QMap<QString, LPTSqlSync> _SqlSync;
//};

//#define qSqlManager     (&GSqlManager::Instance())

//#endif
