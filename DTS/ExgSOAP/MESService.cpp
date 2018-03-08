/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    MESService.cpp
 * @brief   Web Service服务端实现
 * @version 1.0
 *
 * -----------------------------------------------------------------------------
 * @history
 *  <Date>    | <Author>       | <Description>
 * 2018/03/01 | WeiHeng        | Create this file
 * *****************************************************************************
 */

#include "MESServiceSoap.nsmap"
#include "soapMESServiceSoapService.h"
#include "soapMESServiceSoapProxy.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtXml>

#include "GDtsData.h"

int MESServiceSoapService::GetDate(_tempuri__GetDate *tempuri__GetDate, _tempuri__GetDateResponse &tempuri__GetDateResponse)
{
    qDebug() << "MESServiceSoapService::GetDate";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetDate(tempuri__GetDate, tempuri__GetDateResponse);
}

int MESServiceSoapService::GetDocCode(_tempuri__GetDocCode *tempuri__GetDocCode, _tempuri__GetDocCodeResponse &tempuri__GetDocCodeResponse)
{
    qDebug() << "MESServiceSoapService::GetDocCode";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetDocCode(tempuri__GetDocCode, tempuri__GetDocCodeResponse);
}

int MESServiceSoapService::ToolingTimes(_tempuri__ToolingTimes *tempuri__ToolingTimes, _tempuri__ToolingTimesResponse &tempuri__ToolingTimesResponse)
{
    qDebug() << "MESServiceSoapService::ToolingTimes";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.ToolingTimes(tempuri__ToolingTimes, tempuri__ToolingTimesResponse);
}

int MESServiceSoapService::WorkOrder(_tempuri__WorkOrder *tempuri__WorkOrder, _tempuri__WorkOrderResponse &tempuri__WorkOrderResponse)
{
    qDebug() << "MESServiceSoapService::WorkOrder";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    int res = proxy.WorkOrder(tempuri__WorkOrder, tempuri__WorkOrderResponse);

    do
    {
        if (res == SOAP_OK)
        {
            // 修改数据库
            QString strRequest(tempuri__WorkOrder->WorkOrderXml);
            QString strResponse(tempuri__WorkOrderResponse.WorkOrderResult);

            if (strResponse != "OK")
            {
                qDebug() << "MESServiceSoapService::WorkOrder Response is not OK";
                break;
            }

            QDomDocument doc;
            if (!doc.setContent(strRequest))
            {
                break;
            }
            QDomElement root = doc.firstChildElement();
            if (root.isNull())
            {
                break;
            }
            QDomElement table = root.firstChildElement();
            if (table.isNull())
            {
                break;
            }
            QDomElement uid = table.firstChildElement("UID");
            if (uid.isNull())
            {
                break;
            }
            QDomElement state = table.firstChildElement("State");
            if (state.isNull())
            {
                break;
            }

            QSqlQuery dbQuery(QSqlDatabase::database("LOCAL_HOST"));
            QString dbSQL = "UPDATE MES_WorkOrder SET State=? WHERE UID=?";
            QVariantList dbParam;
            dbParam << state.text().toInt() << uid.text().toInt() ;
            if (!qSqlManager->SqlExec(dbQuery, dbSQL, dbParam))
            {
                break;
            }
        }
    }
    while (0);

    return res;
}

int MESServiceSoapService::LineInventory(_tempuri__LineInventory *tempuri__LineInventory, _tempuri__LineInventoryResponse &tempuri__LineInventoryResponse)
{
    qDebug() << "MESServiceSoapService::LineInventory";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.LineInventory(tempuri__LineInventory, tempuri__LineInventoryResponse);
}

int MESServiceSoapService::LineInventoryUpdate(_tempuri__LineInventoryUpdate *tempuri__LineInventoryUpdate, _tempuri__LineInventoryUpdateResponse &tempuri__LineInventoryUpdateResponse)
{
    qDebug() << "MESServiceSoapService::LineInventoryUpdate";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.LineInventoryUpdate(tempuri__LineInventoryUpdate, tempuri__LineInventoryUpdateResponse);
}

