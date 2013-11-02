// -*- coding:utf-8 -*-
/*
 *  Copyright (c) 2013  Yukio Obuchi
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction,
 *  including without limitation the rights to use, copy, modify, merge,
 *  publish, distribute, sublicense, and/or sell copies of the Software,
 *  and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 *  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 *  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

#ifndef H_HCI_DEFS_H_131023040849_
#define H_HCI_DEFS_H_131023040849_

/// \ingroup Bluetooth
/// \defgroup HostControllerInterface HCI関連の定義
/// @{
///  \defgroup HCICommandGroups HCIコマンドグループ
///  @{

///   \defgroup LinkControl Link Control (OGF = 0x01)
///   @{
#define HCI_CMD_INQUIRY                                    (0x0401) ///< HCI_Inquiry
#define HCI_CMD_INQUIRY_CANCEL                             (0x0402) ///< HCI_Inquiry_Cancel
#define HCI_CMD_PERIODIC_INQUIRY_MODE                      (0x0403) ///< HCI_Periodic_Inquiry_Mode
#define HCI_CMD_EXIT_PERIODIC_INQUIRY_MODE                 (0x0404) ///< HCI_Exit_Periodic_Inquiry_Mode
#define HCI_CMD_CREATE_CONNECTION                          (0x0405) ///< HCI_Create_Connection
#define HCI_CMD_DISCONNECT                                 (0x0406) ///< HCI_Disconnect
#define HCI_CMD_CREATE_CONNECTION_CANCEL                   (0x0408) ///< HCI_Create_Connection_Cancel
#define HCI_CMD_ACCEPT_CONNECTION_REQUEST                  (0x0409) ///< HCI_Accept_Connection_Request
#define HCI_CMD_REJECT_CONNECTION_REQUEST                  (0x040A) ///< HCI_Reject_Connection_Request
#define HCI_CMD_LINK_KEY_REQUEST_REPLY                     (0x040B) ///< HCI_Link_Key_Request_Reply
#define HCI_CMD_LINK_KEY_REQUEST_NEGATIVE_REPLY            (0x040C) ///< HCI_Link_Key_Request_Negative_Reply
#define HCI_CMD_PIN_CODE_REQUEST_REPLY                     (0x040D) ///< HCI_PIN_Code_Request_Reply
#define HCI_CMD_PIN_CODE_REQUEST_NEGATIVE_REPLY            (0x040E) ///< HCI_PIN_Code_Request_Negative_Reply
#define HCI_CMD_CHANGE_CONNECTION_PACKET_TYPE              (0x040F) ///< HCI_Change_Connection_Packet_Type
#define HCI_CMD_AUTHENTICATION_REQUESTED                   (0x0411) ///< HCI_Authentication_Requested
#define HCI_CMD_SET_CONNECTION_ENCRYPTION                  (0x0413) ///< HCI_Set_Connection_Encryption
#define HCI_CMD_CHANGE_CONNECTION_LINK_KEY                 (0x0415) ///< HCI_Change_Connection_Link_Key
#define HCI_CMD_MASTER_LINK_KEY                            (0x0417) ///< HCI_Master_Link_Key
#define HCI_CMD_REMOTE_NAME_REQUEST                        (0x0419) ///< HCI_Remote_Name_Request
#define HCI_CMD_REMOTE_NAME_REQUEST_CANCEL                 (0x041A) ///< HCI_Remote_Name_Request_Cancel
#define HCI_CMD_READ_REMOTE_SUPPORTED_FEATURES             (0x041B) ///< HCI_Read_Remote_Supported_Features
#define HCI_CMD_READ_REMOTE_EXTENDED_FEATURES              (0x041C) ///< HCI_Read_Remote_Extended_Features
#define HCI_CMD_READ_REMOTE_VERSION_INFORMATION            (0x041D) ///< HCI_Read_Remote_Version_Information
#define HCI_CMD_READ_CLOCK_OFFSET                          (0x041F) ///< HCI_Read_Clock_Offset
#define HCI_CMD_READ_LMP_HANDLE                            (0x0420) ///< HCI_Read_LMP_Handle
#define HCI_CMD_SETUP_SYNCHRONOUS_CONNECTION               (0x0428) ///< HCI_Setup_Synchronous_Connection
#define HCI_CMD_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST      (0x0429) ///< HCI_Accept_Synchronous_Connection_Request
#define HCI_CMD_REJECT_SYNCHRONOUS_CONNECTION_REQUEST      (0x042A) ///< HCI_Reject_Synchronous_Connection_Request
#define HCI_CMD_IO_CAPABILITY_REQUEST_REPLY                (0x042B) ///< HCI_IO_Capability_Request_Reply
#define HCI_CMD_USER_CONFIRMATION_REQUEST_REPLY            (0x042C) ///< HCI_User_Confirmation_Request_Reply
#define HCI_CMD_USER_CONFIRMATION_REQUEST_NEGATIVE_REPLY   (0x042D) ///< HCI_User_Confirmation_Request_Negative_Reply
#define HCI_CMD_USER_PASSKEY_REQUEST_REPLY                 (0x042E) ///< HCI_User_Passkey_Request_Reply
#define HCI_CMD_USER_PASSKEY_REQUESTNEGATIVE_REPLY         (0x042F) ///< HCI_User_Passkey_RequestNegative_Reply
#define HCI_CMD_REMOTE_OOB_DATA_REQUEST_REPLY              (0x0430) ///< HCI_Remote_OOB_Data_Request_Reply
#define HCI_CMD_REMOTE_OOB_DATA_REQUEST_NEGATIVE_REPLY     (0x0433) ///< HCI_Remote_OOB_Data_Request_Negative_Reply
#define HCI_CMD_IO_CAPABILITY_REQUEST_NEGATIVE_REPLY       (0x0434) ///< HCI_IO_Capability_Request_Negative_Reply
#define HCI_CMD_CREATE_PHYSICAL_LINK                       (0x0435) ///< HCI_Create_Physical_Link
#define HCI_CMD_ACCEPT_PHYSICAL_LINK                       (0x0436) ///< HCI_Accept_Physical_Link
#define HCI_CMD_DISCONNECT_PHYSICAL_LINK                   (0x0437) ///< HCI_Disconnect_Physical_Link
#define HCI_CMD_CREATE_LOGICAL_LINK                        (0x0438) ///< HCI_Create_Logical_Link
#define HCI_CMD_ACCEPT_LOGICAL_LINK                        (0x0439) ///< HCI_Accept_Logical_Link
#define HCI_CMD_DISCONNECT_LOGICAL_LINK                    (0x043A) ///< HCI_Disconnect_Logical_Link
#define HCI_CMD_LOGICAL_LINK_CANCEL                        (0x043B) ///< HCI_Logical_Link_Cancel
#define HCI_CMD_FLOW_SPEC_MODIFY                           (0x043C) ///< HCI_Flow_Spec_Modify
///   @}

///   \defgroup LinkPolicy Link Policy (OGF = 0x02)
///   @{
#define HCI_CMD_READ_DEFAULT_LINK                          (0x0800) ///< HCI_Read_Default_Link
#define HCI_CMD_HOLD_MODE                                  (0x0801) ///< HCI_Hold_Mode
#define HCI_CMD_SNIFF_MODE                                 (0x0803) ///< HCI_Sniff_Mode
#define HCI_CMD_EXIT_SNIFF_MODE                            (0x0804) ///< HCI_Exit_Sniff_Mode
#define HCI_CMD_PARK_STATE                                 (0x0805) ///< HCI_Park_State
#define HCI_CMD_EXIT_PARK_STATE                            (0x0806) ///< HCI_Exit_Park_State
#define HCI_CMD_QOS_SETUP                                  (0x0807) ///< HCI_QoS_Setup
#define HCI_CMD_ROLE_DISCOVERY                             (0x0809) ///< HCI_Role_Discovery
#define HCI_CMD_SWITCH_ROLE                                (0x080B) ///< HCI_Switch_Role
#define HCI_CMD_READ_LINK_POLICY_SETTINGS                  (0x080C) ///< HCI_Read_Link_Policy_Settings
#define HCI_CMD_WRITE_LINK_POLICY_SETTINGS                 (0x080D) ///< HCI_Write_Link_Policy_Settings
#define HCI_CMD_WRITE_DEFAULT_LINK_POLICY_SETTINGS         (0x080F) ///< HCI_Write_Default_Link_Policy_Settings
#define HCI_CMD_FLOW_SPECIFICATION                         (0x0810) ///< HCI_Flow_Specification
#define HCI_CMD_SNIFF_SUBRATING                            (0x0811) ///< HCI_Sniff_Subrating
///   @}

///   \defgroup HCIControlandBaseband HCI Control and Baseband (OGF = 0x03)
///   @{
#define HCI_CMD_SET_EVENT_MASK                             (0x0C01) ///< HCI_Set_Event_Mask
#define HCI_CMD_RESET                                      (0x0C03) ///< HCI_Reset
#define HCI_CMD_SET_EVENT_FILTER                           (0x0C05) ///< HCI_Set_Event_Filter
#define HCI_CMD_FLUSH                                      (0x0C08) ///< HCI_Flush
#define HCI_CMD_READ_PIN_TYPE                              (0x0C09) ///< HCI_Read_PIN_Type
#define HCI_CMD_WRITE_PIN_TYPE                             (0x0C0A) ///< HCI_Write_PIN_Type
#define HCI_CMD_CREATE_NEW_UNIT_KEY                        (0x0C0B) ///< HCI_Create_New_Unit_Key
#define HCI_CMD_READ_STORED_LINK_KEY                       (0x0C0D) ///< HCI_Read_Stored_Link_Key
#define HCI_CMD_WRITE_STORED_LINK_KEY                      (0x0C11) ///< HCI_Write_Stored_Link_Key
#define HCI_CMD_DELETE_STORED_LINK_KEY                     (0x0C12) ///< HCI_Delete_Stored_Link_Key
#define HCI_CMD_WRITE_LOCAL_NAME                           (0x0C13) ///< HCI_Write_Local_Name
#define HCI_CMD_READ_LOCAL_NAME                            (0x0C14) ///< HCI_Read_Local_Name
#define HCI_CMD_READ_CONNECTION_ACCEPT_TIMEOUT             (0x0C15) ///< HCI_Read_Connection_Accept_Timeout
#define HCI_CMD_WRITE_CONNECTION_ACCEPT_TIMEOUT            (0x0C16) ///< HCI_Write_Connection_Accept_Timeout
#define HCI_CMD_READ_PAGE_TIMEOUT                          (0x0C17) ///< HCI_Read_Page_Timeout
#define HCI_CMD_WRITE_PAGE_TIMEOUT                         (0x0C18) ///< HCI_Write_Page_Timeout
#define HCI_CMD_READ_SCAN_ENABLE                           (0x0C19) ///< HCI_Read_Scan_Enable
#define HCI_CMD_WRITE_SCAN_ENABLE                          (0x0C1A) ///< HCI_Write_Scan_Enable
#define HCI_CMD_READ_PAGE_SCAN_ACTIVITY                    (0x0C1B) ///< HCI_Read_Page_Scan_Activity
#define HCI_CMD_WRITE_PAGE_SCAN_ACTIVITY                   (0x0C1C) ///< HCI_Write_Page_Scan_Activity
#define HCI_CMD_READ_INQUIRY_SCAN_ACTIVITY                 (0x0C1D) ///< HCI_Read_Inquiry_Scan_Activity
#define HCI_CMD_WRITE_INQUIRY_SCAN_ACTIVITY                (0x0C1E) ///< HCI_Write_Inquiry_Scan_Activity
#define HCI_CMD_READ_AUTHENTICATION_ENABLE                 (0x0C1F) ///< HCI_Read_Authentication_Enable
#define HCI_CMD_WRITE_AUTHENTICATION_ENABLE                (0x0C20) ///< HCI_Write_Authentication_Enable
#define HCI_CMD_READ_CLASS_OF_DEVICE                       (0x0C23) ///< HCI_Read_Class_of_Device
#define HCI_CMD_WRITE_CLASS_OF_DEVICE                      (0x0C24) ///< HCI_Write_Class_of_Device
#define HCI_CMD_READ_VOICE_SETTING                         (0x0C25) ///< HCI_Read_Voice_Setting
#define HCI_CMD_WRITE_VOICE_SETTING                        (0x0C26) ///< HCI_Write_Voice_Setting
#define HCI_CMD_READ_AUTOMATIC_FLUSH_TIMEOUT               (0x0C27) ///< HCI_Read_Automatic_Flush_Timeout
#define HCI_CMD_WRITE_AUTOMATIC_FLUSH_TIMEOUT              (0x0C28) ///< HCI_Write_Automatic_Flush_Timeout
#define HCI_CMD_READ_NUM_BROADCAST_RETRANSMISSIONS         (0x0C29) ///< HCI_Read_Num_Broadcast_Retransmissions
#define HCI_CMD_WRITE_NUM_BROADCAST_RETRANSMISSIONS        (0x0C2A) ///< HCI_Write_Num_Broadcast_Retransmissions
#define HCI_CMD_READ_HOLD_MODE_ACTIVITY                    (0x0C2B) ///< HCI_Read_Hold_Mode_Activity
#define HCI_CMD_WRITE_HOLD_MODE_ACTIVITY                   (0x0C2C) ///< HCI_Write_Hold_Mode_Activity
#define HCI_CMD_READ_TRANSMIT_POWER_LEVEL                  (0x0C2D) ///< HCI_Read_Transmit_Power_Level
#define HCI_CMD_READ_SYNCHRONOUS_FLOW_CONTROL_ENABLE       (0x0C2E) ///< HCI_Read_Synchronous_Flow_Control_Enable
#define HCI_CMD_WRITE_SYNCHRONOUS_FLOW_CONTROL_ENABLE      (0x0C2F) ///< HCI_Write_Synchronous_Flow_Control_Enable
#define HCI_CMD_SET_CONTROLLER_TO_HOST_FLOW_CONTROL        (0x0C31) ///< HCI_Set_Controller_To_Host_Flow_Control
#define HCI_CMD_HOST_BUFFER_SIZE                           (0x0C33) ///< HCI_Host_Buffer_Size
#define HCI_CMD_HOST_NUMBER_OF_COMPLETED_PACKETS           (0x0C35) ///< HCI_Host_Number_Of_Completed_Packets
#define HCI_CMD_READ_LINK_SUPERVISION_TIMEOUT              (0x0C36) ///< HCI_Read_Link_Supervision_Timeout
#define HCI_CMD_WRITE_LINK_SUPERVISION_TIMEOUT             (0x0C37) ///< HCI_Write_Link_Supervision_Timeout
#define HCI_CMD_READ_NUMBER_OF_SUPPORTED_IAC               (0x0C38) ///< HCI_Read_Number_Of_Supported_IAC
#define HCI_CMD_READ_CURRENT_IAC_LAP                       (0x0C39) ///< HCI_Read_Current_IAC_LAP
#define HCI_CMD_WRITE_CURRENT_IAC_LAP                      (0x0C3A) ///< HCI_Write_Current_IAC_LAP
#define SET_AFH_HOST_CHANNEL_CLASSIFICATION                (0x0C3F) ///< Set_AFH_Host_Channel_Classification
#define HCI_CMD_READ_INQUIRY_SCAN_TYPE                     (0x0C42) ///< HCI_Read_Inquiry_Scan_Type
#define HCI_CMD_WRITE_INQUIRY_SCAN_TYPE                    (0x0C43) ///< HCI_Write_Inquiry_Scan_Type
#define HCI_CMD_READ_INQUIRY_MODE                          (0x0C44) ///< HCI_Read_Inquiry_Mode
#define HCI_CMD_WRITE_INQUIRY_MODE                         (0x0C45) ///< HCI_Write_Inquiry_Mode
#define HCI_CMD_READ_PAGE_SCAN_TYPE                        (0x0C46) ///< HCI_Read_Page_Scan_Type
#define HCI_CMD_WRITE_PAGE_SCAN_TYPE                       (0x0C47) ///< HCI_Write_Page_Scan_Type
#define READ_AFH_CHANNEL_ASSESSMENT_MODE                   (0x0C48) ///< Read_AFH_Channel_Assessment_Mode
#define WRITE_AFH_CHANNEL_ASSESSMENT_MODE                  (0x0C49) ///< Write_AFH_Channel_Assessment_Mode
#define HCI_CMD_READ_EXTENDED_INQUIRY_RESPONSE             (0x0C51) ///< HCI_Read_Extended_Inquiry_Response
#define HCI_CMD_WRITE_EXTENDED_INQUIRY_RESPONSE            (0x0C52) ///< HCI_Write_Extended_Inquiry_Response
#define HCI_CMD_REFRESH_ENCRYPTION_KEY                     (0x0C53) ///< HCI_Refresh_Encryption_Key
#define HCI_CMD_READ_SIMPLE_PAIRING_MODE                   (0x0C55) ///< HCI_Read_Simple_Pairing_Mode
#define HCI_CMD_WRITE_SIMPLE_PAIRING_MODE                  (0x0C56) ///< HCI_Write_Simple_Pairing_Mode
#define HCI_CMD_READ_LOCAL_OOB_DATA                        (0x0C57) ///< HCI_Read_Local_OOB_Data
#define HCI_CMD_READ_INQUIRY_RESPONSE_TRANSMIT_POWER_LEVEL (0x0C58) ///< HCI_Read_Inquiry_Response_Transmit_Power_Level
#define HCI_CMD_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL         (0x0C59) ///< HCI_Write_Inquiry_Transmit_Power_Level
#define HCI_CMD_READ_DEFAULT_ERRONEOUS_DATA_REPORTING      (0x0C5A) ///< HCI_Read_Default_Erroneous_Data_Reporting
#define HCI_CMD_WRITE_DEFAULT_ERRONEOUS_DATA_REPORTING     (0x0C5B) ///< HCI_Write_Default_Erroneous_Data_Reporting
#define HCI_CMD_ENHANCED_FLUSH                             (0x0C5F) ///< HCI_Enhanced_Flush
#define HCI_CMD_SEND_KEYPRESS_NOTIFICATION                 (0x0C60) ///< HCI_Send_Keypress_Notification
#define HCI_CMD_READ_LOGICAL_LINK_ACCEPT_TIMEOUT           (0x0C61) ///< HCI_Read_Logical_Link_Accept_Timeout
#define HCI_CMD_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT          (0x0C62) ///< HCI_Write_Logical_Link_Accept_Timeout
#define HCI_CMD_SET_EVENT_MASK_PAGE_2                      (0x0C63) ///< HCI_Set_Event_Mask_Page_2
#define HCI_CMD_READ_LOCATION_DATA                         (0x0C64) ///< HCI_Read_Location_Data
#define HCI_CMD_WRITE_LOCATION_DATA                        (0x0C65) ///< HCI_Write_Location_Data
#define HCI_CMD_READ_FLOW_CONTROL_MODE                     (0x0C66) ///< HCI_Read_Flow_Control_Mode
#define HCI_CMD_WRITE_FLOW_CONTROL_MODE                    (0x0C67) ///< HCI_Write_Flow_Control_Mode
#define HCI_CMD_READ_ENHANCE_TRANSMIT_POWER_LEVEL          (0x0C68) ///< HCI_Read_Enhance_Transmit_Power_Level
#define HCI_CMD_READ_BEST_EFFORT_FLUSH_TIMEOUT             (0x0C69) ///< HCI_Read_Best_Effort_Flush_Timeout
#define HCI_CMD_WRITE_BEST_EFFORT_FLUSH_TIMEOUT            (0x0C6A) ///< HCI_Write_Best_Effort_Flush_Timeout
#define HCI_CMD_SHORT_RANGE_MODE                           (0x0C6B) ///< HCI_Short_Range_Mode
#define HCI_CMD_READ_LE_HOST_SUPPORT                       (0x0C6C) ///< HCI_Read_LE_Host_Support
#define HCI_CMD_WRITE_LE_HOST_SUPPORT                      (0x0C6D) ///< HCI_Write_LE_Host_Support
///   @}

///   \defgroup ForInformationalParameters For Informational Parameters (OGF = 0x04)
///   @{
#define HCI_CMD_READ_LOCAL_VERSION_INFORMATION             (0x1001) ///< HCI_Read_Local_Version_Information
#define HCI_CMD_READ_LOCAL_SUPPORTED_COMMANDS              (0x1002) ///< HCI_Read_Local_Supported_Commands
#define HCI_CMD_READ_LOCAL_SUPPORTED_FEATURES              (0x1003) ///< HCI_Read_Local_Supported_Features
#define HCI_CMD_READ_LOCAL_EXTENDED_FEATURES               (0x1004) ///< HCI_Read_Local_Extended_Features
#define HCI_CMD_READ_BUFFER_SIZE                           (0x1005) ///< HCI_Read_Buffer_Size
#define HCI_CMD_READ_BD_ADDR                               (0x1009) ///< HCI_Read_BD_ADDR
#define HCI_CMD_READ_DATA_BLOCK_SIZE                       (0x100A) ///< HCI_Read_Data_Block_Size
///   @}

///   \defgroup StatusParameters Status Parameters (OGF = 0x05)
///   @{
#define HCI_CMD_READ_FAILED_CONTACT_COUNTER                (0x1401) ///< HCI_Read_Failed_Contact_Counter
#define HCI_CMD_RESET_FAILED_CONTACT_COUNTER               (0x1402) ///< HCI_Reset_Failed_Contact_Counter
#define HCI_CMD_READ_LINK_QUALITY                          (0x1403) ///< HCI_Read_Link_Quality
#define HCI_CMD_READ_RSSI                                  (0x1405) ///< HCI_Read_RSSI
#define HCI_CMD_READ_AFH_CHANNEL_MAP                       (0x1406) ///< HCI_Read_AFH_Channel_Map
#define HCI_CMD_READ_CLOCK                                 (0x1407) ///< HCI_Read_Clock
#define HCI_CMD_READ_ENCRYPTION_KEY_SIZE                   (0x1408) ///< HCI_Read_Encryption_Key_Size
#define HCI_CMD_READ_LOCAL_AMP_INFO                        (0x1409) ///< HCI_Read_Local_AMP_Info
#define HCI_CMD_READ_LOCAL_AMP_ASSOC                       (0x140A) ///< HCI_Read_Local_AMP_ASSOC
#define HCI_CMD_WRITE_REMOTE_AMP_ASSOC                     (0x140B) ///< HCI_Write_Remote_AMP_ASSOC
///   @}

///   \defgroup Testing Testing (OGF = 0x06)
///   @{
#define HCI_CMD_READ_LOOPBACK_MODE                         (0x1801) ///< HCI_Read_Loopback_Mode
#define HCI_CMD_WRITE_LOOPBACK_MODE                        (0x1802) ///< HCI_Write_Loopback_Mode
#define HCI_CMD_ENABLE_DEVICE_UNDER_TEST_MODE              (0x1803) ///< HCI_Enable_Device_Under_Test_Mode
#define HCI_CMD_WRITE_SIMPLE_PAIRING_DEBUG_MODE            (0x1804) ///< HCI_Write_Simple_Pairing_Debug_Mode
#define HCI_CMD_ENABLE_AMP_RECEIVER_REPORTS                (0x1807) ///< HCI_Enable_AMP_Receiver_Reports
#define HCI_CMD_AMP_TEST_END                               (0x1808) ///< HCI_AMP_Test_End
#define HCI_CMD_AMP_TEST                                   (0x1809) ///< HCI_AMP_Test
///   @}

///   \defgroup LEController LE Controller (OGF = 0x08)
///   @{
#define HCI_CMD_LE_SET_EVENT_MASK                          (0x2001) ///< HCI_LE_Set_Event_Mask
#define HCI_CMD_LE_READ_BUFFER_SIZE                        (0x2002) ///< HCI_LE_Read_Buffer_Size
#define HCI_CMD_LE_READ_LOCAL_SUPPORTED_FEATURES           (0x2003) ///< HCI_LE_Read_Local_Supported_Features
#define HCI_CMD_LE_SET_RANDOM_ADDRESS                      (0x2005) ///< HCI_LE_Set_Random_Address
#define HCI_CMD_LE_SET_ADVERTISING_PARAMETERS              (0x2006) ///< HCI_LE_Set_Advertising_Parameters
#define HCI_CMD_LE_READ_ADVERTISING_CHANNEL_TX_POWER       (0x2007) ///< HCI_LE_Read_Advertising_Channel_Tx_Power
#define HCI_CMD_LE_SET_ADVERTISING_DATA                    (0x2008) ///< HCI_LE_Set_Advertising_Data
#define HCI_CMD_LE_SET_SCAN_RESPONSE_DATA                  (0x2009) ///< HCI_LE_Set_Scan_Response_Data
#define HCI_CMD_LE_SET_ADVERTISE_ENABLE                    (0x200A) ///< HCI_LE_Set_Advertise_Enable
#define HCI_CMD_LE_SET_SCAN_PARAMETERS                     (0x200B) ///< HCI_LE_Set_Scan_Parameters
#define HCI_CMD_LE_SET_SCAN_ENABLE                         (0x200C) ///< HCI_LE_Set_Scan_Enable
#define HCI_CMD_LE_CREATE_CONNECTION                       (0x200D) ///< HCI_LE_Create_Connection
#define HCI_CMD_LE_CREATE_CONNECTION_CANCEL                (0x200E) ///< HCI_LE_Create_Connection_Cancel
#define HCI_CMD_LE_READ_WHITE_LIST_SIZE                    (0x200F) ///< HCI_LE_Read_White_List_Size
#define HCI_CMD_LE_CLEAR_WHITE_LIST                        (0x2010) ///< HCI_LE_Clear_White_List
#define HCI_CMD_LE_ADD_DEVICE_TO_WHITE_LIST                (0x2011) ///< HCI_LE_Add_Device_To_White_List
#define HCI_CMD_LE_REMOVE_DEVICE_FROM_WHITE_LIST           (0x2012) ///< HCI_LE_Remove_Device_From_White_List
#define HCI_CMD_LE_CONNECTION_UPDATE                       (0x2013) ///< HCI_LE_Connection_Update
#define HCI_CMD_LE_SET_HOST_CHANNEL_CLASSIFICATION         (0x2014) ///< HCI_LE_Set_Host_Channel_Classification
#define HCI_CMD_LE_READ_CHANNEL_MAP                        (0x2015) ///< HCI_LE_Read_Channel_Map
#define HCI_CMD_LE_READ_REMOTE_USED_FEATURES               (0x2016) ///< HCI_LE_Read_Remote_Used_Features
#define HCI_CMD_LE_ENCRYPT                                 (0x2017) ///< HCI_LE_Encrypt
#define HCI_CMD_LE_RAND                                    (0x2018) ///< HCI_LE_Rand
#define HCI_CMD_LE_START_ENCRYPTION                        (0x2019) ///< HCI_LE_Start_Encryption
#define HCI_CMD_LE_LONG_TERM_KEY_REQUEST_REPLY             (0x201A) ///< HCI_LE_Long_Term_Key_Request_Reply
#define HCI_CMD_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY  (0x201B) ///< HCI_LE_Long_Term_Key_Requested_Negative_Reply
#define HCI_CMD_LE_READ_SUPPORTED_STATES                   (0x201C) ///< HCI_LE_Read_Supported_States
#define HCI_CMD_LE_RECEIVER_TEST                           (0x201D) ///< HCI_LE_Receiver_Test
#define HCI_CMD_LE_TRANSMITTER_TEST                        (0x201E) ///< HCI_LE_Transmitter_Test
#define HCI_CMD_LE_TEST_END                                (0x201F) ///< HCI_LE_Test_End
///   @}
///  @}

///  \defgroup HCIEvent HCIイベント
///  @{
#define HCI_EVT_INQUIRY_COMPLETE                             (0x01) ///< InquiryComplete
#define HCI_EVT_INQUIRY_RESULT                               (0x02) ///< InquiryResult
#define HCI_EVT_CONNECTION_COMPLETE                          (0x03) ///< ConnectionComplete
#define HCI_EVT_CONNECTION_REQUEST                           (0x04) ///< ConnectionRequest
#define HCI_EVT_DISCONNECTION_COMPLETE                       (0x05) ///< DisconnectionComplete
#define HCI_EVT_AUTHENTICATION_COMPLETE                      (0x06) ///< AuthenticationComplete
#define HCI_EVT_REMOTE_NAME_REQUEST_COMPLETE                 (0x07) ///< RemoteNameRequestComplete
#define HCI_EVT_ENCRYPTION_CHANGE                            (0x08) ///< EncryptionChange
#define HCI_EVT_CHANGE_CONNECTION_LINK_KEY_COMPLETE          (0x09) ///< ChangeConnectionLinkKeyComplete
#define HCI_EVT_MASTER_LINK_KEY_COMPLETE                     (0x0A) ///< MasterLinkKeyComplete
#define HCI_EVT_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE      (0x0B) ///< ReadRemoteSupportedFeaturesComplete
#define HCI_EVT_READ_REMOTE_VERSION_INFORMATION_COMPLETE     (0x0C) ///< ReadRemoteVersionInformationComplete
#define HCI_EVT_QOS_SETUP_COMPLETE                           (0x0D) ///< QoSSetupComplete
#define HCI_EVT_COMMAND_COMPLETE                             (0x0E) ///< CommandComplete
#define HCI_EVT_COMMAND_STATUS                               (0x0F) ///< CommandStatus
#define HCI_EVT_HARDWARE_ERROR                               (0x10) ///< HardwareError
#define HCI_EVT_FLUSH_OCCURRED                               (0x11) ///< FlushOccurred
#define HCI_EVT_ROLE_CHANGE                                  (0x12) ///< RoleChange
#define HCI_EVT_NUMBER_OF_COMPLETED_PACKETS                  (0x13) ///< NumberOfCompletedPackets
#define HCI_EVT_MODE_CHANGE                                  (0x14) ///< ModeChange
#define HCI_EVT_RETURN_LINK_KEYS                             (0x15) ///< ReturnLinkKeys
#define HCI_EVT_PIN_CODE_REQUEST                             (0x16) ///< PINCodeRequest
#define HCI_EVT_LINK_KEY_REQUEST                             (0x17) ///< LinkKeyRequest
#define HCI_EVT_LINK_KEY_NOTIFICATION                        (0x18) ///< LinkKeyNotification
#define HCI_EVT_LOOPBACK_COMMAND                             (0x19) ///< LoopbackCommand
#define HCI_EVT_DATA_BUFFER_OVERFLOW                         (0x1A) ///< DataBufferOverflow
#define HCI_EVT_MAX_SLOTS_CHANGE                             (0x1B) ///< MaxSlotsChange
#define HCI_EVT_READ_CLOCK_OFFSET_COMPLETE                   (0x1C) ///< ReadClockOffsetComplete
#define HCI_EVT_CONNECTION_PACKET_TYPE_CHANGED               (0x1D) ///< ConnectionPacketTypeChanged
#define HCI_EVT_QOS_VIOLATION                                (0x1E) ///< QoSViolation
#define HCI_EVT_PAGE_SCAN_REPETITION_MODE_CHANGE             (0x20) ///< PageScanRepetitionModeChange
#define HCI_EVT_FLOW_SPECIFICATION_COMPLETE                  (0x21) ///< FlowSpecificationComplete
#define HCI_EVT_INQUIRY_RESULTWITHR_SS_I                     (0x22) ///< InquiryResultwithRSSI
#define HCI_EVT_READ_REMOTE_EXTENDED_FEATURES_COMPLETE       (0x23) ///< ReadRemoteExtendedFeaturesComplete
#define HCI_EVT_SYNCHRONOUS_CONNECTION_COMPLETE              (0x2C) ///< SynchronousConnectionComplete
#define HCI_EVT_SYNCHRONOUS_CONNECTION_CHANGED               (0x2D) ///< SynchronousConnectionChanged
#define HCI_EVT_SNIFF_SUBRATING                              (0x2E) ///< SniffSubrating
#define HCI_EVT_EXTENDED_INQUIRY_RESULT                      (0x2F) ///< ExtendedInquiryResult
#define HCI_EVT_ENCRYPTION_KEY_REFRESH_COMPLETE              (0x30) ///< EncryptionKeyRefreshComplete
#define HCI_EVT_IO_CAPABILITY_REQUEST                        (0x31) ///< IOCapabilityRequest
#define HCI_EVT_IO_CAPABILITY_RESPONSE                       (0x32) ///< IOCapabilityResponse
#define HCI_EVT_USER_CONFIRMATION_REQUEST                    (0x33) ///< UserConfirmationRequest
#define HCI_EVT_USER_PASSKEY_REQUEST                         (0x34) ///< UserPasskeyRequest
#define HCI_EVT_REMOTEO_OB_DATA_REQUEST                      (0x35) ///< RemoteOOBDataRequest
#define HCI_EVT_SIMPLE_PAIRING_COMPLETE                      (0x36) ///< SimplePairingComplete
#define HCI_EVT_LINK_SUPERVISION_TIMEOUT_CHANGED             (0x38) ///< LinkSupervisionTimeoutChanged
#define HCI_EVT_ENHANCED_FLUSH_COMPLETE                      (0x39) ///< EnhancedFlushComplete
#define HCI_EVT_USER_PASSKEY_NOTIFICATION                    (0x3B) ///< UserPasskeyNotification
#define HCI_EVT_KEYPRESS_NOTIFICATION                        (0x3C) ///< KeypressNotification
#define HCI_EVT_REMOTE_HOST_SUPPORTED_FEATURES_NOTIFICATION  (0x3D) ///< RemoteHostSupportedFeaturesNotification
#define HCI_EVT_LE_LONG_TERM_KEY_REQUEST                     (0x3E) ///< LELongTermKeyRequest
#define HCI_EVT_PHYSICAL_LINK_COMPLETE                       (0x40) ///< PhysicalLinkComplete
#define HCI_EVT_CHANNEL_SELECTED                             (0x41) ///< ChannelSelected
#define HCI_EVT_DISCONNECTION_PHYSICAL_LINK_COMPLETE         (0x42) ///< DisconnectionPhysicalLinkComplete
#define HCI_EVT_PHYSICAL_LINK_LOSS_EARLY_WARNING             (0x43) ///< PhysicalLinkLossEarlyWarning
#define HCI_EVT_PHYSICAL_LINK_RECOVERY                       (0x44) ///< PhysicalLinkRecovery
#define HCI_EVT_LOGICAL_LINK_COMPLETE                        (0x45) ///< LogicalLinkComplete
#define HCI_EVT_DISCONNECTION_LOGICAL_LINK_COMPLETE          (0x46) ///< DisconnectionLogicalLinkComplete
#define HCI_EVT_FLOW_SPEC_MODIFY_COMPLETE                    (0x47) ///< FlowSpecModifyComplete
#define HCI_EVT_NUMBER_OF_COMPLETED_DATA_BLOCKS              (0x48) ///< NumberOfCompletedDataBlocks
#define HCI_EVT_AMP_START_TEST                               (0x49) ///< AMPStartTest
#define HCI_EVT_AMP_TEST_END                                 (0x4A) ///< AMPTestEnd
#define HCI_EVT_AMP_RECEIVER_REPORT                          (0x4B) ///< AMPReceiverReport
#define HCI_EVT_SHORT_RANGE_MODE_CHANGE_COMPLETE             (0x4C) ///< Short_Range_Mode_Change_Complete
#define HCI_EVT_AMP_STATUS_CHANGE                            (0x4D) ///< AMP_Status_Change
///  @}
/// @}

const char *hciErrorCodeToText( unsigned char code );

#endif //H_HCI_DEFS_H_131023040849_
