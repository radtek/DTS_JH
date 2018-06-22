/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYLogManager.cpp
 *
 * @example
 * @code
 * int main()
 * {
 *      qInstallMessageHandler(XYLogManager::OutputMessage);
 *      qDebug() << "Test Debug()";
 *      qInfo() << "Test Info()";
 * }
 * @endcode
 *
 * @brief
 *  Default configure file is `config.ini`.
 *
 * @date 2018-03-27 @author weiheng             @note create this file
 * @date 2018-03-28 @author weiheng             @note remove level `qFatal`
 * @date 2018-03-30 @author weiheng             @note add console color
 * @date 2018-04-09 @author weiheng             @note add reloader in another thread
 * @date 2018-04-10 @author weiheng             @note add macro `qTrace`
 * *****************************************************************************
 */
#include "XYLogManager.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#ifdef Q_OS_WIN
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#endif

#define DEFAULT_CONFIG_LOGMANAGER "config.ini"

#define DEFAULT_CONFIG_LG_DIR "./run/"
#define DEFAULT_CONFIG_LG_FILE "mes"
#define DEFAULT_CONFIG_LG_SUFFIX ".log"
#define DEFAULT_CONFIG_LG_MAXCOUNT 100
#define DEFAULT_CONFIG_LG_MAXSIZE 10
#define DEFAULT_CONFIG_LG_LEVEL static_cast<quint32>(XYLogManager::LG_DBG)
#define DEFAULT_CONFIG_LG_PATTERN                                                                                         \
    "%{time yyyy-MM-dd HH:mm:ss.zzz} "                                                                                    \
    "%{if-debug}DBG%{endif}%{if-info}INF%{endif}%{if-warning}WAR%{endif}%{if-critical}ERR%{endif}%{if-fatal}CRI%{endif} " \
    "PID:%{pid} TID:%{threadid} "                                                                                         \
    "%{message}"

#define TAG_CONFIG_LGGROUP "LOG"
#define TAG_CONFIG_LG_PATH "Path"
#define TAG_CONFIG_LG_FILE "File"
#define TAG_CONFIG_LG_MCNT "MaxCount"
#define TAG_CONFIG_LG_MSIZ "MaxSize"
#define TAG_CONFIG_LG_DLVL "DefaultLevel"
#define TAG_CONFIG_LG_PATT "Pattern"

#define CONSOLE_COLOR_BEGIN_DEBUG "\033[0m"
#define CONSOLE_COLOR_BEGIN_INFO "\033[0;32m"
#define CONSOLE_COLOR_BEGIN_WARNING "\033[47;31m"
#define CONSOLE_COLOR_BEGIN_CRITICAL "\033[40;33m"

#define CONSOLE_COLOR_END "\033[0m"

const QMap<QtMsgType, QString> consoleColorBegin = {
    {QtDebugMsg, CONSOLE_COLOR_BEGIN_DEBUG},
    {QtInfoMsg, CONSOLE_COLOR_BEGIN_INFO},
    {QtWarningMsg, CONSOLE_COLOR_BEGIN_WARNING},
    {QtCriticalMsg, CONSOLE_COLOR_BEGIN_CRITICAL},
    {QtFatalMsg, CONSOLE_COLOR_BEGIN_CRITICAL},
};

const QMap<QtMsgType, QString> consoleColorEnd = {
    {QtDebugMsg, CONSOLE_COLOR_END},
    {QtInfoMsg, CONSOLE_COLOR_END},
    {QtWarningMsg, CONSOLE_COLOR_END},
    {QtCriticalMsg, CONSOLE_COLOR_END},
    {QtFatalMsg, CONSOLE_COLOR_END},
};

////////////////////////////////////////////////////////////////////////////////
/// \brief The XYLogManagerPrivate class
///
class XYLogManagerPrivate
{
    Q_DISABLE_COPY(XYLogManagerPrivate)
    Q_DECLARE_PUBLIC(XYLogManager)

public:
    XYLogManagerPrivate(XYLogManager *q);
    ~XYLogManagerPrivate();

    void init();
    void changeDir();
    void changeFile();
    void write(const QString &message);

    XYLogManager *const q_ptr;

    QMutex mutex;
    QFile file;

    QString dirName;
    QString fileName;
    quint32 maxCount;
    quint32 maxSize;
    quint32 currLevel;

    bool hasConsole;
};

XYLogManagerPrivate::XYLogManagerPrivate(XYLogManager *q)
    : q_ptr(q)
    , mutex(QMutex::Recursive)
    , dirName(DEFAULT_CONFIG_LG_DIR)
    , fileName(DEFAULT_CONFIG_LG_FILE)
    , maxCount(DEFAULT_CONFIG_LG_MAXCOUNT)
    , maxSize(DEFAULT_CONFIG_LG_MAXSIZE)
    , currLevel(DEFAULT_CONFIG_LG_LEVEL)
    , hasConsole(false)
{
}

XYLogManagerPrivate::~XYLogManagerPrivate()
{
    file.close();
}

void XYLogManagerPrivate::init()
{
    changeDir();
}

