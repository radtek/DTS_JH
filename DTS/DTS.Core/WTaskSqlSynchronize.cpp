/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    WTaskSqlSynchronize.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "WTaskSqlSynchronize.h"
#include "CPCS_Upload_Info.h"
#include "CSynchData.h"
#include "GCfgManager.h"
#include "IDatabaseTableDao.h"
#include "XYLogManager.h"

Q_GLOBAL_STATIC(WTaskSqlSynchronize, theInstance);

WTaskSqlSynchronize *WTaskSqlSynchronize::instance()
{
    return theInstance();
}

WTaskSqlSynchronize::WTaskSqlSynchronize()
    : QThread()
    , m_bStop(true)
{
}

WTaskSqlSynchronize::~WTaskSqlSynchronize()
{
    unInitialize();
}

bool WTaskSqlSynchronize::initialize()
{
    qDebug().noquote() << "WTaskSqlSynchronize initialize ...";

    if (!this->isRunning())
    {
        this->start();
    }

    qDebug().noquote() << "WTaskSqlSynchronize initialize success.";
    return true;
}

bool WTaskSqlSynchronize::unInitialize()
{
    setStop(true);

    if (this->isRunning())
    {
        this->wait();
    }

    return true;
}

bool WTaskSqlSynchronize::setDownloadTableInit(const QString &name)
{
    return IDatabaseTableDao::setUploadInfoInit(name);
}

void WTaskSqlSynchronize::setStop(bool bStop)
{
    QMutexLocker lock(&m_mutexRun);
    m_bStop = bStop;
}

bool WTaskSqlSynchronize::getStop()
{
    QMutexLocker lock(&m_mutexRun);
    return m_bStop;
}

void WTaskSqlSynchronize::run()
{
    qInfo().noquote() << "WTaskSqlSynchronize begin.";

    setStop(false);
    QThread::sleep(10);

    int nTimeCount = 0;    // set large interval could not be effective immediately
    while (true)
    {
        if (getStop())
        {
            break;
        }

        qDebug() << nTimeCount;

        if (nTimeCount % qCfgManager->getDownloadTimeSpan() == 0)
        {
            slotTaskDownloadTable();
        }
        if (nTimeCount % qCfgManager->getUploadTimeSpan() == 0)
        {
            slotTaskUploadTable();
        }

        nTimeCount++;

        for (int i = 0; i < 60; ++i)
        {
            if (getStop())
            {
                break;
            }
            QThread::sleep(1);
        }
    }

    qWarning().noquote() << "WTaskSqlSynchronize end.";
}

