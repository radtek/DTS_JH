/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      WTaskDownload.h
 *  \brief
 *      Task-Download Basis Data
 */
//!============================================================================

#ifndef DTS_CORE_WTASKDOWNLOAD_H
#define DTS_CORE_WTASKDOWNLOAD_H

#include <GDtsData.h>

class WTaskDownload : public QObject
{
    Q_OBJECT
    Q_DECL_SINGLETON(WTaskDownload)

public:
    GERROR Initialize();
    GERROR UnInitialize();

private:
    QBool taskDownloadTable(const QString &name);

public Q_SLOTS:
    void Slot_stopTimer();
    void Slot_resetTimer();
    void Slot_taskWork();

private:
    QSqlDatabase    _SQL;
    QThread         _RunThread;
    QTimer          _RunTimer;
};

#endif
