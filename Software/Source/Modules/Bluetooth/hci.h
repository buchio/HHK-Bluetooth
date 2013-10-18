#ifndef H_HCI_H_131018122532_
#define H_HCI_H_131018122532_

/// \ingroup Modules
/// \ingroup Bluetooth
/// \defgroup HostControllerInterface HCI関連の定義
/// @{

/// \defgroup LinkControlCommands Link Control Commands (OGF = 0x01)
/// @{
#define HCI_CMD_INQUIRY		0x0401 ///< Command used to enter Inquiry mode where it discovers other Bluetooth devices.
#define HCI_CMD_INQUIRY_CANCEL		0x0402 ///< Command to cancel the Inquiry mode in which the Bluetooth device is in.
#define HCI_CMD_PERIODIC_INQUIRY_MODE	0x0403 ///< Command to set the device to enter Inquiry modes periodically according to the time interval set.
#define HCI_CMD_EXIT_PERIODIC_INQUIRY_MODE	0x0404 ///< Command to exit the periodic Inquiry mode
#define HCI_CMD_CREATE_CONNECTION	0x0405 ///< Command to create an ACL connection to the device specified by the BD_ADDR in the parameters.
#define HCI_CMD_DISCONNECT		0x0406 ///< Command to terminate the existing connection to a device
#define HCI_CMD_ADD_SCO_CONNECTION	0x0407 ///< Create an SCO connection defined by the connection handle parameters.
#define HCI_CMD_ACCEPT_CONNECTION_REQUEST	0x0409 ///< Command to accept a new connection request
#define HCI_CMD_REJECT_CONNECTION_REQUEST	0x040A ///< Command to reject a new connection request
#define HCI_CMD_LINK_KEY_REQUEST_REPLY	0x040B ///< Reply command to a link key request event sent from controller to the host
#define HCI_CMD_LINK_KEY_REQUEST_NEGATIVE_REPLY	0x040C ///< Reply command to a link key request event from the controller to the host if there is no link key associated with the connection.
#define HCI_CMD_PIN_CODE_REQUEST_REPLY	0x040D ///< Reply command to a PIN code request event sent from a controller to the host.
#define HCI_CMD_PIN_CODE_REQUEST_NEGATIVE_REPLY	0x040E ///< Reply command to a PIN code request event sent from the controller to the host if there is no PIN associated with the connection.
#define HCI_CMD_CHANGE_CONNECTION_PACKET_TYPE	0x040F ///< Command to change the type of packets to be sent for an existing connection.
#define HCI_CMD_AUTHENTICATION_REQUESTED	0x0411 ///< Command to establish authentication between two devices specified by the connection handle.
#define HCI_CMD_SET_CONNECTION_ENCRYPTION	0x0413 ///< Command to enable or disable the link level encryption.
#define HCI_CMD_CHANGE_CONNECTION_LINK_KEY	0x0415 ///< Command to force the change of a link key to a new one between two connected devices.
#define HCI_CMD_MASTER_LINK_KEY		0x0417 ///< Command to force two devices to use the master's link key temporarily.
#define HCI_CMD_REMOTE_NAME_REQUEST	0x0419 ///< Command to determine the user friendly name of the connected device.
#define HCI_CMD_READ_REMOTE_SUPPORTED_FEATURES	0x041B ///< Command to determine the features supported by the connected device.
#define HCI_CMD_READ_REMOTE_VERSION_INFORMATION	0x041D ///< Command to determine the version information of the connected device.
#define HCI_CMD_READ_CLOCK_OFFSET	0x041F ///< Command to read the clock offset of the remote device.
/// @}

