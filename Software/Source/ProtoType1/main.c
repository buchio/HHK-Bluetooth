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

/**
 * \file
 * \author Yukio Obuchi 
 * \date 2013/10/06
 *
 * エントリモジュール
 *  
 * 現在はフィジビリティスタディを行っているので、仕様理解中の処理をここ
 * に記述しているが、将来的には処理は全て modules.h に定義されるモジュー
 * ルに記述し、このモジュールは、初期化後にメインループを起動するだけの
 * 役割とする予定
 *
 * 
 */

/**
 * \dir Modules
 *
 * Modules
 *
 *
 */

/**
 * \dir Microchip
 *
 * Microchip社製のコード
 *
 */

/**
 * \dir ProtoType1
 *
 * ProtoType1
 *
 *
 */


/**
 * \mainpage
 *
 * このソフトウェアは無保証です。私が作成した部分についてはMIT license
 * で提供しますが、Microchipディレクトリ にはMicrochip社製のファイルを
 * 含んでいます。それらのファイルについてはMicrochip社のライセンス条項
 * に従うものとします。
 *
 * \li \ref modules.h
 * \li \ref queue.h
 *
 *
 */

#include <stdio.h>
#include <stdarg.h>

#define USE_AND_OR
#include <xc.h>
#include <ports.h>
#include <timer.h>
#include <dpslp.h>

#include "../modules.h"


_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

#include "GenericTypeDefs.h"
#include "../Microchip/HardwareProfile.h"
#include "../Microchip/Include/USB/usb_config.h"
#include "../Microchip/Include/USB/usb.h"
#include "../Microchip/Include/USB/usb_host_generic.h"
#include "../Microchip/Include/timer.h"
#include "../Microchip/Include/DEE_Emulation_16-bit.h"


#define USB_DEBUG

#ifdef USB_DEBUG
#define DEBUG_PRINTF( ... ) printf( __VA_ARGS__ )
#define DEBUG_DUMP( xName, xBuf, xSize ) { \
        printf( xName );                   \
        for( i = 0; i < xSize; i++ ) {                  \
            printf( " %02X", *( (BYTE*)xBuf + i ) );    \
        }                                               \
        printf( "\r\n" );                               \
    }
#else
#define DEBUG_PRINTF( ... ) {}
#define DEBUG_DUMP( xName, xBuf, xSize ) {}
#endif

/// \defgroup USB USBホスト処理
/// @{

///
/// \defgroup USBVariables USBホスト共有変数
/// @{

///
/// Application States
///
typedef enum
{
    BT_INITIALIZE = 0,              ///< 0  Initialize the app when a device is attached
    BT_STATE_IDLE,                  ///< 1  Inactive State
    BT_STATE_ATTACHED,              ///< 2  Attached.
    BT_STATE_WRITE_CLASS,           ///< 3
    BT_STATE_READ_CLASS,            ///< 4
    BT_STATE_READ_CLASS_WAITING,    ///< 5
    BT_STATE_WRITE_ACL,             ///< 6
    BT_STATE_READ_ACL_HCI,          ///< 7
    BT_STATE_READ_ACL_WAITING,      ///< 8
    BT_STATE_READ_HCI,              ///< 9
    BT_STATE_READ_HCI_WAITING,      ///< 10
    BT_STATE_ERROR,                 ///< 11 An error has occured
    BT_STATE_END                    ///< 12
} BT_STATE;

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
    HCI_CMD_CONNECTION_COMP,        ///< 14
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

#define DATA_PACKET_LENGTH  64 ///< データパケット長


///
/// USBHostGenericClassRequestで指定するパラメータ定義
///
typedef struct {
    int size;
    char buf[DATA_PACKET_LENGTH];
} writeParam_t;

///
/// USBHostGenericReadで指定するパラメータ定義
///
typedef struct {
    int end_num;
    unsigned char buf[DATA_PACKET_LENGTH];
} readClassParam_t;


static int i;                             ///< 汎用イテレータ


static BYTE         sDeviceAddress;       ///< Address of the device on the USB
static BT_STATE     btState;              ///< Current state of Bluetooth dongle controler.
static HCI_SEQUENCE hciSequence;          ///< Current sequence position of HCI connection.


