/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GCfgManager.cpp
 * @brief   Configuration Manager
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "GCfgManager.h"
#include "CSynchData.h"

QSqlDatabase DB_LOCAL;
QSqlDatabase DB_REMOTE;

Q_GLOBAL_STATIC(GCfgManager, theInstance);

GCfgManager *GCfgManager::instance()
{
    return theInstance();
}

GCfgManager::GCfgManager()
{
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, "SOFTWARE");
}

GCfgManager::~GCfgManager()
{
}

bool GCfgManager::initialize()
{
    // check settings status
    if (settings.status() != QSettings::NoError)
    {
        qCritical().noquote() << QString("Open regedit failed, error[%1]").arg(settings.status());
        return false;
    }

    // delete invalid
    Q_FOREACH (auto &key, settings.childGroups())
    {
        if (settings.value(QString("%1/%2").arg(key).arg(CONFIG_SYS_APPNAME)).isNull())
        {
            qWarning().noquote() << QString("delete invalid regedit item[%1]").arg(key);
            settings.remove(key);
            settings.sync();
        }
    }
    return true;
}

QMap<QString, QString> GCfgManager::getApplicationList()
{
    QMap<QString, QString> map;
    Q_FOREACH (auto &key, settings.childGroups())
    {
        QString var = settings.value(QString("%1/%2").arg(key).arg(CONFIG_SYS_APPNAME)).toString();
        if (!var.isEmpty())
        {
            map.insert(key, var);
        }
    }
    return map;
}

bool GCfgManager::checkApplicationExist(const QString &appID)
{
    return settings.childGroups().contains(appID);
}

bool GCfgManager::createApplication(const QMap<QString, QString> &appConfig, bool update)
{
    QString strAppID = appConfig[CONFIG_SYS_APPID];
    if (strAppID.isEmpty())
    {
        return false;
    }

    if (update != checkApplicationExist(strAppID))
    {
        return false;
    }

    settings.beginGroup(strAppID);
    for (auto it = appConfig.begin(); it != appConfig.end(); ++it)
    {
        settings.setValue(it.key(), it.value());
    }
    settings.endGroup();
    settings.sync();

    return true;
}

bool GCfgManager::deleteApplication(const QString &appID)
{
    settings.remove(appID);
    settings.sync();

    return true;
}

bool GCfgManager::loadApplication(const QString &appID)
{
    if (!checkApplicationExist(appID))
    {
        return false;
    }

    config.clear();
    synch.clear();

    settings.beginGroup(appID);
    Q_FOREACH (auto &key, settings.childKeys())
    {
        config[key] = settings.value(key).toString();
    }
    settings.endGroup();
    currAppID = appID;

    if (!initConnection())
    {
        return false;
    }

    if (!importSynchData(DEFAULT_SQLSYNC_PATH))
    {
        return false;
    }
    return true;
}

bool GCfgManager::initConnection()
{
    QString strConnect = "Driver={sql server};SERVER=%1;PORT=%2;UID=%3;PWD=%4;DATABASE=%5;";
    do
    {
        QString dsn = strConnect.arg(config[CONFIG_DB_LOCAL_ADDR])
                          .arg(config[CONFIG_DB_LOCAL_PORT])
                          .arg(config[CONFIG_DB_LOCAL_USER])
                          .arg(config[CONFIG_DB_LOCAL_PASS])
                          .arg(config[CONFIG_DB_LOCAL_NAME]);

        DB_LOCAL = QSqlDatabase::addDatabase("QODBC", QSQL_DB_STRING_LOCAL);
        DB_LOCAL.setDatabaseName(dsn);
        qDebug().noquote() << QString("Create local database connection,[%1]").arg(dsn);
        if (DB_LOCAL.open())
        {
            qInfo().noquote() << "Create success.";
        }
        else
        {
            qWarning().noquote() << "Create failed.";
            return false;
        }

    } while (false);

    // Remote
    do
    {
        QString dsn = strConnect.arg(config[CONFIG_DB_REMOTE_ADDR])
                          .arg(config[CONFIG_DB_REMOTE_PORT])
                          .arg(config[CONFIG_DB_REMOTE_USER])
                          .arg(config[CONFIG_DB_REMOTE_PASS])
                          .arg(config[CONFIG_DB_REMOTE_NAME]);

        DB_REMOTE = QSqlDatabase::addDatabase("QODBC", QSQL_DB_STRING_REMOTE);
        DB_REMOTE.setDatabaseName(dsn);

        qDebug().noquote() << QString("Create remote database connection,[%1]").arg(dsn);
        if (DB_REMOTE.open())
        {
            qInfo().noquote() << "Create success.";
        }
        else
        {
            qWarning().noquote() << "Create failed.";
            return false;
        }

    } while (false);

    return true;
}

