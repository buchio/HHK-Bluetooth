#include <stdio.h>
#include <stdarg.h>

#define USE_AND_OR
#include <xc.h>
#include <ports.h>

#define DEBUG_OUTPUT
#include "../../modules.h"
#include "dongle.h"
#include "bluetooth.h"

#include "../../Microchip/Include/DEE_Emulation_16-bit.h"

HCI_SEQUENCE hciSequence;          ///< Current sequence position of HCI connection.
unsigned char hidState = 0;        ///< HID接続状態
unsigned char src_cid[2];          ///< HID interrupt (data) after hidState == 2
unsigned char dst_cid[2];          ///< 
unsigned char src_cid1[2];         ///< HID control after hidState == 3
unsigned char dst_cid1[2];         ///< 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static unsigned char ConnectSwitch;       ///< 接続スイッチの状態
static unsigned char aclHandle[2];        ///< Handle for ACL 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BluetoothInit( void )
{
    hciSequence = HCI_CMD_RESET;
    DataEEInit();
    dataEEFlags.val = 0;
}

void BluetoothTask( void )
{
    // Update ConnectSwitch Status
    ConnectSwitch=PORTBbits.RB7;
}

static void SendHciCommand( HCI_SEQUENCE nextSequence, size_t length, ... )
{
    va_list args;
    va_start( args, length );
    sendHciCommandParam.size = length;

    size_t i = 0;
    for( i = 0; i < length; i++ )
    {
        sendHciCommandParam.buf[i] = va_arg( args, BYTE );
    }
    btState  = BT_STATE_SEND_HCI_COMMAND;
    hciSequence = nextSequence;
}

static void RecvHciEvent( HCI_SEQUENCE nextSequence, int endnum )
{
    recvHciCommandParam.eventCode = endnum;
    btState  = BT_STATE_RECV_HCI_EVENT;
    hciSequence = nextSequence;
}

static void SendAclData( HCI_SEQUENCE nextSequence, size_t length, ... )
{
    va_list args;
    va_start( args, length );
    sendAclDataParam.size = length;

    size_t i = 0;
    for( i = 0; i < length; i++ )
    {
        sendAclDataParam.buf[i] = va_arg( args, BYTE );
    }
    btState = BT_STATE_SEND_ACL;
    hciSequence = nextSequence;
}

static void RecvAclData( HCI_SEQUENCE nextSequence )
{
    btState  = BT_STATE_RECV_ACL_HCI;
    hciSequence = nextSequence;
}