/// \defgroup PolicyCommand HCI Policy Command (OGF=0x02)
/// @{
#define HCI_CMD_HOLD_MODE		0x0801 ///< Command to place the current or remote device into the Hold mode state.
#define HCI_CMD_SNIFF_MODE		0x0803 ///< Command to place the current or remote device into the Sniff mode state.
#define HCI_CMD_EXIT_SNIFF_MODE		0x0804 ///< Command to exit the current or remote device from the Sniff mode state.
#define HCI_CMD_PARK_MODE		0x0805 ///< Command to place the current or remote device into the Park mode state.
#define HCI_CMD_EXIT_PARK_MODE		0x0806 ///< Command to exit the current or remote device from the Park mode state.
#define HCI_CMD_QOS_SETUP		0x0807 ///< Command to setup the Quality of Service parameters of the device.
#define HCI_CMD_ROLE_DISCOVERY		0x0809 ///< Command to determine the role of the device for a particular connection.
#define HCI_CMD_SWITCH_ROLE		0x080B ///< Command to allow the device to switch roles for a particular connection.
#define HCI_CMD_READ_LINK_POLICY_SETTINGS	0x080C ///< Command to determine the link policy that the LM can use to establish connections.
#define HCI_CMD_WRITE_LINK_POLICY_SETTINGS	0x080D ///< Command to set the link policy that the LM can use for a particular connection.
/// @}

