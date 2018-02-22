/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      GLogManager.cpp
 *  \brief
 *      Manager-Logger
 *  \since
 *  001     2017/11/01      weiheng     create this file
 */
//!============================================================================

#include "GLogManager.h"


GLogManager &GLogManager::Instance()
{
    static GLogManager inst;
    return inst;
}

void GLogManager::OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &buf)
{
    int msgLevel = qLogManager->_LogConfig.DefLevel();
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

    if (msgLevel < qLogManager->_LogConfig.DefLevel())
    {
        return;
    }

    QString message =  qFormatLogMessage(type, context, buf);

    Q_EMIT qLogManager->recvMsg(message);
}

GLogManager::GLogManager()
    : QObject(Q_NULLPTR)
{
    QObject::connect(this, &GLogManager::recvMsg, this, &GLogManager::dealMsg, Qt::QueuedConnection);

    changeDir();

    _FileWrite.open((_LogConfig.PathName() + _LogConfig.FileName()).toStdString(), std::ios::app | std::ios::out);
}

GLogManager::~GLogManager()
{
    QObject::disconnect(this, &GLogManager::recvMsg, this, &GLogManager::dealMsg);

    if (_RunThread.isRunning())
    {
        _RunThread.quit();
    }
}

bool GLogManager::Initialize(const TLogConfig &config)
{
    ResetConfig(config);

    if (_LogConfig.IsEnable())
    {
        qInstallMessageHandler(GLogManager::OutputMessage);
    }
    else
    {
        qInstallMessageHandler(Q_NULLPTR);
    }

    if (_LogConfig.SPattern().isEmpty())
    {
        qSetMessagePattern(DEFAULT_CONFIG_LG_PATT);
    }
    else
    {
        qSetMessagePattern(_LogConfig.SPattern());
    }

    if (!_RunThread.isRunning())
    {
        moveToThread(&_RunThread);
        _RunThread.start();
    }
    return true;
}

void GLogManager::ResetConfig(const TLogConfig &config)
{
    QMutexLocker locker(&_CfgLock);

    _LogConfig = config;
}

void GLogManager::changeDir()
{
    QDir dir(_LogConfig.PathName());
    dir.mkdir(".");
}

void GLogManager::changeFile()
{
    QString strOldName = _LogConfig.PathName() + _LogConfig.FileName();
    QString strNewName = strOldName + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QFile::rename(strOldName, strNewName);

    QDir dir(_LogConfig.PathName());
    QStringList entryList = dir.entryList(QStringList(_LogConfig.FileName() + "*"));
    int num = entryList.size() - _LogConfig.MaxCount();
    if (num > 0)
    {
        for (int i = 0; i < num; ++i)
        {
            QFile::remove(_LogConfig.PathName() + entryList[i]);
        }
    }
}

void GLogManager::writeLog(const QString &strMsg)
{
    QMutexLocker locker(&_FileLock);

    if (_FileWrite.is_open())
    {
        if (_FileWrite.tellg() > _LogConfig.MaxSizeM() * (1 * 1024 * 1024))
        {
            _FileWrite.close();

            changeFile();

            _FileWrite.open((_LogConfig.PathName() + _LogConfig.FileName()).toStdString(), std::ios::app | std::ios::out);
        }
    }
    else
    {
        changeDir();
        _FileWrite.open((_LogConfig.PathName() + _LogConfig.FileName()).toStdString(), std::ios::app | std::ios::out);
    }

    if (_FileWrite.is_open())
    {
        _FileWrite << strMsg.toStdString() << std::endl;
        _FileWrite.flush();
    }
}

void GLogManager::dealMsg(const QString &strMsg)
{
    writeLog(strMsg);

    Q_EMIT sendMsg(strMsg);
}