#define HCI_EXEC_CMD( xCurrentState, xNextState, ... )        \
	case xCurrentState:                                       \
        SendHciCommand( xCurrentState ## _END, __VA_ARGS__ ); \
        break;                                                \
    case xCurrentState ## _END:                               \
        RecvHciEvent( xNextState, 0x0e );                     \
        break

void ManageHciSequence( void )
{
    static HCI_SEQUENCE recentHciSequence = HCI_SEQUENCE_END;
    if( recentHciSequence != hciSequence ) {
        DEBUG_PRINTF( "hciSequence Changed %d -> %d\r\n", recentHciSequence, hciSequence );
        recentHciSequence = hciSequence;
    }
    
    switch (hciSequence) {
        HCI_EXEC_CMD( HCI_CMD_RESET,        HCI_CMD_READ_BD_ADDR, 3, 0x03, 0x0C, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_READ_BD_ADDR, HCI_CMD_LOCAL_NAME,   3, 0x09, 0x10, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_LOCAL_NAME,   HCI_CMD_CLASS_DEVICE, 7, 0x13, 0x0c, 0x04, 'H', 'H', 'K', 0x00 );
        HCI_EXEC_CMD( HCI_CMD_CLASS_DEVICE, HCI_CMD_SIMPLE_PAIR,  6, 0x24, 0x0c, 0x03, 0x40, 0x05, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_SIMPLE_PAIR,  HCI_CMD_EVENT_MASK,   4, 0x56, 0x0c, 0x01, 0x01 );
        HCI_EXEC_CMD( HCI_CMD_EVENT_MASK,   HCI_CMD_SCAN_ENABLE,  11, 0x01, 0x0c, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_SCAN_ENABLE,  HCI_CMD_CREATE_CONNECTION, 4, 0x1a, 0x0c, 0x01, 0x03 );
        
        case HCI_CMD_WAIT_CONNECTION:
            RecvHciEvent( HCI_CMD_CONNECTION_ACCEPT, 0x04 );
            break;

        case HCI_CMD_CONNECTION_ACCEPT:
            DataEEWrite( recvHciCommandParam.buf[2], 1 );
            DataEEWrite( recvHciCommandParam.buf[3], 2 );
            DataEEWrite( recvHciCommandParam.buf[4], 3 );
            DataEEWrite( recvHciCommandParam.buf[5], 4 );
            DataEEWrite( recvHciCommandParam.buf[6], 5 );
            DataEEWrite( recvHciCommandParam.buf[7], 6 );
            SendHciCommand( HCI_CMD_CONNECTION_ACCEPT_END, 10, 0x09, 0x04, 0x07,
                            DataEERead(1),DataEERead(2),DataEERead(3),
                            DataEERead(4),DataEERead(5),DataEERead(6), 0x01 );
            break;
                          
        case HCI_CMD_CONNECTION_ACCEPT_END:
            RecvHciEvent( L2CAP_CON_REQ, 0x03 );
            break;
            
        case HCI_CMD_CREATE_CONNECTION:
			if( ConnectSwitch != 1 ) { //GOTO FIRST CONNECTION
                hciSequence = HCI_CMD_WAIT_CONNECTION;
                break;
            }
            SendHciCommand( HCI_CMD_CONNECTION_COMP, 16,
                            0x05, 0x04, 0x0d,
                            DataEERead(1),DataEERead(2),DataEERead(3),
                            DataEERead(4),DataEERead(5),DataEERead(6),
                            0x10, 0x00, 0x00, 0x00, 0xc2, 0xb0, 0x00 );
            break;
        case HCI_CMD_CONNECTION_COMP:
            RecvHciEvent( HCI_AUTH_REQ, 0x03 );
            break;
            
        case HCI_AUTH_REQ:
        case HCI_AUTH_REQ_END:

        case HCI_IOC_REPLY:
        case HCI_IOC_REPLY_END:
        case HCI_CONF_REPLY:
        case HCI_CONF_REPLY_END:
        case HCI_SAVE_LINK_KEY:

        case HCI_LINK_KEY_REP:
        case HCI_LINK_KEY_REP_END:
        case HCI_SET_ENCRYPT:
        case HCI_SET_ENCRYPT_END:

        case L2CAP_CON_REQ:
            aclHandle[0] = recvHciCommandParam.buf[3];
            aclHandle[1] = recvHciCommandParam.buf[4] + 0x20;
            RecvAclData( L2CAP_CON_RESP );
            break;
            
        case L2CAP_CON_RESP:
            if( recvAclBuf[12] == 0x11 ) {
                hidState = 2;
            }

            dst_cid[0] = recvAclBuf[14] + 4;
            dst_cid[1] = recvAclBuf[15];
            src_cid[0] = recvAclBuf[14];
            src_cid[1] = recvAclBuf[15];

            if( hidState == 2 ) {
                dst_cid1[0] = recvAclBuf[14] + 4;
                dst_cid1[1] = recvAclBuf[15];
                src_cid1[0] = recvAclBuf[14];
                src_cid1[1] = recvAclBuf[15];
            }

            SendAclData( L2CAP_CONFIG_REQ, 20,
                         recvAclBuf[0],      // 0
                         recvAclBuf[1],      // 1
                         0x10,               // 2
                         recvAclBuf[3],      // 3
                         0x0c,               // 4
                         recvAclBuf[5],      // 5
                         recvAclBuf[6],      // 6
                         recvAclBuf[7],      // 7
                         0x03,               // 8
                         recvAclBuf[9],      // 9
                         0x08,               // 10
                         recvAclBuf[11],     // 11
                         recvAclBuf[14] + 4, // 12
                         recvAclBuf[15],     // 13
                         recvAclBuf[14],     // 14
                         recvAclBuf[15],     // 15
                         0x00,               // 16
                         0x00,               // 17
                         0x00,               // 18
                         0x00 );             // 19

            break;

        case L2CAP_CONFIG_REQ:
            

        case L2CAP_CONFIG_RESP:
        case L2CAP_CONFIG_REQ_HOST:
        case L2CAP_CONFIG_RESP_HOST:
        case L2CAP_CONFIG_REQ_HOST_READ_END:
        case L2CAP_DISCONNECT_REQ:
        case L2CAP_DISCONNECT_REQ_END:

        case L2CAP_CON_REQ_TO:
        case L2CAP_CON_RESP_FROM:
        case L2CAP_CON_RESP1_FROM:
        case L2CAP_CONFIG_REQ_FROM:
        case L2CAP_CONFIG_RESP_TO:
        case L2CAP_CONFIG_REQ_HOST_TO:
        case L2CAP_CONFIG_RESP_HOST_FROM:

        case SDP_SEARCH_REQ:
        case SDP_SERCH_RESP:
        case SDP_SERCH_RESP_MICRO:
        case SDP_ATTR_REQ:
        case SDP_ATTR_REQ_READ_END:
        case SDP_ATTR_RESP2a:
        case SDP_ATTR_RESP2a_READ:
        case SDP_ATTR_RESP2a_READ_END:
        case SDP_ATTR_RESP2b:
        case SDP_ATTR_RESP2b_READ:
        case SDP_ATTR_RESP2b_READ_END:
        case SDP_ATTR_RESP2c:
        case SDP_ATTR_RESP2c_READ:
        case SDP_ATTR_RESP2d_READ_END:
        case SDP_ATTR_RESP3:
        case SDP_ATTR_RESP3_READ:
        case SDP_ATTR_RESP3_READ_END:
        case SDP_ATTR_RESP4a:
        case SDP_ATTR_RESP4a_READ:
        case SDP_ATTR_RESP4a_READ_END:
        case SDP_ATTR_RESP4b:
        case SDP_ATTR_RESP4b_READ:
        case SDP_ATTR_RESP4b_READ_END:
        case SDP_ATTR_RESP4c:
        case SDP_ATTR_RESP4c_READ:
        case SDP_ATTR_RESP4c_READ_END:
        case SDP_ATTR_RESP5:
        case SDP_ATTR_RESP5_READ:
        case SDP_ATTR_RESP5_READ_END:
        case SDP_ATTR_END:

        case HID_START:
        case HID_START_RESP:
        case HID_HANDSHAKE_ERR:
        case HID_READ_LED:
        case HCI_CMD_SCAN_DISABLE:
        case HCI_CMD_SCAN_DISABLE_END:
        case HID_WRITE_DATA:
        case HID_WRITE_DATA1:
        case HID_WRITE_DATA2:
        case HID_WRITE_DATA3:
        case HCI_SEQUENCE_END:
            break;
    }
}
