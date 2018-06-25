/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogAbout.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_WIDGET_DIALOGABOUT_H
#define DTS_WIDGET_DIALOGABOUT_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "ui_DialogAbout.h"

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = Q_NULLPTR);
    virtual ~DialogAbout();

private:
    void init();

private:
    QScopedPointer<Ui::DialogAbout> ui;
};

#endif
