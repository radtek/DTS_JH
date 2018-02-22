/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      GDtsError.h
 *  \brief
 *      Global Error Code
 */
//!============================================================================
#ifndef GDTSERROR_H
#define GDTSERROR_H

/*!
 * Error Code Definition
 */
enum GERROR
{
    GERROR_OK                           = 0x00000000,   ///! 正确



    GERROR_XML_OPEN_FILE_FAIL           = 0x00002001,   ///< 打开XML文件失败
    GERROR_XML_PARSE_FILE_FAIL          = 0x00002002,   ///< 解析XML文件失败






    GERROR_APP_COMMAND_PARSE            = 0x00000010,   ///! 应用命令行解析错误
    GERROR_APP_INVALID_NAME             = 0x00000011,   ///! 应用名称非法
    GERROR_APP_ALREADY_RUN              = 0x00000012,   ///! 应用已经开启
    GERROR_APP_CREATE_FAIL              = 0x00000013,   ///! 应用锁创建失败
    GERROR_APP_CREATE_SQLMANAGER        = 0x00000017,   ///! 应用读取NAMINGSQL失败
    GERROR_APP_TASKWS_ERROR             = 0x00000014,   ///! 应用启动WebService失败
    GERROR_APP_TASKUL_ERROR             = 0x00000015,   ///! 应用启动数据库上传任务失败
    GERROR_APP_TASKSN_ERROR             = 0x00000016,   ///! 应用启动数据库同步任务失败


    GERROR_SYSTEM_REGEDIT_ERROR         = 0x00001000,   ///! 系统注册列表打开失败
    GERROR_SYSTEM_REGEDIT_NOTFOUND      = 0x00001001,   ///! 系统注册列表未查到
    GERROR_SYSTEM_FILENOTEXIST          = 0x00001002,   ///! 系统文件不存在

    GERROR_TASK_BINDPORT                = 0x00002001,   ///! 任务绑定端口失败
    GERROR_SQL_OPENFAILED                 = 0x00002002,   ///< Connect Database Failed
    GERROR_XML_READ_ERROR           = 0x00002003,   ///! 任务读取XML失败

    GERROR_FAIL                         = 0xFFFFFFFF
};

#endif // GDTSERROR_H
