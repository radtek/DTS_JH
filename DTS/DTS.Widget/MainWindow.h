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

#ifndef DTS_WIDGET_MAINWINDOW_H
#define DTS_WIDGET_MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    virtual ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    void init();

public Q_SLOTS:
    void slotPushButtonClickedConfig();
    void slotPushButtonClickedDebug();
    void slotSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void slotActionQuit();
    void slotActionShow();
    void slotActionAbout();

private:
    QScopedPointer<Ui::MainWindow> ui;
    QScopedPointer<QSystemTrayIcon> iconTray;
    QScopedPointer<QMenu> menuTray;
};

#endif