int MESServiceSoapService::LineInventoryReturn(_tempuri__LineInventoryReturn *tempuri__LineInventoryReturn, _tempuri__LineInventoryReturnResponse &tempuri__LineInventoryReturnResponse)
{
    qDebug() << "MESServiceSoapService::LineInventoryReturn";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.LineInventoryReturn(tempuri__LineInventoryReturn, tempuri__LineInventoryReturnResponse);
}

int MESServiceSoapService::AnDon(_tempuri__AnDon *tempuri__AnDon, _tempuri__AnDonResponse &tempuri__AnDonResponse)
{
    qDebug() << "MESServiceSoapService::AnDon";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.AnDon(tempuri__AnDon, tempuri__AnDonResponse);
}

int MESServiceSoapService::AnDonTest(_tempuri__AnDonTest *tempuri__AnDonTest, _tempuri__AnDonTestResponse &tempuri__AnDonTestResponse)
{
    qDebug() << "MESServiceSoapService::AnDonTest";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.AnDonTest(tempuri__AnDonTest, tempuri__AnDonTestResponse);
}

int MESServiceSoapService::ItemsAdd(_tempuri__ItemsAdd *tempuri__ItemsAdd, _tempuri__ItemsAddResponse &tempuri__ItemsAddResponse)
{
    qDebug() << "MESServiceSoapService::ItemsAdd";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.ItemsAdd(tempuri__ItemsAdd, tempuri__ItemsAddResponse);
}

int MESServiceSoapService::DocAdd(_tempuri__DocAdd *tempuri__DocAdd, _tempuri__DocAddResponse &tempuri__DocAddResponse)
{
    qDebug() << "MESServiceSoapService::DocAdd" ;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.DocAdd(tempuri__DocAdd, tempuri__DocAddResponse);
}

int MESServiceSoapService::SetExMail(_tempuri__SetExMail *tempuri__SetExMail, _tempuri__SetExMailResponse &tempuri__SetExMailResponse)
{
    qDebug() << "MESServiceSoapService::SetExMail";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.SetExMail(tempuri__SetExMail, tempuri__SetExMailResponse);
}

int MESServiceSoapService::GetDept(_tempuri__GetDept *tempuri__GetDept, _tempuri__GetDeptResponse &tempuri__GetDeptResponse)
{
    qDebug() << "MESServiceSoapService::GetDept";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetDept(tempuri__GetDept, tempuri__GetDeptResponse);
}

int MESServiceSoapService::GetPower(_tempuri__GetPower *tempuri__GetPower, _tempuri__GetPowerResponse &tempuri__GetPowerResponse)
{
    qDebug() << "MESServiceSoapService::GetPower";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetPower(tempuri__GetPower, tempuri__GetPowerResponse);
}

int MESServiceSoapService::Login(_tempuri__Login *tempuri__Login, _tempuri__LoginResponse &tempuri__LoginResponse)
{
    qDebug() << "MESServiceSoapService::Login";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.Login(tempuri__Login, tempuri__LoginResponse);
}

int MESServiceSoapService::AnDonCall(_tempuri__AnDonCall *tempuri__AnDonCall, _tempuri__AnDonCallResponse &tempuri__AnDonCallResponse)
{
    qDebug() << "MESServiceSoapService::AnDonCall";

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.AnDonCall(tempuri__AnDonCall, tempuri__AnDonCallResponse);
}

int MESServiceSoapService::GetDate_(_tempuri__GetDate *tempuri__GetDate, _tempuri__GetDateResponse &tempuri__GetDateResponse)
{
    return GetDate(tempuri__GetDate, tempuri__GetDateResponse);
}

int MESServiceSoapService::GetDocCode_(_tempuri__GetDocCode *tempuri__GetDocCode, _tempuri__GetDocCodeResponse &tempuri__GetDocCodeResponse)
{
    return GetDocCode(tempuri__GetDocCode, tempuri__GetDocCodeResponse);
}

