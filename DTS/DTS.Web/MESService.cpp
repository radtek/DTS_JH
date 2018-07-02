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
#include "soapMESServiceSoapProxy.h"
#include "soapMESServiceSoapService.h"

#include <QtCore>
#include <QtSql>
#include <QtXml>

#include "GCfgManager.h"

int MESServiceSoapService::GetDate(_tempuri__GetDate *request, _tempuri__GetDateResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::GetDate " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.GetDate(qCfgManager->getWSRemoteUrl().toUtf8().data(), Q_NULLPTR, request, response);
}

int MESServiceSoapService::GetDocCode(_tempuri__GetDocCode *request, _tempuri__GetDocCodeResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::GetDocCode " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.GetDocCode(request, response);
}

int MESServiceSoapService::ToolingTimes(_tempuri__ToolingTimes *request, _tempuri__ToolingTimesResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::ToolingTimes " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.ToolingTimes(request, response);
}

int MESServiceSoapService::WorkOrder(_tempuri__WorkOrder *request, _tempuri__WorkOrderResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::WorkOrder " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    int result = proxy.WorkOrder(request, response);
    do
    {
        if (result == SOAP_OK)
        {
            QString strRequest(request->WorkOrderXml->c_str());

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
            QDomElement WOCode = table.firstChildElement("WOCode");
            if (WOCode.isNull())
            {
                break;
            }
            QDomElement State = table.firstChildElement("State");
            if (State.isNull())
            {
                break;
            }

            const QString dbSQL = R"(UPDATE [MES_WorkOrder] SET [State]=:State WHERE [WOCode]=:WOCode )";

            QSqlQuery dbQuery(QSqlDatabase::database("LOCAL_HOST"));
            dbQuery.prepare(dbSQL);
            dbQuery.bindValue(":State", State.text());
            dbQuery.bindValue(":WOCode", WOCode.text());
            if (!dbQuery.exec())
            {
                break;
            }
        }
    } while (false);

    return result;
}

int MESServiceSoapService::LineInventory(_tempuri__LineInventory *request, _tempuri__LineInventoryResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::LineInventory " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.LineInventory(request, response);
}

int MESServiceSoapService::LineInventoryUpdate(_tempuri__LineInventoryUpdate *request, _tempuri__LineInventoryUpdateResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::LineInventoryUpdate " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.LineInventoryUpdate(request, response);
}

int MESServiceSoapService::LineInventoryReturn(_tempuri__LineInventoryReturn *request, _tempuri__LineInventoryReturnResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::LineInventoryReturn " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.LineInventoryReturn(request, response);
}

int MESServiceSoapService::ItemsAdd(_tempuri__ItemsAdd *request, _tempuri__ItemsAddResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::ItemsAdd " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.ItemsAdd(request, response);
}

int MESServiceSoapService::DocAdd(_tempuri__DocAdd *request, _tempuri__DocAddResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::DocAdd " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.DocAdd(request, response);
}

int MESServiceSoapService::GetDept(_tempuri__GetDept *request, _tempuri__GetDeptResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::GetDate " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.GetDept(request, response);
}

int MESServiceSoapService::GetPower(_tempuri__GetPower *request, _tempuri__GetPowerResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::GetDate " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.GetPower(request, response);
}

int MESServiceSoapService::Login(_tempuri__Login *request, _tempuri__LoginResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::Login " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.Login(request, response);
}

int MESServiceSoapService::AnDan1Send(_tempuri__AnDan1Send *request, _tempuri__AnDan1SendResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::AnDan1Send " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.AnDan1Send(request, response);
}

int MESServiceSoapService::AnDan4UpdPeoplePresent(_tempuri__AnDan4UpdPeoplePresent *request, _tempuri__AnDan4UpdPeoplePresentResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::AnDan4UpdPeoplePresent " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.AnDan4UpdPeoplePresent(request, response);
}

int MESServiceSoapService::AnDan3AddUsers(_tempuri__AnDan3AddUsers *request, _tempuri__AnDan3AddUsersResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::AnDan3AddUsers " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.AnDan3AddUsers(request, response);
}

