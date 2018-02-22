/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      GAppInstance.h
 *  \brief
 *      Application Single Instance
 */
//!============================================================================

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