static writeParam_t writeClassParam;      ///< BT_STATE_WRITE_CLASS で指定するパラメータ
static readClassParam_t readClassParam;   ///< BE_STATE_READ_CLASS で指定するパラメータ定義

static writeParam_t writeAclParam;        ///< BT_STATE_WRITE_ACL で指定するパラメータ

static unsigned char readAclBuf[DATA_PACKET_LENGTH];
static unsigned char readHciBuf[DATA_PACKET_LENGTH];

static unsigned char ConnectSwitch;       ///< 接続スイッチの状態
static unsigned char aclHandle[2];        ///< Handle for ACL 
static unsigned char src_cid[2];          ///< HID interrupt (data) after hidState == 2
static unsigned char dst_cid[2];          ///< 
static unsigned char src_cid1[2];         ///< HID control after hidState == 3
static unsigned char dst_cid1[2];         ///< 

static unsigned char hidState = 0;        ///< HID接続状態
static unsigned char ep2BusyFlag = 0;     ///< エンドポイント2のビジーフラグ

/// @}


static BOOL CheckForNewAttach ( void )
{
    // Try to get the device address, if we don't have one.
    if (sDeviceAddress == 0) {
        GENERIC_DEVICE_ID DevID;

        DevID.vid   = 0x04D8;
        DevID.pid   = 0x000C;

        if (USBHostGenericGetDeviceAddress(&DevID)) {
            sDeviceAddress = DevID.deviceAddress;
            DEBUG_PRINTF( "Generic demo device attached - polled, deviceAddress= %d\r\n", sDeviceAddress );
            return TRUE;
        }
    }

    return FALSE;

}

static void WriteClass( HCI_SEQUENCE nextSequence, size_t length, ... )
{
    va_list args;
    va_start( args, length );
    writeClassParam.size = length;

    size_t i = 0;
    for( i = 0; i < length; i++ )
    {
        writeClassParam.buf[i] = va_arg( args, BYTE );
    }
    btState  = BT_STATE_WRITE_CLASS;
    hciSequence = nextSequence;
}

static void ReadClass( HCI_SEQUENCE nextSequence, int endnum )
{
    readClassParam.end_num = endnum;
    btState  = BT_STATE_READ_CLASS;
    hciSequence = nextSequence;
}

