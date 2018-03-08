/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    WTaskWebService.cpp
 * @brief   任务-webservice
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "WTaskWebService.h"
#include <soapMESServiceSoapService.h>

WTaskWebService &WTaskWebService::Instance()
{
    static WTaskWebService inst;
    return inst;
}

WTaskWebService::WTaskWebService()
    : QObject()
    , _WebService(new MESServiceSoapService(SOAP_C_UTFSTRING))
    , _RunThread()
{

}

WTaskWebService::~WTaskWebService()
{
    UnInitialize();
}

GERROR WTaskWebService::Initialize()
{
    qDebug().noquote() << "WebService 任务初始化开始……";

    if (SOAP_INVALID_SOCKET == _WebService->bind(qCfgManager->_Config[CONFIG_SYS_LWSADDR].toString().toStdString().c_str(),
                                                 qCfgManager->_Config[CONFIG_SYS_LWSPORT].toInt(), 0))
    {
        qWarning().noquote() << "WebService 绑定地址错误!";
        return GERROR_TASK_BINDPORT;
    }

    (*this).moveToThread(&_RunThread);
    _RunThread.start();

    QTimer::singleShot(0, this, &WTaskWebService::Slot_taskWork);

    qDebug().noquote() << "WebService 任务初始化成功。";
    return GERROR_OK;
}

GERROR WTaskWebService::UnInitialize()
{
    if (_RunThread.isRunning())
    {
        _RunThread.terminate();
        _RunThread.wait();
    }

    return GERROR_OK;
}

void WTaskWebService::Slot_taskWork()
{
    qInfo().noquote() << "WebService 任务线程开启";

    while (true)
    {
        if (SOAP_INVALID_SOCKET != _WebService->accept())
        {
            qInfo().noquote() << "WebService 接收到一个请求";
            QInt32 ret = _WebService->serve();
            if (ret != SOAP_OK)
            {
                qWarning().noquote() << QString("WebService 产生了一个错误，ERROR=%1").arg(ret);
            }
            qInfo().noquote() << "WebService 请求处理完毕";
        }
    }

    qWarning().noquote() << "WebService 任务线程结束";
}


