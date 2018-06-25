/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogAbout.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "DialogAbout.h"
#include "GCfgManager.h"

DialogAbout::DialogAbout(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
}

DialogAbout::~DialogAbout()
{
}

void DialogAbout::init()
{
    ui->label->setText(qCfgManager->getVersion());
}
