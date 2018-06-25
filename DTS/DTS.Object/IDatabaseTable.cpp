/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    IDatabaseTable.cpp
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "IDatabaseTable.h"

IDatabaseTable::IDatabaseTable()
{
}

IDatabaseTable::~IDatabaseTable()
{
}

QString IDatabaseTable::className() const
{
    return "Unkown";
}

QDebug operator<<(QDebug dbg, const IDatabaseTable &item)
{
    QDebugStateSaver saver(dbg);

    QString value = item.className() + "{";
    for (auto it = item.data().constBegin(); it != item.data().constEnd(); ++it)
    {
        if (it != item.data().constBegin())
        {
            value += ",";
        }
        value += "(" % it.key() % ":\"" % it.value().toString() % "\")";
    }
    value += "}";

    dbg.nospace() << value;
    return dbg;
}
