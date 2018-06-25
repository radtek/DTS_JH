/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    WTaskSynDatabase.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_CORE_WTASKSQLSYNCHRONIZE_H
#define DTS_CORE_WTASKSQLSYNCHRONIZE_H

#include <QtCore>
#include <QtSql>

class WTaskSqlSynchronize : public QThread
{
    Q_OBJECT

public:
    static WTaskSqlSynchronize *instance();

public:
    WTaskSqlSynchronize();
    virtual ~WTaskSqlSynchronize();

public:
    bool initialize();
    bool unInitialize();
    bool setDownloadTableInit(const QString &name);
    void setStop(bool bStop);
    bool getStop();

protected:
    virtual void run() Q_DECL_OVERRIDE;

private:
    bool taskDownloadTable(const QString &name);
    bool taskUploadTable(const QString &name);
    bool taskDownloadWorkOrder();
    bool taskDownloadGasWeight();

public Q_SLOTS:
    void slotTaskDownloadTable();
    void slotTaskUploadTable();
    void slotTaskDownloadWorkOrder();
    void slotTaskDownloadGasWeight();

private:
    bool m_bStop;
    QMutex m_mutexRun;
    QMutex m_mutexWork;
};

#define qTaskSqlSynchronize (WTaskSqlSynchronize::instance())

extern QSqlDatabase DB_LOCAL;
extern QSqlDatabase DB_REMOTE;

#endif