bool GCfgManager::importSynchData(const QString &filename)
{
    QDomDocument xmldoc;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning().noquote() << QString("Open XML file error, FileName[%1]").arg(file.fileName());
        return false;
    }

    QByteArray strContent = file.readAll();
    file.close();

    QString errorStr;
    int errorLine = 0;
    int errorCol = 0;
    if (!xmldoc.setContent(strContent, true, &errorStr, &errorLine, &errorCol))
    {
        qWarning().noquote() << QString("Parse XML file error, Error[%1], Position[ln:%2 col:%3]").arg(errorStr).arg(errorLine).arg(errorCol);
        return false;
    }

    do
    {
        QDomElement xmlElemRoot = xmldoc.documentElement();

        QDomElement xmlElemItemSQL = xmlElemRoot.firstChildElement("ItemSQL");
        while (!xmlElemItemSQL.isNull())
        {
            do
            {
                QString ID = xmlElemItemSQL.attribute("id");
                if (ID.isEmpty() || synch.contains(ID))
                {
                    break;
                }

                QDomElement xmlElemTable = xmlElemItemSQL.firstChildElement("table");
                if (xmlElemTable.isNull())
                {
                    break;
                }
                QString Table = xmlElemTable.text();

                QDomElement xmlElemColumn = xmlElemItemSQL.firstChildElement("column");
                if (xmlElemColumn.isNull())
                {
                    break;
                }
                QString ColumnList = xmlElemColumn.text();
                QStringList Columns = ColumnList.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);
                for (auto &str : Columns)
                {
                    str = str.trimmed();
                }

                do
                {
                    CSynchDataPtr tSynchData(new CSynchData());
                    tSynchData->synchID = ID;
                    tSynchData->synchTable = Table;
                    tSynchData->synchColumns = Columns;

                    synch.insert(ID, tSynchData);
                } while (false);
            } while (false);

            xmlElemItemSQL = xmlElemItemSQL.nextSiblingElement();
        }
    } while (false);

    return true;
}

bool GCfgManager::exportSynchData(const QString &filename)
{
    QSqlDatabase dbSQL = QSqlDatabase::database(QSQL_DB_STRING_REMOTE);
    if (!dbSQL.isOpen())
    {
        return false;
    }

    QStringList dbTable = dbSQL.tables();
    QMap<QString, QStringList> dbTableColumn;
    for (auto &str : dbTable)
    {
        QSqlRecord record = dbSQL.record(str);
        QStringList ColumnName;
        for (int i = 0; i < record.count(); ++i)
        {
            ColumnName.append(record.fieldName(i));
        }
        dbTableColumn.insert(str, ColumnName);
    }

    do
    {
        QDomDocument document;
        QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", R"(version="1.0" encoding="UTF-8")");
        document.appendChild(instruction);

        QDomElement xmlElemRoot = document.createElement("root");
        document.appendChild(xmlElemRoot);

        for (auto iter = dbTableColumn.begin(); iter != dbTableColumn.end(); ++iter)
        {
            QDomElement xmlElemItemSQL = document.createElement("ItemSQL");
            xmlElemRoot.appendChild(xmlElemItemSQL);

            xmlElemItemSQL.setAttribute("id", iter.key());

            do
            {
                QDomElement xmlElemTable = document.createElement("table");
                xmlElemItemSQL.appendChild(xmlElemTable);
                QDomText xmlTextTable = document.createTextNode(iter.key());
                xmlElemTable.appendChild(xmlTextTable);
            } while (0);

            do
            {
                QDomElement xmlElemColumn = document.createElement("column");
                xmlElemItemSQL.appendChild(xmlElemColumn);
                QString str;
                str = iter.value()[0];
                for (int i = 1; i < iter.value().size(); i++)
                {
                    str += "," + iter.value()[i];
                }
                QDomText xmlTextColumn = document.createTextNode(str);
                xmlElemColumn.appendChild(xmlTextColumn);
            } while (false);
        }

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            qDebug() << "file error";
            return false;
        }
        QTextStream out(&file);
        out.setCodec("UTF-8");
        document.save(out, 4, QDomNode::EncodingFromTextStream);
        file.close();
    } while (false);

    return true;
}

void GCfgManager::setConfig(const QMap<QString, QString> &currConfig)
{
    for (auto it = currConfig.constBegin(); it != currConfig.constEnd(); ++it)
    {
        config[it.key()] = it.value();
    }

    settings.beginGroup(currAppID);
    for (auto it = config.constBegin(); it != config.constEnd(); ++it)
    {
        settings.setValue(it.key(), it.value());
    }
    settings.endGroup();
    settings.sync();
}

QStringList GCfgManager::getSynchDataKeys()
{
    return synch.keys();
}

QString GCfgManager::getVersion()
{
    static const char *major_version = "0";
    static const char *minor_version = "0";
    static const char *datetime = __TIMESTAMP__;

    return QString("%1.%2.%3")
        .arg(major_version)
        .arg(minor_version)
        .arg(QDateTime::fromString(datetime).toString("yyMMdd"));
}

QString GCfgManager::getWorkDir()
{
    return config[CONFIG_SYS_APPID];
}

QString GCfgManager::getWSLocalAddr()
{
    return config[CONFIG_WS_LOCAL_ADDR];
}

QString GCfgManager::getWSRemoteUrl()
{
    return config[CONFIG_WS_REMOTE_URL];
}

int GCfgManager::getWSLocalPort()
{
    return config[CONFIG_WS_LOCAL_PORT].toInt();
}

QStringList GCfgManager::getDownload()
{
    return config[CONFIG_RUN_DOWNLOAD].split(",", QString::SkipEmptyParts, Qt::CaseSensitive);
}

QStringList GCfgManager::getUpload()
{
    return config[CONFIG_RUN_UPLOAD].split(",", QString::SkipEmptyParts, Qt::CaseSensitive);
}

int GCfgManager::getDownloadTimeSpan()
{
    return config[CONFIG_RUN_DOWNLOAD_TIMESPAN].toInt();
}

int GCfgManager::getUploadTimeSpan()
{
    return config[CONFIG_RUN_UPLOAD_TIMESPAN].toInt();
}

CSynchDataPtr GCfgManager::getSynchData(const QString &id)
{
    return synch.value(id);
}

int GCfgManager::getGasWeightInitTime()
{
    return config[CONFIG_RUN_UPLOAD_TIMESPAN].toInt();
}
