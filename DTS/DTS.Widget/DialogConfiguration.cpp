/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogSysConfig.cpp
 * @brief   配置系统参数
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
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

    QString appID = map.value(CONFIG_SYS_MYAPPID);
    update = !appID.isEmpty();

    ui->editAppID->setText(map.value(CONFIG_SYS_MYAPPID));
    ui->editAppName->setText(map.value(CONFIG_SYS_APPNAME));
    ui->editLWSAddr->setText(map.value(CONFIG_SYS_LWSADDR));
    ui->spinLWSPort->setValue(map.value(CONFIG_SYS_LWSPORT).toInt());
    ui->editLDBAddr->setText(map.value(CONFIG_SYS_LDBADDR));
    ui->spinLDBPort->setValue(map.value(CONFIG_SYS_LDBPORT).toInt());
    ui->editLDBUser->setText(map.value(CONFIG_SYS_LDBUSER));
    ui->editLDBPass->setText(map.value(CONFIG_SYS_LDBPASS));
    ui->editLDBName->setText(map.value(CONFIG_SYS_LDBNAME));
    ui->editRWSAddr->setText(map.value(CONFIG_SYS_RWSADDR));
    ui->spinRWSPort->setValue(map.value(CONFIG_SYS_RWSPORT).toInt());
    ui->editRDBAddr->setText(map.value(CONFIG_SYS_RDBADDR));
    ui->spinRDBPort->setValue(map.value(CONFIG_SYS_RDBPORT).toInt());
    ui->editRDBUser->setText(map.value(CONFIG_SYS_RDBUSER));
    ui->editRDBPass->setText(map.value(CONFIG_SYS_RDBPASS));
    ui->editRDBName->setText(map.value(CONFIG_SYS_RDBNAME));
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
        ui->editRWSAddr->text().isEmpty() ||
        ui->editRDBAddr->text().isEmpty() ||
        ui->editRDBUser->text().isEmpty() ||
        ui->editRDBPass->text().isEmpty() ||
        ui->editRDBName->text().isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, "WARN", "Please input all!");
        return;
    }

    QMap<QString, QString> map;
    map[CONFIG_SYS_MYAPPID] = ui->editAppID->text();
    map[CONFIG_SYS_APPNAME] = ui->editAppName->text();
    map[CONFIG_SYS_LWSADDR] = ui->editLWSAddr->text();
    map[CONFIG_SYS_LWSPORT] = ui->spinLWSPort->text();
    map[CONFIG_SYS_LDBADDR] = ui->editLDBAddr->text();
    map[CONFIG_SYS_LDBPORT] = ui->spinLDBPort->text();
    map[CONFIG_SYS_LDBUSER] = ui->editLDBUser->text();
    map[CONFIG_SYS_LDBPASS] = ui->editLDBPass->text();
    map[CONFIG_SYS_LDBNAME] = ui->editLDBName->text();
    map[CONFIG_SYS_RWSADDR] = ui->editRWSAddr->text();
    map[CONFIG_SYS_RWSPORT] = ui->spinRWSPort->text();
    map[CONFIG_SYS_RDBADDR] = ui->editRDBAddr->text();
    map[CONFIG_SYS_RDBPORT] = ui->spinRDBPort->text();
    map[CONFIG_SYS_RDBUSER] = ui->editRDBUser->text();
    map[CONFIG_SYS_RDBPASS] = ui->editRDBPass->text();
    map[CONFIG_SYS_RDBNAME] = ui->editRDBName->text();

    if (qCfgManager->createApplication(map, update))
    {
        QMessageBox::information(Q_NULLPTR, "INFO", "Success");
    }
    else
    {
        return;
    }

    accept();
}
