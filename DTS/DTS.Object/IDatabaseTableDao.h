/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    IDatabaseTableDao.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_OBJECT_IDATABASETABLEDAO_H
#define DTS_OBJECT_IDATABASETABLEDAO_H

#include <QtCore>
#include <QtSql>

extern QSqlDatabase DB_LOCAL;

class CPCS_Upload_Info;
typedef QSharedPointer<CPCS_Upload_Info> PCS_Upload_InfoPtr;

class IDatabaseTableDao
{
public:
    static PCS_Upload_InfoPtr selectUploadInfo(const QString &code);
    static bool insertUploadInfo(const QString &code);
    static bool updateUploadInfo(const QString &code, const QDateTime &datetime);
    static bool setUploadInfoInit(const QString &code);
};

#endif
