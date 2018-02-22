/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      DialogRunConfig.h
 *  \brief
 *      Interface-DialogRunConfig
 */
//!============================================================================

#ifndef DTS_EXWIDGET_DIALOGRUNCONFIG_H
#define DTS_EXWIDGET_DIALOGRUNCONFIG_H

#include <GDtsData.h>
#include "ui_DialogRunConfig.h"

class DialogRunConfig : public QDialog
{
    Q_OBJECT

public:
    DialogRunConfig(QWidget *parent = Q_NULLPTR);
    virtual ~DialogRunConfig();

private:
    void initUI();
    void resetModel();

Q_SIGNALS:
    void Signal_Modify();

public Q_SLOTS:
    void Slot_PushButton_clicked_Accept();
    void Slot_PushButton_clicked_Reject();
    void Slot_PushButton_clicked_Select();

private:
    Ui::DialogRunConfig _UI;
    QVariantMap         _Config;
};

#endif
