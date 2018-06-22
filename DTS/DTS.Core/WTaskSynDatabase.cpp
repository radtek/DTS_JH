///*!
// * *****************************************************************************
// * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
// *
// * @file    WTaskSynDatabase.cpp
// * @brief   任务-数据库同步
// * @version 1.0
// *
// * -----------------------------------------------------------------------------
// * @history
// *  <Date>    | <Author>       | <Description>
// * 2018/03/01 | WeiHeng        | Create this file
// * *****************************************************************************
// */

//#include "WTaskSynDatabase.h"

//WTaskSynDatabase &WTaskSynDatabase::Instance()
//{
//    static WTaskSynDatabase inst;
//    return inst;
//}

//WTaskSynDatabase::WTaskSynDatabase()
//    : QThread()
//    , m_bStop(false)
//{

//}

//WTaskSynDatabase::~WTaskSynDatabase()
//{
//    UnInitialize();
//}

//GERROR WTaskSynDatabase::Initialize()
//{
//    qDebug().noquote() << "数据库同步 初始化开始……";

//    if (!this->isRunning())
//    {
//        this->start();
//    }

//    qDebug().noquote() << "数据库同步 初始化成功。";
//    return GERROR_OK;
//}

//GERROR WTaskSynDatabase::UnInitialize()
//{
//    setStop(true);

//    if (this->isRunning())
//    {
//        this->wait();
//    }

//    return GERROR_OK;
//}

//void WTaskSynDatabase::setStop(QBool bStop)
//{
//    QMutexLocker lock(&m_mutexRun);
//    m_bStop = bStop;
//}

//QBool WTaskSynDatabase::getStop()
//{
//    QMutexLocker lock(&m_mutexRun);
//    return m_bStop;
//}

//void WTaskSynDatabase::run()
//{
//    qInfo().noquote() << "数据库同步 任务线程开启";

//    setStop(false);
//    QThread::sleep(10);

//    QUInt32 nTimeCount = 0;  // set large interval could not be effective immediately
//    while (true)
//    {
//        if (getStop())
//        {
//            break;
//        }
//        if (nTimeCount % qCfgManager->_Config.value(CONFIG_RUN_TM_DNLD, DEFAULT_CONFIG_RUN_TM_DNLD).toUInt() == 0)
//        {
//            slotTaskDownloadTable();
//        }
//        if (nTimeCount % qCfgManager->_Config.value(CONFIG_RUN_TM_UPLD, DEFAULT_CONFIG_RUN_TM_UPLD).toUInt() == 0)
//        {
//            slotTaskUploadTable();
//        }
//        if (nTimeCount % qCfgManager->_Config.value(CONFIG_RUN_TM_DNLD, DEFAULT_CONFIG_RUN_TM_DNLD).toUInt() == 0)
//        {
//            slotTaskDownloadWorkOrder();
//        }

//        qDebug() << nTimeCount++;
//        QThread::sleep(1);
//    }

//    qWarning().noquote() << "数据库同步 任务线程结束";
//}

//QBool WTaskSynDatabase::taskDownloadTable(const QString &name)
//{
//    TRACE_FUNCTION();

//    qInfo().noquote() << QString("[数据下载] 任务[%1] 开始……").arg(name);

//    LPTSqlSync p = qSqlManager->_SqlSync[name];
//    if (!p)
//    {
//        qWarning().noquote() << QString("[数据下载] 任务[%1] 未找到！").arg(name);
//        return false;
//    }

//    QSqlDatabase dbSQLRemote = QSqlDatabase::database(p->db_remote, false);
//    if (!dbSQLRemote.isOpen())
//    {
//        qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库未打开！").arg(name);
//        return false;
//    }

//    QSqlDatabase dbSQLLocal = QSqlDatabase::database(p->db_local, false);
//    if (!dbSQLLocal.isOpen())
//    {
//        qWarning().noquote() << QString("[数据下载] 任务[%1] 本地数据库未打开！").arg(name);
//        return false;
//    }

//    QSqlQuery dbQuery(dbSQLLocal);

//    bool bSuccess = true;
//    QString LastTime;
//    // 获取上次更新时间
//    qInfo().noquote() << QString("[数据下载] 任务[%1] 获取上一次更新时间……").arg(name);
//    do
//    {
//        QString dbStr = "SELECT RecodeLastTime FROM PCS_Upload_Info WHERE UploadTable = :UploadTable";
//        QVariantMap dbParam;
//        dbParam.insert(":UploadTable", p->table_local);
//        if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//        {
//            bSuccess = false;
//            break;
//        }