/// \defgroup ControllerAndBasebandCommands Host Controller and Baseband Commands (OGF=0x03)
/// @{
#define HCI_CMD_SET_EVENT_MASK		0x0c01 ///< Command to set which events are generated by the HCI for the host.
#define HCI_CMD_RESET		0x0c03 ///< Command to reset the host controller, link manager and the radio module.
#define HCI_CMD_SET_EVENT_FILTER	0x0c05 ///< Command used by host to set the different types of event filters that the host needs to receive.
#define HCI_CMD_FLUSH		0x0c08 ///< Command used to flush all pending data packets for transmission for a particular connection handle.
#define HCI_CMD_READ_PIN_TYPE		0x0c09 ///< Command used by host to determine if the link manager assumes that the host requires a variable PIN type or fixed PIN code. PIN is used during pairing.
#define HCI_CMD_WRITE_PIN_TYPE		0x0c0A ///< Command used by host to write to the host controller on the PIN type supported by the host.
#define HCI_CMD_CREATE_NEW_UNIT_KEY	0x0c0B ///< Command used to create a new unit key.
#define HCI_CMD_READ_STORED_LINK_KEY	0x0c0D ///< Command to read the link key stored in the host controller.
#define HCI_CMD_WRITE_STORED_LINK_KEY	0x0c11 ///< Command to write the link key to the host controller.
#define HCI_CMD_DELETE_STORED_LINK_KEY	0x0c12 ///< Command to delete a stored link key in the host controller.
#define HCI_CMD_CHANGE_LOCAL_NAME	0x0c13 ///< Command to modify the user friendly name of the device.
#define HCI_CMD_READ_LOCAL_NAME		0x0c14 ///< Command to read the user friendly name of the device.
#define HCI_CMD_READ_CONNECTION_ACCEPT_TIMEOUT	0x0c15 ///< Command to determine the timeout session before the host denies and rejects a new connection request.
#define HCI_CMD_WRITE_CONNECTION_ACCEPT_TIMEOUT	0x0c16 ///< Command to set the timeout session before a device can deny or reject a connection request.
#define HCI_CMD_READ_PAGE_TIMEOUT	0x0c17 ///< Command to read the timeout value where a device will wait for a connection acceptance before sending a connection failure is returned.
#define HCI_CMD_WRITE_PAGE_TIMEOUT	0x0c18 ///< Command to write the timeout value where a device will wait for a connection acceptance before sending a connection failure is returned.
#define HCI_CMD_READ_SCAN_ENABLE	0x0c19 ///< Command to read the status of the Scan_Enable configuration.
#define HCI_CMD_WRITE_SCAN_ENABLE	0x0c1A ///< Command to set the status of the Scan_Enable configuration.
#define HCI_CMD_READ_PAGE_SCAN_ACTIVITY	0x0c1B ///< Command to read the value of the Page_Scan_Interval and Page_Scan_Window configurations.
#define HCI_CMD_WRITE_PAGE_SCAN_ACTIVITY	0x0c1C ///< Command to write the value of the Page_Scan_Interval and Page_Scan_Window configurations.
#define HCI_CMD_READ_INQUIRY_SCAN_ACTIVITY	0x0c1D ///< Command to read the value of the Inquiry_Scan_Interval and Inquiry_Scan_Window configurations.
#define HCI_CMD_WRITE_INQUIRY_SCAN_ACTIVITY	0x0c1E ///< Command to set the value of the Inquiry_Scan_Interval and Inquiry_Scan_Window configurations.
#define HCI_CMD_READ_AUTHENTICATION_ENABLE	0x0c1F ///< Command to read the Authentication_Enable parameter.
#define HCI_CMD_WRITE_AUTHENTICATION_ENABLE	0x0c20 ///< Command to set the Authentication_Enable parameter.
#define HCI_CMD_READ_ENCRYPTION_MODE	0x0c21 ///< Command to read the Encryption_Mode parameter.
#define HCI_CMD_WRITE_ENCRYPTION_MODE	0x0c22 ///< Command to write the Encryption_Mode parameter.
#define HCI_CMD_READ_CLASS_OF_DEVICE	0x0c23 ///< Command to read the Class_Of_Device parameter.
#define HCI_CMD_WRITE_CLASS_OF_DEVICE	0x0c24 ///< Command to set the Class_Of_Device parameter.
#define HCI_CMD_READ_VOICE_SETTING	0x0c25 ///< Command to read the Voice_Setting parameter. Used for voice connections.
#define HCI_CMD_WRITE_VOICE_SETTING	0x0c26 ///< Command to set the Voice_Setting parameter. Used for voice connections.
#define HCI_CMD_READ_AUTOMATIC_FLUSH_TIMEOUT	0x0c27 ///< Command to read the Flush_Timeout parameter. Used for ACL connections only.
#define HCI_CMD_WRITE_AUTOMATIC_FLUSH_TIMEOUT	0x0c28 ///< Command to set the Flush_Timeout parameter. Used for ACL connections only.
#define HCI_CMD_READ_NUM_BROADCAST_RETRANSMISSIONS	0x0c29 ///< Command to read the number of time a broadcast message is retransmitted.
#define HCI_CMD_WRITE_NUM_BROADCAST_RETRANSMISSIONS	0x0c2A ///< Command to set the number of time a broadcast message is retransmitted.
#define HCI_CMD_READ_HOLD_MODE_ACTIVITY	0x0c2B ///< Command to set the Hold_Mode activity to instruct the device to perform an activity during hold mode.
#define HCI_CMD_WRITE_HOLD_MODE_ACTIVITY	0x0c2C ///< Command to set the Hold_Mode_Activity parameter.
#define HCI_CMD_READ_TRANSMIT_POWER_LEVEL	0x0c2D ///< Command to read the power level required for transmission for a connection handle.
#define HCI_CMD_READ_SCO_FLOW_CONTROL_ENABLE	0x0c2E ///< Command to check the current status of the flow control for the SCO connection.
#define HCI_CMD_WRITE_SCO_FLOW_CONTROL_ENABLE	0x0c2F ///< Command to set the status of the flow control for a connection handle.
#define HCI_CMD_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL	0x0c31 ///< Command to set the flow control from the host controller to host in on or off state.
#define HCI_CMD_HOST_BUFFER_SIZE	0x0c33 ///< Command set by host to inform the host controller of the buffer size of the host for ACL and SCO connections.
#define HCI_CMD_HOST_NUMBER_OF_COMPLETED_PACKETS	0x0c35 ///< Command set from host to host controller when it is ready to receive more data packets.
#define HCI_CMD_READ_LINK_SUPERVISION_TIMEOUT	0x0c36 ///< Command to read the timeout for monitoring link losses.
#define HCI_CMD_WRITE_LINK_SUPERVISION_TIMEOUT	0x0c37 ///< Command to set the timeout for monitoring link losses.
#define HCI_CMD_READ_NUMBER_OF_SUPPORTED_IAC	0x0c38 ///< Command to read the number of IACs that the device can listen on during Inquiry access.
#define HCI_CMD_READ_CURRENT_IAC_LAP	0x0c39 ///< Command to read the LAP for the current IAC.
#define HCI_CMD_WRITE_CURRENT_IAC_LAP	0x0c3A ///< Command to set the LAP for the current IAC.
#define HCI_CMD_READ_PAGE_SCAN_PERIOD_MODE	0x0c3B ///< Command to read the timeout session of a page scan.
#define HCI_CMD_WRITE_PAGE_SCAN_PERIOD_MODE	0x0c3C ///< Command to set the timeout session of a page scan.
#define HCI_CMD_READ_PAGE_SCAN_MODE	0x0c3D ///< Command to read the default Page scan mode.
#define HCI_CMD_WRITE_PAGE_SCAN_MODE	0x0c3E ///< Command to set the default page scan mode.
/// @}
 
