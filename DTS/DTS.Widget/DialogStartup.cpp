/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogStartup.cpp
 * @brief   startup page
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "DialogStartup.h"
#include "DialogConfiguration.h"
#include "GCfgManager.h"

DialogStartup::DialogStartup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogStartup)
{
    ui->setupUi(this);

    init();
}

DialogStartup::~DialogStartup()
{
}

QString DialogStartup::getSelection()
{
    return ui->comboSelect->currentData().toString();
}

void DialogStartup::init()
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    refresh();

    QObject::connect(ui->buttonStart, &QPushButton::clicked, this, &DialogStartup::slotPushButtonClickedStart);
    QObject::connect(ui->buttonCreate, &QPushButton::clicked, this, &DialogStartup::slotPushButtonClickedCreate);
    QObject::connect(ui->buttonDelete, &QPushButton::clicked, this, &DialogStartup::slotPushButtonClickedDelete);
}

void DialogStartup::refresh()
{
    QMap<QString, QString> appList = qCfgManager->getApplicationList();

    ui->comboSelect->clear();
    for (auto it = appList.begin(); it != appList.end(); ++it)
    {
        ui->comboSelect->addItem(it.value(), it.key());
    }
}

void DialogStartup::slotPushButtonClickedStart()
{
    qInfo() << "slotPushButtonClickedStart";

    if (ui->comboSelect->currentIndex() == -1)
    {
        QMessageBox::warning(Q_NULLPTR, "Warning", "Please select an application!");
        return;
    }

    accept();
}

void DialogStartup::slotPushButtonClickedCreate()
{
    qInfo() << "slotPushButtonClickedCreate";

    DialogConfiguration dialog;
    if (QDialog::Accepted == dialog.exec())
    {
        refresh();
    }
}

void DialogStartup::slotPushButtonClickedDelete()
{
    qInfo() << "slotPushButtonClickedDelete";

    QString strAppID = ui->comboSelect->currentData().toString();
    if (strAppID.isEmpty())
    {
        return;
    }

    if (QMessageBox::question(Q_NULLPTR, "Question", "Are you sure?") == QMessageBox::Yes)
    {
        qCfgManager->deleteApplication(strAppID);
        refresh();
    }
}
