/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DialogStartup.cpp
 * @brief   启动页
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "DialogStartup.h"
#include "DialogSysConfig.h"

DialogStartup::DialogStartup(QWidget *parent)
    : QDialog(parent)
{
    _UI.setupUi(this);
    this->initUI();
}

DialogStartup::~DialogStartup()
{
}

QString DialogStartup::GetAppID()
{
    return m_strAppID;
}

void DialogStartup::initUI()
{
    this->setWindowIcon(QIcon(PICTURE_ACTION_LOGIN));
    this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    resetModel();

    QObject::connect(_UI.buttonStart, &QPushButton::clicked, this, &DialogStartup::Slot_PushButton_clicked_Start);
    QObject::connect(_UI.buttonCreate, &QPushButton::clicked, this, &DialogStartup::Slot_PushButton_clicked_Create);
    QObject::connect(_UI.buttonDelete, &QPushButton::clicked, this, &DialogStartup::Slot_PushButton_clicked_Delete);
}

void DialogStartup::resetModel()
{
    QStringMap appList;
    qCfgManager->GetAppList(appList);

    _UI.comboSelect->clear();
    for (auto it = appList.begin(); it != appList.end(); ++it)
    {
        _UI.comboSelect->addItem(QIcon(PICTURE_WORKSHOP), it.value(), it.key());
    }
}

void DialogStartup::Slot_PushButton_clicked_Start()
{
    qInfo() << "Slot_PushButton_clicked_Start";

    if (_UI.comboSelect->currentIndex() == -1)
    {
        MessageBox_Warning("请选择一个应用！");
        return;
    }

    m_strAppID = _UI.comboSelect->currentData().toString();

    accept();
}

void DialogStartup::Slot_PushButton_clicked_Create()
{
    qInfo() << "Slot_PushButton_clicked_Create";

    QVariantMap config;
    DialogSysConfig dialog(config);
    if (QDialog::Accepted == dialog.exec())
    {
        resetModel();
    }
}

void DialogStartup::Slot_PushButton_clicked_Delete()
{
    qInfo() << "Slot_PushButton_clicked_Delete";

    QString strAppID = _UI.comboSelect->currentData().toString();
    if (strAppID.isEmpty())
    {
        return;
    }

    if (MessageBox_Question("确定删除？"))
    {
        qCfgManager->DeleteApp(strAppID);
        resetModel();
    }
}