bool WTaskSqlSynchronize::taskDownloadTable(const QString &name)
{
    TRACE_FUNCTION();

    qInfo().noquote() << QString("[MES-DOWNLOAD] Task[%1] start ...").arg(name);

    CSynchDataPtr ptr = qCfgManager->getSynchData(name);
    if (ptr.isNull())
    {
        qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] configuration not found").arg(name);
        return false;
    }

    if (!DB_REMOTE.isOpen())
    {
        qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] remote database not open!").arg(name);
        return false;
    }

    if (!DB_LOCAL.isOpen())
    {
        qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] local database not open!").arg(name);
        return false;
    }

    bool bSuccess = true;
    QString LastTime;
    int FailedFlag;
    QString FailedTime;

    // get last synchronize time
    qInfo().noquote() << QString("[MES-DOWNLOAD] Task[%1] get last finish time...").arg(name);
    do
    {
        PCS_Upload_InfoPtr infoUpdate = IDatabaseTableDao::selectUploadInfo(ptr->synchTable);
        if (infoUpdate.isNull())
        {
            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] last finish time not found. So set NULL.").arg(name);

            if (!IDatabaseTableDao::insertUploadInfo(ptr->synchTable))
            {
                bSuccess = false;
                break;
            }
        }
        else
        {
            LastTime = infoUpdate->getData(CPCS_Upload_Info::RecodeLastTime).toString();
            FailedFlag = infoUpdate->getData(CPCS_Upload_Info::RecodeUploadCount).toInt();
            FailedTime = infoUpdate->getData(CPCS_Upload_Info::UploadTime).toString();
        }
    } while (false);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] local database error, end task!").arg(name);
        return false;
    }

    qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] last finish time: %2").arg(name).arg(LastTime);

    QDateTime CurrTime = QDateTime::currentDateTime();
    if (LastTime.isEmpty())
    {
        QSqlQuery dbQueryRemoteCreate(DB_REMOTE);
        do
        {
            const QString dbStr = ptr->Select();
            dbQueryRemoteCreate.prepare(dbStr);
            if (!dbQueryRemoteCreate.exec())
            {
                qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQueryRemoteCreate.lastError().text());
                bSuccess = false;
                break;
            }
            if (dbQueryRemoteCreate.numRowsAffected())
            {
                qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] contains %2 records to insert.").arg(ptr->synchTable).arg(dbQueryRemoteCreate.numRowsAffected());
            }
            else
            {
                qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] contains 0 record to insert.").arg(ptr->synchTable);
            }
        } while (false);

        if (!bSuccess)
        {
            qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] remote database error, end task!").arg(name);
            return false;
        }

        qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] last finish time is NULL, so empty database!").arg(name);

        qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] transaction:").arg(name) << DB_LOCAL.transaction();
        do
        {
            QSqlQuery dbQuery(DB_LOCAL);

            if (bSuccess)
            {
                const QString dbStr = ptr->Delete();

                dbQuery.prepare(dbStr);
                if (!dbQuery.exec())
                {
                    qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
                    bSuccess = false;
                    break;
                }
            }

            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] begin to insert...").arg(ptr->synchTable);
            if (bSuccess)
            {
                int cnt = 0;
                const QString dbStr = ptr->Insert();

                dbQuery.prepare(dbStr);
                while (dbQueryRemoteCreate.next())
                {
                    for (int i = 0; i < ptr->synchColumns.size(); ++i)
                    {
                        dbQuery.bindValue(":" % ptr->synchColumns[i], dbQueryRemoteCreate.value(i));
                    }

                    if (!dbQuery.exec())
                    {
                        qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
                        bSuccess = false;
                        break;
                    }
                    if (++cnt % 1000 == 0)
                    {
                        qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] already insert %2 records.").arg(ptr->synchTable).arg(cnt);
                    }
                }

                if (bSuccess)
                {
                    qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] finish insert, total %2 records.").arg(ptr->synchTable).arg(cnt);
                }
            }
        } while (false);

        if (bSuccess)
        {
            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] download finish, so modify last finish time.").arg(name);

            bSuccess = IDatabaseTableDao::updateUploadInfo(ptr->synchTable, CurrTime);
        }

        if (bSuccess)
        {
            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] commit:").arg(name) << DB_LOCAL.commit();
        }
        else
        {
            qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] rollback:").arg(name) << DB_LOCAL.rollback();
        }

        return bSuccess;
    }
    else
    {
        QSqlQuery dbQueryRemoteCreate(DB_REMOTE);
        QSqlQuery dbQueryRemoteUpdate(DB_REMOTE);

        do
        {
            const QString dbStr = ptr->Select() + " WHERE CreateTime >= :last AND CreateTime < :curr ";

            dbQueryRemoteCreate.prepare(dbStr);
            dbQueryRemoteCreate.bindValue(":last", LastTime);
            dbQueryRemoteCreate.bindValue(":curr", CurrTime);
            if (!dbQueryRemoteCreate.exec())
            {
                qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQueryRemoteCreate.lastError().text());

                bSuccess = false;
                dbQueryRemoteCreate.clear();
                break;
            }

            if (dbQueryRemoteCreate.numRowsAffected())
            {
                qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] contains %2 records to insert.").arg(ptr->synchTable).arg(dbQueryRemoteCreate.numRowsAffected());
            }
            else
            {
                qInfo().noquote() << QString("[MES-DOWNLOAD] Task[%1] contains 0 record to insert.").arg(ptr->synchTable);
            }
        } while (false);

        if (!bSuccess)
        {
            qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] Task[%1] remote database error, end task!").arg(name);
            return false;
        }

        do
        {
            const QString dbStr = ptr->Select() + " WHERE UpdateTime >= :last AND UpdateTime < :curr";

            dbQueryRemoteUpdate.prepare(dbStr);
            dbQueryRemoteUpdate.bindValue(":last", LastTime);
            dbQueryRemoteUpdate.bindValue(":curr", CurrTime);
            if (!dbQueryRemoteUpdate.exec())
            {
                qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQueryRemoteUpdate.lastError().text());

                bSuccess = false;
                dbQueryRemoteUpdate.clear();
                break;
            }

            if (dbQueryRemoteUpdate.numRowsAffected())
            {
                qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] contains %2 records to update.").arg(ptr->synchTable).arg(dbQueryRemoteUpdate.numRowsAffected());
            }
            else
            {
                qInfo().noquote() << QString("[MES-DOWNLOAD] Task[%1] contains 0 record to update.").arg(ptr->synchTable);
            }
        } while (false);

        if (!bSuccess)
        {
            qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] remote database error, end task!").arg(name);
            return false;
        }

        qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] transaction:").arg(name) << DB_LOCAL.transaction();
        do
        {
            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] begin to insert...").arg(ptr->synchTable);

            QSqlQuery dbQuery(DB_LOCAL);

            if (bSuccess)
            {
                int cnt = 0;
                const QString dbStr = ptr->Insert();

                dbQuery.prepare(dbStr);
                while (dbQueryRemoteCreate.next())
                {
                    for (int i = 0; i < ptr->synchColumns.size(); ++i)
                    {
                        dbQuery.bindValue(":" % ptr->synchColumns[i], dbQueryRemoteCreate.value(i));
                    }
                    if (!dbQuery.exec())
                    {
                        qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());

                        bSuccess = false;
                        break;
                    }
                    if (++cnt % 1000 == 0)
                    {
                        qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] already insert %2 records.").arg(ptr->synchTable).arg(cnt);
                    }
                }

                if (bSuccess)
                {
                    qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] finish insert, total %2 records.").arg(ptr->synchTable).arg(cnt);
                }
            }

            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] begin to update").arg(ptr->synchTable);
            if (bSuccess)
            {
                int cnt = 0;
                const QString dbStrDelete = ptr->Update();

                dbQuery.prepare(dbStrDelete);
                while (dbQueryRemoteUpdate.next())
                {
                    for (int i = 0; i < ptr->synchColumns.size(); ++i)
                    {
                        dbQuery.bindValue(":" % ptr->synchColumns[i], dbQueryRemoteUpdate.value(i));
                    }
                    if (!dbQuery.exec())
                    {
                        qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
                        bSuccess = false;
                        break;
                    }

                    if (++cnt % 1000 == 0)
                    {
                        qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] already update %2 records.").arg(ptr->synchTable).arg(cnt);
                    }
                }
                if (bSuccess)
                {
                    qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] finish update, total %2 records.").arg(ptr->synchTable).arg(cnt);
                }
            }
        } while (false);

        if (bSuccess)
        {
            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] download finish, so modify last finish time.").arg(name);

            bSuccess = IDatabaseTableDao::updateUploadInfo(ptr->synchTable, CurrTime);
        }

        if (bSuccess)
        {
            qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] commit:").arg(name) << DB_LOCAL.commit();
        }
        else
        {
            qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] rollback:").arg(name) << DB_LOCAL.rollback();
        }

        return bSuccess;
    }
}