int MESServiceSoapService::AnDan2PeoplePresent(_tempuri__AnDan2PeoplePresent *request, _tempuri__AnDan2PeoplePresentResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::AnDan2PeoplePresent " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.AnDan2PeoplePresent(request, response);
}

int MESServiceSoapService::AnDanSLTypeGet(_tempuri__AnDanSLTypeGet *request, _tempuri__AnDanSLTypeGetResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::AnDanSLTypeGet " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.AnDanSLTypeGet(request, response);
}

int MESServiceSoapService::AnDanSLTypeAdd(_tempuri__AnDanSLTypeAdd *request, _tempuri__AnDanSLTypeAddResponse &response)
{
    static int count = 0;
    qDebug() << "MESServiceSoapService::AnDanSLTypeAdd " << ++count;

    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);
    return proxy.AnDanSLTypeAdd(request, response);
}

int MESServiceSoapService::GetDate_(_tempuri__GetDate *request, _tempuri__GetDateResponse &response)
{
    return GetDate(request, response);
}

int MESServiceSoapService::GetDocCode_(_tempuri__GetDocCode *request, _tempuri__GetDocCodeResponse &response)
{
    return GetDocCode(request, response);
}

int MESServiceSoapService::ToolingTimes_(_tempuri__ToolingTimes *request, _tempuri__ToolingTimesResponse &response)
{
    return ToolingTimes(request, response);
}

int MESServiceSoapService::WorkOrder_(_tempuri__WorkOrder *request, _tempuri__WorkOrderResponse &response)
{
    return WorkOrder(request, response);
}

int MESServiceSoapService::LineInventory_(_tempuri__LineInventory *request, _tempuri__LineInventoryResponse &response)
{
    return LineInventory(request, response);
}

int MESServiceSoapService::LineInventoryUpdate_(_tempuri__LineInventoryUpdate *request, _tempuri__LineInventoryUpdateResponse &response)
{
    return LineInventoryUpdate(request, response);
}

int MESServiceSoapService::LineInventoryReturn_(_tempuri__LineInventoryReturn *request, _tempuri__LineInventoryReturnResponse &response)
{
    return LineInventoryReturn(request, response);
}

int MESServiceSoapService::ItemsAdd_(_tempuri__ItemsAdd *request, _tempuri__ItemsAddResponse &response)
{
    return ItemsAdd(request, response);
}

int MESServiceSoapService::DocAdd_(_tempuri__DocAdd *request, _tempuri__DocAddResponse &response)
{
    return DocAdd(request, response);
}

int MESServiceSoapService::GetDept_(_tempuri__GetDept *request, _tempuri__GetDeptResponse &response)
{
    return GetDept(request, response);
}

int MESServiceSoapService::GetPower_(_tempuri__GetPower *request, _tempuri__GetPowerResponse &response)
{
    return GetPower(request, response);
}

int MESServiceSoapService::Login_(_tempuri__Login *request, _tempuri__LoginResponse &response)
{
    return Login(request, response);
}

int MESServiceSoapService::AnDan1Send_(_tempuri__AnDan1Send *request, _tempuri__AnDan1SendResponse &response)
{
    return AnDan1Send(request, response);
}

int MESServiceSoapService::AnDan4UpdPeoplePresent_(_tempuri__AnDan4UpdPeoplePresent *request, _tempuri__AnDan4UpdPeoplePresentResponse &response)
{
    return AnDan4UpdPeoplePresent(request, response);
}

int MESServiceSoapService::AnDan3AddUsers_(_tempuri__AnDan3AddUsers *request, _tempuri__AnDan3AddUsersResponse &response)
{
    return AnDan3AddUsers(request, response);
}

int MESServiceSoapService::AnDan2PeoplePresent_(_tempuri__AnDan2PeoplePresent *request, _tempuri__AnDan2PeoplePresentResponse &response)
{
    return AnDan2PeoplePresent(request, response);
}

int MESServiceSoapService::AnDanSLTypeGet_(_tempuri__AnDanSLTypeGet *request, _tempuri__AnDanSLTypeGetResponse &response)
{
    return AnDanSLTypeGet(request, response);
}

int MESServiceSoapService::AnDanSLTypeAdd_(_tempuri__AnDanSLTypeAdd *request, _tempuri__AnDanSLTypeAddResponse &response)
{
    return AnDanSLTypeAdd(request, response);
}
