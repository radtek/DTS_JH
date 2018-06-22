/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogStartup.h
 * @brief   startup page
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_WIDGET_DIALOGSTARTUP_H
#define DTS_WIDGET_DIALOGSTARTUP_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "ui_DialogStartup.h"

class DialogStartup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStartup(QWidget *parent = Q_NULLPTR);
    virtual ~DialogStartup();

public:
    QString getSelection();

private:
    void init();
    void refresh();

public Q_SLOTS:
    void slotPushButtonClickedStart();
    void slotPushButtonClickedCreate();
    void slotPushButtonClickedDelete();

private:
    QScopedPointer<Ui::DialogStartup> ui;
};

#endif
