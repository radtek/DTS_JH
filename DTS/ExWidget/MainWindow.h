/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    MainWindow.h
 * @brief   主界面
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */


#ifndef DTS_EXWIDGET_MAINWINDOW_H
#define DTS_EXWIDGET_MAINWINDOW_H


#include <GDtsData.h>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    virtual ~MainWindow();

public Q_SLOTS:
    void Slot_Action_triggered_Exit();
    void Slot_Action_triggered_Show();
    void Slot_Action_triggered_SysConfig();
    void Slot_Action_triggered_RunConfig();
    void Slot_Action_triggered_SynConfig();
    void Slot_QSystemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason);
    void Slot_Custom_sendMsg(const QString &strMsg);


protected:
    virtual bool event(QEvent *event);

private:
    Ui::MainWindow _UI;
    QSystemTrayIcon     systemTrayIcon;
    QMenu               menuTray;
};

#endif // MAINWINDOW_H
