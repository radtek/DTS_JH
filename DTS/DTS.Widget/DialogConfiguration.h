/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogConfiguration.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_WIDGET_DIALOGCONFIGURATION_H
#define DTS_WIDGET_DIALOGCONFIGURATION_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "ui_DialogConfiguration.h"

class DialogConfiguration : public QDialog
{
    Q_OBJECT

public:
    DialogConfiguration(QWidget *parent = Q_NULLPTR);
    virtual ~DialogConfiguration();

public:
    void initData(const QMap<QString, QString> &map);

private:
    void init();

public Q_SLOTS:
    void slotPushButtonClickedAccept();

private:
    QScopedPointer<Ui::DialogConfiguration> ui;
    bool update;
};

#endif
