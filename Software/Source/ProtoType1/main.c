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


/// \defgroup USB USBホスト処理
/// @{

///
/// Application States
///
typedef enum
{
    BT_INITIALIZE = 0,                ///< Initialize the app when a device is attached
    BT_STATE_IDLE,                    ///< Inactive State
    BT_STATE_ATTACHED,                ///< Attached.

    BT_STATE_WRITE_CLASS,
    BT_STATE_READ_CLASS,
    BT_STATE_READ_CLASS_WAITING,
    BT_STATE_WRITE_ACL,
    BT_STATE_READ_ACL_HCI,
    BT_STATE_READ_ACL_WAITING,
    BT_STATE_READ_HCI_WAITING,
    BT_STATE_READ_HCI,

    BT_STATE_ERROR,                   ///< An error has occured
    BT_STATE_END
} BT_STATE;

///
/// Hci States
///
typedef enum
{
	HCI_CMD_RESET = 0,                ///< Initialize the hci when a device is attached
    HCI_CMD_RESET_END,
    HCI_CMD_READ_BD_ADDR,
    HCI_CMD_READ_BD_ADDR_END,
    HCI_CMD_LOCAL_NAME,
    HCI_CMD_LOCAL_NAME_END,
    HCI_CMD_CLASS_DEVICE_WRITE,
    HCI_CMD_CLASS_DEVICE_WRITE_END,
	HCI_CMD_SCAN_ENABLE_WRITE,
    HCI_CMD_SCAN_ENABLE_WRITE_END,
	HCI_CMD_WAIT_CONNECTION,
	HCI_CMD_CONNECTION_ACCEPT,
    HCI_CMD_CONNECTION_ACCEPT_WRITE_END,

	HCI_CMD_CREATE_CONNECTION,
    HCI_CMD_CONNECTION_COMP,

	HCI_CMD_SIMPLE_PAIR,
	HCI_CMD_SIMPLE_PAIR_END,
	HCI_CMD_EVENT_MASK,
	HCI_CMD_EVENT_MASK_END,
	HCI_IOC_REPLY,
	HCI_IOC_REPLY_END,
	HCI_CONF_REPLY,
	HCI_CONF_REPLY_END,
	HCI_SAVE_LINK_KEY,

	HCI_AUTH_REQ,
	HCI_AUTH_REQ_END,
	HCI_LINK_KEY_REP,
	HCI_LINK_KEY_REP_END,
	HCI_SET_ENCRYPT,
	HCI_SET_ENCRYPT_END,

	L2CAP_CON_REQ,
	L2CAP_CON_RESP,
	L2CAP_CONFIG_REQ,
	L2CAP_CONFIG_RESP,
	L2CAP_CONFIG_REQ_HOST,
    L2CAP_CONFIG_RESP_HOST,
    L2CAP_CONFIG_REQ_HOST_READ_END,
	L2CAP_DISCONNECT_REQ,
	L2CAP_DISCONNECT_REQ_END,

	L2CAP_CON_REQ_TO,
	L2CAP_CON_RESP_FROM,
	L2CAP_CON_RESP1_FROM,
	L2CAP_CONFIG_REQ_FROM,
	L2CAP_CONFIG_RESP_TO,
	L2CAP_CONFIG_REQ_HOST_TO,
    L2CAP_CONFIG_RESP_HOST_FROM,

	SDP_SEARCH_REQ,
	SDP_SERCH_RESP,
	SDP_SERCH_RESP_MICRO,
	SDP_ATTR_REQ,
    SDP_ATTR_REQ_READ_END,
	SDP_ATTR_RESP2a,
	SDP_ATTR_RESP2a_READ,
    SDP_ATTR_RESP2a_READ_END,
	SDP_ATTR_RESP2b,
	SDP_ATTR_RESP2b_READ,
    SDP_ATTR_RESP2b_READ_END,
	SDP_ATTR_RESP2c,
    SDP_ATTR_RESP2c_READ,
    SDP_ATTR_RESP2d_READ_END,
	SDP_ATTR_RESP3,
    SDP_ATTR_RESP3_READ,
	SDP_ATTR_RESP3_READ_END,
	SDP_ATTR_RESP4a,
	SDP_ATTR_RESP4a_READ,
    SDP_ATTR_RESP4a_READ_END,
	SDP_ATTR_RESP4b,
	SDP_ATTR_RESP4b_READ,
    SDP_ATTR_RESP4b_READ_END,
	SDP_ATTR_RESP4c,
    SDP_ATTR_RESP4c_READ,
    SDP_ATTR_RESP4c_READ_END,
	SDP_ATTR_RESP5,
    SDP_ATTR_RESP5_READ,
    SDP_ATTR_RESP5_READ_END,
	SDP_ATTR_END,

	HID_START,
    HID_START_RESP,
    HID_HANDSHAKE_ERR,
	HID_READ_LED,
	HCI_CMD_SCAN_DISABLE,
	HCI_CMD_SCAN_DISABLE_WRITE_END,
	HID_WRITE_DATA,
	HID_WRITE_DATA1,
	HID_WRITE_DATA2,
	HID_WRITE_DATA3,
    HCI_STATE_END
} HCI_STATE;


