/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogSynchronize.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "DialogSynchronize.h"
#include "GCfgManager.h"

DialogSynchronize::DialogSynchronize(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSynchronize)
{
    ui->setupUi(this);

    init();
}

DialogSynchronize::~DialogSynchronize()
{
}

QMap<QString, QString> DialogSynchronize::getConfig()
{
    QMap<QString, QString> map;

    QString listDown;
    do
    {
        int rows = ui->listWidget02->count();
        for (int row = 0; row < rows; row++)
        {
            listDown += ui->listWidget02->item(row)->text() + ",";
        }
    } while (false);
    if (ui->checkOrder->isChecked())
    {
        listDown.append(TABLE_WORKORDER).append(",");
    }
    if (ui->checkGas->isChecked())
    {
        listDown.append(TABLE_GASDATA).append(",");
    }

    QString listUp;
    do
    {
        int rows = ui->listWidget03->count();
        for (int row = 0; row < rows; row++)
        {
            listUp += ui->listWidget03->item(row)->text() + ",";
        }
    } while (false);

    map[CONFIG_RUN_DOWNLOAD] = listDown;
    map[CONFIG_RUN_UPLOAD] = listUp;
    map[CONFIG_RUN_DOWNLOAD_TIMESPAN] = QString::number(ui->spinDownload->value());
    map[CONFIG_RUN_UPLOAD_TIMESPAN] = QString::number(ui->spinUpload->value());
    map[CONFIG_RUN_GASWEIGHT_STORE] = QString::number(ui->spinGasWeight->value());

    return map;
}

void DialogSynchronize::init()
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QStringList listAll = qCfgManager->getSynchDataKeys();
    QStringList listDown = qCfgManager->getDownload();
    QStringList listUp = qCfgManager->getUpload();

    // 以下载为主，将上传重复的踢除
    Q_FOREACH (auto &str, listDown)
    {
        listUp.removeOne(str);
    }
    // 所有中，踢除下载和上传
    Q_FOREACH (auto &str, (listDown + listUp))
    {
        listAll.removeOne(str);
    }

    // 展示
    Q_FOREACH (auto &str, listAll)
    {
        ui->listWidget01->addItem(str);
    }

    bool bFindOrder = false;
    bool bFindGas = false;
    Q_FOREACH (auto &str, listDown)
    {
        if (!bFindOrder && str == TABLE_WORKORDER)
        {
            bFindOrder = true;
            continue;
        }
        if (!bFindGas && str == TABLE_GASDATA)
        {
            bFindGas = true;
            continue;
        }
        ui->listWidget02->addItem(str);
    }
    ui->checkOrder->setChecked(bFindOrder);
    ui->checkGas->setChecked(bFindGas);

    Q_FOREACH (auto &str, listUp)
    {
        ui->listWidget03->addItem(str);
    }

    ui->spinDownload->setValue(qCfgManager->getDownloadTimeSpan());
    ui->spinUpload->setValue(qCfgManager->getUploadTimeSpan());
    ui->spinGasWeight->setValue(qCfgManager->getGasWeightInitTime());

    QObject::connect(ui->buttonAccept, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(ui->buttonReject, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(ui->listWidget01, &QListWidget::itemDoubleClicked, this, &DialogSynchronize::slotListWidgetItemDoubleClicked01);
    QObject::connect(ui->listWidget02, &QListWidget::itemDoubleClicked, this, &DialogSynchronize::slotListWidgetItemDoubleClicked02);
    QObject::connect(ui->listWidget03, &QListWidget::itemDoubleClicked, this, &DialogSynchronize::slotListWidgetItemDoubleClicked03);
}

void DialogSynchronize::slotListWidgetItemDoubleClicked01(QListWidgetItem *item)
{
    qInfo() << "slotListWidgetItemDoubleClicked01" << item;

    int row = ui->listWidget01->row(item);

    if (ui->radioDownload->isChecked())
    {
        QList<QListWidgetItem *> ii = ui->listWidget02->findItems(item->text(), Qt::MatchFixedString);
        if (ii.isEmpty())
        {
            ui->listWidget02->addItem(ui->listWidget01->takeItem(row));
        }
    }
    else
    {
        QList<QListWidgetItem *> ii = ui->listWidget03->findItems(item->text(), Qt::MatchFixedString);
        if (ii.isEmpty())
        {
            ui->listWidget03->addItem(ui->listWidget01->takeItem(row));
        }
    }
}

void DialogSynchronize::slotListWidgetItemDoubleClicked02(QListWidgetItem *item)
{
    qInfo() << "slotListWidgetItemDoubleClicked02" << item;

    int row = ui->listWidget02->row(item);

    QListWidgetItem *aitem = ui->listWidget02->takeItem(row);

    ui->listWidget01->addItem(aitem);
}

void DialogSynchronize::slotListWidgetItemDoubleClicked03(QListWidgetItem *item)
{
    qInfo() << "slotListWidgetItemDoubleClicked03" << item;

    int row = ui->listWidget03->row(item);

    QListWidgetItem *aitem = ui->listWidget03->takeItem(row);

    ui->listWidget01->addItem(aitem);
}