bool WTaskSqlSynchronize::taskUploadTable(const QString &name)
{
    TRACE_FUNCTION();

    qInfo().noquote() << QString("[MES-UPLOAD] Task[%1] begin...").arg(name);

    CSynchDataPtr ptr = qCfgManager->getSynchData(name);
    if (!ptr)
    {
        qWarning().noquote() << QString("[MES-UPLOAD] Task[%1] not found!").arg(name);
        return false;
    }

    if (!DB_REMOTE.isOpen())
    {
        qWarning().noquote() << QString("[MES-UPLOAD] Task[%1] remote database not open!").arg(name);
        return false;
    }

    if (!DB_LOCAL.isOpen())
    {
        qWarning().noquote() << QString("[MES-UPLOAD] Task[%1] local database not open!").arg(name);
        return false;
    }

    QSqlQuery dbQuery(DB_LOCAL);
    QSqlQuery dbQueryRemote(DB_REMOTE);

    bool bSuccess = true;
    QString LastTime;

    // get last synchronize time
    qDebug().noquote() << QString("[MES-UPLOAD] Task[%1] get last finish time...").arg(name);
    do
    {
        PCS_Upload_InfoPtr infoUpdate = IDatabaseTableDao::selectUploadInfo(ptr->synchTable);
        if (infoUpdate.isNull())
        {
            qDebug().noquote() << QString("[MES-UPLOAD] Task[%1] last finish time not found. So set NULL.").arg(name);

            if (!IDatabaseTableDao::insertUploadInfo(ptr->synchTable))
            {
                bSuccess = false;
                break;
            }
        }
        else
        {
            LastTime = infoUpdate->getData(CPCS_Upload_Info::RecodeLastTime).toString();
        }
    } while (false);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[MES-UPLOAD] Task[%1] local database error, end task!").arg(name);
        return false;
    }

    qDebug().noquote() << QString("[MES-UPLOAD] Task[%1] last finish time: %2").arg(name).arg(LastTime);

    QDateTime CurrTime = QDateTime::currentDateTime();
    do
    {
        if (LastTime.isEmpty())
        {
            const QString dbStr = ptr->Select() + " WHERE UploadTime < :curr ";

            dbQuery.prepare(dbStr);
        }
        else
        {
            const QString dbStr = ptr->Select() + " WHERE UploadTime >= :last AND UploadTime < :curr ";

            dbQuery.prepare(dbStr);
            dbQuery.bindValue(":last", LastTime);
            dbQuery.bindValue(":curr", CurrTime);
        }
        if (!dbQuery.exec())
        {
            qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
            bSuccess = false;
            break;
        }
        if (dbQuery.numRowsAffected())
        {
            qWarning().noquote() << QString("[MES-UPLOAD] Task[%1] contains %2 records to insert.").arg(ptr->synchTable).arg(dbQuery.numRowsAffected());
        }
        else
        {
            qInfo().noquote() << QString("[MES-UPLOAD] Task[%1] contains 0 record to insert.").arg(ptr->synchTable);
        }
    } while (false);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[MES-UPLOAD] Task[%1] local database error, end task！").arg(name);
        return false;
    }

    qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] transaction:").arg(name) << DB_REMOTE.transaction();
    do
    {
        int cnt = 0;
        const QString dbStr = ptr->InsertWithoutUID();

        dbQuery.prepare(dbStr);
        while (dbQuery.next())
        {
            for (int i = 0; i < ptr->synchColumns.size(); ++i)
            {
                dbQuery.bindValue(":" % ptr->synchColumns[i], dbQuery.value(i));
            }

            if (!dbQuery.exec())
            {
                qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
                bSuccess = false;
                break;
            }
            if (++cnt % 1000 == 0)
            {
                qDebug().noquote() << QString("[MES-UPLOAD] Task[%1] already insert %2 records.").arg(ptr->synchTable).arg(cnt);
            }
        }

        if (bSuccess)
        {
            qDebug().noquote() << QString("[MES-UPLOAD] Task[%1] finish insert, total %2 records.").arg(ptr->synchTable).arg(cnt);
        }
    } while (false);

    if (bSuccess)
    {
        qDebug().noquote() << QString("[MES-UPLOAD] Task[%1] upload finish, so modify last finish time.").arg(name);

        bSuccess = IDatabaseTableDao::updateUploadInfo(ptr->synchTable, CurrTime);
    }

    if (bSuccess)
    {
        qDebug().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] commit:").arg(name) << DB_REMOTE.commit();
    }
    else
    {
        qWarning().noquote() << QString("[MES-DOWNLOAD] Task[%1] [SQL] rollback:").arg(name) << DB_REMOTE.rollback();
    }

    return bSuccess;
}

