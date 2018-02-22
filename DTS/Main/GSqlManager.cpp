/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      GSqlManager.cpp
 *  \brief
 *      Manager-SQL
 *  \since
 *  001     2017/11/01      weiheng     create this file
 */
//!============================================================================

#include "GDtsData.h"


QString TSqlSync::SelectTable(bool local)
{
    QString strSQL;
    QString strTable;
    QStringList strColumn;
    if (local)
    {
        strTable = table_local;
        strColumn = column_local;
    }
    else
    {
        strTable = table_remote;
        strColumn = column_remote;
    }

    strSQL += "SELECT ";
    if (strColumn.size())
    {
        strSQL += QString("[%1]").arg(strColumn[0]);
        for (int i = 1; i < strColumn.size(); ++i)
        {
            strSQL += "," + QString("[%1]").arg(strColumn[i]);
        }
    }
    else
    {
        strSQL += "*";
    }
    strSQL += " FROM " + strTable;
    return strSQL;
}

QString TSqlSync::DeleteTable(bool local)
{
    QString strSQL;
    QString strTable;
    if (local)
    {
        strTable = table_local;
    }
    else
    {
        strTable = table_remote;
    }

    strSQL += "DELETE FROM ";
    strSQL += strTable;

    return strSQL;
}

QString TSqlSync::InsertTable(bool local)
{
    QString strSQL;
    QString strTable;
    QStringList strColumn;
    if (local)
    {
        strTable = table_local;
        strColumn = column_local;
    }
    else
    {
        strTable = table_remote;
        strColumn = column_remote;
    }

    strSQL += QString("SET IDENTITY_INSERT [%1] ON \n").arg(strTable);
    if (strColumn.size())
    {
        strSQL += "INSERT INTO " + strTable + "(";

        strSQL += QString("[%1]").arg(strColumn[0]);
        for (int i = 1; i < strColumn.size(); ++i)
        {
            strSQL += "," + QString("[%1]").arg(strColumn[i]) ;
        }

        strSQL += ") VALUES (";

        strSQL += ":" + strColumn[0];
        for (int i = 1; i < strColumn.size(); ++i)
        {
            strSQL += ",:" + strColumn[i] ;
        }

        strSQL += ")\n";
    }
    strSQL += QString("SET IDENTITY_INSERT [%1] OFF \n").arg(strTable);
    return strSQL;
}

QString TSqlSync::InsertTableNoUID(bool local)
{
    QString strSQL;
    QString strTable;
    QStringList strColumn;
    if (local)
    {
        strTable = table_local;
        strColumn = column_local;
    }
    else
    {
        strTable = table_remote;
        strColumn = column_remote;
    }

    if (strColumn.size())
    {
        strSQL += "INSERT INTO " + strTable + " VALUES (";

        for (int i = 0; i < strColumn.size(); ++i)
        {
            if (strColumn[i] == "UID")
            {
                continue;
            }
            strSQL += ":" + strColumn[i];
            if (i != strColumn.size() - 1)
            {
                strSQL += ",";
            }
        }

        strSQL += ")\n";
    }

    return strSQL;
}


GSqlManager &GSqlManager::Instance()
{
    static GSqlManager inst;
    return inst;
}


GSqlManager::GSqlManager()
{

}

GSqlManager::~GSqlManager()
{

}


GERROR GSqlManager::Initialize()
{
    QMap<QString, LPTSqlSync> tmpSync;
    if (GERROR_OK == readSqlSync(DEFAULT_SQLSYNC_PATH, tmpSync))
    {
        _SqlSync = tmpSync;
    }

    return GERROR_OK;
}

GERROR GSqlManager::UnInitialize()
{
    _SqlSync.clear();

    return GERROR_OK;
}

bool GSqlManager::SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantMap &dbParam)
{
    qDebug().noquote() << "\n==========\n" << dbSQL << "\n==========\n" << dbParam << "\n==========";

    dbQuery.clear();

    if (!dbQuery.prepare(dbSQL))
    {
        qWarning().noquote() << QString("[SQL] PREPARE Failed! Error{%1}").arg(dbQuery.lastError().text());
        return false;
    }

    for (auto it = dbParam.begin(); it != dbParam.end(); ++it)
    {
        dbQuery.bindValue(it.key(), it.value());
    }

    if (!dbQuery.exec())
    {
        qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
        return false;
    }

    return true;
}

