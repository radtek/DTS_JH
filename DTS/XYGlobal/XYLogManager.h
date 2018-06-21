/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYLogManager.h
 * @brief   日志输出类
 * @version 1.0
 * @note
 *  不采用独立线程，及时输出到控制台和文件
 * *****************************************************************************
 */

#ifndef XYTECH_XYLOGMANAGER_H
#define XYTECH_XYLOGMANAGER_H

#include <QtCore>

class XYLogManagerPrivate;
class XYLogReloader;
class XYLogManager
{
    Q_DISABLE_COPY(XYLogManager)
    Q_DECLARE_PRIVATE(XYLogManager)

    friend class XYLogReloader;

public:
    enum ELogLevel
    {
        LG_ALL = 0,
        LG_DBG,
        LG_INF,
        LG_WAR,
        LG_ERR,
        LG_OFF,
    };

public:
    static XYLogManager *instance();
    static void OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf);

public:
    XYLogManager();
    virtual ~XYLogManager();

public:
    void setConfigFile(const QString &file);
    void allocConsole();
    void freeConsole();
    bool hasConsole();

private:
    void execMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &buf);
    void execAllocConsole();
    void execFreeConsole();

public:
    static bool isTrace;

private:
    const QScopedPointer<XYLogManagerPrivate> d_ptr;
    QScopedPointer<XYLogReloader> d_reloader;
};

class FunctionTrace
{
public:
    FunctionTrace(const char *fileName, int lineNumber, const char *functionName, bool isEnable = true)
        : file(fileName)
        , line(lineNumber)
        , func(functionName)
        , enable(isEnable)
        , tick()
    {
        tick.start();
        if (enable && XYLogManager::isTrace)
        {
            QMessageLogger(file, line, func).debug().noquote() << QString("<------ IN ") % func;
        }
    }
    ~FunctionTrace()
    {
        if (enable)
        {
            QMessageLogger(file, line, func).debug().noquote() << QString("------> COST [%1ms]").arg(tick.elapsed());
        }
    }

private:
    const char *file;
    int line;
    const char *func;
    bool enable;
    QElapsedTimer tick;
};

#define TRACE_FUNCTION0(enable) \
    FunctionTrace functionTrace(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, enable);

#define TRACE_FUNCTION() \
    TRACE_FUNCTION0(true)

#define qTrace                 \
    if (XYLogManager::isTrace) \
    qDebug

#endif
