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


#include "DialogSysConfig.h"

DialogSysConfig::DialogSysConfig(QVariantMap &config, QWidget *parent)
    : QDialog(parent)
    , _Config(config)
    , m_bUpdate(true)
{
    _UI.setupUi(this);

    m_bUpdate = !_Config[CONFIG_SYS_MYAPPID].isNull();

    this->initUI();
}

DialogSysConfig::~DialogSysConfig()
{
}

void DialogSysConfig::initUI()
{
    this->setWindowIcon(QIcon(PICTURE_ACTION_SYSCONFIG));
    this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    _UI.editAppID->setDisabled(m_bUpdate);
    _UI.labelTipsLWS->setHidden(m_bUpdate);
    _UI.labelTipsRWS->setHidden(m_bUpdate);

    _UI.editAppID->setText(_Config.value(CONFIG_SYS_MYAPPID, DEFAULT_CONFIG_SYS_MYAPPID).toString());
    _UI.editAppName->setText(_Config.value(CONFIG_SYS_APPNAME, DEFAULT_CONFIG_SYS_APPNAME).toString());
    _UI.editLWSAddr->setText(_Config.value(CONFIG_SYS_LWSADDR, DEFAULT_CONFIG_SYS_LWSADDR).toString());
    _UI.spinLWSPort->setValue(_Config.value(CONFIG_SYS_LWSPORT, DEFAULT_CONFIG_SYS_LWSPORT).toInt());
    _UI.editLDBAddr->setText(_Config.value(CONFIG_SYS_LDBADDR, DEFAULT_CONFIG_SYS_LDBADDR).toString());
    _UI.spinLDBPort->setValue(_Config.value(CONFIG_SYS_LDBPORT, DEFAULT_CONFIG_SYS_LDBPORT).toInt());
    _UI.editLDBUser->setText(_Config.value(CONFIG_SYS_LDBUSER, DEFAULT_CONFIG_SYS_LDBUSER).toString());
    _UI.editLDBPass->setText(_Config.value(CONFIG_SYS_LDBPASS, DEFAULT_CONFIG_SYS_LDBPASS).toString());
    _UI.editLDBName->setText(_Config.value(CONFIG_SYS_LDBNAME, DEFAULT_CONFIG_SYS_LDBNAME).toString());
    _UI.editRWSAddr->setText(_Config.value(CONFIG_SYS_RWSADDR, DEFAULT_CONFIG_SYS_RWSADDR).toString());
    _UI.spinRWSPort->setValue(_Config.value(CONFIG_SYS_RWSPORT, DEFAULT_CONFIG_SYS_RWSPORT).toInt());
    _UI.editRDBAddr->setText(_Config.value(CONFIG_SYS_RDBADDR, DEFAULT_CONFIG_SYS_RDBADDR).toString());
    _UI.spinRDBPort->setValue(_Config.value(CONFIG_SYS_RDBPORT, DEFAULT_CONFIG_SYS_RDBPORT).toInt());
    _UI.editRDBUser->setText(_Config.value(CONFIG_SYS_RDBUSER, DEFAULT_CONFIG_SYS_RDBUSER).toString());
    _UI.editRDBPass->setText(_Config.value(CONFIG_SYS_RDBPASS, DEFAULT_CONFIG_SYS_RDBPASS).toString());
    _UI.editRDBName->setText(_Config.value(CONFIG_SYS_RDBNAME, DEFAULT_CONFIG_SYS_RDBNAME).toString());

    _UI.labelCheckApp->setPixmap(QPixmap());
    _UI.labelCheckLWS->setPixmap(QPixmap());
    _UI.labelCheckLDB->setPixmap(QPixmap());
    _UI.labelCheckRWS->setPixmap(QPixmap());
    _UI.labelCheckRDB->setPixmap(QPixmap());

    QObject::connect(_UI.buttonAccept, &QPushButton::clicked, this, &DialogSysConfig::Slot_PushButton_clicked_Accept);
    QObject::connect(_UI.buttonReject, &QPushButton::clicked, this, &DialogSysConfig::Slot_PushButton_clicked_Reject);
    QObject::connect(_UI.buttonTestLWS, &QPushButton::clicked, this, &DialogSysConfig::Slot_PushButton_clicked_TestLWS);
    QObject::connect(_UI.buttonTestLDB, &QPushButton::clicked, this, &DialogSysConfig::Slot_PushButton_clicked_TestLDB);
    QObject::connect(_UI.buttonTestRWS, &QPushButton::clicked, this, &DialogSysConfig::Slot_PushButton_clicked_TestRWS);
    QObject::connect(_UI.buttonTestRDB, &QPushButton::clicked, this, &DialogSysConfig::Slot_PushButton_clicked_TestRDB);
    QObject::connect(_UI.editAppID, &QLineEdit::editingFinished, this, &DialogSysConfig::Slot_LineEdit_editingFinished_App);
    QObject::connect(_UI.editAppName, &QLineEdit::editingFinished, this, &DialogSysConfig::Slot_LineEdit_editingFinished_App);
}

