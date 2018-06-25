/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    IDatabaseTable.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/06/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_OBJECT_IDATABASETABLE_H
#define DTS_OBJECT_IDATABASETABLE_H

#include <QtCore>

class IDatabaseTable
{
public:
    IDatabaseTable();
    virtual ~IDatabaseTable();

public:
    virtual QString className() const;

public:
    QVariant getData(const QString &column) const
    {
        return map.value(column);
    }
    void setData(const QString &column, const QVariant &value)
    {
        map[column] = value;
    }
    const QMap<QString, QVariant> &data() const
    {
        return map;
    }

protected:
    QMap<QString, QVariant> map;
};

QDebug operator<<(QDebug dbg, const IDatabaseTable &item);

#endif
