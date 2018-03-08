/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GDtsDef.h
 * @brief   公共定义
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */


#ifndef DTS_MAIN_GDTSDEF_H
#define DTS_MAIN_GDTSDEF_H

#include "GStdDef.h"
#include "GDtsError.h"


typedef quint64                 QUInt64;
typedef qint64                  QInt64;
typedef quint32                 QUInt32;
typedef qint32                  QInt32;
typedef quint16                 QUInt16;
typedef qint16                  QInt16;
typedef quint8                  QUInt8;
typedef qint8                   QInt8;
typedef bool                    QBool;





#define Q_DECL_SINGLETON(Class)                \
    public:                                 \
    static Class &Instance();                \
    private:                                \
    Class();                                \
    ~Class();                               \
    Q_DISABLE_COPY(Class)                   \




#endif