void DialogSysConfig::Slot_PushButton_clicked_Accept()
{
    qInfo() << "Slot_PushButton_clicked_Accept";

    if (_UI.editAppID->text().isEmpty()
        || _UI.editAppName->text().isEmpty()
        || _UI.editLWSAddr->text().isEmpty()
        || _UI.editLDBAddr->text().isEmpty()
        || _UI.editLDBUser->text().isEmpty()
        || _UI.editLDBPass->text().isEmpty()
        || _UI.editLDBName->text().isEmpty()
        || _UI.editRWSAddr->text().isEmpty()
        || _UI.editRDBAddr->text().isEmpty()
        || _UI.editRDBUser->text().isEmpty()
        || _UI.editRDBPass->text().isEmpty()
        || _UI.editRDBName->text().isEmpty())
    {
        MessageBox_Warning("请填写所有项！");
        return;
    }

    _Config[CONFIG_SYS_MYAPPID] = _UI.editAppID->text();
    _Config[CONFIG_SYS_APPNAME] = _UI.editAppName->text();
    _Config[CONFIG_SYS_LWSADDR] = _UI.editLWSAddr->text();
    _Config[CONFIG_SYS_LWSPORT] = _UI.spinLWSPort->value();
    _Config[CONFIG_SYS_LDBADDR] = _UI.editLDBAddr->text();
    _Config[CONFIG_SYS_LDBPORT] = _UI.spinLDBPort->value();
    _Config[CONFIG_SYS_LDBUSER] = _UI.editLDBUser->text();
    _Config[CONFIG_SYS_LDBPASS] = _UI.editLDBPass->text();
    _Config[CONFIG_SYS_LDBNAME] = _UI.editLDBName->text();
    _Config[CONFIG_SYS_RWSADDR] = _UI.editRWSAddr->text();
    _Config[CONFIG_SYS_RWSPORT] = _UI.spinRWSPort->value();
    _Config[CONFIG_SYS_RDBADDR] = _UI.editRDBAddr->text();
    _Config[CONFIG_SYS_RDBPORT] = _UI.spinRDBPort->value();
    _Config[CONFIG_SYS_RDBUSER] = _UI.editRDBUser->text();
    _Config[CONFIG_SYS_RDBPASS] = _UI.editRDBPass->text();
    _Config[CONFIG_SYS_RDBNAME] = _UI.editRDBName->text();

    if (m_bUpdate)
    {
        qCfgManager->SetConfig(_Config);

        MessageBox_Information("保存成功！");

        Q_EMIT Signal_Modify();
    }
    else
    {
        if (!qCfgManager->CreateApp(_Config))
        {
            MessageBox_Warning("创建失败！");
            return;
        }
    }
    accept();
}

void DialogSysConfig::Slot_PushButton_clicked_Reject()
{
    qInfo() << "Slot_PushButton_clicked_Reject";

    reject();
}

