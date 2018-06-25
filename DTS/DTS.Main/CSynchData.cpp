/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    CSynchData.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "CSynchData.h"

QString CSynchData::Select()
{
    QString strSQL;

    strSQL += "SELECT ";
    if (synchColumns.size())
    {
        strSQL += QString("[%1]").arg(synchColumns[0]);
        for (int i = 1; i < synchColumns.size(); ++i)
        {
            strSQL += "," + QString("[%1]").arg(synchColumns[i]);
        }
    }
    else
    {
        strSQL += "*";
    }
    strSQL += " FROM " + synchTable;
    return strSQL;
}

QString CSynchData::Delete()
{
    QString strSQL = "DELETE FROM " + synchTable;

    return strSQL;
}

QString CSynchData::Insert()
{
    QString strSQL;

    strSQL += QString("SET IDENTITY_INSERT [%1] ON \n").arg(synchTable);
    if (synchColumns.size())
    {
        strSQL += "INSERT INTO " + synchTable + "(";

        strSQL += QString("[%1]").arg(synchColumns[0]);
        for (int i = 1; i < synchColumns.size(); ++i)
        {
            strSQL += "," + QString("[%1]").arg(synchColumns[i]);
        }

        strSQL += ") VALUES (";

        strSQL += ":" + synchColumns[0];
        for (int i = 1; i < synchColumns.size(); ++i)
        {
            strSQL += ",:" + synchColumns[i];
        }

        strSQL += ")\n";
    }
    strSQL += QString("SET IDENTITY_INSERT [%1] OFF ").arg(synchTable);
    return strSQL;
}

QString CSynchData::InsertWithoutUID()
{
    QString strSQL;

    if (synchColumns.size())
    {
        strSQL += "INSERT INTO " + synchTable + " VALUES (";

        for (int i = 0; i < synchColumns.size(); ++i)
        {
            if (synchColumns[i] == "UID")
            {
                continue;
            }
            strSQL += ":" + synchColumns[i];
            if (i != synchColumns.size() - 1)
            {
                strSQL += ",";
            }
        }

        strSQL += ")\n";
    }

    return strSQL;
}

QString CSynchData::Update()
{
    QString strSQL;

    if (synchColumns.size())
    {
        strSQL += "UPDATE " + synchTable + " SET ";

        strSQL += QString("[%1]=:%2").arg(synchColumns[0]).arg(synchColumns[0]);
        for (int i = 0; i < synchColumns.size(); ++i)
        {
            if (synchColumns[i] == "UID")
            {
                continue;
            }

            strSQL += QString("[%1]=:%2").arg(synchColumns[i]).arg(synchColumns[i]);

            if (i != synchColumns.size() - 1)
            {
                strSQL += ",";
            }
        }
        strSQL += " WHERE [UID]=:UID";
    }

    return strSQL;
}

QString CSynchData::Truncate()
{
    QString strSQL = "TRUNCATE TABLE " + synchTable;

    return strSQL;
}
