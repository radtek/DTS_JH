/* soapMESServiceSoapProxy.h
   Generated by gSOAP 2.8.64 for MESService.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapMESServiceSoapProxy_H
#define soapMESServiceSoapProxy_H
#include "soapH.h"

    class SOAP_CMAC MESServiceSoapProxy : public soap {
      public:
        /// Endpoint URL of service 'MESServiceSoapProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in MESService.h, if any
        /// Construct a proxy with new managing context
        MESServiceSoapProxy();
        /// Copy constructor
        MESServiceSoapProxy(const MESServiceSoapProxy& rhs);
        /// Construct proxy given a managing context
        MESServiceSoapProxy(const struct soap&);
        /// Constructor taking an endpoint URL
        MESServiceSoapProxy(const char *endpoint);
        /// Constructor taking input and output mode flags for the new managing context
        MESServiceSoapProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        MESServiceSoapProxy(const char *endpoint, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        MESServiceSoapProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and managing context
        virtual ~MESServiceSoapProxy();
        /// Initializer used by constructors
        virtual void MESServiceSoapProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual MESServiceSoapProxy *copy();
        /// Copy assignment
        MESServiceSoapProxy& operator=(const MESServiceSoapProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault string (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail as string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        /// Web service operation 'GetDate' (returns SOAP_OK or error code)
        virtual int GetDate(_tempuri__GetDate *tempuri__GetDate, _tempuri__GetDateResponse &tempuri__GetDateResponse)
        { return this->GetDate(NULL, NULL, tempuri__GetDate, tempuri__GetDateResponse); }
        virtual int GetDate(const char *soap_endpoint, const char *soap_action, _tempuri__GetDate *tempuri__GetDate, _tempuri__GetDateResponse &tempuri__GetDateResponse);
        /// Web service operation 'GetDocCode' (returns SOAP_OK or error code)
        virtual int GetDocCode(_tempuri__GetDocCode *tempuri__GetDocCode, _tempuri__GetDocCodeResponse &tempuri__GetDocCodeResponse)
        { return this->GetDocCode(NULL, NULL, tempuri__GetDocCode, tempuri__GetDocCodeResponse); }
        virtual int GetDocCode(const char *soap_endpoint, const char *soap_action, _tempuri__GetDocCode *tempuri__GetDocCode, _tempuri__GetDocCodeResponse &tempuri__GetDocCodeResponse);
        /// Web service operation 'ToolingTimes' (returns SOAP_OK or error code)
        virtual int ToolingTimes(_tempuri__ToolingTimes *tempuri__ToolingTimes, _tempuri__ToolingTimesResponse &tempuri__ToolingTimesResponse)
        { return this->ToolingTimes(NULL, NULL, tempuri__ToolingTimes, tempuri__ToolingTimesResponse); }
        virtual int ToolingTimes(const char *soap_endpoint, const char *soap_action, _tempuri__ToolingTimes *tempuri__ToolingTimes, _tempuri__ToolingTimesResponse &tempuri__ToolingTimesResponse);
        /// Web service operation 'WorkOrder' (returns SOAP_OK or error code)
        virtual int WorkOrder(_tempuri__WorkOrder *tempuri__WorkOrder, _tempuri__WorkOrderResponse &tempuri__WorkOrderResponse)
        { return this->WorkOrder(NULL, NULL, tempuri__WorkOrder, tempuri__WorkOrderResponse); }
        virtual int WorkOrder(const char *soap_endpoint, const char *soap_action, _tempuri__WorkOrder *tempuri__WorkOrder, _tempuri__WorkOrderResponse &tempuri__WorkOrderResponse);
        /// Web service operation 'LineInventory' (returns SOAP_OK or error code)
        virtual int LineInventory(_tempuri__LineInventory *tempuri__LineInventory, _tempuri__LineInventoryResponse &tempuri__LineInventoryResponse)
        { return this->LineInventory(NULL, NULL, tempuri__LineInventory, tempuri__LineInventoryResponse); }
        virtual int LineInventory(const char *soap_endpoint, const char *soap_action, _tempuri__LineInventory *tempuri__LineInventory, _tempuri__LineInventoryResponse &tempuri__LineInventoryResponse);
        /// Web service operation 'LineInventoryUpdate' (returns SOAP_OK or error code)
        virtual int LineInventoryUpdate(_tempuri__LineInventoryUpdate *tempuri__LineInventoryUpdate, _tempuri__LineInventoryUpdateResponse &tempuri__LineInventoryUpdateResponse)
        { return this->LineInventoryUpdate(NULL, NULL, tempuri__LineInventoryUpdate, tempuri__LineInventoryUpdateResponse); }
        virtual int LineInventoryUpdate(const char *soap_endpoint, const char *soap_action, _tempuri__LineInventoryUpdate *tempuri__LineInventoryUpdate, _tempuri__LineInventoryUpdateResponse &tempuri__LineInventoryUpdateResponse);
        /// Web service operation 'LineInventoryReturn' (returns SOAP_OK or error code)
        virtual int LineInventoryReturn(_tempuri__LineInventoryReturn *tempuri__LineInventoryReturn, _tempuri__LineInventoryReturnResponse &tempuri__LineInventoryReturnResponse)
        { return this->LineInventoryReturn(NULL, NULL, tempuri__LineInventoryReturn, tempuri__LineInventoryReturnResponse); }
        virtual int LineInventoryReturn(const char *soap_endpoint, const char *soap_action, _tempuri__LineInventoryReturn *tempuri__LineInventoryReturn, _tempuri__LineInventoryReturnResponse &tempuri__LineInventoryReturnResponse);
        /// Web service operation 'ItemsAdd' (returns SOAP_OK or error code)
        virtual int ItemsAdd(_tempuri__ItemsAdd *tempuri__ItemsAdd, _tempuri__ItemsAddResponse &tempuri__ItemsAddResponse)
        { return this->ItemsAdd(NULL, NULL, tempuri__ItemsAdd, tempuri__ItemsAddResponse); }
        virtual int ItemsAdd(const char *soap_endpoint, const char *soap_action, _tempuri__ItemsAdd *tempuri__ItemsAdd, _tempuri__ItemsAddResponse &tempuri__ItemsAddResponse);
        /// Web service operation 'DocAdd' (returns SOAP_OK or error code)
        virtual int DocAdd(_tempuri__DocAdd *tempuri__DocAdd, _tempuri__DocAddResponse &tempuri__DocAddResponse)
        { return this->DocAdd(NULL, NULL, tempuri__DocAdd, tempuri__DocAddResponse); }
        virtual int DocAdd(const char *soap_endpoint, const char *soap_action, _tempuri__DocAdd *tempuri__DocAdd, _tempuri__DocAddResponse &tempuri__DocAddResponse);
        /// Web service operation 'GetDept' (returns SOAP_OK or error code)
        virtual int GetDept(_tempuri__GetDept *tempuri__GetDept, _tempuri__GetDeptResponse &tempuri__GetDeptResponse)
        { return this->GetDept(NULL, NULL, tempuri__GetDept, tempuri__GetDeptResponse); }
        virtual int GetDept(const char *soap_endpoint, const char *soap_action, _tempuri__GetDept *tempuri__GetDept, _tempuri__GetDeptResponse &tempuri__GetDeptResponse);
        /// Web service operation 'GetPower' (returns SOAP_OK or error code)
        virtual int GetPower(_tempuri__GetPower *tempuri__GetPower, _tempuri__GetPowerResponse &tempuri__GetPowerResponse)
        { return this->GetPower(NULL, NULL, tempuri__GetPower, tempuri__GetPowerResponse); }
        virtual int GetPower(const char *soap_endpoint, const char *soap_action, _tempuri__GetPower *tempuri__GetPower, _tempuri__GetPowerResponse &tempuri__GetPowerResponse);
        /// Web service operation 'Login' (returns SOAP_OK or error code)
        virtual int Login(_tempuri__Login *tempuri__Login, _tempuri__LoginResponse &tempuri__LoginResponse)
        { return this->Login(NULL, NULL, tempuri__Login, tempuri__LoginResponse); }
        virtual int Login(const char *soap_endpoint, const char *soap_action, _tempuri__Login *tempuri__Login, _tempuri__LoginResponse &tempuri__LoginResponse);
        /// Web service operation 'AnDan1Send' (returns SOAP_OK or error code)
        virtual int AnDan1Send(_tempuri__AnDan1Send *tempuri__AnDan1Send, _tempuri__AnDan1SendResponse &tempuri__AnDan1SendResponse)
        { return this->AnDan1Send(NULL, NULL, tempuri__AnDan1Send, tempuri__AnDan1SendResponse); }
        virtual int AnDan1Send(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan1Send *tempuri__AnDan1Send, _tempuri__AnDan1SendResponse &tempuri__AnDan1SendResponse);
        /// Web service operation 'AnDan4UpdPeoplePresent' (returns SOAP_OK or error code)
        virtual int AnDan4UpdPeoplePresent(_tempuri__AnDan4UpdPeoplePresent *tempuri__AnDan4UpdPeoplePresent, _tempuri__AnDan4UpdPeoplePresentResponse &tempuri__AnDan4UpdPeoplePresentResponse)
        { return this->AnDan4UpdPeoplePresent(NULL, NULL, tempuri__AnDan4UpdPeoplePresent, tempuri__AnDan4UpdPeoplePresentResponse); }
        virtual int AnDan4UpdPeoplePresent(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan4UpdPeoplePresent *tempuri__AnDan4UpdPeoplePresent, _tempuri__AnDan4UpdPeoplePresentResponse &tempuri__AnDan4UpdPeoplePresentResponse);
        /// Web service operation 'AnDan3AddUsers' (returns SOAP_OK or error code)
        virtual int AnDan3AddUsers(_tempuri__AnDan3AddUsers *tempuri__AnDan3AddUsers, _tempuri__AnDan3AddUsersResponse &tempuri__AnDan3AddUsersResponse)
        { return this->AnDan3AddUsers(NULL, NULL, tempuri__AnDan3AddUsers, tempuri__AnDan3AddUsersResponse); }
        virtual int AnDan3AddUsers(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan3AddUsers *tempuri__AnDan3AddUsers, _tempuri__AnDan3AddUsersResponse &tempuri__AnDan3AddUsersResponse);
        /// Web service operation 'AnDan2PeoplePresent' (returns SOAP_OK or error code)
        virtual int AnDan2PeoplePresent(_tempuri__AnDan2PeoplePresent *tempuri__AnDan2PeoplePresent, _tempuri__AnDan2PeoplePresentResponse &tempuri__AnDan2PeoplePresentResponse)
        { return this->AnDan2PeoplePresent(NULL, NULL, tempuri__AnDan2PeoplePresent, tempuri__AnDan2PeoplePresentResponse); }
        virtual int AnDan2PeoplePresent(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan2PeoplePresent *tempuri__AnDan2PeoplePresent, _tempuri__AnDan2PeoplePresentResponse &tempuri__AnDan2PeoplePresentResponse);
        /// Web service operation 'AnDanSLTypeGet' (returns SOAP_OK or error code)
        virtual int AnDanSLTypeGet(_tempuri__AnDanSLTypeGet *tempuri__AnDanSLTypeGet, _tempuri__AnDanSLTypeGetResponse &tempuri__AnDanSLTypeGetResponse)
        { return this->AnDanSLTypeGet(NULL, NULL, tempuri__AnDanSLTypeGet, tempuri__AnDanSLTypeGetResponse); }
        virtual int AnDanSLTypeGet(const char *soap_endpoint, const char *soap_action, _tempuri__AnDanSLTypeGet *tempuri__AnDanSLTypeGet, _tempuri__AnDanSLTypeGetResponse &tempuri__AnDanSLTypeGetResponse);
        /// Web service operation 'AnDanSLTypeAdd' (returns SOAP_OK or error code)
        virtual int AnDanSLTypeAdd(_tempuri__AnDanSLTypeAdd *tempuri__AnDanSLTypeAdd, _tempuri__AnDanSLTypeAddResponse &tempuri__AnDanSLTypeAddResponse)
        { return this->AnDanSLTypeAdd(NULL, NULL, tempuri__AnDanSLTypeAdd, tempuri__AnDanSLTypeAddResponse); }
        virtual int AnDanSLTypeAdd(const char *soap_endpoint, const char *soap_action, _tempuri__AnDanSLTypeAdd *tempuri__AnDanSLTypeAdd, _tempuri__AnDanSLTypeAddResponse &tempuri__AnDanSLTypeAddResponse);
        /// Web service operation 'GetDate' (returns SOAP_OK or error code)
        virtual int GetDate_(_tempuri__GetDate *tempuri__GetDate, _tempuri__GetDateResponse &tempuri__GetDateResponse)
        { return this->GetDate_(NULL, NULL, tempuri__GetDate, tempuri__GetDateResponse); }
        virtual int GetDate_(const char *soap_endpoint, const char *soap_action, _tempuri__GetDate *tempuri__GetDate, _tempuri__GetDateResponse &tempuri__GetDateResponse);
        /// Web service operation 'GetDocCode' (returns SOAP_OK or error code)
        virtual int GetDocCode_(_tempuri__GetDocCode *tempuri__GetDocCode, _tempuri__GetDocCodeResponse &tempuri__GetDocCodeResponse)
        { return this->GetDocCode_(NULL, NULL, tempuri__GetDocCode, tempuri__GetDocCodeResponse); }
        virtual int GetDocCode_(const char *soap_endpoint, const char *soap_action, _tempuri__GetDocCode *tempuri__GetDocCode, _tempuri__GetDocCodeResponse &tempuri__GetDocCodeResponse);
        /// Web service operation 'ToolingTimes' (returns SOAP_OK or error code)
        virtual int ToolingTimes_(_tempuri__ToolingTimes *tempuri__ToolingTimes, _tempuri__ToolingTimesResponse &tempuri__ToolingTimesResponse)
        { return this->ToolingTimes_(NULL, NULL, tempuri__ToolingTimes, tempuri__ToolingTimesResponse); }
        virtual int ToolingTimes_(const char *soap_endpoint, const char *soap_action, _tempuri__ToolingTimes *tempuri__ToolingTimes, _tempuri__ToolingTimesResponse &tempuri__ToolingTimesResponse);
        /// Web service operation 'WorkOrder' (returns SOAP_OK or error code)
        virtual int WorkOrder_(_tempuri__WorkOrder *tempuri__WorkOrder, _tempuri__WorkOrderResponse &tempuri__WorkOrderResponse)
        { return this->WorkOrder_(NULL, NULL, tempuri__WorkOrder, tempuri__WorkOrderResponse); }
        virtual int WorkOrder_(const char *soap_endpoint, const char *soap_action, _tempuri__WorkOrder *tempuri__WorkOrder, _tempuri__WorkOrderResponse &tempuri__WorkOrderResponse);
        /// Web service operation 'LineInventory' (returns SOAP_OK or error code)
        virtual int LineInventory_(_tempuri__LineInventory *tempuri__LineInventory, _tempuri__LineInventoryResponse &tempuri__LineInventoryResponse)
        { return this->LineInventory_(NULL, NULL, tempuri__LineInventory, tempuri__LineInventoryResponse); }
        virtual int LineInventory_(const char *soap_endpoint, const char *soap_action, _tempuri__LineInventory *tempuri__LineInventory, _tempuri__LineInventoryResponse &tempuri__LineInventoryResponse);
        /// Web service operation 'LineInventoryUpdate' (returns SOAP_OK or error code)
        virtual int LineInventoryUpdate_(_tempuri__LineInventoryUpdate *tempuri__LineInventoryUpdate, _tempuri__LineInventoryUpdateResponse &tempuri__LineInventoryUpdateResponse)
        { return this->LineInventoryUpdate_(NULL, NULL, tempuri__LineInventoryUpdate, tempuri__LineInventoryUpdateResponse); }
        virtual int LineInventoryUpdate_(const char *soap_endpoint, const char *soap_action, _tempuri__LineInventoryUpdate *tempuri__LineInventoryUpdate, _tempuri__LineInventoryUpdateResponse &tempuri__LineInventoryUpdateResponse);
        /// Web service operation 'LineInventoryReturn' (returns SOAP_OK or error code)
        virtual int LineInventoryReturn_(_tempuri__LineInventoryReturn *tempuri__LineInventoryReturn, _tempuri__LineInventoryReturnResponse &tempuri__LineInventoryReturnResponse)
        { return this->LineInventoryReturn_(NULL, NULL, tempuri__LineInventoryReturn, tempuri__LineInventoryReturnResponse); }
        virtual int LineInventoryReturn_(const char *soap_endpoint, const char *soap_action, _tempuri__LineInventoryReturn *tempuri__LineInventoryReturn, _tempuri__LineInventoryReturnResponse &tempuri__LineInventoryReturnResponse);
        /// Web service operation 'ItemsAdd' (returns SOAP_OK or error code)
        virtual int ItemsAdd_(_tempuri__ItemsAdd *tempuri__ItemsAdd, _tempuri__ItemsAddResponse &tempuri__ItemsAddResponse)
        { return this->ItemsAdd_(NULL, NULL, tempuri__ItemsAdd, tempuri__ItemsAddResponse); }
        virtual int ItemsAdd_(const char *soap_endpoint, const char *soap_action, _tempuri__ItemsAdd *tempuri__ItemsAdd, _tempuri__ItemsAddResponse &tempuri__ItemsAddResponse);
        /// Web service operation 'DocAdd' (returns SOAP_OK or error code)
        virtual int DocAdd_(_tempuri__DocAdd *tempuri__DocAdd, _tempuri__DocAddResponse &tempuri__DocAddResponse)
        { return this->DocAdd_(NULL, NULL, tempuri__DocAdd, tempuri__DocAddResponse); }
        virtual int DocAdd_(const char *soap_endpoint, const char *soap_action, _tempuri__DocAdd *tempuri__DocAdd, _tempuri__DocAddResponse &tempuri__DocAddResponse);
        /// Web service operation 'GetDept' (returns SOAP_OK or error code)
        virtual int GetDept_(_tempuri__GetDept *tempuri__GetDept, _tempuri__GetDeptResponse &tempuri__GetDeptResponse)
        { return this->GetDept_(NULL, NULL, tempuri__GetDept, tempuri__GetDeptResponse); }
        virtual int GetDept_(const char *soap_endpoint, const char *soap_action, _tempuri__GetDept *tempuri__GetDept, _tempuri__GetDeptResponse &tempuri__GetDeptResponse);
        /// Web service operation 'GetPower' (returns SOAP_OK or error code)
        virtual int GetPower_(_tempuri__GetPower *tempuri__GetPower, _tempuri__GetPowerResponse &tempuri__GetPowerResponse)
        { return this->GetPower_(NULL, NULL, tempuri__GetPower, tempuri__GetPowerResponse); }
        virtual int GetPower_(const char *soap_endpoint, const char *soap_action, _tempuri__GetPower *tempuri__GetPower, _tempuri__GetPowerResponse &tempuri__GetPowerResponse);
        /// Web service operation 'Login' (returns SOAP_OK or error code)
        virtual int Login_(_tempuri__Login *tempuri__Login, _tempuri__LoginResponse &tempuri__LoginResponse)
        { return this->Login_(NULL, NULL, tempuri__Login, tempuri__LoginResponse); }
        virtual int Login_(const char *soap_endpoint, const char *soap_action, _tempuri__Login *tempuri__Login, _tempuri__LoginResponse &tempuri__LoginResponse);
        /// Web service operation 'AnDan1Send' (returns SOAP_OK or error code)
        virtual int AnDan1Send_(_tempuri__AnDan1Send *tempuri__AnDan1Send, _tempuri__AnDan1SendResponse &tempuri__AnDan1SendResponse)
        { return this->AnDan1Send_(NULL, NULL, tempuri__AnDan1Send, tempuri__AnDan1SendResponse); }
        virtual int AnDan1Send_(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan1Send *tempuri__AnDan1Send, _tempuri__AnDan1SendResponse &tempuri__AnDan1SendResponse);
        /// Web service operation 'AnDan4UpdPeoplePresent' (returns SOAP_OK or error code)
        virtual int AnDan4UpdPeoplePresent_(_tempuri__AnDan4UpdPeoplePresent *tempuri__AnDan4UpdPeoplePresent, _tempuri__AnDan4UpdPeoplePresentResponse &tempuri__AnDan4UpdPeoplePresentResponse)
        { return this->AnDan4UpdPeoplePresent_(NULL, NULL, tempuri__AnDan4UpdPeoplePresent, tempuri__AnDan4UpdPeoplePresentResponse); }
        virtual int AnDan4UpdPeoplePresent_(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan4UpdPeoplePresent *tempuri__AnDan4UpdPeoplePresent, _tempuri__AnDan4UpdPeoplePresentResponse &tempuri__AnDan4UpdPeoplePresentResponse);
        /// Web service operation 'AnDan3AddUsers' (returns SOAP_OK or error code)
        virtual int AnDan3AddUsers_(_tempuri__AnDan3AddUsers *tempuri__AnDan3AddUsers, _tempuri__AnDan3AddUsersResponse &tempuri__AnDan3AddUsersResponse)
        { return this->AnDan3AddUsers_(NULL, NULL, tempuri__AnDan3AddUsers, tempuri__AnDan3AddUsersResponse); }
        virtual int AnDan3AddUsers_(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan3AddUsers *tempuri__AnDan3AddUsers, _tempuri__AnDan3AddUsersResponse &tempuri__AnDan3AddUsersResponse);
        /// Web service operation 'AnDan2PeoplePresent' (returns SOAP_OK or error code)
        virtual int AnDan2PeoplePresent_(_tempuri__AnDan2PeoplePresent *tempuri__AnDan2PeoplePresent, _tempuri__AnDan2PeoplePresentResponse &tempuri__AnDan2PeoplePresentResponse)
        { return this->AnDan2PeoplePresent_(NULL, NULL, tempuri__AnDan2PeoplePresent, tempuri__AnDan2PeoplePresentResponse); }
        virtual int AnDan2PeoplePresent_(const char *soap_endpoint, const char *soap_action, _tempuri__AnDan2PeoplePresent *tempuri__AnDan2PeoplePresent, _tempuri__AnDan2PeoplePresentResponse &tempuri__AnDan2PeoplePresentResponse);
        /// Web service operation 'AnDanSLTypeGet' (returns SOAP_OK or error code)
        virtual int AnDanSLTypeGet_(_tempuri__AnDanSLTypeGet *tempuri__AnDanSLTypeGet, _tempuri__AnDanSLTypeGetResponse &tempuri__AnDanSLTypeGetResponse)
        { return this->AnDanSLTypeGet_(NULL, NULL, tempuri__AnDanSLTypeGet, tempuri__AnDanSLTypeGetResponse); }
        virtual int AnDanSLTypeGet_(const char *soap_endpoint, const char *soap_action, _tempuri__AnDanSLTypeGet *tempuri__AnDanSLTypeGet, _tempuri__AnDanSLTypeGetResponse &tempuri__AnDanSLTypeGetResponse);
        /// Web service operation 'AnDanSLTypeAdd' (returns SOAP_OK or error code)
        virtual int AnDanSLTypeAdd_(_tempuri__AnDanSLTypeAdd *tempuri__AnDanSLTypeAdd, _tempuri__AnDanSLTypeAddResponse &tempuri__AnDanSLTypeAddResponse)
        { return this->AnDanSLTypeAdd_(NULL, NULL, tempuri__AnDanSLTypeAdd, tempuri__AnDanSLTypeAddResponse); }
        virtual int AnDanSLTypeAdd_(const char *soap_endpoint, const char *soap_action, _tempuri__AnDanSLTypeAdd *tempuri__AnDanSLTypeAdd, _tempuri__AnDanSLTypeAddResponse &tempuri__AnDanSLTypeAddResponse);
    };
#endif