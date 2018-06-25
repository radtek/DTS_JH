/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    WTaskWebService.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_CORE_WTASKWEBSERVICE_H
#define DTS_CORE_WTASKWEBSERVICE_H

#include <QtCore>

class MESServiceSoapService;

class WTaskWebService : public QObject
{
    Q_OBJECT

public:
    static WTaskWebService *instance();

public:
    WTaskWebService();
    virtual ~WTaskWebService();

public:
    bool initialize();
    void unInitialize();

public Q_SLOTS:
    void slotTaskWork();

private:
    QScopedPointer<MESServiceSoapService> webService;
    QScopedPointer<QThread> runThread;
};

#define qTaskWebService (WTaskWebService::instance())

#endif
