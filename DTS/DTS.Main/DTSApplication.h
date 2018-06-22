/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    DTSApplication.h
 * @brief
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#ifndef DTS_MAIN_DTSAPPLICATION_H
#define DTS_MAIN_DTSAPPLICATION_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class MainWindow;
class DTSApplication : public QApplication
{
    Q_OBJECT

public:
    static DTSApplication *instance();

public:
    DTSApplication(int argc, char **argv);
    virtual ~DTSApplication();

public:
    bool initialize();
    QString parseApplication();
    bool checkApplication(const QString &app);
    bool startApplication(const QString &app);

private:
    QScopedPointer<MainWindow> mainwindow;
};

#endif
