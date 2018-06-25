/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DTSApplication.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "DTSApplication.h"
#include "DialogStartup.h"
#include "GCfgManager.h"
#include "MainWindow.h"
#include "WTaskSqlSynchronize.h"
#include "WTaskWebService.h"
#include "XYLogManager.h"

#include <windows.h>

static Qt::HANDLE hSingleInstMutex = Q_NULLPTR;

int main(int argc, char *argv[])
{
    DTSApplication app(argc, argv);
    QApplication::setOrganizationName("JHMES");
    QApplication::setApplicationName("DTS");
    QApplication::setApplicationVersion(qCfgManager->getVersion());

    QApplication::setStyle("fusion");

    qInstallMessageHandler(XYLogManager::OutputMessage);

    if (!qCfgManager->initialize())
    {
        return -1;
    }

    if (app.startApplication())
    {
        return -1;
    }

    return app.exec();
}

DTSApplication *DTSApplication::instance()
{
    return static_cast<DTSApplication *>(QCoreApplication::instance());
}

DTSApplication::DTSApplication(int argc, char **argv)
    : QApplication(argc, argv)
    , mainwindow(new MainWindow)
{
}

DTSApplication::~DTSApplication()
{
    if (hSingleInstMutex)
    {
        CloseHandle(hSingleInstMutex);
        hSingleInstMutex = Q_NULLPTR;
    }
}

QString DTSApplication::ParseApplication()
{
    QString appID;

    QStringList args = qApp->arguments();
    if (args.size() > 1)
    {
        QCommandLineParser parser;
        QCommandLineOption opName(QStringList({"n", "name"}), "AppName", "Application Name.", "");

        parser.addOption(opName);

        if (parser.parse(args))
        {
            appID = parser.value("n");
        }
    }
    else
    {
        DialogStartup dialog;
        if (QDialog::Accepted == dialog.exec())
        {
            appID = dialog.getSelection();
        }
    }
    return appID;
}

bool DTSApplication::CheckApplication(const QString &app)
{
    hSingleInstMutex = CreateMutex(Q_NULLPTR, TRUE, app.toStdWString().c_str());
    if (hSingleInstMutex == Q_NULLPTR)
    {
        return false;
    }
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hSingleInstMutex);
        hSingleInstMutex = Q_NULLPTR;
        return false;
    }
    return true;
}

bool DTSApplication::startApplication()
{
    QString app = DTSApplication::ParseApplication();
    if (app.isEmpty())
    {
        return false;
    }

    if (!DTSApplication::CheckApplication(app))
    {
        return false;
    }

    if (!qCfgManager->loadApplication(app))
    {
        return false;
    }

    mainwindow->show();

    qTaskWebService->initialize();
    qTaskSqlSynchronize->initialize();

    return true;
}