void XYLogManagerPrivate::changeDir()
{
    QMutexLocker locker(&mutex);

    QDir dir(".");
    if (dir.mkpath(dirName))
    {
        file.close();
        file.setFileName(dirName + fileName + DEFAULT_CONFIG_LG_SUFFIX);
        file.open(QIODevice::Append);
    }
}

void XYLogManagerPrivate::changeFile()
{
    QMutexLocker locker(&mutex);

    QString strOldName = dirName + fileName;
    QString strNewName = strOldName + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QFile::rename(strOldName + DEFAULT_CONFIG_LG_SUFFIX, strNewName + DEFAULT_CONFIG_LG_SUFFIX);

    QDir dir(dirName);
    QStringList entryList = dir.entryList(QStringList(fileName + "*"));
    int num = entryList.size() - static_cast<int>(maxCount);
    if (num > 0)
    {
        for (int i = 0; i < num; ++i)
        {
            QFile::remove(dirName + entryList[i]);
        }
    }
}

void XYLogManagerPrivate::write(const QString &message)
{
    QMutexLocker locker(&mutex);

    if (file.isOpen())
    {
        if (file.size() > maxSize * (1 * 1024 * 1024))
        {
            file.close();
            changeFile();
        }
    }
    else
    {
        changeDir();
    }

    if (file.isOpen())
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << message << endl;
        stream.flush();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \brief The XYLogReloader class
///
class XYLogReloader : public QThread
{
public:
    XYLogReloader(XYLogManager *parent)
        : QThread()
        , parent(parent)
        , filename(DEFAULT_CONFIG_LOGMANAGER)
    {
    }
    virtual ~XYLogReloader()
    {
    }

public:
    void init();
    void setConfigFile(const QString &file);

protected:
    virtual void run() Q_DECL_OVERRIDE;

private:
    void reloadConfig();

private:
    XYLogManager *parent;
    QString filename;
};

void XYLogReloader::init()
{
    reloadConfig();
}

void XYLogReloader::setConfigFile(const QString &file)
{
    filename = file;
}

void XYLogReloader::run()
{
    while (true)
    {
        QThread::sleep(30);
        reloadConfig();
    }
}

void XYLogReloader::reloadConfig()
{
    QString pattern = DEFAULT_CONFIG_LG_PATTERN;
    QString dirName = DEFAULT_CONFIG_LG_DIR;
    QString fileName = DEFAULT_CONFIG_LG_FILE;
    quint32 maxCount = DEFAULT_CONFIG_LG_MAXCOUNT;
    quint32 maxSize = DEFAULT_CONFIG_LG_MAXSIZE;
    quint32 currLevel = DEFAULT_CONFIG_LG_LEVEL;

    bool bSuccess = true;
    do
    {
        QSettings settings(filename, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        if (settings.status() != QSettings::NoError)
        {
            bSuccess = false;
            break;
        }

        settings.beginGroup(TAG_CONFIG_LGGROUP);

        pattern = settings.value(TAG_CONFIG_LG_PATT, DEFAULT_CONFIG_LG_PATTERN).toString();
        if (pattern.isEmpty())
        {
            pattern = DEFAULT_CONFIG_LG_PATTERN;
        }

        dirName = settings.value(TAG_CONFIG_LG_PATH, DEFAULT_CONFIG_LG_DIR).toString();
        if (dirName.right(1) != QLatin1Char('/') && dirName.right(1) != QLatin1Char('\\'))
        {
            dirName.append(QLatin1Char('/'));
        }

        fileName = settings.value(TAG_CONFIG_LG_FILE, DEFAULT_CONFIG_LG_FILE).toString();
        if (fileName.isEmpty())
        {
            fileName = DEFAULT_CONFIG_LG_FILE;
        }

        maxCount = settings.value(TAG_CONFIG_LG_MCNT, DEFAULT_CONFIG_LG_MAXCOUNT).toUInt();

        maxSize = settings.value(TAG_CONFIG_LG_MSIZ, DEFAULT_CONFIG_LG_MAXSIZE).toUInt();
        if (maxSize == 0)
        {
            maxSize = DEFAULT_CONFIG_LG_MAXSIZE;
        }

        QString tmpStr = settings.value(TAG_CONFIG_LG_DLVL, DEFAULT_CONFIG_LG_LEVEL).toString();
        if (tmpStr.isEmpty())
        {
            currLevel = DEFAULT_CONFIG_LG_LEVEL;
        }
        else
        {
            currLevel = tmpStr.toUInt();
        }

        settings.endGroup();
    } while (0);

    if (bSuccess)
    {
        QMutexLocker locker(&parent->d_func()->mutex);

        parent->d_func()->maxCount = maxCount;
        parent->d_func()->maxSize = maxSize;

        if (parent->d_func()->dirName != dirName || parent->d_func()->fileName != fileName)
        {
            parent->d_func()->dirName = dirName;
            parent->d_func()->fileName = fileName;
            parent->d_func()->changeDir();
        }

        // 增加Trace级别日志
        if (XYLogManager::LG_ALL == currLevel)
        {
            XYLogManager::isTrace = true;
        }
        else
        {
            XYLogManager::isTrace = false;
        }

        parent->d_func()->currLevel = currLevel;

        // DEBUG级别以下打印文件和行号
        if (currLevel <= XYLogManager::LG_DBG)
        {
            if (!pattern.contains("%{file}"))
            {
                pattern.append("\t\t\t(%{file}:%{line})");
            }
        }
        qSetMessagePattern(pattern);
    }
}

////////////////////////////////////////////////////////////////////////////////

Q_GLOBAL_STATIC(XYLogManager, theInstance);

bool XYLogManager::isTrace = true;

XYLogManager *XYLogManager::instance()
{
    return theInstance();
}

void XYLogManager::OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf)
{
    XYLogManager::instance()->execMessageHandler(type, context, buf);
}

XYLogManager::XYLogManager()
    : d_ptr(new XYLogManagerPrivate(this))
    , d_reloader(new XYLogReloader(this))
{
    qSetMessagePattern(DEFAULT_CONFIG_LG_PATTERN);

    d_reloader->start();
}

XYLogManager::~XYLogManager()
{
    d_reloader->terminate();
    d_reloader->wait();
}

void XYLogManager::setConfigFile(const QString &file)
{
    if (d_reloader)
    {
        d_reloader->setConfigFile(file);
        d_reloader->init();
        d_func()->init();
    }
}

void XYLogManager::allocConsole()
{
    Q_D(XYLogManager);

    QMutexLocker locker(&d->mutex);
    if (!d->hasConsole)
    {
        execAllocConsole();

        d->hasConsole = true;
    }
}

void XYLogManager::freeConsole()
{
    Q_D(XYLogManager);

    QMutexLocker locker(&d->mutex);
    if (d->hasConsole)
    {
        execFreeConsole();

        d->hasConsole = false;
    }
}

bool XYLogManager::hasConsole()
{
    Q_D(XYLogManager);

    QMutexLocker locker(&d->mutex);
    return d->hasConsole;
}

void XYLogManager::execMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &buf)
{
    QString message = qFormatLogMessage(type, context, buf);

#ifdef Q_OS_WIN
    QString console = consoleColorBegin[type] + message + consoleColorEnd[type] + QLatin1Char('\n');
    OutputDebugString(reinterpret_cast<const wchar_t *>(console.utf16()));
#endif

    Q_D(XYLogManager);
    if (d->currLevel >= LG_OFF)
    {
        return;
    }

    if (d->currLevel != LG_ALL)
    {
        quint32 msgLevel = d->currLevel;
        switch (type)
        {
        case QtDebugMsg:
            msgLevel = ELogLevel::LG_DBG;
            break;
        case QtInfoMsg:
            msgLevel = ELogLevel::LG_INF;
            break;
        case QtWarningMsg:
            msgLevel = ELogLevel::LG_WAR;
            break;
        case QtCriticalMsg:
        default:
            msgLevel = ELogLevel::LG_ERR;
            break;
        }
        if (msgLevel < d->currLevel)
        {
            return;
        }
    }

    if (d->hasConsole)
    {
        fprintf(stderr, "%s\n", message.toLocal8Bit().constData());
        fflush(stderr);
    }

    d->write(message);
}

