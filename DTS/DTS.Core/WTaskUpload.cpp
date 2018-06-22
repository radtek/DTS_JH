/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      WTaskUpload.cpp
 *  \brief
 *      Task-Upload Product Data
 *  \since
 *  001     2017/11/01      weiheng     create this file
 */
//!============================================================================

#include "WTaskUpload.h"

WTaskUpload &WTaskUpload::Instance()
{
    static WTaskUpload inst;
    return inst;
}

WTaskUpload::WTaskUpload()
    : QObject()
    , _SQL()
    , _RunThread()
    , _RunTimer()
{
}

WTaskUpload::~WTaskUpload()
{
    UnInitialize();
}

GERROR WTaskUpload::Initialize()
{
    qDebug().noquote() << "数据上传：任务初始化开始……";

    _SQL = QSqlDatabase::database(QSQL_DB_STRING_DNLD);

    if (!_SQL.isOpen())
    {
        if (!_SQL.open())
        {
            qWarning().noquote() << QString("数据上传：数据库连接失败，STR=%1").arg(QSQL_DB_STRING_UPLD);
            return GERROR_SQL_OPENFAILED;
        }
    }

    (*this).moveToThread(&_RunThread);
    _RunTimer.moveToThread(&_RunThread);
    QObject::connect(&_RunTimer, &QTimer::timeout, this, &WTaskUpload::Slot_taskWork);
    _RunThread.start();

    QTimer::singleShot(qCfgManager->_Config.value(CONFIG_RUN_TM_FDND, DEFAULT_CONFIG_RUN_TM_FDND).toInt() * 1000, this, &WTaskUpload::Slot_resetTimer);

    qDebug().noquote() << "数据上传：任务初始化成功。";
    return GERROR_OK;
}

GERROR WTaskUpload::UnInitialize()
{
    if (_RunThread.isRunning())
    {
        _RunThread.quit();
    }

    return GERROR_OK;
}

QBool WTaskUpload::taskUploadTable(const QString &name)
{
    TRACE_FUNCTION();

    qDebug().noquote() << QString("[数据上传] 任务[%1] 开始……").arg(name);

    LPTSqlSync p = qSqlManager->_SqlSync[name];
    if (!p)
    {
        qWarning().noquote() << QString("[数据上传] 任务[%1] 未找到！").arg(name);
        return false;
    }

    QSqlDatabase dbSQLRemote = QSqlDatabase::database(p->db_remote, false);
    if (!dbSQLRemote.isOpen())
    {
        qWarning().noquote() << QString("[数据上传] 任务[%1] 远程数据库未打开！").arg(name);
        return false;
    }

    QSqlDatabase dbSQLLocal = QSqlDatabase::database(p->db_local, false);
    if (!dbSQLLocal.isOpen())
    {
        qWarning().noquote() << QString("[数据上传] 任务[%1] 本地数据库未打开！").arg(name);
        return false;
    }

    QSqlQuery dbQuery(dbSQLLocal);
    QSqlQuery dbQueryRemote(dbSQLRemote);

    bool bSuccess = true;
    QString LastTime;
    // 获取上次更新时间
    qDebug().noquote() << QString("[数据上传] 任务[%1] 获取上一次更新时间……").arg(name);
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
            qDebug().noquote() << QString("[数据上传] 任务[%1] 未找到上一次更新时间。设置上一次更新时间为NULL。").arg(name);

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
        qWarning().noquote() << QString("[数据上传] 任务[%1] 本地数据库存在异常，终止任务！").arg(name);
        return false;
    }

    qDebug().noquote() << QString("[数据上传] 任务[%1] 上一次更新时间为：%2").arg(name).arg(LastTime);

    QDateTime CurrTime = QDateTime::currentDateTime();

    do
    {
        QString dbStr;
        QVariantMap dbParam;
        if (LastTime.isEmpty())
        {
            // 上次更新时间为NULL
            dbStr = p->SelectTable(true);
        }
        else
        {
            dbStr = p->SelectTable(true) + " WHERE FinishTime >= :last AND FinishTime < :curr ";
            dbParam.insert(":last", LastTime);
            dbParam.insert(":curr", CurrTime);
        }
        if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
        {
            bSuccess = false;
            break;
        }
        qDebug().noquote() << QString("[数据上传] 表[%1] 包含 %2 条新增记录。").arg(p->table_remote).arg(dbQuery.numRowsAffected());
    }
    while (0);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[数据上传] 任务[%1] 本地数据库存在异常，终止任务！").arg(name);
        return false;
    }

    do
    {
        int cnt = 0;
        QString dbStr = p->InsertTableNoUID(false);
        QVariantMap dbParam;
        while (dbQuery.next())
        {
            dbParam.clear();
            for (int i = 0; i < p->column_local.size(); ++i)
            {
                dbParam.insert(":" + p->column_local[i], dbQuery.value(i));
            }

            if (!qSqlManager->SqlExec(dbQueryRemote, dbStr, dbParam))
            {
                bSuccess = false;
                break;
            }
            if (++cnt % 1000 == 0)
            {
                qDebug().noquote() << QString("[数据上传] 表[%1] 已经插入 %2 条记录。").arg(p->table_local).arg(cnt);
            }
        }

        if (bSuccess)
        {
            qDebug().noquote() << QString("[数据上传] 表[%1] 完成插入数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
        }
    }
    while (0);

    if (bSuccess)
    {
        qDebug().noquote() << QString("[数据上传] 任务[%1] 更新完成，修改上一次完成时间。").arg(name) ;
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

    return bSuccess;
}

void WTaskUpload::Slot_stopTimer()
{
    _RunTimer.stop();
}

void WTaskUpload::Slot_resetTimer()
{
    Slot_taskWork();
    _RunTimer.setInterval(qCfgManager->_Config.value(CONFIG_RUN_TM_DNLD, DEFAULT_CONFIG_RUN_TM_DNLD).toInt() * 1000);
    _RunTimer.start();
}

void WTaskUpload::Slot_taskWork()
{
    qDebug().noquote() << QString("数据上传：任务执行开始……");

    const QString &uploadConfig = qCfgManager->_Config.value(CONFIG_SYN_UPLDSQL).toString();

    QStringList upload = uploadConfig.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);

    for (auto str : upload)
    {
        taskUploadTable(str);
    }

    qDebug().noquote() << QString("数据上传：任务执行结束。");
}
