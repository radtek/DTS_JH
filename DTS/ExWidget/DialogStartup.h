/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      DialogStartup.h
 *  \brief
 *      Interface-Startup
 */
//!============================================================================


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
