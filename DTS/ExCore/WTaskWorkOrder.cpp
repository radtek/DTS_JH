#include "WTaskWorkOrder.h"

WTaskWorkOrder &WTaskWorkOrder::Instance()
{
    static WTaskWorkOrder inst;
    return inst;
}

WTaskWorkOrder::WTaskWorkOrder()
    : QObject()
    , _SQL()
    , _RunThread()
    , _RunTimer()
{

}

WTaskWorkOrder::~WTaskWorkOrder()
{
    UnInitialize();
}

GERROR WTaskWorkOrder::Initialize()
{
    qDebug().noquote() << "工单下载：任务初始化开始……";

    _SQL = QSqlDatabase::database(QSQL_DB_STRING_DNWO);

    if (!_SQL.isOpen())
    {
        if (!_SQL.open())
        {
            qWarning().noquote() << QString("工单下载：数据库连接失败，STR=%1").arg(QSQL_DB_STRING_DNWO);
            return GERROR_SQL_OPENFAILED;
        }
    }

    (*this).moveToThread(&_RunThread);
    _RunTimer.moveToThread(&_RunThread);
    QObject::connect(&_RunTimer, &QTimer::timeout, this, &WTaskWorkOrder::Slot_taskWork);
    _RunThread.start();

    QTimer::singleShot(qCfgManager->_Config.value(CONFIG_RUN_TM_FDND, DEFAULT_CONFIG_RUN_TM_FDND).toInt() * 1000, this, &WTaskWorkOrder::Slot_resetTimer);

    qDebug().noquote() << "工单下载：任务初始化成功。";
    return GERROR_OK;
}

GERROR WTaskWorkOrder::UnInitialize()
{
    if (_RunThread.isRunning())
    {
        _RunThread.quit();
    }

    return GERROR_OK;
}

QBool WTaskWorkOrder::taskDownloadWorkOrder()
{
    qDebug().noquote() << QString("[工单下载] 任务开始……");

    LPTSqlSync p = qSqlManager->_SqlSync["MES_WorkOrder"];
    if (!p)
    {
        qWarning().noquote() << QString("[工单下载] 任务未找到！");
        return false;
    }

    QSqlDatabase dbSQLRemote = QSqlDatabase::database(p->db_remote, false);
    if (!dbSQLRemote.isOpen())
    {
        qWarning().noquote() << QString("[工单下载] 远程数据库未打开！");
        return false;
    }
    QSqlDatabase dbSQLLocal = QSqlDatabase::database(p->db_local, false);
    if (!dbSQLLocal.isOpen())
    {
        qWarning().noquote() << QString("[工单下载] 本地数据库未打开！");
        return false;
    }

    bool bSuccess = true;

    QSqlQuery dbQuery(dbSQLLocal);
    QSqlQuery dbQueryRemote(dbSQLRemote);

    do
    {
        QString dbStr = p->SelectTable(false) + " WHERE State = :State";
        QVariantMap dbParam;
        dbParam.insert(":State", 1);
        if (!qSqlManager->SqlExec(dbQueryRemote, dbStr, dbParam))
        {
            bSuccess = false;
            break;
        }
        qDebug().noquote() << QString("[工单下载] 包含 %2 条新增记录。").arg(dbQueryRemote.numRowsAffected());
    }
    while (0);

    if (!bSuccess)
    {
        qWarning().noquote() << QString("[工单下载] 远程数据库存在异常，终止任务！");
        return false;
    }

    qDebug().noquote() << QString("[工单下载] 开始插入数据。");
    int cnt = 0;
    QString dbStr = p->InsertTable(true);
    QVariantMap dbParam;
    while (dbQueryRemote.next())
    {
        dbParam.clear();
        for (int i = 0; i < p->column_local.size(); ++i)
        {
            dbParam.insert(QString(":%1").arg(p->column_local[i]), dbQueryRemote.value(i));
        }
        if (!qSqlManager->SqlExec(dbQuery, dbStr, dbParam))
        {
            qWarning().noquote() << QString("[工单下载] 插入数据存在错误。%1").arg(dbQuery.lastError().text());
        }
        if (++cnt % 1000 == 0)
        {
            qDebug().noquote() << QString("[工单下载] 表[%1] 已经插入 %2 条记录。").arg(p->table_local).arg(cnt);
        }
    }
    qDebug().noquote() << QString("[工单下载] 表[%1] 完成更新数据。总计 %2 条记录。").arg(p->table_local).arg(cnt);

    return true;
}

void WTaskWorkOrder::Slot_stopTimer()
{
    _RunTimer.stop();
}

void WTaskWorkOrder::Slot_resetTimer()
{
    Slot_taskWork();
    _RunTimer.setInterval(qCfgManager->_Config.value(CONFIG_RUN_TM_DNLD, DEFAULT_CONFIG_RUN_TM_DNLD).toInt() * 1000);
    _RunTimer.start();
}

void WTaskWorkOrder::Slot_taskWork()
{
    qDebug().noquote() << QString("工单下载：任务执行开始……");

    const QString &downloadConfig = qCfgManager->_Config.value(CONFIG_SYN_DOWNSQL).toString();

    QStringList download = downloadConfig.split(",", QString::SkipEmptyParts, Qt::CaseSensitive);

    taskDownloadWorkOrder();

    qDebug().noquote() << QString("工单下载：任务执行结束。");
}