bool WTaskSqlSynchronize::taskDownloadWorkOrder()
{
    qInfo().noquote() << QString("[MES-ORDER] Task begin...");

    CSynchDataPtr ptr = qCfgManager->getSynchData("MES_WorkOrder");
    if (ptr.isNull())
    {
        qWarning().noquote() << QString("[MES-ORDER] Task not found!");
        return false;
    }

    if (!DB_REMOTE.isOpen())
    {
        qWarning().noquote() << QString("[MES-ORDER] Task remote database not open!");
        return false;
    }

    if (!DB_LOCAL.isOpen())
    {
        qWarning().noquote() << QString("[MES-ORDER] Task local database not open!");
        return false;
    }

    bool bSuccess = true;

    QSqlQuery dbQuery(DB_LOCAL);
    QSqlQuery dbQueryRemote(DB_REMOTE);

    do
    {
        const QString dbStr = ptr->Select() + " WHERE State = :State";
        dbQueryRemote.prepare(dbStr);

        dbQueryRemote.bindValue(":State", 1);
        if (!dbQueryRemote.exec())
        {
            qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());

            bSuccess = false;
            break;
        }
        if (dbQueryRemote.numRowsAffected())
        {
            qWarning().noquote() << QString("[MES-ORDER] Task contains %1 records to insert.").arg(dbQueryRemote.numRowsAffected());
        }
        else
        {
            qDebug().noquote() << QString("[MES-ORDER] Task contains 0 record to insert.");
        }
    } while (0);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[MES-ORDER] Task remote database error, end task!");
        return false;
    }

    qDebug().noquote() << QString("[MES-ORDER] Task begin to insert...");
    do
    {
        int cnt = 0;
        const QString dbStr = ptr->Insert();

        dbQuery.prepare(dbStr);
        while (dbQueryRemote.next())
        {
            for (int i = 0; i < ptr->synchColumns.size(); ++i)
            {
                dbQuery.bindValue(QString(":%1").arg(ptr->synchColumns[i]), dbQueryRemote.value(i));
            }
            if (!dbQuery.exec())
            {
                qWarning().noquote() << QString("[MES-ORDER] Task insert error: %1").arg(dbQuery.lastError().text());
                qInfo().noquote() << QString("[MES-ORDER] Task duplicate workorder[%1]").arg(dbQueryRemote.value("UID").toString());
            }
            if (++cnt % 1000 == 0)
            {
                qDebug().noquote() << QString("[MES-ORDER] Task already insert %1 records.").arg(cnt);
            }
        }
        qInfo().noquote() << QString("[MES-ORDER] Task finish insert, total %1 records.").arg(cnt);
    } while (false);

    return true;
}

