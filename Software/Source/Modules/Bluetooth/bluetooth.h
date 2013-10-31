#ifndef H_BLUETOOTH_H_131023082120_
#define H_BLUETOOTH_H_131023082120_

/// \ingroup Bluetooth
/// \defgroup BluetoothCommunication
/// @{

///
/// Hci Sequence
///
typedef enum
{
    HCI_CMD_RESET = 0,              ///< 0	Initialize the hci when a device is attached
    HCI_CMD_RESET_END,              ///< 1
    HCI_CMD_READ_BD_ADDR,           ///< 2
    HCI_CMD_READ_BD_ADDR_END,       ///< 3
    HCI_CMD_LOCAL_NAME,             ///< 4
    HCI_CMD_LOCAL_NAME_END,         ///< 5
    HCI_CMD_CLASS_DEVICE,           ///< 6
    HCI_CMD_CLASS_DEVICE_END,       ///< 7
    HCI_CMD_SCAN_ENABLE,            ///< 8
    HCI_CMD_SCAN_ENABLE_END,        ///< 9
    HCI_CMD_WAIT_CONNECTION,        ///< 10
    HCI_CMD_CONNECTION_ACCEPT,      ///< 11
    HCI_CMD_CONNECTION_ACCEPT_END,  ///< 12
    HCI_CMD_CREATE_CONNECTION,      ///< 13
    HCI_CMD_CONNECTION_END,         ///< 14
    HCI_CMD_SIMPLE_PAIR,            ///< 15
    HCI_CMD_SIMPLE_PAIR_END,        ///< 16
    HCI_CMD_EVENT_MASK,             ///< 17
    HCI_CMD_EVENT_MASK_END,         ///< 18
    HCI_IOC_REPLY,                  ///< 19
    HCI_IOC_REPLY_END,              ///< 20
    HCI_CONF_REPLY,                 ///< 21
    HCI_CONF_REPLY_END,             ///< 22
    HCI_SAVE_LINK_KEY,              ///< 23
    HCI_AUTH_REQ,                   ///< 24
    HCI_AUTH_REQ_END,               ///< 25
    HCI_LINK_KEY_REP,               ///< 26
    HCI_LINK_KEY_REP_END,           ///< 27
    HCI_SET_ENCRYPT,                ///< 28
    HCI_SET_ENCRYPT_END,            ///< 29
    L2CAP_CON_REQ,                  ///< 30
    L2CAP_CON_RESP,                 ///< 31
    L2CAP_CONFIG_REQ,               ///< 32
    L2CAP_CONFIG_RESP,              ///< 33
    L2CAP_CONFIG_REQ_HOST,          ///< 34
    L2CAP_CONFIG_RESP_HOST,         ///< 35
    L2CAP_CONFIG_REQ_HOST_READ_END, ///< 36
    L2CAP_DISCONNECT_REQ,           ///< 37
    L2CAP_DISCONNECT_REQ_END,       ///< 38
    L2CAP_CON_REQ_TO,               ///< 39
    L2CAP_CON_RESP_FROM,            ///< 40
    L2CAP_CON_RESP1_FROM,           ///< 41
    L2CAP_CONFIG_REQ_FROM,          ///< 42
    L2CAP_CONFIG_RESP_TO,           ///< 43
    L2CAP_CONFIG_REQ_HOST_TO,       ///< 44
    L2CAP_CONFIG_RESP_HOST_FROM,    ///< 45
    SDP_SEARCH_REQ,                 ///< 46
    SDP_SERCH_RESP,                 ///< 47
    SDP_SERCH_RESP_MICRO,           ///< 48
    SDP_ATTR_REQ,                   ///< 49
    SDP_ATTR_REQ_READ_END,          ///< 50
    SDP_ATTR_RESP2a,                ///< 51
    SDP_ATTR_RESP2a_READ,           ///< 52
    SDP_ATTR_RESP2a_READ_END,       ///< 53
    SDP_ATTR_RESP2b,                ///< 54
    SDP_ATTR_RESP2b_READ,           ///< 55
    SDP_ATTR_RESP2b_READ_END,       ///< 56
    SDP_ATTR_RESP2c,                ///< 57
    SDP_ATTR_RESP2c_READ,           ///< 58
    SDP_ATTR_RESP2d_READ_END,       ///< 59
    SDP_ATTR_RESP3,                 ///< 60
    SDP_ATTR_RESP3_READ,            ///< 61
    SDP_ATTR_RESP3_READ_END,        ///< 62
    SDP_ATTR_RESP4a,                ///< 63
    SDP_ATTR_RESP4a_READ,           ///< 64
    SDP_ATTR_RESP4a_READ_END,       ///< 65
    SDP_ATTR_RESP4b,                ///< 66
    SDP_ATTR_RESP4b_READ,           ///< 67
    SDP_ATTR_RESP4b_READ_END,       ///< 68
    SDP_ATTR_RESP4c,                ///< 69
    SDP_ATTR_RESP4c_READ,           ///< 70
    SDP_ATTR_RESP4c_READ_END,       ///< 71
    SDP_ATTR_RESP5,                 ///< 72
    SDP_ATTR_RESP5_READ,            ///< 73
    SDP_ATTR_RESP5_READ_END,        ///< 74
    SDP_ATTR_END,                   ///< 75
    HID_START,                      ///< 76
    HID_START_RESP,                 ///< 77
    HID_HANDSHAKE_ERR,              ///< 78
    HID_READ_LED,                   ///< 79
    HCI_CMD_SCAN_DISABLE,           ///< 80
    HCI_CMD_SCAN_DISABLE_END,       ///< 81
    HID_WRITE_DATA,                 ///< 82
    HID_WRITE_DATA1,                ///< 83
    HID_WRITE_DATA2,                ///< 84
    HID_WRITE_DATA3,                ///< 85
    HCI_SEQUENCE_END                ///< 86
} HCI_SEQUENCE;

extern HCI_SEQUENCE hciSequence; ///< HCI通信シーケンス番号
extern unsigned char hidState;   ///< HID状態
extern unsigned char src_cid[2]; ///< HHK側のCID
extern unsigned char dst_cid[2]; ///< PC側のCID
extern unsigned char src_cid1[2];///< HHK側のCID (予備)
extern unsigned char dst_cid1[2];///< PC側のCID (予備)

void BluetoothInit( void );
void BluetoothTask( void );

void ManageHciSequence( void );


/// @}

#endif //H_BLUETOOTH_H_131023082120_
