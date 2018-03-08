/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogSynConfig.h
 * @brief   配置同步参数
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_EXWIDGET_DIALOGSYNCONFIG_H
#define DTS_EXWIDGET_DIALOGSYNCONFIG_H

#include <GDtsData.h>
#include "ui_DialogSynConfig.h"

class DialogSynConfig : public QDialog
{
    Q_OBJECT

public:
    DialogSynConfig(QWidget *parent = Q_NULLPTR);
    virtual ~DialogSynConfig();

private:
    void initUI();

public Q_SLOTS:
    void Slot_PushButton_clicked_Accept();
    void Slot_PushButton_clicked_Reject();
    void Slot_QListWidget_itemDoubleClicked_listWidget01(QListWidgetItem *item);
    void Slot_QListWidget_itemDoubleClicked_listWidget02(QListWidgetItem *item);
    void Slot_QListWidget_itemDoubleClicked_listWidget03(QListWidgetItem *item);

private:
    Ui::DialogSynConfig    _UI;
};

#endif