//        if (!dbQuery.next())
//        {
//            qDebug().noquote() << QString("[数据下载] 任务[%1] 未找到上一次更新时间。设置上一次更新时间为NULL。").arg(name);

//            QString dbStr = "INSERT INTO [PCS_Upload_Info](UploadTable) VALUES(:UploadTable)";
//            QVariantMap dbParam;
//            dbParam.insert(":UploadTable", p->table_local);
//            if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//            {
//                bSuccess = false;
//                break;
//            }
//        }
//        else
//        {
//            LastTime = dbQuery.value("RecodeLastTime").toString();
//        }
//    }
//    while (0);

//    if (!bSuccess)
//    {
//        qWarning().noquote() << QString("[数据下载] 任务[%1] 本地数据库存在异常，终止任务！").arg(name);
//        return false;
//    }

//    qDebug().noquote() << QString("[数据下载] 任务[%1] 上一次更新时间为：%2").arg(name).arg(LastTime);

//    QDateTime CurrTime = QDateTime::currentDateTime();
//    if (LastTime.isEmpty())
//    {
//        // 上次更新时间为NULL
//        QSqlQuery dbQueryRemoteCreate(dbSQLRemote);
//        do
//        {
//            QString dbStr = p->SelectTable(false);
//            QVariantMap dbParam;
//            if (!qSqlManager->SqlExec(dbQueryRemoteCreate, dbStr, dbParam))
//            {
//                bSuccess = false;
//                break;
//            }
//            qDebug().noquote() << QString("[数据下载] 表[%1] 包含 %2 条新增记录。").arg(p->table_remote).arg(dbQueryRemoteCreate.numRowsAffected());
//        }
//        while (0);

//        if (!bSuccess)
//        {
//            qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库存在异常，终止任务！").arg(name);
//            return false;
//        }

//        qDebug().noquote() << QString("[数据下载] 任务[%1] 上一次更新时间为NULL，清空本地数据库。").arg(name);

//        qDebug() << "[SQL] 开启事务：" << dbSQLLocal.transaction();
//        do
//        {
//            QString dbStr = p->DeleteTable();
//            QVariantMap dbParam;

//            if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//            {
//                bSuccess = false;
//                break;
//            }

//            qDebug().noquote() << QString("[数据下载] 表[%1] 开始插入数据。").arg(p->table_local);
//            int cnt = 0;
//            dbStr = p->InsertTable(true);
//            while (dbQueryRemoteCreate.next())
//            {
//                dbParam.clear();
//                for (int i = 0; i < p->column_local.size(); ++i)
//                {
//                    dbParam.insert(":" + p->column_local[i], dbQueryRemoteCreate.value(i));
//                }

//                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//                {
//                    bSuccess = false;
//                    break;
//                }
//                if (++cnt % 1000 == 0)
//                {
//                    qDebug().noquote() << QString("[数据下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_local).arg(cnt);
//                }
//            }

//            if (bSuccess)
//            {
//                qDebug().noquote() << QString("[数据下载] 表[%1] 完成插入数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
//            }
//        }
//        while (0);

//        if (bSuccess)
//        {
//            qDebug().noquote() << QString("[数据下载] 任务[%1] 更新完成，修改上一次完成时间。").arg(name) ;
//            do
//            {
//                QString dbStr = "UPDATE PCS_Upload_Info SET RecodeLastTime = :curr WHERE UploadTable = :UploadTable ";
//                QVariantMap dbParam;
//                dbParam.insert(":curr", CurrTime);
//                dbParam.insert(":UploadTable", p->table_local);
//                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//                {
//                    bSuccess = false;
//                    break;
//                }
//            }
//            while (0);
//        }

//        if (bSuccess)
//        {
//            qInfo() << "[SQL] 执行成功，提交事务：" << dbSQLLocal.commit();
//        }
//        else
//        {
//            qInfo() << "[SQL] 执行失败，回滚事务：" << dbSQLLocal.rollback();
//        }

//        return bSuccess;
//    }
//    else
//    {
//        QSqlQuery dbQueryRemoteCreate(dbSQLRemote);
//        QSqlQuery dbQueryRemoteUpdate(dbSQLRemote);

//        QString dbStr;
//        QVariantMap dbParam;
//        dbParam.insert(":last", LastTime);
//        dbParam.insert(":curr", CurrTime);
//        do
//        {
//            dbStr = p->SelectTable(false) + " WHERE CreateTime >= :last AND CreateTime < :curr ";
//            if (!qSqlManager->SqlExec(dbQueryRemoteCreate, dbStr, dbParam))
//            {
//                bSuccess = false;
//                dbQueryRemoteCreate.clear();
//                break;
//            }

