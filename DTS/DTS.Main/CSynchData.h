/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    CSynchData.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_MAIN_CSYNCHDATA_H
#define DTS_MAIN_CSYNCHDATA_H

#include <QtCore>

class CSynchData
{
public:
    QString synchID;
    QString synchTable;
    QStringList synchColumns;

public:
    QString Select();
    QString Delete();
    QString Insert();
    QString InsertWithoutUID();
    QString Update();
    QString Truncate();
};

#endif
