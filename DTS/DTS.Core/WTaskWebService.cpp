/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    WTaskWebService.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "WTaskWebService.h"
#include "GCfgManager.h"
#include "soapMESServiceSoapService.h"

Q_GLOBAL_STATIC(WTaskWebService, theInstance);

WTaskWebService *WTaskWebService::instance()
{
    return theInstance();
}

WTaskWebService::WTaskWebService()
    : QObject()
    , webService(new MESServiceSoapService(SOAP_C_UTFSTRING))
    , runThread(new QThread())
{
}

WTaskWebService::~WTaskWebService()
{
    unInitialize();
}

bool WTaskWebService::initialize()
{
    qDebug().noquote() << "WebService init start...";

    if (!soap_valid_socket(webService->bind(qCfgManager->getWSLocalAddr().toUtf8().data(), qCfgManager->getWSLocalPort(), 0)))
    {
        qWarning().noquote() << "WebService bind error";
        return false;
    }

    moveToThread(runThread.data());
    runThread->start();

    QTimer::singleShot(0, this, &WTaskWebService::slotTaskWork);

    qDebug().noquote() << "WebService init success.";
    return true;
}

void WTaskWebService::unInitialize()
{
    if (runThread->isRunning())
    {
        runThread->terminate();
        runThread->wait();
    }
    qWarning().noquote() << "WebService task end.";
}

void WTaskWebService::slotTaskWork()
{
    qInfo().noquote() << "WebService task begin.";

    while (true)
    {
        if (soap_valid_socket(webService->accept()))
        {
            qInfo().noquote() << "WebService receive a request";
            int ret = webService->serve();
            if (ret != SOAP_OK)
            {
                qWarning().noquote() << QString("WebService serve failed，ERROR=%1").arg(ret);
            }
            qInfo().noquote() << "WebService handle out";
        }
    }

    qWarning().noquote() << "WebService task end.";
}
