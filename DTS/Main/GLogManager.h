/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      GLogManager.h
 *  \brief
 *      Manager-Logger
 */
//!============================================================================

#ifndef XYTEC_GLOGMANAGER_H
#define XYTEC_GLOGMANAGER_H

#include <QThread>
#include <QList>
#include <QMutex>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QElapsedTimer>
#include <fstream>


enum class ELogLevel
{
    LG_DBG  = 0,
    LG_INF  = 1,
    LG_WAR  = 2,
    LG_ERR  = 3,
};


#define DEFAULT_CONFIG_LG_ENAB      0
#define DEFAULT_CONFIG_LG_PATH      "../meslog/"
#define DEFAULT_CONFIG_LG_NAME      "mes.log"
#define DEFAULT_CONFIG_LG_MCNT      100
#define DEFAULT_CONFIG_LG_MSIZ      10
#define DEFAULT_CONFIG_LG_DLVL      static_cast<int>(ELogLevel::LG_DBG)
#define DEFAULT_CONFIG_LG_PATT      "%{time yyyy-MM-dd HH:mm:ss.zzz} %{if-debug}DBG%{endif}%{if-info}INF%{endif}%{if-warning}WAR%{endif}%{if-critical}ERR%{endif} TID:%{threadid} %{message}"


class TLogConfig
{
public:
    TLogConfig(bool isEnable = DEFAULT_CONFIG_LG_ENAB,
               const QString &pathName = DEFAULT_CONFIG_LG_PATH,
               const QString &fileName = DEFAULT_CONFIG_LG_NAME,
               int maxCount = DEFAULT_CONFIG_LG_MCNT,
               int maxSizeM = DEFAULT_CONFIG_LG_MSIZ,
               int defLevel = DEFAULT_CONFIG_LG_DLVL,
               const QString &pattern = DEFAULT_CONFIG_LG_PATT)
        : _IsEnable(isEnable), _PathName(pathName), _FileName(fileName), _MaxCount(maxCount), _MaxSizeM(maxSizeM), _DefLevel(defLevel), _SPattern(pattern)
    {

    }

private:
    bool        _IsEnable;
    QString     _PathName;
    QString     _FileName;
    int         _MaxCount;
    int         _MaxSizeM;
    int         _DefLevel;
    QString     _SPattern;

public:
    inline bool IsEnable()
    {
        return _IsEnable;
    }
    inline const QString &PathName()
    {
        return _PathName;
    }
    inline const QString &FileName()
    {
        return _FileName;
    }
    inline int MaxCount()
    {
        return _MaxCount;
    }
    inline int MaxSizeM()
    {
        return _MaxSizeM;
    }
    inline int DefLevel()
    {
        return _DefLevel;
    }
    inline const QString &SPattern()
    {
        return _SPattern;
    }
};


class GLogManager : public QObject
{
    Q_OBJECT

public:
    static GLogManager &Instance();
    static void OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf);

private:
    GLogManager();
    virtual ~GLogManager();

public:
    bool Initialize(const TLogConfig &config = TLogConfig());
    bool UnInitialize();

    void ResetConfig(const TLogConfig &config);

private:
    void changeDir();
    void changeFile();
    void writeLog(const QString &strMsg);

Q_SIGNALS:
    void sendMsg(const QString &strMsg);
    void recvMsg(const QString &strMsg);

public Q_SLOTS:
    void dealMsg(const QString &strMsg);

private:
    QThread             _RunThread;

    QMutex              _CfgLock;
    TLogConfig          _LogConfig;

    QMutex              _FileLock;
    std::fstream        _FileWrite;
};


class FunctionTrace
{
public:
    FunctionTrace(const char *fileName, int lineNumber, const char *functionName)
        : _File(fileName), _Line(lineNumber), _Func(functionName), _Tick()
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

#define qLogManager (&GLogManager::Instance())

#define TRACE_FUNCTION() \
    FunctionTrace functionTrace(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC);

#endif

