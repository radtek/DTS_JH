/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogConfiguration.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "DialogConfiguration.h"
#include "GCfgManager.h"

DialogConfiguration::DialogConfiguration(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogConfiguration)
    , update(false)
{
    ui->setupUi(this);

    init();
}

DialogConfiguration::~DialogConfiguration()
{
}

void DialogConfiguration::initData(const QMap<QString, QString> &map)
{
    if (map.isEmpty())
    {
        return;
    }

    QString appID = map.value(CONFIG_SYS_APPID);
    update = !appID.isEmpty();

    ui->editAppID->setText(map.value(CONFIG_SYS_APPID));
    ui->editAppName->setText(map.value(CONFIG_SYS_APPNAME));

    ui->editLWSAddr->setText(map.value(CONFIG_WS_LOCAL_ADDR));
    ui->spinLWSPort->setValue(map.value(CONFIG_WS_LOCAL_PORT).toInt());
    ui->editRWSUrl->setText(map.value(CONFIG_WS_REMOTE_URL));

    ui->editLDBAddr->setText(map.value(CONFIG_DB_LOCAL_ADDR));
    ui->spinLDBPort->setValue(map.value(CONFIG_DB_LOCAL_PORT).toInt());
    ui->editLDBUser->setText(map.value(CONFIG_DB_LOCAL_USER));
    ui->editLDBPass->setText(map.value(CONFIG_DB_LOCAL_PASS));
    ui->editLDBName->setText(map.value(CONFIG_DB_LOCAL_NAME));

    ui->editRDBAddr->setText(map.value(CONFIG_DB_REMOTE_ADDR));
    ui->spinRDBPort->setValue(map.value(CONFIG_DB_REMOTE_PORT).toInt());
    ui->editRDBUser->setText(map.value(CONFIG_DB_REMOTE_USER));
    ui->editRDBPass->setText(map.value(CONFIG_DB_REMOTE_PASS));
    ui->editRDBName->setText(map.value(CONFIG_DB_REMOTE_NAME));
}

void DialogConfiguration::init()
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QObject::connect(ui->buttonAccept, &QPushButton::clicked, this, &DialogConfiguration::slotPushButtonClickedAccept);
    QObject::connect(ui->buttonReject, &QPushButton::clicked, this, &QDialog::reject);
}

void DialogConfiguration::slotPushButtonClickedAccept()
{
    qInfo() << "slotPushButtonClickedAccept";

    if (ui->editAppID->text().isEmpty() ||
        ui->editAppName->text().isEmpty() ||
        ui->editLWSAddr->text().isEmpty() ||
        ui->editLDBAddr->text().isEmpty() ||
        ui->editLDBUser->text().isEmpty() ||
        ui->editLDBPass->text().isEmpty() ||
        ui->editLDBName->text().isEmpty() ||
        ui->editRWSUrl->text().isEmpty() ||
        ui->editRDBAddr->text().isEmpty() ||
        ui->editRDBUser->text().isEmpty() ||
        ui->editRDBPass->text().isEmpty() ||
        ui->editRDBName->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "Warning", "Please input all!");
        return;
    }

    QMap<QString, QString> map;
    map[CONFIG_SYS_APPID] = ui->editAppID->text();
    map[CONFIG_SYS_APPNAME] = ui->editAppName->text();

    map[CONFIG_WS_LOCAL_ADDR] = ui->editLWSAddr->text();
    map[CONFIG_WS_LOCAL_PORT] = ui->spinLWSPort->text();
    map[CONFIG_WS_REMOTE_URL] = ui->editRWSUrl->text();

    map[CONFIG_DB_LOCAL_ADDR] = ui->editLDBAddr->text();
    map[CONFIG_DB_LOCAL_PORT] = ui->spinLDBPort->text();
    map[CONFIG_DB_LOCAL_USER] = ui->editLDBUser->text();
    map[CONFIG_DB_LOCAL_PASS] = ui->editLDBPass->text();
    map[CONFIG_DB_LOCAL_NAME] = ui->editLDBName->text();

    map[CONFIG_DB_REMOTE_ADDR] = ui->editRDBAddr->text();
    map[CONFIG_DB_REMOTE_PORT] = ui->spinRDBPort->text();
    map[CONFIG_DB_REMOTE_USER] = ui->editRDBUser->text();
    map[CONFIG_DB_REMOTE_PASS] = ui->editRDBPass->text();
    map[CONFIG_DB_REMOTE_NAME] = ui->editRDBName->text();

    if (qCfgManager->createApplication(map, update))
    {
        QMessageBox::information(Q_NULLPTR, "Information", "Success");
    }
    else
    {
        return;
    }

    accept();
}
