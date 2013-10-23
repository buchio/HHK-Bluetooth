#ifndef H_DONGLE_H_131023082113_
#define H_DONGLE_H_131023082113_

///
/// Bluetooth Dongle States
///
typedef enum
{
    BT_INITIALIZE = 0,              ///< 0  Initialize the app when a device is attached
    BT_STATE_IDLE,                  ///< 1  Inactive State
    BT_STATE_ATTACHED,              ///< 2  Attached.
    BT_STATE_SEND_HCI_COMMAND,      ///< 3
    BT_STATE_RECV_HCI_EVENT,        ///< 4
    BT_STATE_RECV_HCI_EVENT_WAITING,///< 5
    BT_STATE_SEND_ACL,              ///< 6
    BT_STATE_RECV_ACL_HCI,          ///< 7
    BT_STATE_RECV_ACL_WAITING,      ///< 8
    BT_STATE_RECV_HCI,              ///< 9
    BT_STATE_RECV_HCI_WAITING,      ///< 10
    BT_STATE_ERROR,                 ///< 11 An error has occured
    BT_STATE_END                    ///< 12
} BT_STATE;

#define DATA_PACKET_LENGTH  64 ///< データパケット長

///
/// USBHostGenericClassRequestで指定するパラメータ定義
///
typedef struct {
    int size;
    char buf[DATA_PACKET_LENGTH];
} sendParam_t;

///
/// USBHostGenericReadで指定するパラメータ定義
///
typedef struct {
    int eventCode;
    unsigned char buf[DATA_PACKET_LENGTH];
} recvHciCommandParam_t;


extern sendParam_t sendHciCommandParam;
extern recvHciCommandParam_t recvHciCommandParam;
extern sendParam_t sendAclDataParam;
extern unsigned char recvAclBuf[DATA_PACKET_LENGTH];
extern unsigned char recvHciBuf[DATA_PACKET_LENGTH];
extern BT_STATE     btState;


void ManageBluetoothState ( void );


#endif //H_DONGLE_H_131023082113_
