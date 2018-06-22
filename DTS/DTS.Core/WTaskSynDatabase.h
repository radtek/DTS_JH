///*!
// * *****************************************************************************
// * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
// *
// * @file    WTaskSynDatabase.h
// * @brief   任务-数据库同步
// * @version 1.0
// *
// * -----------------------------------------------------------------------------
// * @history
// *  <Date>    | <Author>       | <Description>
// * 2018/03/01 | WeiHeng        | Create this file
// * *****************************************************************************
// */

//#ifndef DTS_CORE_WTASKSYNDATABASE_H
//#define DTS_CORE_WTASKSYNDATABASE_H

//#include <GDtsData.h>

//class WTaskSynDatabase : public QThread
//{
//    Q_OBJECT
//    Q_DECL_SINGLETON(WTaskSynDatabase)

//public:
//    GERROR Initialize();
//    GERROR UnInitialize();
//    void setStop(QBool bStop);
//    QBool getStop();

//protected:
//    virtual void run() Q_DECL_OVERRIDE;

//private:
//    QBool taskDownloadTable(const QString &name);
//    QBool taskUploadTable(const QString &name);
//    QBool taskDownloadWorkOrder();

//public Q_SLOTS:
//    void slotTaskDownloadTable();
//    void slotTaskUploadTable();
//    void slotTaskDownloadWorkOrder();

//private:
//    bool m_bStop;
//    QMutex m_mutexRun;
//    QMutex m_mutexWork; // 数据库连接公用，不能冲突
//};

//#endif
