/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogSynchronize.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_EXWIDGET_DIALOGSYNCHRONIZE_H
#define DTS_EXWIDGET_DIALOGSYNCHRONIZE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "ui_DialogSynchronize.h"

class DialogSynchronize : public QDialog
{
    Q_OBJECT

public:
    DialogSynchronize(QWidget *parent = Q_NULLPTR);
    virtual ~DialogSynchronize();

public:
    QMap<QString, QString> getConfig();

private:
    void init();

public Q_SLOTS:
    void slotListWidgetItemDoubleClicked01(QListWidgetItem *item);
    void slotListWidgetItemDoubleClicked02(QListWidgetItem *item);
    void slotListWidgetItemDoubleClicked03(QListWidgetItem *item);

private:
    QScopedPointer<Ui::DialogSynchronize> ui;
};

#endif