#define HCI_EXEC_CMD( xCurrentState, xNextState, ... )     \
	case xCurrentState:                                    \
        WriteClass( xCurrentState ## _END, __VA_ARGS__ );  \
        break;                                             \
    case xCurrentState ## _END:                            \
        ReadClass( xNextState, 0x0e );                     \
        break

static void ManageHciSequence( void )
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
            ReadClass( HCI_CMD_CONNECTION_ACCEPT, 0x04 );
            break;

        case HCI_CMD_CONNECTION_ACCEPT:
            DataEEWrite( readClassParam.buf[2], 1 );
            DataEEWrite( readClassParam.buf[3], 2 );
            DataEEWrite( readClassParam.buf[4], 3 );
            DataEEWrite( readClassParam.buf[5], 4 );
            DataEEWrite( readClassParam.buf[6], 5 );
            DataEEWrite( readClassParam.buf[7], 6 );
            WriteClass( HCI_CMD_CONNECTION_ACCEPT_END, 10, 0x09, 0x04, 0x07,
                        DataEERead(1),DataEERead(2),DataEERead(3),
                        DataEERead(4),DataEERead(5),DataEERead(6), 0x01 );
            break;
                          
        case HCI_CMD_CONNECTION_ACCEPT_END:
            ReadClass( L2CAP_CON_REQ, 0x03 );
            break;
            
        case HCI_CMD_CREATE_CONNECTION:
			if( ConnectSwitch != 1 ) { //GOTO FIRST CONNECTION
                hciSequence = HCI_CMD_WAIT_CONNECTION;
                break;
            }
            WriteClass( HCI_CMD_CONNECTION_COMP, 16,
                        0x05, 0x04, 0x0d,
                        DataEERead(1),DataEERead(2),DataEERead(3),
                        DataEERead(4),DataEERead(5),DataEERead(6),
                        0x10, 0x00, 0x00, 0x00, 0xc2, 0xb0, 0x00 );
            break;
        case HCI_CMD_CONNECTION_COMP:
            ReadClass( HCI_AUTH_REQ, 0x03 );
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
            aclHandle[0] = readClassParam.buf[3];
            aclHandle[1] = readClassParam.buf[4] + 0x20;
            btState = BT_STATE_READ_ACL_HCI;
            hciSequence = L2CAP_CON_RESP;
            break;
            
        case L2CAP_CON_RESP:
            if( readAclBuf[12] == 0x11 ) {
                hidState = 2;
            }
            writeAclParam.size = 20;
            memcpy( writeAclParam.buf, readAclBuf, 20 ); // 元のソースでは特に指定がない
            writeAclParam.buf[2]  = 0x10;
            writeAclParam.buf[4]  = 0x0c;
            writeAclParam.buf[8]  = 0x03;
            writeAclParam.buf[10] = 0x08;
            writeAclParam.buf[12] = readAclBuf[14] + 4;
            writeAclParam.buf[13] = readAclBuf[15];
            writeAclParam.buf[16] = 0x00;
            writeAclParam.buf[17] = 0x00;
            writeAclParam.buf[18] = 0x00;
            writeAclParam.buf[19] = 0x00;

            dst_cid[0] = writeAclParam.buf[12];
            dst_cid[1] = writeAclParam.buf[13];
            src_cid[0] = writeAclParam.buf[14];
            src_cid[1] = writeAclParam.buf[15];

            if( hidState == 2 ) {
                dst_cid1[0] = writeAclParam.buf[12];
                dst_cid1[1] = writeAclParam.buf[13];
                src_cid1[0] = writeAclParam.buf[14];
                src_cid1[1] = writeAclParam.buf[15];
            }

            btState = BT_STATE_WRITE_ACL;
            hciSequence = L2CAP_CONFIG_REQ;

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

static void ManageBluetoothState ( void )
{
    BYTE RetVal;
    static BT_STATE recentBtState = BT_STATE_END;
    static BT_STATE recentRecentBtState = BT_STATE_END;
    if( recentBtState != btState && recentRecentBtState != btState ) {
        DEBUG_PRINTF( "btState Changed %d -> %d -> %d\r\n", recentRecentBtState, recentBtState, btState );
        recentRecentBtState = recentBtState;
        recentBtState = btState;
    }

    // Watch for device detaching
    if (USBHostGenericDeviceDetached(sDeviceAddress) && sDeviceAddress != 0) {
        DEBUG_PRINTF( "Generic demo device detached - polled\r\n" );
        btState  = BT_INITIALIZE;
		hciSequence = HCI_CMD_RESET;
        sDeviceAddress   = 0;
    }

    switch (btState) {
        case BT_INITIALIZE:
            btState = BT_STATE_IDLE;
            break;

            /** Idle State:  Loops here until attach **/
        case BT_STATE_IDLE:
            if (CheckForNewAttach()) {
                btState = BT_STATE_ATTACHED;
            }
            break;

        case BT_STATE_ATTACHED:
            ManageHciSequence();
            break;

        case BT_STATE_WRITE_CLASS:
            if (!USBHostGenericTxIsBusy(sDeviceAddress)) {
                if ( (RetVal = USBHostGenericClassRequest( sDeviceAddress,
                                                           (BYTE *)writeClassParam.buf,
                                                           writeClassParam.size )) == USB_SUCCESS ) {
                    DEBUG_DUMP( "CMD ", writeClassParam.buf, writeClassParam.size );
                    btState = BT_STATE_ATTACHED;
                } else {
                    DEBUG_PRINTF( "Write Class Error (%02X) !\r\n", RetVal );	
                    btState = BT_STATE_ERROR;
                }
            }
            break;
            
        case BT_STATE_READ_CLASS:
            if (!USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                if ( (RetVal = USBHostGenericRead( sDeviceAddress, readClassParam.buf, DATA_PACKET_LENGTH)) == USB_SUCCESS ) {
                    btState = BT_STATE_READ_CLASS_WAITING;
                } else {
                    DEBUG_PRINTF( "Device Read Error !\r\n" );
                    btState = BT_STATE_ERROR;
                }
            }
            break;
            
        case BT_STATE_READ_CLASS_WAITING:
            if ( !USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                if( readClassParam.buf[0] != readClassParam.end_num) {
                    btState = BT_STATE_READ_CLASS;
                } else {
                    DEBUG_DUMP( "EVT ", readClassParam.buf, readClassParam.buf[1]+2 );
                    btState = BT_STATE_ATTACHED;
                }
            }
            break;
            
        case BT_STATE_WRITE_ACL:
            if ( !USBHostGenericTxIsBusy(sDeviceAddress) ) {
                if ( (RetVal = USBHostGenericAclWrite( sDeviceAddress, writeAclParam.buf, writeAclParam.size )) == USB_SUCCESS ) {
                    DEBUG_DUMP( "W ACL ", writeAclParam.buf, writeAclParam.size );
                    btState = BT_STATE_ATTACHED;
                } else {
                    DEBUG_PRINTF( "Write Acl Error !\r\n" );	
                }
            }
            break;
            
        case BT_STATE_READ_ACL_HCI:
            if( ep2BusyFlag == 1 ) {
                btState = BT_STATE_READ_ACL_WAITING;
            } else if ( !USBHostGenericRx2IsBusy( sDeviceAddress ) ) {
                if ( ( RetVal = USBHostGenericAclRead( sDeviceAddress, readAclBuf, DATA_PACKET_LENGTH)) == USB_SUCCESS ) {
                    btState = BT_STATE_READ_ACL_WAITING;
                } else {
                    DEBUG_PRINTF( "Read Acl Error !\r\n" );
                }
            }
            break;
            
        case BT_STATE_READ_ACL_WAITING:
            if ( !USBHostGenericRx2IsBusy( sDeviceAddress) )  {
                DEBUG_DUMP( "R ACL ", readAclBuf, readAclBuf[2] + 4 );
                ep2BusyFlag=0;			
                btState = BT_STATE_READ_HCI;
            } else if( hidState != 3 ) {
                ep2BusyFlag = 1;
                btState = BT_STATE_READ_HCI;
            }
            break;
            
        case BT_STATE_READ_HCI:
            if( ep2BusyFlag == 0 ) {
                readHciBuf[0] = 0xff;
            }
            if ( !USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                if ( (RetVal = USBHostGenericRead( sDeviceAddress, readHciBuf, DATA_PACKET_LENGTH ) ) == USB_SUCCESS ) {
                    btState = BT_STATE_READ_HCI_WAITING;
                    DEBUG_PRINTF( "READ HCI\r\n" );
                } else {
                    DEBUG_PRINTF( "Device Read Error !\r\n" );
                }
            }
            break;

        case BT_STATE_READ_HCI_WAITING:
            if( !USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                DEBUG_DUMP( "R HCI ", readHciBuf, readHciBuf[2] + 4 );
                if( ep2BusyFlag == 0 ) {
                    if(readHciBuf[0] == 0xff ) {
                        btState = BT_STATE_ATTACHED;
                    } else if( readHciBuf[0] == 0x31 ) {
                        hciSequence = HCI_IOC_REPLY;
                        btState = BT_STATE_ATTACHED;
                    } else {
                        btState = BT_STATE_READ_HCI;
                    }
                } else {
                    if( readHciBuf[0] == 0x05 ) {
                        hciSequence = L2CAP_DISCONNECT_REQ;
                        btState = BT_STATE_ATTACHED;
                    } else if( readHciBuf[0] == 0x31 ) {
                        hciSequence = HCI_IOC_REPLY;
                        btState = BT_STATE_ATTACHED;
                    } else {
                        btState = BT_STATE_READ_ACL_WAITING;}
                }
            }
            break;
            
        case BT_STATE_ERROR:
            btState = BT_INITIALIZE;
            break;

        case BT_STATE_END:
            btState = BT_INITIALIZE;
            break;
    }

    DelayMs(1); // 1ms delay

} // ManageDemoState


/// \defgroup USBSupportFunctions usb_host.cから呼ばれるサポート関数
/// @{

/**
 * Function:        USB_ApplicationEventHandler
 *
 * \param event     Identifies the bus event that occured
 * \param data      Pointer to event-specific data
 * \param size      Size of the event-specific data
 * \return          TRUE if the event was handled, FALSE if not
 *
 * Preconditions:   The USB must be initialized.
 *
 * Output:          deviceAddress (global)
 *                  Updates device address when an attach or detach occurs.
 *
 *                  btState (global)
 *                  Updates the bluetooth dongle controler state as appropriate when events occur.
 *
 * Side Effects:    Event-specific actions have been taken.
 *
 * Overview:        This routine is called by the Host layer or client
 *                  driver to notify the application of events that occur.
 *                  If the event is recognized, it is handled and the
 *                  routine returns TRUE.  Otherwise, it is ignored (or
 *                  just "sniffed" and the routine returns FALSE.
 */
BOOL USB_ApplicationEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    // DEBUG_PRINTF( "USB_ApplicationEventHandler : %d\r\n", (int)event );
    
    // Handle specific events.
    switch ( (INT)event )
    {
        case EVENT_GENERIC_ATTACH:
            if (size == sizeof(GENERIC_DEVICE_ID))
            {
                sDeviceAddress   = ((GENERIC_DEVICE_ID *)data)->deviceAddress;
                btState  = BT_STATE_ATTACHED;
                hciSequence = HCI_CMD_RESET;
                DEBUG_PRINTF( "Bluetooth dongle attached - event, deviceAddress=%d\r\n", sDeviceAddress );
                return TRUE;
            }
            break;

        case EVENT_GENERIC_DETACH:
            sDeviceAddress   = 0;
            btState = BT_INITIALIZE;
            DEBUG_PRINTF( "Bluetooth dongle detached - event\r\n" );
            return TRUE;

        case EVENT_GENERIC_TX_DONE:           // The main state machine will poll the driver.
            return TRUE;
        case EVENT_GENERIC_RX1_DONE:
            return TRUE;
        case EVENT_GENERIC_RX2_DONE:
            return TRUE;

        case EVENT_VBUS_REQUEST_POWER:
            // We'll let anything attach.
            return TRUE;

        case EVENT_VBUS_RELEASE_POWER:
            // We aren't keeping track of power.
            return TRUE;

        case EVENT_HUB_ATTACH:
            // printf( "\r\n***** USB Error - hubs are not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            // printf( "\r\n***** USB Error - device is not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            // printf( "\r\n***** USB Error - cannot enumerate device *****\r\n" );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            // printf( "\r\n***** USB Error - client driver initialization error *****\r\n" );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            // printf( "\r\n***** USB Error - out of heap memory *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            // printf( "\r\n***** USB Error - unspecified *****\r\n" );
            return TRUE;
            break;

        case EVENT_SUSPEND:
        case EVENT_DETACH:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
            return TRUE;
            break;

        default:
            break;
    }

    return FALSE;

} // USB_ApplicationEventHandler
/// @}

