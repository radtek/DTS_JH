/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogConfirm.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "DialogConfirm.h"

DialogConfirm::DialogConfirm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogConfirm)
{
    ui->setupUi(this);

    init();
}

DialogConfirm::~DialogConfirm()
{
}

QString DialogConfirm::getSelection()
{
    return ui->editPassword->text();
}

void DialogConfirm::init()
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QObject::connect(ui->buttonAccept, &QPushButton::clicked, this, &DialogConfirm::accept);
    QObject::connect(ui->buttonReject, &QPushButton::clicked, this, &DialogConfirm::reject);
}
