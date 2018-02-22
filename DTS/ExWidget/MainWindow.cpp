#include "MainWindow.h"
#include "WTaskDownload.h"
#include "WTaskUpload.h"
#include "WTaskWebService.h"
#include "WTaskWorkOrder.h"
#include "DialogSynConfig.h"
#include "DialogRunConfig.h"
#include "DialogSysConfig.h"


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

    QObject::connect(&systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(Slot_QSystemTrayIcon_activated(QSystemTrayIcon::ActivationReason)));
    QObject::connect(_UI.m_ActionExit, SIGNAL(triggered()), this, SLOT(Slot_Action_triggered_Exit()));
    QObject::connect(_UI.m_ActionShow, SIGNAL(triggered()), this, SLOT(Slot_Action_triggered_Show()));
    QObject::connect(_UI.m_ActionSysConfig, SIGNAL(triggered()), this, SLOT(Slot_Action_triggered_SysConfig()));
    QObject::connect(_UI.m_ActionRunConfig, SIGNAL(triggered()), this, SLOT(Slot_Action_triggered_RunConfig()));
    QObject::connect(_UI.m_ActionSynConfig, SIGNAL(triggered()), this, SLOT(Slot_Action_triggered_SynConfig()));
    QObject::connect(_UI.buttonExit, SIGNAL(clicked()), this, SLOT(Slot_Action_triggered_Exit()));
    QObject::connect(_UI.buttonSysConfig, SIGNAL(clicked()), this, SLOT(Slot_Action_triggered_SysConfig()));
    QObject::connect(_UI.buttonRunConfig, SIGNAL(clicked()), this, SLOT(Slot_Action_triggered_RunConfig()));
    QObject::connect(_UI.buttonSynConfig, SIGNAL(clicked()), this, SLOT(Slot_Action_triggered_SynConfig()));
    QObject::connect(qLogManager, SIGNAL(sendMsg(const QString &)), this, SLOT(Slot_Custom_sendMsg(const QString &)), Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
}



void MainWindow::Slot_Action_triggered_Exit()
{
    qDebug() << "Slot_Action_triggered_Logout";

    if (!MessageBox_Question("确认退出？"))
    {
        return;
    }
    qCfgManager->UnInitialize();

    WTaskDownload::Instance().UnInitialize();
    WTaskUpload::Instance().UnInitialize();
    WTaskWebService::Instance().UnInitialize();

    //qSqlManager->UnInitialize();

    QApplication::quit();
}

void MainWindow::Slot_Action_triggered_Show()
{
    qDebug() << "Slot_Action_triggered_Show";

    this->systemTrayIcon.hide();
    this->show();
}

void MainWindow::Slot_Action_triggered_SysConfig()
{
    qDebug() << "Slot_Action_triggered_SysConfig";

    DialogSysConfig dialog(qCfgManager->_Config);
    dialog.exec();
}

void MainWindow::Slot_Action_triggered_RunConfig()
{
    qDebug() << "Slot_Action_triggered_RunConfig";

    DialogRunConfig dialog;
    dialog.exec();
}

void MainWindow::Slot_Action_triggered_SynConfig()
{
    qDebug() << "Slot_Action_triggered_SynConfig";

    DialogSynConfig dialog;
    dialog.exec();
}

void MainWindow::Slot_QSystemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "Slot_QSystemTrayIcon_activated";

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