//            qDebug().noquote() << QString("[数据下载] 表[%1] 包含 %2 条新增记录。").arg(p->table_remote).arg(dbQueryRemoteCreate.numRowsAffected());
//        }
//        while (0);
//        if (!bSuccess)
//        {
//            qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库存在异常，终止任务！").arg(name);
//            return false;
//        }

//        do
//        {
//            QString dbStr = p->SelectTable(false) + " WHERE UpdateTime >= :last AND UpdateTime < :curr";

//            if (!qSqlManager->SqlExec(dbQueryRemoteUpdate, dbStr, dbParam))
//            {
//                bSuccess = false;
//                dbQueryRemoteUpdate.clear();
//                break;
//            }

//            qDebug().noquote() << QString("[数据下载] 表[%1] 包含 %2 条更新记录。").arg(p->table_remote).arg(dbQueryRemoteUpdate.numRowsAffected());
//        }
//        while (0);

//        if (!bSuccess)
//        {
//            qWarning().noquote() << QString("[数据下载] 任务[%1] 远程数据库存在异常，终止任务！").arg(name);
//            return false;
//        }

//        qDebug() << "[SQL] 开启事务：" << dbSQLLocal.transaction();
//        do
//        {
//            qDebug().noquote() << QString("[数据下载] 表[%1] 开始插入数据。").arg(p->table_local);
//            int cnt = 0;
//            QString dbStr = p->InsertTable(true);
//            QVariantMap dbParam;

//            while (dbQueryRemoteCreate.next())
//            {
//                dbParam.clear();
//                for (int i = 0; i < p->column_local.size(); ++i)
//                {
//                    dbParam.insert(QString(":%1").arg(p->column_local[i]), dbQueryRemoteCreate.value(i));
//                }
//                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//                {
//                    bSuccess = false;
//                    break;
//                }
//                if (++cnt % 1000 == 0)
//                {
//                    qDebug().noquote() << QString("[数据下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_remote).arg(cnt);
//                }
//            }

//            if (bSuccess)
//            {
//                qDebug().noquote() << QString("[数据下载] 表[%1] 完成插入数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
//            }
//            else
//            {
//                break;
//            }

//            qDebug().noquote() << QString("[数据下载] 表[%1] 开始更新数据。").arg(p->table_local);
//            cnt = 0;
//            QString dbStrDelete = p->DeleteTable() + " WHERE UID = :UID";
//            QVariantMap dbParamDelete;
//            while (dbQueryRemoteUpdate.next())
//            {
//                dbParamDelete.clear();
//                dbParamDelete.insert(":UID", dbQueryRemoteUpdate.value(0));
//                if (!qSqlManager->SqlExec(dbQuery, dbStrDelete, dbParamDelete))
//                {
//                    bSuccess = false;
//                    break;
//                }

//                dbParam.clear();
//                for (int i = 0; i < p->column_local.size(); ++i)
//                {
//                    dbParam.insert(QString(":%1").arg(p->column_local[i]), dbQueryRemoteUpdate.value(i));
//                }
//                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//                {
//                    bSuccess = false;
//                    break;
//                }

//                if (++cnt % 1000 == 0)
//                {
//                    qDebug().noquote() << QString("[数据下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_remote).arg(cnt);
//                }
//            }

//            if (bSuccess)
//            {
//                qDebug().noquote() << QString("[数据下载] 表[%1] 完成更新数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
//            }
//        }
//        while (0);

//        if (bSuccess)
//        {
//            qDebug().noquote() << QString("[数据下载] 任务[%1] 更新完成，修改上一次完成时间。").arg(name) ;
//            do
//            {
//                QString dbStr = "UPDATE PCS_Upload_Info SET RecodeLastTime = :curr WHERE UploadTable = :UploadTable ";
//                QVariantMap dbParam;
//                dbParam.insert(":curr", CurrTime);
//                dbParam.insert(":UploadTable", p->table_local);
//                if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//                {
//                    bSuccess = false;
//                    break;
//                }
//            }
//            while (0);
//        }

//        if (bSuccess)
//        {
//            qInfo() << "[SQL] 执行成功，提交事务：" << dbSQLLocal.commit();
//        }
//        else
//        {
//            qInfo() << "[SQL] 执行失败，回滚事务：" << dbSQLLocal.rollback();
//        }