/// @}



/**
 * INT0割り込み
 * 
 * LEDを反転するだけ
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void)
{
    printf( "{%d}", PORTBbits.RB7 );
    
	LATBbits.LATB15=!LATBbits.LATB15;
	Int0_Clear_Intr_Status_Bit;
}

/**
 * タイマ1割り込み
 *
 * \note 仕様理解中
 * メッセージを表示してLEDを点滅するだけ
 * 
 */
void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt (void)
{
    static volatile unsigned int count = 0;
    // printf( "\r\n _T1Interrupt %u\r\n", count );
    count += 1;
    LATBbits.LATB15=!LATBbits.LATB15;
    T1_Clear_Intr_Status_Bit;
}

/**
 * main関数
 *
 */
int main(void) {
    IsResetFromDeepSleep();

    PLL_INITIALIZE();
    TRISB = 0b0111111111111111;// PORTB bit6 OUTPUT for LED
    Uart1Init();

    // Set Default Bluetooth dongle controler state
    btState = BT_INITIALIZE;
    hciSequence = HCI_CMD_RESET;
    
    // EE
    DataEEInit();
    dataEEFlags.val = 0;

    
    if ( USBHostInit(0) == TRUE ) {
        printf( "\r\n\r\nHHK Bluetooth Adapter Initialized.\r\n\r\n" );
    } else {
        printf( "\r\n\r\nCould not initialize HHK Bluetooth Adapter.  Halting.\r\n\r\n" );
        while (1);
    }

#if 1
    // タイマ1の初期化複数のパラメータを結合する際に「&」を用いるように
    // マクロが定義されているが、この場合、デフォルトがデバイスのデフォ
    // ルトとは異なり、0xFFFFになっているので、デフォルト値に期待せず全
    // てのビットを定義する必要がある。一般的なORルールではなく、ANDルー
    // ルになっているのは謎。PICの慣習か？
    //
    // 事前に、「USE_AND_OR」を定義することで回避できることを理解。やは
    // りPICの慣習らしい。
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_1);
    OpenTimer1(T1_ON | T1_PS_1_256, 0xf424);
    IEC0bits.T1IE = 1;
