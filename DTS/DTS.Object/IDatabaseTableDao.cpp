/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    IDatabaseTableDao.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "IDatabaseTableDao.h"
#include "CPCS_Upload_Info.h"

PCS_Upload_InfoPtr IDatabaseTableDao::selectUploadInfo(const QString &code)
{
    const QString dbStr = R"(SELECT RecodeLastTime AS LastTime,
                    RecodeUploadCount AS FailedFlag,
                    UploadTime AS FailedTime
                    FROM PCS_Upload_Info WHERE UploadTable = :UploadTable   )";

    PCS_Upload_InfoPtr ptr;
    do
    {
        QSqlQuery dbQuery(DB_LOCAL);
        dbQuery.prepare(dbStr);
        dbQuery.bindValue(":UploadTable", code);
        if (!dbQuery.exec())
        {
            qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
            break;
        }
        if (dbQuery.first())
        {
            ptr.reset(new CPCS_Upload_Info);
            Q_FOREACH (auto const &it, CPCS_Upload_Info::TAG_COLUMNS.keys())
            {
                ptr->setData(it, dbQuery.value(it));
            }
            qDebug() << ptr;
        }
    } while (false);

    return ptr;
}

bool IDatabaseTableDao::insertUploadInfo(const QString &code)
{
    const QString dbStr = R"(INSERT INTO [PCS_Upload_Info](UploadTable)
                          VALUES(:UploadTable)     )";

    bool bSuccess = true;
    do
    {
        QSqlQuery dbQuery(DB_LOCAL);
        dbQuery.prepare(dbStr);
        dbQuery.bindValue(":UploadTable", code);
        if (!dbQuery.exec())
        {
            qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
            bSuccess = false;
            break;
        }
    } while (false);

    return bSuccess;
}

bool IDatabaseTableDao::updateUploadInfo(const QString &code, const QDateTime &datetime)
{
    const QString dbStr = R"(UPDATE PCS_Upload_Info SET
                         RecodeLastTime = :curr
                         WHERE UploadTable = :UploadTable )";

    bool bSuccess = true;
    do
    {
        QSqlQuery dbQuery(DB_LOCAL);
        dbQuery.prepare(dbStr);
        dbQuery.bindValue(":curr", datetime);
        dbQuery.bindValue(":UploadTable", code);
        if (!dbQuery.exec())
        {
            qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
            bSuccess = false;
            break;
        }
    } while (false);

    return bSuccess;
}

bool IDatabaseTableDao::setUploadInfoInit(const QString &code)
{
    const QString dbStr = R"(UPDATE PCS_Upload_Info SET
                         RecodeLastTime = NULL
                         WHERE UploadTable = :UploadTable )";

    bool bSuccess = true;
    do
    {
        QSqlQuery dbQuery(DB_LOCAL);
        dbQuery.prepare(dbStr);
        dbQuery.bindValue(":UploadTable", code);
        if (!dbQuery.exec())
        {
            qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
            bSuccess = false;
            break;
        }
    } while (false);

    return bSuccess;
}
