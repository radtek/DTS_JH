/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    CPCS_Upload_Info.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_OBJECT_CPCS_UPLOAD_INFO_H
#define DTS_OBJECT_CPCS_UPLOAD_INFO_H

#include "IDatabaseTable.h"

class CPCS_Upload_Info : public IDatabaseTable
{
public:
    CPCS_Upload_Info();

public:
    virtual QString className() const
    {
        return TAG_TABLE;
    }

public:
    static const QString UID;
    static const QString UploadTable;
    static const QString RecodeLastTime;
    static const QString RecodeUploadCount;
    static const QString UploadTime;

public:
    static const QString TAG_ARRAY;
    static const QString TAG_TABLE;
    static const QMap<QString, QVariant::Type> TAG_COLUMNS;
};

#endif
