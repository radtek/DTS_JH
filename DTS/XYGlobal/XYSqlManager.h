#ifndef XYSQLMANAGER_H
#define XYSQLMANAGER_H

#include <QtCore>
#include <QtSql>
#include <QtXml>

class TConfigSQL
{
public:
    bool isEnable;
    bool isForce;
    QString type;
    QString address;
    int port;
    QString username;
    QString password;
    QString database;
};
typedef QSharedPointer<TConfigSQL> TConfigSQLPtr;

class TNamingSQL
{
public:
    QString Name;
    QString StrSQL;
};
typedef QSharedPointer<TNamingSQL> TNamingSQLPtr;

class XYSqlManager
{
public:
    static XYSqlManager *instance();
    static bool SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantMap &dbParam);
    static bool SqlExec(QSqlQuery &dbQuery, const QString &dbSQL, const QVariantList &dbParam);

public:
    XYSqlManager();
    ~XYSqlManager();

public:
    bool Initialize();
    QString getSqlByName(const QString &name);

private:
    bool init();
    void initConfigSQL();
    void initNamingSQL();

private:
    QMap<QString, TConfigSQLPtr> mapConfigSQL;
    QMap<QString, TNamingSQLPtr> mapNamingSQL;
};

#endif
