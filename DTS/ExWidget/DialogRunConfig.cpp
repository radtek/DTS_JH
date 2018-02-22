/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      DTSPageRunConfig.cpp
 *  \brief
 *      Interface-PageRunConfig
 *  \since
 *  001     2017/11/01      weiheng     create this file
 */
//!============================================================================

#include "DialogRunConfig.h"
#include "WTaskDownload.h"


DialogRunConfig::DialogRunConfig(QWidget *parent)
    : QDialog(parent)
    , _Config(qCfgManager->_Config)
{
    _UI.setupUi(this);
    this->initUI();
}

DialogRunConfig::~DialogRunConfig()
{
}

void DialogRunConfig::initUI()
{
    this->setWindowIcon(QIcon(PICTURE_ACTION_RUNCONFIG));
    this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    _UI.comboLGDLVL->addItem("DBG", static_cast<QInt32>(ELogLevel::LG_DBG));
    _UI.comboLGDLVL->addItem("INF", static_cast<QInt32>(ELogLevel::LG_INF));
    _UI.comboLGDLVL->addItem("WAR", static_cast<QInt32>(ELogLevel::LG_WAR));
    _UI.comboLGDLVL->addItem("ERR", static_cast<QInt32>(ELogLevel::LG_ERR));

    _UI.spinTMDNLD->setMaximum(DEFAULT_CONFIG_MAX_RUN_TM_DNLD);
    _UI.spinTMDNLD->setMinimum(DEFAULT_CONFIG_MIN_RUN_TM_DNLD);
    _UI.spinTMUPLD->setMaximum(DEFAULT_CONFIG_MAX_RUN_TM_UPLD);
    _UI.spinTMUPLD->setMaximum(DEFAULT_CONFIG_MIN_RUN_TM_UPLD);

    resetModel();

    QObject::connect(_UI.buttonAccept, &QPushButton::clicked, this, &DialogRunConfig::Slot_PushButton_clicked_Accept);
    QObject::connect(_UI.buttonReject, &QPushButton::clicked, this, &DialogRunConfig::Slot_PushButton_clicked_Reject);
    QObject::connect(_UI.toolLgPath, &QToolButton::click, this, &DialogRunConfig::Slot_PushButton_clicked_Select);
    QObject::connect(this, &DialogRunConfig::Signal_Modify, &WTaskDownload::Instance(), &WTaskDownload::Slot_resetTimer);
}

void DialogRunConfig::resetModel()
{
    _UI.checkLGENAB->setChecked(_Config[CONFIG_RUN_LG_ENAB].toBool());
    _UI.comboLGDLVL->setCurrentIndex(_Config[CONFIG_RUN_LG_DLVL].toInt());

    _UI.editLGPATH->setText(_Config[CONFIG_RUN_LG_PATH].toString());
    _UI.editLGNAME->setText(_Config[CONFIG_RUN_LG_NAME].toString());

    _UI.spinLGMCNT->setValue(_Config[CONFIG_RUN_LG_MCNT].toInt());
    _UI.spinLGMSIZ->setValue(_Config[CONFIG_RUN_LG_MSIZ].toInt());

    _UI.spinTMUPLD->setValue(_Config[CONFIG_RUN_TM_UPLD].toInt());
    _UI.spinTMDNLD->setValue(_Config[CONFIG_RUN_TM_DNLD].toInt());
}

void DialogRunConfig::Slot_PushButton_clicked_Accept()
{
    qDebug() << "Slot_PushButton_clicked_Accept";

    if (_UI.editLGPATH->text().isEmpty()
        || _UI.editLGNAME->text().isEmpty()
        || _UI.comboLGDLVL->currentText().isEmpty())
    {
        return;
    }

    _Config[CONFIG_RUN_LG_ENAB] = _UI.checkLGENAB->isChecked();
    _Config[CONFIG_RUN_LG_PATH] = _UI.editLGPATH->text();
    _Config[CONFIG_RUN_LG_NAME] = _UI.editLGNAME->text();
    _Config[CONFIG_RUN_LG_MCNT] = _UI.spinLGMCNT->value();
    _Config[CONFIG_RUN_LG_MSIZ] = _UI.spinLGMSIZ->value();
    _Config[CONFIG_RUN_LG_DLVL] = _UI.comboLGDLVL->currentIndex();
    _Config[CONFIG_RUN_TM_UPLD] = _UI.spinTMUPLD->value();
    _Config[CONFIG_RUN_TM_DNLD] = _UI.spinTMDNLD->value();

    _Config[CONFIG_RUN_TM_UPLD] = CheckValueRange(_Config[CONFIG_RUN_TM_UPLD].toInt(), DEFAULT_CONFIG_MAX_RUN_TM_UPLD, DEFAULT_CONFIG_MIN_RUN_TM_UPLD);
    _Config[CONFIG_RUN_TM_DNLD] = CheckValueRange(_Config[CONFIG_RUN_TM_DNLD].toInt(), DEFAULT_CONFIG_MAX_RUN_TM_DNLD, DEFAULT_CONFIG_MIN_RUN_TM_DNLD);

    qCfgManager->SetConfig(_Config);

    MessageBox_Information("保存成功！");

    Q_EMIT Signal_Modify();

    accept();
}

void DialogRunConfig::Slot_PushButton_clicked_Reject()
{
    qDebug() << "Slot_PushButton_clicked_Reject";

    reject();
}

void DialogRunConfig::Slot_PushButton_clicked_Select()
{
    qDebug() << "Slot_PushButton_clicked_Select";

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (fileDialog.exec() == QDialog::Accepted)
    {
        _UI.editLGPATH->setText(fileDialog.directory().absolutePath());
    }
}