bool GSqlManager::SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantList &dbParam)
{
    qDebug().noquote() << "\n==========\n" << dbSQL << "\n==========\n" << dbParam << "\n==========";

    dbQuery.clear();

    if (!dbQuery.prepare(dbSQL))
    {
        qWarning().noquote() << QString("[SQL] PREPARE Failed! Error{%1}").arg(dbQuery.lastError().text());
        return false;
    }

    for (auto it = dbParam.begin(); it != dbParam.end(); ++it)
    {
        dbQuery.addBindValue(*it);
    }

    if (!dbQuery.exec())
    {
        qWarning().noquote() << QString("[SQL] EXEC Failed! Error{%1}").arg(dbQuery.lastError().text());
        return false;
    }

    return true;
}

GERROR GSqlManager::readSqlSync(const QString &filename, QMap<QString, LPTSqlSync> &tmpSync)
{
    tmpSync.clear();

    QDomDocument xmldoc;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning().noquote() << QString("Open XML file error, FileName[%1]").arg(file.fileName());
        return GERROR_XML_OPEN_FILE_FAIL;
    }

    QByteArray strContent = file.readAll();
    file.close();

    QString errorStr;
    int errorLine = 0;
    int errorCol = 0;
    if (!xmldoc.setContent(strContent, true, &errorStr, &errorLine, &errorCol))
    {
        qWarning().noquote() << QString("Parse XML file error, Error[%1], Position[ln:%2 col:%3]").arg(errorStr).arg(errorLine).arg(errorCol);
        return GERROR_XML_PARSE_FILE_FAIL;
    }

    do
    {
        QDomElement xmlElemRoot = xmldoc.documentElement();

        QDomElement xmlElemItemSQL = xmlElemRoot.firstChildElement("ItemSQL");
        while (!xmlElemItemSQL.isNull())
        {
            do
            {
                QString id = xmlElemItemSQL.attribute("id");

                if (id.isEmpty() || tmpSync.contains(id))
                {
                    break;
                }

                QDomElement xmlElemDBRemote = xmlElemItemSQL.firstChildElement("DBRemote");
                if (xmlElemDBRemote.isNull())
                {
                    break;
                }
                QString DBRemote = xmlElemDBRemote.text();

                QDomElement xmlElemDBLocal = xmlElemItemSQL.firstChildElement("DBLocal");
                if (xmlElemDBLocal.isNull())
                {
                    break;
                }
                QString DBLocal = xmlElemDBLocal.text();

                QDomElement xmlElemTableRemote = xmlElemItemSQL.firstChildElement("TableRemote");
                if (xmlElemTableRemote.isNull())
                {
                    break;
                }
                QString TableRemote = xmlElemTableRemote.text();

                QDomElement xmlElemTableLocal = xmlElemItemSQL.firstChildElement("TableLocal");
                if (xmlElemTableLocal.isNull())
                {
                    break;
                }
                QString TableLocal = xmlElemTableLocal.text();

                QDomElement xmlElemColumnRemote = xmlElemItemSQL.firstChildElement("ColumnRemote");
                if (xmlElemColumnRemote.isNull())
                {
                    break;
                }
                QString sColumnRemote = xmlElemColumnRemote.text();
                QStringList ColumnRemote = sColumnRemote.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);
                for (auto &str : ColumnRemote)
                {
                    str = str.trimmed();
                }

                QDomElement xmlElemColumnLocal = xmlElemItemSQL.firstChildElement("ColumnLocal");
                if (xmlElemColumnLocal.isNull())
                {
                    break;
                }
                QString sColumnLocal = xmlElemColumnLocal.text();
                QStringList ColumnLocal = sColumnLocal.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);
                for (auto &str : ColumnLocal)
                {
                    str = str.trimmed();
                }

                do
                {
                    LPTSqlSync tSQLManager(new TSqlSync());
                    tSQLManager->id = id;
                    tSQLManager->db_remote = DBRemote;
                    tSQLManager->db_local = DBLocal;
                    tSQLManager->table_remote = TableRemote;
                    tSQLManager->table_local = TableLocal;
                    tSQLManager->column_remote = ColumnRemote;
                    tSQLManager->column_local = ColumnLocal;

                    tmpSync.insert(id, tSQLManager);
                }
                while (0);
            }
            while (0);

            xmlElemItemSQL = xmlElemItemSQL.nextSiblingElement();
        }
    }
    while (0);

    return GERROR_OK;
}

