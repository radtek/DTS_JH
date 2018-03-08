/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    WTaskWebService.h
 * @brief   任务-webservice
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */
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
