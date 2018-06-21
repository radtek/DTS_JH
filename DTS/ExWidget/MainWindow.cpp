/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    MainWindow.cpp
 * @brief   主界面
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "MainWindow.h"
#include "DialogRunConfig.h"
#include "DialogSynConfig.h"
#include "DialogSysConfig.h"
#include "WTaskSynDatabase.h"
#include "WTaskWebService.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , systemTrayIcon(this)
    , menuTray(reinterpret_cast<QWidget *>(QApplication::desktop()))
{
    _UI.setupUi(this);

    this->centralWidget();
    this->setWindowIcon(QIcon(PICTURE_MAIN));
    this->setWindowTitle(qCfgManager->_Config[CONFIG_SYS_APPNAME].toString());
    this->systemTrayIcon.setToolTip(qCfgManager->_Config[CONFIG_SYS_APPNAME].toString());

    menuTray.addAction(_UI.m_ActionSysConfig);
    menuTray.addAction(_UI.m_ActionRunConfig);
    menuTray.addAction(_UI.m_ActionSynConfig);
    menuTray.addSeparator();
    menuTray.addAction(_UI.m_ActionShow);
    menuTray.addAction(_UI.m_ActionExit);

    systemTrayIcon.setIcon(QIcon(PICTURE_MAIN_RUN));
    systemTrayIcon.setContextMenu(&menuTray);

    QObject::connect(&systemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::Slot_QSystemTrayIcon_activated);
    QObject::connect(_UI.m_ActionExit, &QAction::triggered, this, &MainWindow::Slot_Action_triggered_Exit);
    QObject::connect(_UI.m_ActionShow, &QAction::triggered, this, &MainWindow::Slot_Action_triggered_Show);
    QObject::connect(_UI.m_ActionSysConfig, &QAction::triggered, this, &MainWindow::Slot_Action_triggered_SysConfig);
    QObject::connect(_UI.m_ActionRunConfig, &QAction::triggered, this, &MainWindow::Slot_Action_triggered_RunConfig);
    QObject::connect(_UI.m_ActionSynConfig, &QAction::triggered, this, &MainWindow::Slot_Action_triggered_SynConfig);
    QObject::connect(_UI.buttonExit, &QPushButton::clicked, this, &MainWindow::Slot_Action_triggered_Exit);
    QObject::connect(_UI.buttonSysConfig, &QPushButton::clicked, this, &MainWindow::Slot_Action_triggered_SysConfig);
    QObject::connect(_UI.buttonRunConfig, &QPushButton::clicked, this, &MainWindow::Slot_Action_triggered_RunConfig);
    QObject::connect(_UI.buttonSynConfig, &QPushButton::clicked, this, &MainWindow::Slot_Action_triggered_SynConfig);
    //    QObject::connect(qLogManager, &GLogManager::sendMsg, this, &MainWindow::Slot_Custom_sendMsg, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
}

void MainWindow::Slot_Action_triggered_Exit()
{
    qInfo() << "Slot_Action_triggered_Logout";

    if (!MessageBox_Question("确认退出？"))
    {
        return;
    }

    WTaskSynDatabase::Instance().UnInitialize();
    WTaskWebService::Instance().UnInitialize();

    QApplication::quit();
}

void MainWindow::Slot_Action_triggered_Show()
{
    qInfo() << "Slot_Action_triggered_Show";

    this->systemTrayIcon.hide();
    this->show();
}

void MainWindow::Slot_Action_triggered_SysConfig()
{
    qInfo() << "Slot_Action_triggered_SysConfig";

    DialogSysConfig dialog(qCfgManager->_Config);
    dialog.exec();
}

void MainWindow::Slot_Action_triggered_RunConfig()
{
    qInfo() << "Slot_Action_triggered_RunConfig";

    DialogRunConfig dialog;
    dialog.exec();
}

void MainWindow::Slot_Action_triggered_SynConfig()
{
    qInfo() << "Slot_Action_triggered_SynConfig";

    DialogSynConfig dialog;
    dialog.exec();
}

void MainWindow::Slot_QSystemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    qInfo() << "Slot_QSystemTrayIcon_activated";

    switch (reason)
    {
    case QSystemTrayIcon::DoubleClick:
        Slot_Action_triggered_Show();
        break;
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
    case QSystemTrayIcon::Context:
        break;
    default:
        break;
    }
}

void MainWindow::Slot_Custom_sendMsg(const QString &strMsg)
{
    _UI.plainTextEdit->appendPlainText(strMsg);
}

bool MainWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Close:
        this->hide();
        this->systemTrayIcon.show();
        event->ignore();
        return true;
    default:
        return QWidget::event(event);
    }
}