//        return bSuccess;
//    }
//}

//QBool WTaskSynDatabase::taskUploadTable(const QString &name)
//{
//    TRACE_FUNCTION();

//    qInfo().noquote() << QString("[数据上传] 任务[%1] 开始……").arg(name);

//    LPTSqlSync p = qSqlManager->_SqlSync[name];
//    if (!p)
//    {
//        qWarning().noquote() << QString("[数据上传] 任务[%1] 未找到！").arg(name);
//        return false;
//    }

//    QSqlDatabase dbSQLRemote = QSqlDatabase::database(p->db_remote, false);
//    if (!dbSQLRemote.isOpen())
//    {
//        qWarning().noquote() << QString("[数据上传] 任务[%1] 远程数据库未打开！").arg(name);
//        return false;
//    }

//    QSqlDatabase dbSQLLocal = QSqlDatabase::database(p->db_local, false);
//    if (!dbSQLLocal.isOpen())
//    {
//        qWarning().noquote() << QString("[数据上传] 任务[%1] 本地数据库未打开！").arg(name);
//        return false;
//    }

//    QSqlQuery dbQuery(dbSQLLocal);
//    QSqlQuery dbQueryRemote(dbSQLRemote);

//    bool bSuccess = true;
//    QString LastTime;
//    // 获取上次更新时间
//    qDebug().noquote() << QString("[数据上传] 任务[%1] 获取上一次更新时间……").arg(name);
//    do
//    {
//        QString dbStr = "SELECT RecodeLastTime FROM PCS_Upload_Info WHERE UploadTable = :UploadTable";
//        QVariantMap dbParam;
//        dbParam.insert(":UploadTable", p->table_local);
//        if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//        {
//            bSuccess = false;
//            break;
//        }

//        if (!dbQuery.next())
//        {
//            qDebug().noquote() << QString("[数据上传] 任务[%1] 未找到上一次更新时间。设置上一次更新时间为NULL。").arg(name);

//            QString dbStr = "INSERT INTO [PCS_Upload_Info](UploadTable) VALUES(:UploadTable)";
//            QVariantMap dbParam;
//            dbParam.insert(":UploadTable", p->table_local);
//            if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//            {
//                bSuccess = false;
//                break;
//            }
//        }
//        else
//        {
//            LastTime = dbQuery.value("RecodeLastTime").toString();
//        }
//    }
//    while (0);

//    if (!bSuccess)
//    {
//        qWarning().noquote() << QString("[数据上传] 任务[%1] 本地数据库存在异常，终止任务！").arg(name);
//        return false;
//    }

//    qDebug().noquote() << QString("[数据上传] 任务[%1] 上一次更新时间为：%2").arg(name).arg(LastTime);

//    QDateTime CurrTime = QDateTime::currentDateTime();

//    do
//    {
//        QString dbStr;
//        QVariantMap dbParam;
//        if (LastTime.isEmpty())
//        {
//            // 上次更新时间为NULL
//            dbStr = p->SelectTable(true);
//        }
//        else
//        {
//            dbStr = p->SelectTable(true) + " WHERE FinishTime >= :last AND FinishTime < :curr ";
//            dbParam.insert(":last", LastTime);
//            dbParam.insert(":curr", CurrTime);
//        }
//        if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//        {
//            bSuccess = false;
//            break;
//        }
//        qDebug().noquote() << QString("[数据上传] 表[%1] 包含 %2 条新增记录。").arg(p->table_remote).arg(dbQuery.numRowsAffected());
//    }
//    while (0);

//    if (!bSuccess)
//    {
//        qWarning().noquote() << QString("[数据上传] 任务[%1] 本地数据库存在异常，终止任务！").arg(name);
//        return false;
//    }

//    do
//    {
//        int cnt = 0;
//        QString dbStr = p->InsertTableNoUID(false);
//        QVariantMap dbParam;
//        while (dbQuery.next())
//        {
//            dbParam.clear();
//            for (int i = 0; i < p->column_local.size(); ++i)
//            {
//                dbParam.insert(":" + p->column_local[i], dbQuery.value(i));
//            }

//            if (!qSqlManager->SqlExec(dbQueryRemote, dbStr, dbParam))
//            {
//                bSuccess = false;
//                break;
//            }
//            if (++cnt % 1000 == 0)
//            {
//                qDebug().noquote() << QString("[数据上传] 表[%1] 已经插入 %2 条记录。").arg(p->table_local).arg(cnt);
//            }
//        }

