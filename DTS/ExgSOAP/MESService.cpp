#include "MESServiceSoap.nsmap"
#include "soapMESServiceSoapService.h"
#include "soapMESServiceSoapProxy.h"


int MESServiceSoapService::GetDate(_tempuri__GetDate *tempuri__GetDate, _tempuri__GetDateResponse &tempuri__GetDateResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetDate(tempuri__GetDate, tempuri__GetDateResponse);
}

int MESServiceSoapService::GetDocCode(_tempuri__GetDocCode *tempuri__GetDocCode, _tempuri__GetDocCodeResponse &tempuri__GetDocCodeResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetDocCode(tempuri__GetDocCode, tempuri__GetDocCodeResponse);
}

int MESServiceSoapService::ToolingTimes(_tempuri__ToolingTimes *tempuri__ToolingTimes, _tempuri__ToolingTimesResponse &tempuri__ToolingTimesResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.ToolingTimes(tempuri__ToolingTimes, tempuri__ToolingTimesResponse);
}

int MESServiceSoapService::WorkOrder(_tempuri__WorkOrder *tempuri__WorkOrder, _tempuri__WorkOrderResponse &tempuri__WorkOrderResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.WorkOrder(tempuri__WorkOrder, tempuri__WorkOrderResponse);
}

int MESServiceSoapService::LineInventory(_tempuri__LineInventory *tempuri__LineInventory, _tempuri__LineInventoryResponse &tempuri__LineInventoryResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.LineInventory(tempuri__LineInventory, tempuri__LineInventoryResponse);
}

int MESServiceSoapService::LineInventoryUpdate(_tempuri__LineInventoryUpdate *tempuri__LineInventoryUpdate, _tempuri__LineInventoryUpdateResponse &tempuri__LineInventoryUpdateResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.LineInventoryUpdate(tempuri__LineInventoryUpdate, tempuri__LineInventoryUpdateResponse);
}

int MESServiceSoapService::LineInventoryReturn(_tempuri__LineInventoryReturn *tempuri__LineInventoryReturn, _tempuri__LineInventoryReturnResponse &tempuri__LineInventoryReturnResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.LineInventoryReturn(tempuri__LineInventoryReturn, tempuri__LineInventoryReturnResponse);
}

int MESServiceSoapService::AnDon(_tempuri__AnDon *tempuri__AnDon, _tempuri__AnDonResponse &tempuri__AnDonResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.AnDon(tempuri__AnDon, tempuri__AnDonResponse);
}

int MESServiceSoapService::AnDonTest(_tempuri__AnDonTest *tempuri__AnDonTest, _tempuri__AnDonTestResponse &tempuri__AnDonTestResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.AnDonTest(tempuri__AnDonTest, tempuri__AnDonTestResponse);
}

int MESServiceSoapService::ItemsAdd(_tempuri__ItemsAdd *tempuri__ItemsAdd, _tempuri__ItemsAddResponse &tempuri__ItemsAddResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.ItemsAdd(tempuri__ItemsAdd, tempuri__ItemsAddResponse);
}

int MESServiceSoapService::DocAdd(_tempuri__DocAdd *tempuri__DocAdd, _tempuri__DocAddResponse &tempuri__DocAddResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.DocAdd(tempuri__DocAdd, tempuri__DocAddResponse);
}

int MESServiceSoapService::SetExMail(_tempuri__SetExMail *tempuri__SetExMail, _tempuri__SetExMailResponse &tempuri__SetExMailResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.SetExMail(tempuri__SetExMail, tempuri__SetExMailResponse);
}

int MESServiceSoapService::GetDept(_tempuri__GetDept *tempuri__GetDept, _tempuri__GetDeptResponse &tempuri__GetDeptResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetDept(tempuri__GetDept, tempuri__GetDeptResponse);
}

int MESServiceSoapService::GetPower(_tempuri__GetPower *tempuri__GetPower, _tempuri__GetPowerResponse &tempuri__GetPowerResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.GetPower(tempuri__GetPower, tempuri__GetPowerResponse);
}

int MESServiceSoapService::Login(_tempuri__Login *tempuri__Login, _tempuri__LoginResponse &tempuri__LoginResponse)
{
    MESServiceSoapProxy proxy(SOAP_C_UTFSTRING);

    return proxy.Login(tempuri__Login, tempuri__LoginResponse);
}

int MESServiceSoapService::AnDonCall(_tempuri__AnDonCall *tempuri__AnDonCall, _tempuri__AnDonCallResponse &tempuri__AnDonCallResponse)
{
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
