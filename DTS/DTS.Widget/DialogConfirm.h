/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogConfirm.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DIALOGCONFIRM_H
#define DIALOGCONFIRM_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "ui_DialogConfirm.h"

class DialogConfirm : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConfirm(QWidget *parent = Q_NULLPTR);
    virtual ~DialogConfirm();

public:
    QString getSelection();

private:
    void init();

private:
    QScopedPointer<Ui::DialogConfirm> ui;
};

#endif