//        if (bSuccess)
//        {
//            qDebug().noquote() << QString("[数据上传] 表[%1] 完成插入数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);
//        }
//    }
//    while (0);

//    if (bSuccess)
//    {
//        qDebug().noquote() << QString("[数据上传] 任务[%1] 更新完成，修改上一次完成时间。").arg(name) ;
//        do
//        {
//            QString dbStr = "UPDATE PCS_Upload_Info SET RecodeLastTime = :curr WHERE UploadTable = :UploadTable ";
//            QVariantMap dbParam;
//            dbParam.insert(":curr", CurrTime);
//            dbParam.insert(":UploadTable", p->table_local);
//            if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//            {
//                bSuccess = false;
//                break;
//            }
//        }
//        while (0);
//    }

//    return bSuccess;
//}

//QBool WTaskSynDatabase::taskDownloadWorkOrder()
//{
//    qInfo().noquote() << QString("[工单下载] 任务开始……");

//    LPTSqlSync p = qSqlManager->_SqlSync["MES_WorkOrder"];
//    if (!p)
//    {
//        qWarning().noquote() << QString("[工单下载] 任务未找到！");
//        return false;
//    }

//    QSqlDatabase dbSQLRemote = QSqlDatabase::database(p->db_remote, false);
//    if (!dbSQLRemote.isOpen())
//    {
//        qWarning().noquote() << QString("[工单下载] 远程数据库未打开！");
//        return false;
//    }
//    QSqlDatabase dbSQLLocal = QSqlDatabase::database(p->db_local, false);
//    if (!dbSQLLocal.isOpen())
//    {
//        qWarning().noquote() << QString("[工单下载] 本地数据库未打开！");
//        return false;
//    }

//    bool bSuccess = true;

//    QSqlQuery dbQuery(dbSQLLocal);
//    QSqlQuery dbQueryRemote(dbSQLRemote);

//    do
//    {
//        QString dbStr = p->SelectTable(false) + " WHERE State = :State";
//        QVariantMap dbParam;
//        dbParam.insert(":State", 1);
//        if (!qSqlManager->SqlExec(dbQueryRemote, dbStr, dbParam))
//        {
//            bSuccess = false;
//            break;
//        }
//        qDebug().noquote() << QString("[工单下载] 包含 %2 条新增记录。").arg(dbQueryRemote.numRowsAffected());
//    }
//    while (0);

//    if (!bSuccess)
//    {
//        qWarning().noquote() << QString("[工单下载] 远程数据库存在异常，终止任务！");
//        return false;
//    }

//    qDebug().noquote() << QString("[工单下载] 开始插入数据。");
//    int cnt = 0;
//    QString dbStr = p->InsertTable(true);
//    QVariantMap dbParam;
//    while (dbQueryRemote.next())
//    {
//        dbParam.clear();
//        for (int i = 0; i < p->column_local.size(); ++i)
//        {
//            dbParam.insert(QString(":%1").arg(p->column_local[i]), dbQueryRemote.value(i));
//        }
//        if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
//        {
//            qWarning().noquote() << QString("[工单下载] 插入数据存在错误。%1").arg(dbQuery.lastError().text());
//        }
//        if (++cnt % 1000 == 0)
//        {
//            qDebug().noquote() << QString("[工单下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_local).arg(cnt);
//        }
//    }
//    qInfo().noquote() << QString("[工单下载] 表[%1] 完成更新数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);

//    return true;
//}

//void WTaskSynDatabase::slotTaskDownloadTable()
//{
//    QMutexLocker lock(&m_mutexWork);

//    const QString &downloadConfig = qCfgManager->_Config.value(CONFIG_SYN_DOWNSQL).toString();

//    QStringList download = downloadConfig.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);

//    for (auto str : download)
//    {
//        if (getStop())
//        {
//            break;
//        }
//        taskDownloadTable(str);
//    }
//}

//void WTaskSynDatabase::slotTaskUploadTable()
//{
//    QMutexLocker lock(&m_mutexWork);

//    const QString &uploadConfig = qCfgManager->_Config.value(CONFIG_SYN_UPLDSQL).toString();

//    QStringList upload = uploadConfig.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);

//    for (auto str : upload)
//    {
//        if (getStop())
//        {
//            break;
//        }
//        taskUploadTable(str);
//    }
//}

//void WTaskSynDatabase::slotTaskDownloadWorkOrder()
//{
//    QMutexLocker lock(&m_mutexWork);

//    taskDownloadWorkOrder();
//}