#endif

#if 1
	CNPU2=0x80;	//pin#16 pull-up CN23
	ConfigINT0(INT_ENABLE | FALLING_EDGE_INT | INT_PRI_1); /*Enable inerrupt*/
#endif

    printf( "DataEERead(1): %04X\r\n", DataEERead( 1 ) );
    printf( "dataEEFlags: %04X\r\n", dataEEFlags.val );
    printf( "DataEERead(2): %04X\r\n", DataEERead( 2 ) );
    printf( "dataEEFlags: %04X\r\n", dataEEFlags.val );
    
    while( 1 ) {
        static int c = 0;
        static int num = 0;
        
        while( 1 ) {
            ConnectSwitch=PORTBbits.RB7;
            
#if 1
            c = Uart1GetCh();
            if(c != -1) {
                if (c == 'A') LATB &= ~0b1000000000000000; //LED 点灯
                if (c == 'B') LATB |= 0b1000000000000000; //LED 点灯
                if (c == 'C') {
                    DataEEWrite( 0x0123, 1 );
                    printf( "dataEEFlags: %04X\r\n", dataEEFlags.val );
                }
                if (c == 'D') {
                    DataEEWrite( 0x4567, 1 );
                    printf( "dataEEFlags: %04X\r\n", dataEEFlags.val );
                }
                if (c == 'c') {
                    DataEEWrite( 0x89AB, 2 );
                    printf( "dataEEFlags: %04X\r\n", dataEEFlags.val );
                }
                
                if (c == 'd') {
                    DataEEWrite( 0xCDEF, 2 );
                    printf( "dataEEFlags: %04X\r\n", dataEEFlags.val );
                }
                
                if ( c < ' ' ) {
                    printf("[0x%02X]", c);
                } else {
                    printf("[%c]", c);
                }
            }
#endif
            num ++;

                    
#if 0
            // 通信速度を越えて出力する場合にはFlushでキュー内の送信待
            // ちをしないととキューが溢れる。暴走はしないが、文字抜けす
            // るので表示がグチャグチャになる。
            printf( "   Message Number %d\n", num );
            Uart1Flush();
#endif
            
#if 0
            // もしくは以下のようにキューが空でないとメッセージを表示し
            // ないようにしてもよい
            if( Uart1SendQueueSize() == 0 ) {
                printf( "   Message Number %d\n", num );
            }
#endif
            
            // Maintain USB Host State
            USBHostTasks();
            // Maintain Demo Application State
            ManageBluetoothState();

            Uart1Flush();

#if 0
            if( Uart1SendQueueSize() == 0 && Uart1ReceiveQueueSize() == 0 ) {
                // printf( "Going into Idle.\n" );
                Uart1Flush();
                Idle();
            }
#endif
        }
    }
    return 0;
}
