/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      DialogSynConfig.h
 *  \brief
 *      Interface-DialogSynConfig
 */
//!============================================================================

#include "DialogSynConfig.h"


DialogSynConfig::DialogSynConfig(QWidget *parent)
    : QDialog(parent)
{
    _UI.setupUi(this);
    this->initUI();
}

DialogSynConfig::~DialogSynConfig()
{
}

void DialogSynConfig::initUI()
{
    this->setWindowIcon(QIcon(PICTURE_ACTION_SYNCONFIG));
    this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    do
    {
        QStringList listAll = qSqlManager->_SqlSync.keys();

        const QString &down = qCfgManager->_Config.value(CONFIG_SYN_DOWNSQL).toString();
        QStringList listDown = down.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);

        const QString &up = qCfgManager->_Config.value(CONFIG_SYN_UPLDSQL).toString();
        QStringList listUp = up.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);

        // 以下载为主，将上传重复的踢除
        for (auto &str : listDown)
        {
            listUp.removeOne(str);
        }
        // 所有中，踢除下载和上传
        for (auto &str : (listDown + listUp))
        {
            listAll.removeOne(str);
        }

        // 展示
        for (auto &str : listAll)
        {
            _UI.listWidget01->addItem(str);
        }

        for (auto &str : listDown)
        {
            _UI.listWidget02->addItem(str);
        }

        for (auto &str : listUp)
        {
            _UI.listWidget03->addItem(str);
        }
    }
    while (0);

    QObject::connect(_UI.buttonAccept, &QPushButton::clicked, this, &DialogSynConfig::Slot_PushButton_clicked_Accept);
    QObject::connect(_UI.buttonReject, &QPushButton::clicked, this, &DialogSynConfig::Slot_PushButton_clicked_Reject);
    QObject::connect(_UI.listWidget01, &QListWidget::itemDoubleClicked, this, &DialogSynConfig::Slot_QListWidget_itemDoubleClicked_listWidget01);
    QObject::connect(_UI.listWidget02, &QListWidget::itemDoubleClicked, this, &DialogSynConfig::Slot_QListWidget_itemDoubleClicked_listWidget02);

}

void DialogSynConfig::Slot_PushButton_clicked_Accept()
{
    qDebug() << "Slot_PushButton_clicked_Accept";

    QString DOWNSQL;
    do
    {
        int rows = _UI.listWidget02->count();
        for (int row = 0; row < rows; row++)
        {
            DOWNSQL += _UI.listWidget02->item(row)->text() + ",";
        }
    }
    while (0);

    QString UPLDSQL;
    do
    {
        int rows = _UI.listWidget03->count();
        for (int row = 0; row < rows; row++)
        {
            UPLDSQL += _UI.listWidget03->item(row)->text() + ",";
        }
    }
    while (0);

    qCfgManager->_Config[CONFIG_SYN_DOWNSQL] = DOWNSQL;
    qCfgManager->_Config[CONFIG_SYN_UPLDSQL] = UPLDSQL;
    qCfgManager->Slot_QTimer_timeout_SetSync();

    accept();
}

void DialogSynConfig::Slot_PushButton_clicked_Reject()
{
    qDebug() << "Slot_PushButton_clicked_Reject";

    reject();
}

void DialogSynConfig::Slot_QListWidget_itemDoubleClicked_listWidget01(QListWidgetItem *item)
{
    qDebug() << "Slot_QListWidget_itemDoubleClicked_listWidget01" << item;

    int row = _UI.listWidget01->row(item);

    if (_UI.radio02->isChecked())
    {
        QList<QListWidgetItem *> ii = _UI.listWidget02->findItems(item->text(), Qt::MatchFixedString);
        if (ii.isEmpty())
        {
            _UI.listWidget02->addItem(_UI.listWidget01->takeItem(row));
        }
    }
    else
    {
        QList<QListWidgetItem *> ii = _UI.listWidget03->findItems(item->text(), Qt::MatchFixedString);
        if (ii.isEmpty())
        {
            _UI.listWidget03->addItem(_UI.listWidget01->takeItem(row));
        }
    }
}

void DialogSynConfig::Slot_QListWidget_itemDoubleClicked_listWidget02(QListWidgetItem *item)
{
    qDebug() << "Slot_QListWidget_itemDoubleClicked_listWidget02" << item;

    int row = _UI.listWidget02->row(item);

    QListWidgetItem *aitem = _UI.listWidget02->takeItem(row);

    _UI.listWidget01->addItem(aitem);
}

void DialogSynConfig::Slot_QListWidget_itemDoubleClicked_listWidget03(QListWidgetItem *item)
{
    qDebug() << "Slot_QListWidget_itemDoubleClicked_listWidget03" << item;

    int row = _UI.listWidget03->row(item);

    QListWidgetItem *aitem = _UI.listWidget03->takeItem(row);

    _UI.listWidget01->addItem(aitem);
}


