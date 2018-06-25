/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    CPCS_Upload_Info.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "CPCS_Upload_Info.h"

const QString CPCS_Upload_Info::UID = "UID";
const QString CPCS_Upload_Info::UploadTable = "UploadTable";
const QString CPCS_Upload_Info::RecodeLastTime = "RecodeLastTime";
const QString CPCS_Upload_Info::RecodeUploadCount = "RecodeUploadCount";
const QString CPCS_Upload_Info::UploadTime = "UploadTime";

const QString CPCS_Upload_Info::TAG_ARRAY = "ArrayOfPCS_Upload_Info";
const QString CPCS_Upload_Info::TAG_TABLE = "PCS_Upload_Info";
const QMap<QString, QVariant::Type> CPCS_Upload_Info::TAG_COLUMNS = {
    {CPCS_Upload_Info::UID, QVariant::LongLong},
    {CPCS_Upload_Info::UploadTable, QVariant::String},
    {CPCS_Upload_Info::RecodeLastTime, QVariant::DateTime},
    {CPCS_Upload_Info::RecodeUploadCount, QVariant::Int},
    {CPCS_Upload_Info::UploadTime, QVariant::DateTime},

};

CPCS_Upload_Info::CPCS_Upload_Info()
{
}
