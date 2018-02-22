/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      WTaskWebService.h
 *  \brief
 *      Task-Web Service
 */
//!============================================================================

#ifndef DTS_CORE_WTASKWEBSERVICE_H
#define DTS_CORE_WTASKWEBSERVICE_H

#include <GDtsData.h>

class MESServiceSoapService;

class WTaskWebService : public QObject
{
    Q_OBJECT
    Q_DECL_SINGLETON(WTaskWebService)

public:
    GERROR Initialize();
    GERROR UnInitialize();

public Q_SLOTS:
    void Slot_taskWork();

private:
    QScopedPointer<MESServiceSoapService> _WebService;
    QThread         _RunThread;
};

#endif
