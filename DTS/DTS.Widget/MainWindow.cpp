/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    MainWindow.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "MainWindow.h"
#include "DialogAbout.h"
#include "DialogConfiguration.h"
#include "DialogConfirm.h"
#include "DialogSynchronize.h"
#include "GCfgManager.h"
#include "WTaskSqlSynchronize.h"
#include "WTaskWebService.h"
#include "XYLogManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , iconTray(new QSystemTrayIcon(this))
    , menuTray(new QMenu(qobject_cast<QWidget *>(QApplication::desktop())))
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    iconTray->show();
    event->ignore();
}

void MainWindow::init()
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    menuTray->addAction("Show", this, &MainWindow::slotActionShow);
    menuTray->addSeparator();
    menuTray->addAction("Quit", this, &MainWindow::slotActionQuit);

    iconTray->setIcon(QIcon(":/DTS.ico"));
    iconTray->setContextMenu(menuTray.data());
    iconTray->show();

    QObject::connect(ui->buttonConfig, &QPushButton::clicked, this, &MainWindow::slotPushButtonClickedConfig);
    QObject::connect(ui->buttonDebug, &QPushButton::clicked, this, &MainWindow::slotPushButtonClickedDebug);
    QObject::connect(ui->buttonExport, &QPushButton::clicked, this, &MainWindow::slotPushButtonClickedExport);
    QObject::connect(ui->buttonSynch, &QPushButton::clicked, this, &MainWindow::slotPushButtonClickedSynch);
    QObject::connect(ui->buttonExit, &QPushButton::clicked, this, &MainWindow::slotActionQuit);
    QObject::connect(ui->buttonAbout, &QPushButton::clicked, this, &MainWindow::slotActionAbout);
    QObject::connect(iconTray.data(), &QSystemTrayIcon::activated, this, &MainWindow::slotSystemTrayIconActivated);
}

void MainWindow::slotPushButtonClickedConfig()
{
    qInfo() << "slotPushButtonClickedConfig";

    DialogConfiguration dialog;
    dialog.initData(qCfgManager->getConfig());

    dialog.exec();
}

void MainWindow::slotPushButtonClickedDebug()
{
    qInfo() << "slotPushButtonClickedDebug";

    if (qLogManager->hasConsole())
    {
        qLogManager->freeConsole();
    }
    else
    {
        qLogManager->allocConsole();
    }
}

void MainWindow::slotPushButtonClickedExport()
{
    qInfo() << "slotPushButtonClickedExport";

    if (qCfgManager->exportSynchData(DEFAULT_SQLSYNC_PATH))
    {
        QMessageBox::information(Q_NULLPTR, "Information", "Success");
    }
    else
    {
        QMessageBox::warning(Q_NULLPTR, "Warning", "Failed");
    }
}

void MainWindow::slotPushButtonClickedSynch()
{
    qInfo() << "slotPushButtonClickedSynch";

    DialogSynchronize dialog;
    if (QDialog::Accepted == dialog.exec())
    {
        QMap<QString, QString> map = dialog.getConfig();
        qCfgManager->setConfig(map);
    }
}

void MainWindow::slotSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    qInfo() << "slotSystemTrayIconActivated";

    switch (reason)
    {
    case QSystemTrayIcon::DoubleClick:
        slotActionShow();
        break;
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
    case QSystemTrayIcon::Context:
        break;
    default:
        break;
    }
}

void MainWindow::slotActionQuit()
{
    qInfo() << "slotActionQuit";

    DialogConfirm dialog;
    if (QDialog::Accepted == dialog.exec())
    {
        if (dialog.getSelection() == "123456")
        {
            qTaskWebService->unInitialize();
            qTaskSqlSynchronize->unInitialize();

            QApplication::quit();
            return;
        }
        else
        {
            QMessageBox::warning(Q_NULLPTR, "Warning", "Password error!");
        }
    }
    show();
}

void MainWindow::slotActionShow()
{
    qInfo() << "slotActionShow";

    iconTray->hide();
    show();
}

void MainWindow::slotActionAbout()
{
    qInfo() << "slotActionAbout";

    DialogAbout dialog;
    dialog.exec();
}
