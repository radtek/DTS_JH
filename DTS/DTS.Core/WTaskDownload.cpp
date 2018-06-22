/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      WTaskDownload.cpp
 *  \brief
 *      Task-Download Basis Data
 *  \since
 *  001     2017/11/01      weiheng     create this file
 */
//!============================================================================

#include "WTaskDownload.h"


WTaskDownload &WTaskDownload::Instance()
{
    static WTaskDownload inst;
    return inst;
}

WTaskDownload::WTaskDownload()
    : QObject()
    , _SQL()
    , _RunThread()
    , _RunTimer()
{

}
WTaskDownload::~WTaskDownload()
{
    UnInitialize();
}

GERROR WTaskDownload::Initialize()
{
    qDebug().noquote() << "数据下载：任务初始化开始……";

    _SQL = QSqlDatabase::database(QSQL_DB_STRING_DNLD);

    if (!_SQL.isOpen())
    {
        if (!_SQL.open())
        {
            qWarning().noquote() << QString("数据下载：数据库连接失败，STR=%1").arg(QSQL_DB_STRING_DNLD);
            return GERROR_SQL_OPENFAILED;
        }
    }

    (*this).moveToThread(&_RunThread);
    _RunTimer.moveToThread(&_RunThread);
    QObject::connect(&_RunTimer, &QTimer::timeout, this, &WTaskDownload::Slot_taskWork);
    _RunThread.start();

    QTimer::singleShot(qCfgManager->_Config.value(CONFIG_RUN_TM_FDND, DEFAULT_CONFIG_RUN_TM_FDND).toInt() * 1000, this, &WTaskDownload::Slot_resetTimer);

    qDebug().noquote() << "数据下载：任务初始化成功。";
    return GERROR_OK;
}

GERROR WTaskDownload::UnInitialize()
{
    if (_RunThread.isRunning())
    {
        _RunThread.quit();
    }

    return GERROR_OK;
}


