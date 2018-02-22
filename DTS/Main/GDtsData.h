/// -*- C++ -*-

//!============================================================================
/*!
 *  \copyright
 *      Nanjing Xuanyong Co.,Ltd.
 *  \file
 *      GStdData.h
 *  \brief
 *      Global Data Definition
 */
//!============================================================================

#ifndef DTS_MAIN_GDTSDATA_H
#define DTS_MAIN_GDTSDATA_H

#include "GDtsDef.h"
#include "GAppInstance.h"
#include "GCfgManager.h"
#include "GLogManager.h"
#include "GSqlManager.h"


#define KB                          (1 * 1024)
#define MB                          (1 * 1024 * 1024)
#define GB                          (1 * 1024 * 1024 * 1024)
#define KILO                        (1 * 1000)
#define MEGA                        (1 * 1000 * 1000)
#define MSEC_P_SEC                  KILO
#define SEC_P_MI                    (60)
#define SEC_P_HR                    (60 * 60)
#define SEC_P_DY                    (24 * 60 * 60)

#define PICTURE_WORKSHOP                            ":/DTS/workshop"
#define PICTURE_MAIN                                ":/DTS/main"
#define PICTURE_MAIN_RUN                            ":/DTS/main_run"
#define PICTURE_RES_ERROR                           ":/DTS/res_error"
#define PICTURE_RES_OK                              ":/DTS/res_ok"
#define PICTURE_RES_DELETE                          ":/DTS/res_delete"
#define PICTURE_ACTION_LOGIN                        ":/DTS/action_login"
#define PICTURE_ACTION_LOGOUT                       ":/DTS/action_logout"
#define PICTURE_ACTION_SYSCONFIG                    ":/DTS/action_sysconfig"
#define PICTURE_ACTION_RUNCONFIG                    ":/DTS/action_runconfig"
#define PICTURE_ACTION_SYNCONFIG                    ":/DTS/action_synconfig"

#define CONFIG_SYS




template <typename T>
T CheckValueRange(const T &value, const T &max, const T &min)
{
    return (value > max) ? max : (value < min) ? min : value;
}

void MessageBox_Information(const QString &text);
void MessageBox_Warning(const QString &text);
void MessageBox_Critical(const QString &text);
bool MessageBox_Question(const QString &text);
int MessageBox_Question3(const QString &text);



class GDtsData
{
public:
    static GERROR Initialize();

    static QString QLoadString(const QString &id)
    {
        return Def_String.value(id, QString());
    }

    static const QVariant QLoadVariant(const QString &key)
    {
        return Default_Variant.value(key, QVariant());
    }

private:
    static QStringMap Def_String;
    static const QVariantMap Default_Variant;


};




#endif