static BYTE        sDeviceAddress;  ///< Address of the device on the USB
static BT_STATE    btState;        ///< Current state of Bluetooth dongle controler.
static HCI_STATE   hciState;       ///< Current state of the demo application

#define DATA_PACKET_LENGTH  64

typedef struct {
    int size;
    char buf[DATA_PACKET_LENGTH];
} writeClassParam_t;
static writeClassParam_t writeClassParam;

typedef struct {
    int end_num;
    char *message;
    unsigned char buf[DATA_PACKET_LENGTH];
} readClassParam_t;
static readClassParam_t readClassParam;

static BOOL CheckForNewAttach ( void )
{
    // Try to get the device address, if we don't have one.
    if (sDeviceAddress == 0) {
        GENERIC_DEVICE_ID DevID;

        DevID.vid   = 0x04D8;
        DevID.pid   = 0x000C;

        if (USBHostGenericGetDeviceAddress(&DevID)) {
            sDeviceAddress = DevID.deviceAddress;
            printf( "Generic demo device attached - polled, deviceAddress= %d\r\n", sDeviceAddress );
            return TRUE;
        }
    }

    return FALSE;

}

static void WriteClass( HCI_STATE nextState, size_t length, ... )
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
    hciState = nextState;
}

static void ReadClass( HCI_STATE nextState, int endnum, char *message )
{
    readClassParam.end_num = endnum;
    readClassParam.message = message;
    btState  = BT_STATE_READ_CLASS;
    hciState = nextState;
}



static void ManageHciState( void )
{
    static HCI_STATE recentHciState = HCI_STATE_END;
    if( recentHciState != hciState ) {
        printf( "hciState Changed %d -> %d\r\n", recentHciState, hciState );
        recentHciState = hciState;
    }
    
    switch (hciState) {
        case HCI_CMD_RESET:
            WriteClass( HCI_CMD_RESET_END, 3, 0x03, 0x0C, 0x00 );
            break;
        case HCI_CMD_RESET_END:
            ReadClass( HCI_CMD_READ_BD_ADDR, 0x0e, "HCI_CMD_RESET: " );
            break;
        case HCI_CMD_READ_BD_ADDR:
            WriteClass( HCI_CMD_READ_BD_ADDR_END, 3, 0x09, 0x10, 0x00 );
            break;
        case HCI_CMD_READ_BD_ADDR_END:
            ReadClass( HCI_CMD_LOCAL_NAME, 0x0e, "HCI_CMD_BD_ADDR: " );
            break;
        case HCI_CMD_LOCAL_NAME:
            WriteClass( HCI_CMD_LOCAL_NAME_END, 7, 0x13, 0x0c, 0x04, 'k', 'e', 'y', 0x00 );
            break;
        case HCI_CMD_LOCAL_NAME_END:
            ReadClass( HCI_CMD_CLASS_DEVICE_WRITE, 0x0e, "HCI_CMD_LOCAL_NAME: " );
            break;
        case HCI_CMD_CLASS_DEVICE_WRITE:
            WriteClass( HCI_CMD_CLASS_DEVICE_WRITE_END, 6, 0x24, 0x0c, 0x03, 0x40, 0x05, 0x00 );
            break;
        case HCI_CMD_CLASS_DEVICE_WRITE_END:
            ReadClass( HCI_CMD_SIMPLE_PAIR, 0x0e, "HCI_CMD_CLASS_DEVICE_WRITE: " );
            break;
        case HCI_CMD_SIMPLE_PAIR:
            WriteClass( HCI_CMD_SIMPLE_PAIR_END, 4, 0x56, 0x0c, 0x01, 0x01 );
            break;
        case HCI_CMD_SIMPLE_PAIR_END:
            ReadClass( HCI_CMD_EVENT_MASK, 0x0e, "HCI_CMD_SIMPLE_PAIR: " );
            break;

        case HCI_CMD_SCAN_ENABLE_WRITE:
        case HCI_CMD_SCAN_ENABLE_WRITE_END:
        case HCI_CMD_WAIT_CONNECTION:
        case HCI_CMD_CONNECTION_ACCEPT:
        case HCI_CMD_CONNECTION_ACCEPT_WRITE_END:

        case HCI_CMD_CREATE_CONNECTION:
        case HCI_CMD_CONNECTION_COMP:

        case HCI_CMD_EVENT_MASK:
        case HCI_CMD_EVENT_MASK_END:
        case HCI_IOC_REPLY:
        case HCI_IOC_REPLY_END:
        case HCI_CONF_REPLY:
        case HCI_CONF_REPLY_END:
        case HCI_SAVE_LINK_KEY:

        case HCI_AUTH_REQ:
        case HCI_AUTH_REQ_END:
        case HCI_LINK_KEY_REP:
        case HCI_LINK_KEY_REP_END:
        case HCI_SET_ENCRYPT:
        case HCI_SET_ENCRYPT_END:

        case L2CAP_CON_REQ:
        case L2CAP_CON_RESP:
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
        case HCI_CMD_SCAN_DISABLE_WRITE_END:
        case HID_WRITE_DATA:
        case HID_WRITE_DATA1:
        case HID_WRITE_DATA2:
        case HID_WRITE_DATA3:
        default:
            break;
    }
}

