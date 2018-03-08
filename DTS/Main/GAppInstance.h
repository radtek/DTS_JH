/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GAppInstance.h
 * @brief   程序入口
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */


#ifndef DTS_MAIN_GAPPINSTANCE_H
#define DTS_MAIN_GAPPINSTANCE_H


#include "GDtsDef.h"


class GAppInstance
{
    Q_DECL_SINGLETON(GAppInstance)

public:
    static QInt32 Main(QInt32 argc, char *argv[]);

public:
    GERROR Initialize(const QString &single);
    GERROR UnInitialize();
    GERROR ParseCommandLine(QCommandLineParser &parser);
    GERROR StartProcess(const QString &appName);

private:
    Qt::HANDLE m_hSingleInstMutex;
};

#define qAppInstance    (&GAppInstance::Instance())

#endif
