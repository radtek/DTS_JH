/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GLogManager.h
 * @brief   日志输出类，可独立运行
 * @version 1.0
 * @note
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef XYTEC_GLOGMANAGER_H
#define XYTEC_GLOGMANAGER_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QList>
#include <QMutex>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <fstream>

enum class ELogLevel
{
    LG_DBG = 0,
    LG_INF = 1,
    LG_WAR = 2,
    LG_ERR = 3,
};

#define DEFAULT_CONFIG_LOGMANAGER "configlog.ini"

#define TAG_CONFIG_LG_TARG "Target"
#define DEFAULT_CONFIG_LG_TARG 1

#define TAG_CONFIG_LG_PATT "Pattern"
#define DEFAULT_CONFIG_LG_PATT "%{time yyyy-MM-dd HH:mm:ss.zzz} %{message}"

#define TAG_CONFIG_LG_PATH "Path"
#define DEFAULT_CONFIG_LG_PATH "."

#define TAG_CONFIG_LG_FILE "File"
#define DEFAULT_CONFIG_LG_FILE "run.log"

#define TAG_CONFIG_LG_MCNT "MaxCount" *

#define DEFAULT_CONFIG_LG_MCNT 100

#define TAG_CONFIG_LG_MSIZ "MaxSize"
#define DEFAULT_CONFIG_LG_MSIZ 10

#define TAG_CONFIG_LG_DLVL "DefaultLevel"
#define DEFAULT_CONFIG_LG_DLVL static_cast<int>(ELogLevel::LG_DBG)

class GLogManager : public QObject
{
    Q_OBJECT

public:
    static GLogManager &Instance();
    static void OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf);

private:
    GLogManager();
    virtual ~GLogManager();

private:
    void qMessageHandler(QtMsgType type, const QString &message);
    void changeDir();
    void changeFile();
    void writeLog(const QString &message);

Q_SIGNALS:
    void sendMsg(const QString &message);
    void recvMsg(const QString &message);

public Q_SLOTS:
    void dealMsg(const QString &message);
    void resetConfig();

private:
    QThread _RunThread;

    QMutex _FileLock;
    std::fstream _FileWriter;

    QMutex _CfgLock;
    QTimer _CfgReload;
    int _Target;
    QString _Pattern;
    QString _PathName;
    QString _FileName;
    unsigned int _MaxCount;
    unsigned int _MaxSize;
    int _DefaultLevel;
};

#define qLogManager (&GLogManager::Instance())

class FunctionTrace
{
public:
    FunctionTrace(const char *fileName, int lineNumber, const char *functionName)
        : _File(fileName)
        , _Line(lineNumber)
        , _Func(functionName)
        , _Tick()
    {
        _Tick.start();
        QMessageLogger(_File, _Line, _Func).debug().noquote() << QString("->> %1").arg(_Func);
    }
    ~FunctionTrace()
    {
        QMessageLogger(_File, _Line, _Func).debug().noquote() << QString("<<- %1 [%2]").arg(_Func).arg(_Tick.elapsed());
    }

private:
    const char *_File;
    int _Line;
    const char *_Func;
    QElapsedTimer _Tick;
};

#define TRACE_FUNCTION() \
    FunctionTrace functionTrace(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC);

#endif