static void ManageBluetoothState ( void )
{
    BYTE RetVal;
    static BT_STATE recentBtState = BT_STATE_END;
    if( recentBtState != btState ) {
        // printf( "btState Changed %d -> %d\r\n", recentBtState, btState );
        recentBtState = btState;
    }

    // Watch for device detaching
    if (USBHostGenericDeviceDetached(sDeviceAddress) && sDeviceAddress != 0) {
        printf( "Generic demo device detached - polled\r\n" );
        btState  = BT_INITIALIZE;
		hciState = HCI_CMD_RESET;
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
            ManageHciState();
            break;

        case BT_STATE_WRITE_CLASS:
            if (!USBHostGenericTxIsBusy(sDeviceAddress)) {
                if ( (RetVal = USBHostGenericClassRequest( sDeviceAddress,
                                                           (BYTE *)writeClassParam.buf,
                                                           writeClassParam.size )) == USB_SUCCESS ) {
                    printf( "HCI COMMAND SENT\r\n" );	
                    btState = BT_STATE_ATTACHED;
                } else {
                    printf( "Write Class Error (%02X) !\r\n", RetVal );	
                    btState = BT_STATE_ERROR;
                }
            }
            break;
            
        case BT_STATE_READ_CLASS:
            if (!USBHostGenericRx1IsBusy(sDeviceAddress)) {
                if ( (RetVal=USBHostGenericRead(sDeviceAddress, readClassParam.buf, DATA_PACKET_LENGTH)) == USB_SUCCESS ) {
                    btState = BT_STATE_READ_CLASS_WAITING;
                } else {
                    printf( "Device Read Error !\r\n" );
                    btState = BT_STATE_ERROR;
                }
            }
            break;
            
        case BT_STATE_READ_CLASS_WAITING:
            if (!USBHostGenericRx1IsBusy(sDeviceAddress)) {
                if( readClassParam.buf[0] != readClassParam.end_num) {
                    btState = BT_STATE_READ_CLASS;
                } else {
                    int i;
                    printf( readClassParam.message );
                    for( i = 0; i < readClassParam.buf[1]+2; i++ ) {
                        printf( " %02X", readClassParam.buf[i] );
                    }
                    printf( "\r\n" );
                    btState = BT_STATE_ATTACHED;
                }
            }
            break;
            
        case BT_STATE_WRITE_ACL:
            break;
            
        case BT_STATE_READ_ACL_HCI:
            break;
            
        case BT_STATE_READ_ACL_WAITING:
            break;
            
        case BT_STATE_READ_HCI_WAITING:
            break;
            
        case BT_STATE_READ_HCI:
            break;

        case BT_STATE_ERROR:
            break;

        default:
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
    //printf( "USB_ApplicationEventHandler : %d\r\n", (int)event );
    
    // Handle specific events.
    switch ( (INT)event )
    {
        case EVENT_GENERIC_ATTACH:
            if (size == sizeof(GENERIC_DEVICE_ID))
            {
                sDeviceAddress   = ((GENERIC_DEVICE_ID *)data)->deviceAddress;
                btState  = BT_STATE_ATTACHED;
                hciState = HCI_CMD_RESET;
                printf( "Bluetooth dongle attached - event, deviceAddress=%d\r\n", sDeviceAddress );
                return TRUE;
            }
            break;

        case EVENT_GENERIC_DETACH:
            sDeviceAddress   = 0;
            btState = BT_INITIALIZE;
            printf( "Bluetooth dongle detached - event\r\n" );
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
            printf( "\r\n***** USB Error - hubs are not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            printf( "\r\n***** USB Error - device is not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            printf( "\r\n***** USB Error - cannot enumerate device *****\r\n" );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            printf( "\r\n***** USB Error - client driver initialization error *****\r\n" );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            printf( "\r\n***** USB Error - out of heap memory *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            printf( "\r\n***** USB Error - unspecified *****\r\n" );
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
    hciState = HCI_CMD_RESET;
    
    
    printf( "\r\nProtoType1.\r\n");
    Uart1Flush();

    if ( USBHostInit(0) == TRUE ) {
        printf( "\r\n\r\n***** USB Custom Demo App Initialized *****\r\n\r\n" );
    } else {
        printf( "\r\n\r\nCould not initialize USB Custom Demo App - USB.  Halting.\r\n\r\n" );
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
    
    while( 1 ) {
        static int c = 0;
        static int num = 0;

        while( 1 ) {

#if 1
            c = Uart1GetCh();
            if(c != -1) {
                if (c == 'A') LATB &= ~0b1000000000000000; //LED 点灯
                if (c == 'B') LATB |= 0b1000000000000000; //LED 点灯
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