/// \defgroup HCIEvents HCI Events
/// @{
#define HCI_CMD_INQUIRY_COMPLETE_EVENT	0x01 ///< Indicates the Inquiry has finished.
#define HCI_CMD_INQUIRY_RESULT_EVENT	0x02 ///< Indicates that Bluetooth device(s) have responded for the inquiry.
#define HCI_CMD_CONNECTION_COMPLETE_EVENT	0x03 ///< Indicates to both hosts that the new connection has been formed.
#define HCI_CMD_CONNECTION_REQUEST_EVENT	0x04 ///< Indicates that a new connection is trying to be established.
#define HCI_CMD_DISCONNECTION_COMPLETE_EVENT	0x05 ///< Occurs when a connection has been disconnected.
#define HCI_CMD_AUTHENTICATION_COMPLETE_EVENT	0x06 ///< Occurs when an authentication has been completed.
#define HCI_CMD_REMOTE_NAME_REQUEST_COMPLETE_EVENT	0x07 ///< Indicates that the request for the remote name has been completed.
#define HCI_CMD_ENCRYPTION_CHANGE_EVENT	0x08 ///< Indicates that a change in the encryption has been completed.
#define HCI_CMD_CHANGE_CONNECTION_LINK_KEY_COMPLETE_EVENT	0x09 ///< Indicates that the change in the link key has been completed.
#define HCI_CMD_MASTER_LINK_KEY_COMPLETE_EVENT	0x0A ///< Indicates that the change in the temporary link key or semi permanent link key on the master device is complete.
#define HCI_CMD_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE_EVENT	0x0B ///< Indicates that the reading of the supported features on the remote device is complete.
#define HCI_CMD_READ_REMOTE_VERSION_COMPLETE_EVENT	0x0C ///< Indicates that the version number on the remote device has been read and completed.
#define HCI_CMD_Q0S_SETUP_COMPLETE_EVENT	0x0D ///< Indicates that the Quality of Service setup has been complete.
#define HCI_CMD_COMMAND_COMPLETE_EVENT	0x0E ///< Used by controller to send status and event parameters to the host for the particular command.
#define HCI_CMD_COMMAND_STATUS_EVENT	0x0F ///< Indicates that the command has been received and is being processed in the host controller.
#define HCI_CMD_HARDWARE_ERROR_EVENT	0x10 ///< Indicates a hardware failure of the Bluetooth device.
#define HCI_CMD_FLUSH_OCCURED_EVENT	0x11 ///< Indicates that the data has been flushed for a particular connection.
#define HCI_CMD_ROLE_CHANGE_EVENT	0x12 ///< Indicates that the current bluetooth role for a connection has been changed.
#define HCI_CMD_NUMBER_OF_COMPLETED_PACKETS_EVENT	0x13 ///< Indicates to the host the number of data packets sent compared to the last time the same event was sent.
#define HCI_CMD_MODE_CHANGE_EVENT	0x14 ///< Indicates the change in mode from hold, sniff, park or active to another mode.
#define HCI_CMD_RETURN_LINK_KEYS_EVENT	0x15 ///< Used to return stored link keys after a Read_Stored_Link_Key command was issued.
#define HCI_CMD_PIN_CODE_REQUEST_EVENT	0x16 ///< Indicates the a PIN code is required for a new connection.
#define HCI_CMD_LINK_KEY_REQUEST_EVENT	0x17 ///< Indicates that a link key is required for the connection.
#define HCI_CMD_LINK_KEY_NOTIFICATION_EVENT	0x18 ///< Indicates to the host that a new link key has been created.
#define HCI_CMD_LOOPBACK_COMMAND_EVENT	0x19 ///< Indicates that command sent from the host will be looped back.
#define HCI_CMD_DATA_BUFFER_OVERFLOW_EVENT	0x1A ///< Indicates that the data buffers on the host has overflowed.
#define HCI_CMD_MAX_SLOTS_CHANGE_EVENT	0x1B ///< Informs the host when the LMP_Max_Slots parameter changes.
#define HCI_CMD_READ_CLOCK_OFFSET_COMPLETE_EVENT	0x1C ///< Indicates the completion of reading the clock offset information.
#define HCI_CMD_CONNECTION_PACKET_TYPE_CHANGED_EVENT	0x1D ///< Indicate the completion of the packet type change for a connection.
#define HCI_CMD_QOS_VIOLATION_EVENT	0x1E ///< Indicates that the link manager is unable to provide the required Quality of Service.
#define HCI_CMD_PAGE_SCAN_MODE_CHANGE_EVENT	0x1F ///< Indicates that the remote device has successfully changed the Page Scan mode.
#define HCI_CMD_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT	0x20 ///< Indicates that the remote device has successfully changed the Page Scan Repetition mode.
/// @}
 