bool WTaskSqlSynchronize::taskDownloadGasWeight()
{
    qInfo().noquote() << QString("[MES-WEIGHT] Task begin...");

    CSynchDataPtr ptr = qCfgManager->getSynchData("MES_Process_RefData");
    if (ptr.isNull())
    {
        qWarning().noquote() << QString("[MES-WEIGHT] Task not found!");
        return false;
    }

    if (!DB_REMOTE.isOpen())
    {
        qWarning().noquote() << QString("[MES-WEIGHT] Task remote database not open!");
        return false;
    }

    if (!DB_LOCAL.isOpen())
    {
        qWarning().noquote() << QString("[MES-WEIGHT] Task local database not open!");
        return false;
    }

    QSqlQuery dbQuery(DB_LOCAL);
    QSqlQuery dbQueryRemote(DB_REMOTE);
    bool bSuccess = true;

    QString LastTime;

    qDebug().noquote() << QString("[MES-WEIGHT] Task get last finish time...");
    do
    {
        PCS_Upload_InfoPtr infoUpdate = IDatabaseTableDao::selectUploadInfo(ptr->synchTable);
        if (infoUpdate.isNull())
        {
            qDebug().noquote() << QString("[MES-DOWNLOAD] Task last finish time not found. So set NULL.");

            if (!IDatabaseTableDao::insertUploadInfo(ptr->synchTable))
            {
                bSuccess = false;
                break;
            }
        }
        else
        {
            LastTime = infoUpdate->getData(CPCS_Upload_Info::RecodeLastTime).toString();
        }
    } while (false);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[MES-WEIGHT] Task local database error, end task！");
        return false;
    }

    qInfo().noquote() << QString("[MES-WEIGHT] Task last finish time: %1").arg(LastTime);

    QDateTime CurrTime = QDateTime::currentDateTime();
    if (LastTime.isEmpty())
    {
        qWarning().noquote() << QString("[MES-WEIGHT] Task last finish time is NULL, so empty database!");

        do
        {
            const QString dbStr = ptr->Truncate();

            dbQuery.prepare(dbStr);
            if (!dbQuery.exec())
            {
                qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
                bSuccess = false;
                break;
            }
        } while (false);

        if (!bSuccess)
        {
            qWarning().noquote() << QString("[MES-WEIGHT] Task local database error, end task！");
            return false;
        }
    }

    QDateTime dtLastTime = QDateTime::fromString(LastTime, "yyyy-MM-ddThh:mm:ss");
    if (CurrTime.addDays(-qCfgManager->getGasWeightInitTime()) > dtLastTime)
    {
        dtLastTime = CurrTime.addDays(-qCfgManager->getGasWeightInitTime());
        qWarning().noquote() << QString("[MES-WEIGHT] Task last time is %1 days early, so modify it[%2].").arg(qCfgManager->getGasWeightInitTime()).arg(dtLastTime.toString("yyyy-MM-ddThh:mm:ss"));
    }

    do
    {
        const QString dbStr = ptr->Select() + " WHERE ImportTime >= :last AND ImportTime < :curr";

        dbQueryRemote.prepare(dbStr);
        dbQueryRemote.bindValue(":curr", CurrTime);
        dbQueryRemote.bindValue(":last", dtLastTime);
        if (!dbQueryRemote.exec())
        {
            qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQueryRemote.lastError().text());

            bSuccess = false;
            break;
        }
        if (dbQueryRemote.numRowsAffected())
        {
            qWarning().noquote() << QString("[MES-WEIGHT] Task contains %1 records to insert.").arg(dbQueryRemote.numRowsAffected());
        }
        else
        {
            qDebug().noquote() << QString("[MES-WEIGHT] Task contains 0 record to insert.");
        }
    } while (false);

    qDebug().noquote() << QString("[MES-WEIGHT] Task begin to insert...");

    qDebug().noquote() << QString("[MES-DOWNLOAD] Task [SQL] transaction:") << DB_LOCAL.transaction();
    do
    {
        int cnt = 0;
        const QString dbStr = ptr->InsertWithoutUID();

        dbQuery.prepare(dbStr);
        while (dbQueryRemote.next())
        {
            for (int i = 0; i < ptr->synchColumns.size(); ++i)
            {
                dbQuery.bindValue(":" + ptr->synchColumns[i], dbQueryRemote.value(i));
            }

            if (!dbQuery.exec())
            {
                qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
                break;
            }
            if (++cnt % 1000 == 0)
            {
                qDebug().noquote() << QString("[MES-WEIGHT] Task already insert %1 records.").arg(cnt);
            }
        }

        if (bSuccess)
        {
            qDebug().noquote() << QString("[MES-WEIGHT] Task finish insert, total %1 records.").arg(cnt);
        }
    } while (false);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[MES-WEIGHT] Task remote database error, end task!");
        return false;
    }

    if (bSuccess)
    {
        qDebug().noquote() << QString("[MES-WEIGHT] Task download finish, so modify last finish time.");

        bSuccess = IDatabaseTableDao::updateUploadInfo(ptr->synchTable, CurrTime);
    }

    if (bSuccess)
    {
        qDebug().noquote() << QString("[MES-WEIGHT] Task [SQL] commit:") << DB_LOCAL.commit();
    }
    else
    {
        qWarning().noquote() << QString("[MES-WEIGHT] Task [SQL] rollback:") << DB_LOCAL.rollback();
    }

    return bSuccess;
}

void WTaskSqlSynchronize::slotTaskDownloadTable()
{
    QMutexLocker lock(&m_mutexWork);

    QStringList download = qCfgManager->getDownload();

    Q_FOREACH (auto &str, download)
    {
        if (str == TABLE_WORKORDER)
        {
            taskDownloadWorkOrder();
        }
        else if (str == TABLE_GASDATA)
        {
            taskDownloadGasWeight();
        }
        else
        {
            taskDownloadTable(str);
        }
    }
}

void WTaskSqlSynchronize::slotTaskUploadTable()
{
    QMutexLocker lock(&m_mutexWork);

    QStringList upload = qCfgManager->getUpload();

    for (auto str : upload)
    {
        taskUploadTable(str);
    }
}

void WTaskSqlSynchronize::slotTaskDownloadWorkOrder()
{
    QMutexLocker lock(&m_mutexWork);

    taskDownloadWorkOrder();
}

void WTaskSqlSynchronize::slotTaskDownloadGasWeight()
{
    QMutexLocker lock(&m_mutexWork);

    taskDownloadGasWeight();
}