void DialogSysConfig::Slot_PushButton_clicked_TestLWS()
{
    qInfo() << "Slot_PushButton_clicked_TestLWS";

    _UI.labelCheckLWS->setPixmap(QPixmap());
    QString strPic = PICTURE_RES_ERROR;
    do
    {
        QTcpServer server;
        server.listen(QHostAddress(_UI.editLWSAddr->text()), static_cast<QUInt16>(_UI.spinLWSPort->value()));
        if (server.isListening())
        {
            strPic = PICTURE_RES_OK ;
        }
        server.close();
    }
    while (0);

    _UI.labelCheckLWS->setPixmap(QPixmap(strPic));
}

void DialogSysConfig::Slot_PushButton_clicked_TestLDB()
{
    qInfo() << "Slot_PushButton_clicked_TestLDB";

    _UI.labelCheckLDB->setPixmap(QPixmap());

    QString strPic = PICTURE_RES_ERROR;
    do
    {
        QSqlDatabase dbSQL = QSqlDatabase::addDatabase("QODBC", "TEST");
        QString dsn = QString("Driver={sql server};SERVER=%1;PORT=%2;UID=%3;PWD=%4;DATABASE=%5;")
                      .arg(_UI.editLDBAddr->text())
                      .arg(_UI.spinLDBPort->value())
                      .arg(_UI.editLDBUser->text())
                      .arg(_UI.editLDBPass->text())
                      .arg(_UI.editLDBName->text());
        dbSQL.setDatabaseName(dsn);

        if (dbSQL.open())
        {
            strPic = PICTURE_RES_OK;
        }
        dbSQL.close();
    }
    while (0);
    QSqlDatabase::removeDatabase("TEST");

    _UI.labelCheckLDB->setPixmap(QPixmap(strPic));
}

void DialogSysConfig::Slot_PushButton_clicked_TestRWS()
{
    qInfo() << "Slot_PushButton_clicked_TestRWS";

    _UI.labelCheckRWS->setPixmap(QPixmap());
    QString strPic = PICTURE_RES_ERROR;
    do
    {
        QTcpSocket client;
        client.connectToHost(QHostAddress(_UI.editRWSAddr->text()), static_cast<QUInt16>(_UI.spinRWSPort->value()));
        if (client.isValid())
        {
            strPic = PICTURE_RES_OK;
        }
        qDebug() << client.error() << client.state();
        client.close();
    }
    while (0);
    _UI.labelCheckRWS->setPixmap(QPixmap(strPic));
}

void DialogSysConfig::Slot_PushButton_clicked_TestRDB()
{
    qInfo() << "Slot_PushButton_clicked_TestRDB";

    _UI.labelCheckRDB->setPixmap(QPixmap());
    QString strPic = PICTURE_RES_ERROR;
    do
    {
        QSqlDatabase dbSQL = QSqlDatabase::addDatabase("QODBC", "TEST");

        QString dsn = QString("Driver={sql server};SERVER=%1;PORT=%2;UID=%3;PWD=%4;DATABASE=%5;")
                      .arg(_UI.editRDBAddr->text())
                      .arg(_UI.spinRDBPort->value())
                      .arg(_UI.editRDBUser->text())
                      .arg(_UI.editRDBPass->text())
                      .arg(_UI.editRDBName->text());
        dbSQL.setDatabaseName(dsn);

        if (dbSQL.open())
        {
            strPic = PICTURE_RES_OK;
        }
        dbSQL.close();
    }
    while (0);
    QSqlDatabase::removeDatabase("TEST");

    _UI.labelCheckRDB->setPixmap(QPixmap(strPic));
}

void DialogSysConfig::Slot_LineEdit_editingFinished_App()
{
    qInfo() << "Slot_LineEdit_editingFinished_App";

    _UI.labelCheckApp->setPixmap(QPixmap());

    if (_UI.editAppID->text().isEmpty()
        || _UI.editAppName->text().isEmpty())
    {
        return;
    }

    QString strPic = PICTURE_RES_OK;

    if (qCfgManager->CheckAppID(_UI.editAppID->text()))
    {
        strPic = PICTURE_RES_ERROR;
    }

    _UI.labelCheckApp->setPixmap(QPixmap(strPic));
}


