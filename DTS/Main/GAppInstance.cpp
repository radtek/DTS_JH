/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GAppInstance.cpp
 * @brief   程序入口
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */


#include "GDtsData.h"
#include "MainWindow.h"
#include "DialogStartup.h"
#include "WTaskWebService.h"
#include "WTaskSynDatabase.h"

#include <Windows.h>

int main(int argc, char *argv[])
{
    return GAppInstance::Main(argc, argv);
}

QInt32 GAppInstance::Main(QInt32 argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setOrganizationName("JHMES");
    QApplication::setApplicationName("DTS");
    QApplication::setApplicationVersion("1.0.0.0000");
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication::setFont(QFont("Microsoft Yahei", 10));

    qLogManager;
    qSqlManager->Initialize();

    QString thisAppID;
    QBool bRunBack = true;
    if (argc > 1)
    {
        QCommandLineParser parser;
        qAppInstance->ParseCommandLine(parser);

        thisAppID = parser.value("n");
        bRunBack = parser.isSet("w");
        qDebug().noquote() << QString("命令行参数列表，[n]=%1，[w]=%2").arg(thisAppID).arg(bRunBack);
    }
    else
    {
        DialogStartup dialog;
        if (QDialog::Accepted != dialog.exec())
        {
            return -1;
        }

        thisAppID = dialog.GetAppID();
    }

    if (GERROR_OK != qAppInstance->StartProcess(thisAppID))
    {
        return -1;
    }

    MainWindow mainwindow;
    if (bRunBack)
    {
        mainwindow.show();
    }

    return app.exec();
}

GAppInstance &GAppInstance::Instance()
{
    static GAppInstance inst;
    return inst;
}

GAppInstance::GAppInstance()
    : m_hSingleInstMutex(Q_NULLPTR)
{
}

GAppInstance::~GAppInstance()
{
    UnInitialize();
}

GERROR GAppInstance::Initialize(const QString &single)
{
    m_hSingleInstMutex = CreateMutex(Q_NULLPTR, TRUE, single.toStdWString().c_str());
    if (m_hSingleInstMutex == Q_NULLPTR)
    {
        return GERROR_APP_CREATE_FAIL;
    }
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(m_hSingleInstMutex);
        m_hSingleInstMutex = Q_NULLPTR;
        return GERROR_APP_ALREADY_RUN;
    }
    return GERROR_OK;
}

GERROR GAppInstance::UnInitialize()
{
    if (m_hSingleInstMutex)
    {
        CloseHandle(m_hSingleInstMutex);
        m_hSingleInstMutex = Q_NULLPTR;
    }
    return GERROR_OK;
}

GERROR GAppInstance::ParseCommandLine(QCommandLineParser &parser)
{
    static const QStringList gAppCmdlineN =
    {
        "n", "name",
    };
    static const QStringList gAppCmdlineW =
    {
        "w", "window",
    };

    QCommandLineOption opName(gAppCmdlineN, "AppName", "Application Name.", "");
    QCommandLineOption opWindow(gAppCmdlineW, "ShowWindow", "Run with Windows.", "");

    parser.addOption(opName);
    parser.addOption(opWindow);

    if (parser.parse(QApplication::arguments()))
    {
        return GERROR_OK;
    }
    qWarning().noquote() << "未知的命令行参数列表：" << parser.unknownOptionNames();
    return GERROR_APP_COMMAND_PARSE;
}

GERROR GAppInstance::StartProcess(const QString &MyAppID)
{
    if (!qCfgManager->CheckAppID(MyAppID))
    {
        return GERROR_FAIL;
    }

    if (!qCfgManager->SetAppID(MyAppID))
    {
        return GERROR_FAIL;
    }

    if (qAppInstance->Initialize(MyAppID))
    {
        return GERROR_FAIL;
    }


    WTaskWebService::Instance().Initialize();
    WTaskSynDatabase::Instance().Initialize();

    return GERROR_OK;
}