int MESServiceSoapService::ToolingTimes_(_tempuri__ToolingTimes *tempuri__ToolingTimes, _tempuri__ToolingTimesResponse &tempuri__ToolingTimesResponse)
{
    return ToolingTimes(tempuri__ToolingTimes, tempuri__ToolingTimesResponse);
}

int MESServiceSoapService::WorkOrder_(_tempuri__WorkOrder *tempuri__WorkOrder, _tempuri__WorkOrderResponse &tempuri__WorkOrderResponse)
{
    return WorkOrder(tempuri__WorkOrder, tempuri__WorkOrderResponse);
}

int MESServiceSoapService::LineInventory_(_tempuri__LineInventory *tempuri__LineInventory, _tempuri__LineInventoryResponse &tempuri__LineInventoryResponse)
{
    return LineInventory(tempuri__LineInventory, tempuri__LineInventoryResponse);
}

int MESServiceSoapService::LineInventoryUpdate_(_tempuri__LineInventoryUpdate *tempuri__LineInventoryUpdate, _tempuri__LineInventoryUpdateResponse &tempuri__LineInventoryUpdateResponse)
{
    return LineInventoryUpdate(tempuri__LineInventoryUpdate, tempuri__LineInventoryUpdateResponse);
}

int MESServiceSoapService::LineInventoryReturn_(_tempuri__LineInventoryReturn *tempuri__LineInventoryReturn, _tempuri__LineInventoryReturnResponse &tempuri__LineInventoryReturnResponse)
{
    return LineInventoryReturn(tempuri__LineInventoryReturn, tempuri__LineInventoryReturnResponse);
}

int MESServiceSoapService::AnDon_(_tempuri__AnDon *tempuri__AnDon, _tempuri__AnDonResponse &tempuri__AnDonResponse)
{
    return AnDon(tempuri__AnDon, tempuri__AnDonResponse);
}

int MESServiceSoapService::AnDonTest_(_tempuri__AnDonTest *tempuri__AnDonTest, _tempuri__AnDonTestResponse &tempuri__AnDonTestResponse)
{
    return AnDonTest(tempuri__AnDonTest, tempuri__AnDonTestResponse);
}

int MESServiceSoapService::ItemsAdd_(_tempuri__ItemsAdd *tempuri__ItemsAdd, _tempuri__ItemsAddResponse &tempuri__ItemsAddResponse)
{
    return ItemsAdd(tempuri__ItemsAdd, tempuri__ItemsAddResponse);
}

int MESServiceSoapService::DocAdd_(_tempuri__DocAdd *tempuri__DocAdd, _tempuri__DocAddResponse &tempuri__DocAddResponse)
{
    return DocAdd(tempuri__DocAdd, tempuri__DocAddResponse);
}

int MESServiceSoapService::SetExMail_(_tempuri__SetExMail *tempuri__SetExMail, _tempuri__SetExMailResponse &tempuri__SetExMailResponse)
{
    return SetExMail(tempuri__SetExMail, tempuri__SetExMailResponse);
}

int MESServiceSoapService::GetDept_(_tempuri__GetDept *tempuri__GetDept, _tempuri__GetDeptResponse &tempuri__GetDeptResponse)
{
    return GetDept(tempuri__GetDept, tempuri__GetDeptResponse);
}

int MESServiceSoapService::GetPower_(_tempuri__GetPower *tempuri__GetPower, _tempuri__GetPowerResponse &tempuri__GetPowerResponse)
{
    return GetPower(tempuri__GetPower, tempuri__GetPowerResponse);
}

int MESServiceSoapService::Login_(_tempuri__Login *tempuri__Login, _tempuri__LoginResponse &tempuri__LoginResponse)
{
    return Login(tempuri__Login, tempuri__LoginResponse);
}

int MESServiceSoapService::AnDonCall_(_tempuri__AnDonCall *tempuri__AnDonCall, _tempuri__AnDonCallResponse &tempuri__AnDonCallResponse)
{
    return AnDonCall(tempuri__AnDonCall, tempuri__AnDonCallResponse);
}
