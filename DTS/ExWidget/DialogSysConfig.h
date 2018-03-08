/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogSysConfig.h
 * @brief   配置系统参数
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_EXWIDGET_DIALOGSYSCONFIG_H
#define DTS_EXWIDGET_DIALOGSYSCONFIG_H

#include <GDtsData.h>
#include "ui_DialogSysConfig.h"

class DialogSysConfig : public QDialog
{
    Q_OBJECT

public:
    DialogSysConfig(QVariantMap &config, QWidget *parent = Q_NULLPTR);
    virtual ~DialogSysConfig();

private:
    void initUI();

Q_SIGNALS:
    void Signal_Modify();

public Q_SLOTS:
    void Slot_PushButton_clicked_Accept();
    void Slot_PushButton_clicked_Reject();
    void Slot_PushButton_clicked_TestLWS();
    void Slot_PushButton_clicked_TestLDB();
    void Slot_PushButton_clicked_TestRWS();
    void Slot_PushButton_clicked_TestRDB();
    void Slot_LineEdit_editingFinished_App();

private:
    Ui::DialogSysConfig _UI;
    QVariantMap         _Config;
    QBool               m_bUpdate;
};

#endif