void XYLogManager::execAllocConsole()
{
#ifdef Q_OS_WIN
    const WORD MAX_CONSOLE_WIDTH = 1024;
    const WORD MAX_CONSOLE_LINES = 500;

    int hConHandle;
    HANDLE lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;

    // 分配Console
    ::AllocConsole();

    // 删除菜单
    HWND console_hwnd = ::GetConsoleWindow();
    HMENU hmenu = ::GetSystemMenu(console_hwnd, FALSE);
    ::DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);

    // 设置大小
    ::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.X = MAX_CONSOLE_WIDTH;
    coninfo.dwSize.Y = MAX_CONSOLE_LINES;
    ::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    // 设置标题
    SetConsoleTitle(L"PCS-DEBUG");

    // 重定向输出流
    lStdHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = ::_open_osfhandle(reinterpret_cast<INT_PTR>(lStdHandle), _O_TEXT);
    fp = ::_fdopen(hConHandle, "w");
    *stdout = *fp;
    ::setvbuf(stdout, NULL, _IONBF, 0);

    lStdHandle = ::GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = ::_open_osfhandle(reinterpret_cast<INT_PTR>(lStdHandle), _O_TEXT);
    fp = ::_fdopen(hConHandle, "r");
    *stdin = *fp;
    ::setvbuf(stdin, NULL, _IONBF, 0);

    lStdHandle = ::GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = ::_open_osfhandle(reinterpret_cast<INT_PTR>(lStdHandle), _O_TEXT);
    fp = ::_fdopen(hConHandle, "w");
    *stderr = *fp;
    ::setvbuf(stderr, NULL, _IONBF, 0);

    std::ios::sync_with_stdio();
#else

#endif
}

void XYLogManager::execFreeConsole()
{
#ifdef Q_OS_WIN
    ::FreeConsole();
#else

#endif
}
