/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      WTaskUpload.h
 *  \brief
 *      Task-Upload Product Data
 */
//!============================================================================

#ifndef DTS_CORE_WTASKUPLOAD_H
#define DTS_CORE_WTASKUPLOAD_H

#include <GDtsData.h>


class WTaskUpload : public QObject
{
    Q_OBJECT
    Q_DECL_SINGLETON(WTaskUpload)

public:
    GERROR Initialize();
    GERROR UnInitialize();

private:
    QBool taskUploadTable(const QString &name);

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
