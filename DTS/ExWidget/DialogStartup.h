/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogStartup.h
 * @brief   启动页
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_EXWIDGET_DIALOGSTARTUP_H
#define DTS_EXWIDGET_DIALOGSTARTUP_H

#include <GDtsData.h>
#include "ui_DialogStartup.h"


class DialogStartup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStartup(QWidget *parent = Q_NULLPTR);
    virtual ~DialogStartup();

public:
    QString GetAppID();

private:
    void initUI();
    void resetModel();

public Q_SLOTS:
    void Slot_PushButton_clicked_Start();
    void Slot_PushButton_clicked_Create();
    void Slot_PushButton_clicked_Delete();

private:
    Ui::DialogStartup   _UI;
    QString             m_strAppID;
};

#endif