/// HCI Error Codes
//    "Unknown HCI Command", 0x01
//    "No Connection", 0x02
//    "Hardware Failure", 0x03
//    "Page Timeout", 0x04
//    "Authentication Failure", 0x05
//    "Key Missing", 0x06
//    "Memory Full", 0x07
//    "Connection Timeout", 0x08
//    "Max Number Of Connections", 0x09
//    "Max Number Of SCO Connections To A Device", 0x0A
//    "ACL Connection Already Exists", 0x0B
//    "Command Disallowed", 0x0C
//    "Host Rejected Due To Limited Resources", 0x0D
//    "Host Rejected Due To Security Reasons", 0x0E
//    "Host Rejected Due To A Remote Device Only A Personal Device", 0x0F
//    "Host Timeout", 0x10
//    "Unsupported Feature Or Parameter Value", 0x11
//    "Invalid HCI Command Parameters", 0x12
//    "Other End Terminated Connection: User Ended Connection", 0x13
//    "Other End Terminated Connection: Low Resources", 0x14
//    "Other End Terminated Connection: About To Power Off", 0x15
//    "Connection Terminated By Local Host", 0x16
//    "Repeated Attempts", 0x17
//    "Pairing Not Allowed", 0x18
//    "Unknown LMP PDU", 0x19
//    "Unsupported Remote Feature", 0x1A
//    "SCO Offset Rejected", 0x1B
//    "SCO Interval Rejected", 0x1C
//    "SCO Air Mode Rejected", 0x1D
//    "Invalid LMP Parameters", 0x1E
//    "Unspecified Error", 0x1F
//    "Unsupported LMP Parameter", 0x20
//    "Role Change Not Allowed", 0x21
//    "LMP Response Timeout", 0x22
//    "LMP Error Transaction Collision", 0x23
//    "LMP PDU Not Allowed", 0x24
//    "Encryption Mode Not Acceptable", 0x25
//    "Unit Key Used", 0x26
//    "QoS Not Supported", 0x27
//    "Instant Passed", 0x28
//    "Pairing With Unit Key Not Supported", 0x29
//
 
/// @}

#endif //H_HCI_H_131018122532_