QBool WTaskDownload::taskDownloadTable(const QString &name)
{
    TRACE_FUNCTION();

    qDebug().noquote() << QString("[数据下载] 任务[%1] 开始……").arg(name);

    LPTSqlSync p = qSqlManager->_SqlSync[name];
    if (!p)
    {
        qWarning().noquote() << QString("[数据下载] 任务[%1] 未找到！").arg(name);
        return false;
    }

    QSqlDatabase dbSQLRemote = QSqlDatabase::database(p->db_remote, false);
    if (!dbSQLRemote.isOpen())
    {
        qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库未打开！").arg(name);
        return false;
    }

    QSqlDatabase dbSQLLocal = QSqlDatabase::database(p->db_local, false);
    if (!dbSQLLocal.isOpen())
    {
        qWarning().noquote() << QString("[数据下载] 任务[%1] 本地数据库未打开！").arg(name);
        return false;
    }

    QSqlQuery dbQuery(dbSQLLocal);

    bool bSuccess = true;
    QString LastTime;
    // 获取上次更新时间
    qDebug().noquote() << QString("[数据下载] 任务[%1] 获取上一次更新时间……").arg(name);
    do
    {
        QString dbStr = "SELECT RecodeLastTime FROM PCS_Upload_Info WHERE UploadTable = :UploadTable";
        QVariantMap dbParam;
        dbParam.insert(":UploadTable", p->table_local);
        if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
        {
            bSuccess = false;
            break;
        }

        if (!dbQuery.next())
        {
            qDebug().noquote() << QString("[数据下载] 任务[%1] 未找到上一次更新时间。设置上一次更新时间为NULL。").arg(name);

            QString dbStr = "INSERT INTO [PCS_Upload_Info](UploadTable) VALUES(:UploadTable)";
            QVariantMap dbParam;
            dbParam.insert(":UploadTable", p->table_local);
            if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
            {
                bSuccess = false;
                break;
            }
        }
        else
        {
            LastTime = dbQuery.value("RecodeLastTime").toString();
        }
    }
    while (0);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[数据下载] 任务[%1] 本地数据库存在异常，终止任务！").arg(name);
        return false;
    }

    qDebug().noquote() << QString("[数据下载] 任务[%1] 上一次更新时间为：%2").arg(name).arg(LastTime);

    QDateTime CurrTime = QDateTime::currentDateTime();
    if (LastTime.isEmpty())
    {
        // 上次更新时间为NULL
        QSqlQuery dbQueryRemoteCreate(dbSQLRemote);
        do
        {
            QString dbStr = p->SelectTable(false);
            QVariantMap dbParam;
            if (!qSqlManager->SqlExec(dbQueryRemoteCreate, dbStr, dbParam))
            {
                bSuccess = false;
                break;
            }
            qDebug().noquote() << QString("[数据下载] 表[%1] 包含 %2 条新增记录。").arg(p->table_remote).arg(dbQueryRemoteCreate.numRowsAffected());
        }
        while (0);

        if (!bSuccess)
        {
            qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库存在异常，终止任务！").arg(name);
            return false;
        }

        qDebug().noquote() << QString("[数据下载] 任务[%1] 上一次更新时间为NULL，清空本地数据库。").arg(name);

        qDebug() << "[SQL] 开启事务：" << dbSQLLocal.transaction();
        do
        {
            QString dbStr = p->DeleteTable();
            QVariantMap dbParam;

            if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
            {
                bSuccess = false;
                break;
            }

            qDebug().noquote() << QString("[数据下载] 表[%1] 开始插入数据。").arg(p->table_local);
            int cnt = 0;
            dbStr = p->InsertTable(true);
            while (dbQueryRemoteCreate.next())
            {
                dbParam.clear();
                for (int i = 0; i < p->column_local.size(); ++i)
                {
                    dbParam.insert(":" + p->column_local[i], dbQueryRemoteCreate.value(i));
                }

                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
                {
                    bSuccess = false;
                    break;
                }
                if (++cnt % 1000 == 0)
                {
                    qDebug().noquote() << QString("[数据下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_local).arg(cnt);
                }
            }

            if (bSuccess)
            {
                qDebug().noquote() << QString("[数据下载] 表[%1] 完成插入数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
            }
        }
        while (0);

        if (bSuccess)
        {
            qDebug().noquote() << QString("[数据下载] 任务[%1] 更新完成，修改上一次完成时间。").arg(name) ;
            do
            {
                QString dbStr = "UPDATE PCS_Upload_Info SET RecodeLastTime = :curr WHERE UploadTable = :UploadTable ";
                QVariantMap dbParam;
                dbParam.insert(":curr", CurrTime);
                dbParam.insert(":UploadTable", p->table_local);
                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
                {
                    bSuccess = false;
                    break;
                }
            }
            while (0);
        }

        if (bSuccess)
        {
            qDebug() << "[SQL] 执行成功，提交事务：" << dbSQLLocal.commit();
        }
        else
        {
            qDebug() << "[SQL] 执行失败，回滚事务：" << dbSQLLocal.rollback();
        }

        return bSuccess;
    }
    else
    {
        QSqlQuery dbQueryRemoteCreate(dbSQLRemote);
        QSqlQuery dbQueryRemoteUpdate(dbSQLRemote);

        QString dbStr;
        QVariantMap dbParam;
        dbParam.insert(":last", LastTime);
        dbParam.insert(":curr", CurrTime);
        do
        {
            dbStr = p->SelectTable(false) + " WHERE CreateTime >= :last AND CreateTime < :curr ";
            if (!qSqlManager->SqlExec(dbQueryRemoteCreate, dbStr, dbParam))
            {
                bSuccess = false;
                dbQueryRemoteCreate.clear();
                break;
            }

            qDebug().noquote() << QString("[数据下载] 表[%1] 包含 %2 条新增记录。").arg(p->table_remote).arg(dbQueryRemoteCreate.numRowsAffected());
        }
        while (0);
        if (!bSuccess)
        {
            qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库存在异常，终止任务！").arg(name);
            return false;
        }

        do
        {
            QString dbStr = p->SelectTable(false) + " WHERE UpdateTime >= :last AND UpdateTime < :curr";

            if (!qSqlManager->SqlExec(dbQueryRemoteUpdate, dbStr, dbParam))
            {
                bSuccess = false;
                dbQueryRemoteUpdate.clear();
                break;
            }

            qDebug().noquote() << QString("[数据下载] 表[%1] 包含 %2 条更新记录。").arg(p->table_remote).arg(dbQueryRemoteUpdate.numRowsAffected());
        }
        while (0);

        if (!bSuccess)
        {
            qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库存在异常，终止任务！").arg(name);
            return false;
        }

        qDebug() << "[SQL] 开启事务：" << dbSQLLocal.transaction();
        do
        {
            qDebug().noquote() << QString("[数据下载] 表[%1] 开始插入数据。").arg(p->table_local);
            int cnt = 0;
            QString dbStr = p->InsertTable(true);
            QVariantMap dbParam;

            while (dbQueryRemoteCreate.next())
            {
                dbParam.clear();
                for (int i = 0; i < p->column_local.size(); ++i)
                {
                    dbParam.insert(QString(":%1").arg(p->column_local[i]), dbQueryRemoteCreate.value(i));
                }
                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
                {
                    bSuccess = false;
                    break;
                }
                if (++cnt % 1000 == 0)
                {
                    qDebug().noquote() << QString("[数据下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_remote).arg(cnt);
                }
            }

            if (bSuccess)
            {
                qDebug().noquote() << QString("[数据下载] 表[%1] 完成插入数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
            }
            else
            {
                break;
            }

            qDebug().noquote() << QString("[数据下载] 表[%1] 开始更新数据。").arg(p->table_local);
            cnt = 0;
            QString dbStrDelete = p->DeleteTable() + " WHERE UID = :UID";
            QVariantMap dbParamDelete;
            while (dbQueryRemoteUpdate.next())
            {
                dbParamDelete.clear();
                dbParamDelete.insert(":UID", dbQueryRemoteUpdate.value(0));
                if (!qSqlManager->SqlExec(dbQuery, dbStrDelete, dbParamDelete))
                {
                    bSuccess = false;
                    break;
                }

                dbParam.clear();
                for (int i = 0; i < p->column_local.size(); ++i)
                {
                    dbParam.insert(QString(":%1").arg(p->column_local[i]), dbQueryRemoteUpdate.value(i));
                }
                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
                {
                    bSuccess = false;
                    break;
                }

                if (++cnt % 1000 == 0)
                {
                    qDebug().noquote() << QString("[数据下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_remote).arg(cnt);
                }
            }

            if (bSuccess)
            {
                qDebug().noquote() << QString("[数据下载] 表[%1] 完成更新数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
            }
        }
        while (0);

        if (bSuccess)
        {
            qDebug().noquote() << QString("[数据下载] 任务[%1] 更新完成，修改上一次完成时间。").arg(name) ;
            do
            {
                QString dbStr = "UPDATE PCS_Upload_Info SET RecodeLastTime = :curr WHERE UploadTable = :UploadTable ";
                QVariantMap dbParam;
                dbParam.insert(":curr", CurrTime);
                dbParam.insert(":UploadTable", p->table_local);
                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
                {
                    bSuccess = false;
                    break;
                }
            }
            while (0);
        }

        if (bSuccess)
        {
            qDebug() << "[SQL] 执行成功，提交事务：" << dbSQLLocal.commit();
        }
        else
        {
            qDebug() << "[SQL] 执行失败，回滚事务：" << dbSQLLocal.rollback();
        }

        return bSuccess;
    }
}

void WTaskDownload::Slot_stopTimer()
{
    _RunTimer.stop();
}

void WTaskDownload::Slot_resetTimer()
{
    _RunTimer.stop();

    Slot_taskWork();

    _RunTimer.setInterval(qCfgManager->_Config.value(CONFIG_RUN_TM_DNLD, DEFAULT_CONFIG_RUN_TM_DNLD).toInt() * 1000);
    _RunTimer.start();
}

void WTaskDownload::Slot_taskWork()
{
    qDebug().noquote() << QString("数据下载：任务执行开始……");

    const QString &downloadConfig = qCfgManager->_Config.value(CONFIG_SYN_DOWNSQL).toString();

    QStringList download = downloadConfig.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);

    for (auto str : download)
    {
        //taskDownloadTable(str);
    }
    taskDownloadTable("MES_db_BOM");

    qDebug().noquote() << QString("数据下载：任务执行结束。");
}


