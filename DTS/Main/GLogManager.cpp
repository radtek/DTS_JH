/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    GLogManager.cpp
 * @brief   日志输出类
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "GLogManager.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

GLogManager &GLogManager::Instance()
{
    static GLogManager inst;
    return inst;
}

void GLogManager::OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf)
{
    QString message = qFormatLogMessage(type, context, buf);

#ifdef Q_OS_WIN
    OutputDebugString(reinterpret_cast<const wchar_t *>((message + QLatin1Char('\n')).utf16()));
#else
    fprintf(stderr, "%s\n", message.toLocal8Bit().constData());
    fflush(stderr);
#endif

    qLogManager->qMessageHandler(type, message);
}

GLogManager::GLogManager()
    : QObject(Q_NULLPTR)
    , _Target(DEFAULT_CONFIG_LG_TARG)
    , _Pattern(DEFAULT_CONFIG_LG_PATT)
    , _PathName(DEFAULT_CONFIG_LG_PATH)
    , _FileName(DEFAULT_CONFIG_LG_FILE)
    , _MaxCount(DEFAULT_CONFIG_LG_MCNT)
    , _MaxSize(DEFAULT_CONFIG_LG_MSIZ)
    , _DefaultLevel(DEFAULT_CONFIG_LG_DLVL)
{
    QObject::connect(this, &GLogManager::recvMsg, this, &GLogManager::dealMsg, Qt::QueuedConnection);
    QObject::connect(&_CfgReload, &QTimer::timeout, this, &GLogManager::resetConfig);

    resetConfig();

    this->moveToThread(&_RunThread);
    _RunThread.start();
    _CfgReload.start(30000);

    qInstallMessageHandler(GLogManager::OutputMessage);
}

GLogManager::~GLogManager()
{
    QObject::disconnect(this, &GLogManager::recvMsg, this, &GLogManager::dealMsg);
    QObject::disconnect(&_CfgReload, &QTimer::timeout, this, &GLogManager::resetConfig);

    if (_RunThread.isRunning())
    {
        _RunThread.exit(0);
    }
}

void GLogManager::qMessageHandler(QtMsgType type, const QString &message)
{
    int msgLevel = _DefaultLevel;
    switch (type)
    {
    case QtDebugMsg:
        msgLevel = static_cast<int>(ELogLevel::LG_DBG);
        break;
    case QtInfoMsg:
        msgLevel = static_cast<int>(ELogLevel::LG_INF);
        break;
    case QtWarningMsg:
        msgLevel = static_cast<int>(ELogLevel::LG_WAR);
        break;
    case QtCriticalMsg:
    case QtFatalMsg:
        msgLevel = static_cast<int>(ELogLevel::LG_ERR);
        break;
    }

    if (msgLevel < _DefaultLevel)
    {
        return;
    }

    Q_EMIT recvMsg(message);
}

void GLogManager::changeDir()
{
    QDir dir(".");
    dir.mkpath(_PathName);
}

void GLogManager::changeFile()
{
    QString strOldName = _PathName + _FileName;
    QString strNewName = strOldName + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QFile::rename(strOldName, strNewName);

    QDir dir(_PathName);
    QStringList entryList = dir.entryList(QStringList(_FileName + "*"));
    int num = entryList.size() - static_cast<int>(_MaxCount);
    if (num > 0)
    {
        for (int i = 0; i < num; ++i)
        {
            QFile::remove(_PathName + entryList[i]);
        }
    }
}

void GLogManager::writeLog(const QString &message)
{
    QMutexLocker locker(&_FileLock);

    if (_FileWriter.is_open())
    {
        if (_FileWriter.tellg() > _MaxSize * (1 * 1024 * 1024))
        {
            _FileWriter.close();

            changeFile();

            _FileWriter.open((_PathName + _FileName).toStdString(), std::ios::app | std::ios::out);
        }
    }
    else
    {
        changeDir();
        _FileWriter.open((_PathName + _FileName).toStdString(), std::ios::app | std::ios::out);
    }

    if (_FileWriter.is_open())
    {
        _FileWriter << message.toStdString() << std::endl;
        _FileWriter.flush();
    }
}

void GLogManager::dealMsg(const QString &message)
{
    if (_Target & 0x01)
    {
        writeLog(message);
    }

    Q_EMIT sendMsg(message);
}

void GLogManager::resetConfig()
{
    // 提升效率
    int                 Target = DEFAULT_CONFIG_LG_TARG;
    QString             Pattern = DEFAULT_CONFIG_LG_PATT;
    QString             Path = DEFAULT_CONFIG_LG_PATH;
    QString             File = DEFAULT_CONFIG_LG_FILE;
    unsigned int        MaxCount = DEFAULT_CONFIG_LG_MCNT;
    unsigned int        MaxSize = DEFAULT_CONFIG_LG_MSIZ;
    int                 DefaultLevel = DEFAULT_CONFIG_LG_DLVL;

    bool bSuccess = true;
    do
    {
        QSettings settings(DEFAULT_CONFIG_LOGMANAGER, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        if (settings.status() != QSettings::NoError)
        {
            bSuccess = false;
            break;
        }

        Target = settings.value(TAG_CONFIG_LG_TARG, DEFAULT_CONFIG_LG_TARG).toInt();

        Pattern = settings.value(TAG_CONFIG_LG_PATT, DEFAULT_CONFIG_LG_PATT).toString();
        if (Pattern.isEmpty())
        {
            Pattern = DEFAULT_CONFIG_LG_PATT;
        }

        Path = settings.value(TAG_CONFIG_LG_PATH, DEFAULT_CONFIG_LG_PATH).toString();
        if (Path.right(1) != QLatin1Char('/') && Path.right(1) != QLatin1Char('\\'))
        {
            Path.append(QLatin1Char('/'));
        }

        File = settings.value(TAG_CONFIG_LG_FILE, DEFAULT_CONFIG_LG_FILE).toString();
        if (File.isEmpty())
        {
            File = DEFAULT_CONFIG_LG_FILE;
        }

        MaxCount = settings.value(TAG_CONFIG_LG_MCNT, DEFAULT_CONFIG_LG_MCNT).toUInt();

        MaxSize = settings.value(TAG_CONFIG_LG_MSIZ, DEFAULT_CONFIG_LG_MSIZ).toUInt();
        if (MaxSize == 0)
        {
            MaxSize = DEFAULT_CONFIG_LG_MSIZ;
        }

        DefaultLevel = settings.value(TAG_CONFIG_LG_DLVL, DEFAULT_CONFIG_LG_DLVL).toInt();
        if (DefaultLevel < static_cast<int>(ELogLevel::LG_DBG) || DefaultLevel > static_cast<int>(ELogLevel::LG_ERR))
        {
            DefaultLevel = DEFAULT_CONFIG_LG_DLVL;
        }
    }
    while (0);

    if (bSuccess)
    {
        QMutexLocker locker(&_CfgLock);

        _Target = Target;
        _Pattern = Pattern;
        _PathName = Path;
        _FileName = File;
        _MaxCount = MaxCount;
        _MaxSize = MaxSize;
        _DefaultLevel = DefaultLevel;

        qSetMessagePattern(_Pattern);
        changeDir();
    }
}

