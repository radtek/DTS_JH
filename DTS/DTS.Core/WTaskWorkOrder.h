/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      WTaskUpload.h
 *  \brief
 *      Task-Download WorkOrder
 */
//!============================================================================

#ifndef DTS_CORE_WTASKWORKORDER_H
#define DTS_CORE_WTASKWORKORDER_H

#include <GDtsData.h>

class WTaskWorkOrder : public QObject
{
    Q_OBJECT
    Q_DECL_SINGLETON(WTaskWorkOrder)

public:
    GERROR Initialize();
    GERROR UnInitialize();

private:
    QBool taskDownloadWorkOrder();

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
