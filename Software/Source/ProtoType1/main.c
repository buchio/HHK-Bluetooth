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

#define USE_AND_OR /* To enable AND_OR mask setting */
#include <xc.h>
#include <ports.h>
#include <timer.h>
#include <dpslp.h>

#include "../modules.h"

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL &
         PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// USB TEST
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Microchip/HardwareProfile.h"
#include "../Microchip/Include/GenericTypeDefs.h"
#include "../Microchip/Include/USB/usb.h"
#include "../Microchip/Include/USB/usb_host_generic.h"
#include "../Microchip/Include/timer.h"
#include "user.h"

//for storing a BD_address of PC (6 bytes)
#define PROG_ADDR1 0xA700
#define PROG_ADDR2 0xA702
int addrOffset;

// Application States
 typedef enum
{
    BT_INITIALIZE = 0,                // Initialize the app when a device is attached
    BT_STATE_IDLE,                    // Inactive State
    BT_STATE_PROCESS,
    BT_STATE_WRITE_CLASS,
    BT_STATE_READ_EP1,
    BT_STATE_READ_CLASS_WAITING,
    BT_STATE_WRITE_ACL,
    BT_STATE_READ_ACL_HCI,
    BT_STATE_READ_ACL_WAITING,
    BT_STATE_READ_HCI_WAITING,
    BT_STATE_READ_HCI,
    
    BT_STATE_ERROR                    // An error has occured
} BT_STATE;

// Hci States
typedef enum
{
	HCI_CMD_RESET = 0,                // Initialize the hci when a device is attached
    HCI_CMD_RESET_END,
	HCI_CMD_READ_BD_ADDR,
    HCI_CMD_READ_BD_ADDR_END,
	HCI_CMD_LOCAL_NAME,
    HCI_CMD_LOCAL_NAME_END,
	HCI_CMD_CLASS_DEVICE,
    HCI_CMD_CLASS_DEVICE_WRITE_END,
	HCI_CMD_SCAN_ENABLE,
    HCI_CMD_SCAN_ENABLE_WRITE_END,
	HCI_CMD_WAIT_CONNECTION,
	HCI_CMD_CONNECTION_ACCEPT,
    HCI_CMD_CONNECTION_ACCEPT_WRITE_END,
    
	HCI_CMD_CREATE_CONNECTION,//
    HCI_CMD_CONNECTION_COMP,//
    

	PROGRAM_MEMORY_CLEAR,
	PROGRAM_MEMORY_READ1,
	PROGRAM_MEMORY_READ2,
	PROGRAM_MEMORY_WRITE1,
	PROGRAM_MEMORY_WRITE2,

	L2CAP_CON_REQ,
	L2CAP_CON_RESP,
	L2CAP_CONFIG_REQ,
	L2CAP_CONFIG_RESP,
	L2CAP_CONFIG_REQ_HOST,
    L2CAP_CONFIG_RESP_HOST,
    L2CAP_CONFIG_REQ_HOST_READ_END,
	L2CAP_DISCONNECT_REQ,
	L2CAP_DISCONNECT_REQ_END,
    
	L2CAP_CON_REQ_TO,//
	L2CAP_CON_RESP_FROM,//
	L2CAP_CON_RESP1_FROM,//
	L2CAP_CONFIG_REQ_FROM,//
	L2CAP_CONFIG_RESP_TO,//
	L2CAP_CONFIG_REQ_HOST_TO,//
    L2CAP_CONFIG_RESP_HOST_FROM,//

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
	PROG_END
} HCI_STATE;


// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

BYTE	deviceAddress;  // Address of the device on the USB
BT_STATE  DemoState;      // Current state of the demo application
HCI_STATE  HciState;      // Current state of the demo application
WORD data_size;
int data_num;
unsigned char local_bd_addr[6];//PIC
unsigned char remote_bd_addr[6];//PC
unsigned char buf[64];
unsigned char buf1[64];
unsigned char handle[2];//a handle for ACL 
unsigned char src_cid[2];//HID interrupt (data) after hid_flag=2
unsigned char dst_cid[2];
unsigned char src_cid1[2];//HID control after hid_flag=3
unsigned char dst_cid1[2];
unsigned char hid_flag=0;
unsigned char soleil_flag=0;
unsigned char EP2busy_flag=0;
int pdata;
int step;
char message[30];
int end_num;
unsigned char ConnectSwitch,Led;

// data for keyboard
unsigned char data[4];

// data for SDP

// for Bluesoleil bluetooth stack
unsigned char att2a[5][31]=
{
    {0x26,0x20,0x1b,0x00,0x80,0x00,0xFF,0xFF,0x05,0xFF,0xFF,0x00,0x7b,0x00,0x76,0x36,
     0x00,0xfd,0x09,0x00,0x00,0x0a,0x00,0x01,0x00,0x00,0x09,0x00,0x01,0x35,0x03},
    {0x26,0x10,0x1b,0x00,0x19,0x11,0x24,0x09,0x00,0x04,0x35,0x0d,0x35,0x06,0x19,0x01,
     0x00,0x09,0x00,0x11,0x35,0x03,0x19,0x00,0x11,0x09,0x00,0x05,0x35,0x03,0x19},
    {0x26,0x10,0x1b,0x00,0x10,0x02,0x09,0x00,0x06,0x35,0x09,0x09,0x65,0x6e,0x09,0x00,
     0x6a,0x09,0x01,0x00,0x09,0x00,0x09,0x35,0x08,0x35,0x06,0x19,0x11,0x24,0x09},
    {0x26,0x10,0x1b,0x00,0x01,0x00,0x09,0x00,0x0d,0x35,0x0f,0x35,0x0d,0x35,0x06,0x19,
     0x01,0x00,0x09,0x00,0x13,0x35,0x03,0x19,0x00,0x11,0x09,0x01,0x00,0x25,0x03},
    {0x26,0x10,0x18,0x00,0x6b,0x65,0x79,0x09,0x02,0x00,0x09,0x01,0x00,0x09,0x02,0x01,
     0x09,0x01,0x11,0x09,0x02,0x02,0x08,0x04,0x09,0x02,0x00,0x76,0xff,0xff,0xff}
};
unsigned char att2b[5][31]=
{
    {0x26,0x20,0x1b,0x00,0x80,0x00,0xFF,0xFF,0x05,0xFF,0xFF,0x00,0x7b,0x00,0x76,0x02,
     0x03,0x08,0x33,0x09,0x02,0x04,0x28,0x00,0x09,0x02,0x05,0x28,0x01,0x09,0x02},
    {0x26,0x10,0x1b,0x00,0x06,0x35,0x45,0x35,0x43,0x08,0x22,0x25,0x3f,0x05,0x01,0x09,
     0x06,0xa1,0x01,0x05,0x07,0x19,0xe0,0x29,0xe7,0x15,0x00,0x25,0x01,0x75,0x01},
    {0x26,0x10,0x1b,0x00,0x95,0x08,0x81,0x02,0x95,0x01,0x75,0x08,0x81,0x01,0x95,0x05,
     0x75,0x01,0x05,0x08,0x19,0x01,0x29,0x05,0x91,0x02,0x95,0x01,0x75,0x03,0x91},
    {0x26,0x10,0x1b,0x00,0x01,0x95,0x06,0x75,0x08,0x15,0x00,0x25,0x65,0x05,0x07,0x19,
     0x00,0x29,0x65,0x81,0x00,0xc0,0x09,0x02,0x07,0x35,0x08,0x35,0x06,0x09,0x04},
    {0x26,0x10,0x18,0x00,0x09,0x09,0x01,0x00,0x09,0x02,0x08,0x28,0x00,0x09,0x02,0x09,
     0x28,0x01,0x09,0x02,0x0a,0x28,0x01,0x09,0x02,0x02,0x00,0xec,0xff,0xff,0xff}
};
unsigned char att2c[5][31]=
{
    {0x26,0x20,0x1b,0x00,0x1c,0x00,0xFF,0xFF,0x05,0xFF,0xFF,0x00,0x17,0x00,0x14,0x0b,
     0x09,0x01,0x00,0x09,0x02,0x0c,0x09,0x0c,0x80,0x09,0x02,0x0d,0x28,0x00,0x09},
    {0x26,0x10,0x05,0x00,0x02,0x0e,0x28,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
     0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
};

unsigned char att3[4][31]=
{
    {0x26,0x20,0x1b,0x00,0x65,0x00,0xFF,0xFF,0x05,0xFF,0xFF,0x00,0x60,0x00,0x5d,0x36,
     0x00,0x5a,0x09,0x00,0x00,0x0a,0x00,0x01,0x00,0x01,0x09,0x00,0x01,0x35,0x03},
    {0x26,0x10,0x1b,0x00,0x19,0x12,0x00,0x09,0x00,0x04,0x35,0x0d,0x35,0x06,0x19,0x01,
     0x00,0x09,0x00,0x01,0x35,0x03,0x19,0x00,0x01,0x09,0x00,0x05,0x35,0x03,0x19},
    {0x26,0x10,0x1b,0x00,0x10,0x02,0x09,0x00,0x09,0x35,0x08,0x35,0x06,0x19,0x12,0x00,
     0x09,0x01,0x00,0x09,0x02,0x00,0x09,0x01,0x00,0x09,0x02,0x01,0x09,0x05,0x7e},
    {0x26,0x10,0x18,0x00,0x09,0x02,0x02,0x09,0x03,0x09,0x09,0x02,0x03,0x09,0x06,0x00,
     0x09,0x02,0x04,0x28,0x01,0x09,0x02,0x05,0x09,0x00,0x02,0x00,0xff,0xff,0xff}
};

// for Microchip bluetooth stack
unsigned char att4a[5][31]=
{
    {0x26,0x20,0x1b,0x00,0x80,0x00,0xFF,0xFF,0x07,0xFF,0xFF,0x00,0x7b,0x00,0x76,0x36,
     0x01,0x00,0x36,0x00,0xfd,0x09,0x00,0x00,0x0a,0x00,0x01,0x00,0x00,0x09,0x00},
    {0x26,0x10,0x1b,0x00,0x01,0x35,0x03,0x19,0x11,0x24,0x09,0x00,0x04,0x35,0x0d,0x35,
     0x06,0x19,0x01,0x00,0x09,0x00,0x11,0x35,0x03,0x19,0x00,0x11,0x09,0x00,0x05},
    {0x26,0x10,0x1b,0x00,0x35,0x03,0x19,0x10,0x02,0x09,0x00,0x06,0x35,0x09,0x09,0x65,
     0x6e,0x09,0x00,0x6a,0x09,0x01,0x00,0x09,0x00,0x09,0x35,0x08,0x35,0x06,0x19},
    {0x26,0x10,0x1b,0x00,0x11,0x24,0x09,0x01,0x00,0x09,0x00,0x0d,0x35,0x0f,0x35,0x0d,
     0x35,0x06,0x19,0x01,0x00,0x09,0x00,0x13,0x35,0x03,0x19,0x00,0x11,0x09,0x01},
    {0x26,0x10,0x18,0x00,0x00,0x25,0x03,0x6b,0x65,0x79,0x09,0x02,0x00,0x09,0x01,0x00,
     0x09,0x02,0x01,0x09,0x01,0x11,0x09,0x02,0x02,0x02,0x00,0x76,0xff,0xff,0xff}
};

unsigned char att4b[5][31]=
{
    {0x26,0x20,0x1b,0x00,0x80,0x00,0xFF,0xFF,0x07,0xFF,0xFF,0x00,0x7b,0x00,0x76,0x08,
     0x04,0x09,0x02,0x03,0x08,0x33,0x09,0x02,0x04,0x28,0x00,0x09,0x02,0x05,0x28},
    {0x26,0x10,0x1b,0x00,0x01,0x09,0x02,0x06,0x35,0x45,0x35,0x43,0x08,0x22,0x25,0x3f,
     0x05,0x01,0x09,0x06,0xa1,0x01,0x05,0x07,0x19,0xe0,0x29,0xe7,0x15,0x00,0x25},
    {0x26,0x10,0x1b,0x00,0x01,0x75,0x01,0x95,0x08,0x81,0x02,0x95,0x01,0x75,0x08,0x81,
     0x01,0x95,0x05,0x75,0x01,0x05,0x08,0x19,0x01,0x29,0x05,0x91,0x02,0x95,0x01},
    {0x26,0x10,0x1b,0x00,0x75,0x03,0x91,0x01,0x95,0x06,0x75,0x08,0x15,0x00,0x25,0x65,
     0x05,0x07,0x19,0x00,0x29,0x65,0x81,0x00,0xc0,0x09,0x02,0x07,0x35,0x08,0x35},
    {0x26,0x10,0x18,0x00,0x06,0x09,0x04,0x09,0x09,0x01,0x00,0x09,0x02,0x08,0x28,0x00,
     0x09,0x02,0x09,0x28,0x01,0x09,0x02,0x0a,0x28,0x02,0x00,0xec,0xff,0xff,0xff}
};

unsigned char att4c[2][31]=
{
    {0x26,0x20,0x1b,0x00,0x1f,0x00,0xFF,0xFF,0x07,0xFF,0xFF,0x00,0x1a,0x00,0x17,0x01,
     0x09,0x02,0x0b,0x09,0x01,0x00,0x09,0x02,0x0c,0x09,0x0c,0x80,0x09,0x02,0x0d},
    {0x26,0x10,0x08,0x00,0x28,0x00,0x09,0x02,0x0e,0x28,0x00,0x00,0xff,0xff,0xff,0xff,
     0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
};

unsigned char att5[4][31]=
{
    {0x26,0x20,0x1b,0x00,0x68,0x00,0xFF,0xFF,0x07,0xFF,0xFF,0x00,0x63,0x00,0x60,0x36,
     0x00,0x5d,0x36,0x00,0x5a,0x09,0x00,0x00,0x0a,0x00,0x01,0x00,0x01,0x09,0x00},
    {0x26,0x10,0x1b,0x00,0x01,0x35,0x03,0x19,0x12,0x00,0x09,0x00,0x04,0x35,0x0d,0x35,
     0x06,0x19,0x01,0x00,0x09,0x00,0x01,0x35,0x03,0x19,0x00,0x01,0x09,0x00,0x05},
    {0x26,0x10,0x1b,0x00,0x35,0x03,0x19,0x10,0x02,0x09,0x00,0x09,0x35,0x08,0x35,0x06,
     0x19,0x12,0x00,0x09,0x01,0x00,0x09,0x02,0x00,0x09,0x01,0x00,0x09,0x02,0x01},
    {0x26,0x10,0x1b,0x00,0x09,0x05,0x7e,0x09,0x02,0x02,0x09,0x03,0x09,0x09,0x02,0x03,
     0x09,0x06,0x00,0x09,0x02,0x04,0x28,0x01,0x09,0x02,0x05,0x09,0x00,0x02,0x00}
};


BOOL InitializeSystem ( void )
{
    // Set Default demo state
    DemoState = BT_INITIALIZE;
	HciState=HCI_CMD_RESET;

	ConnectSwitch=PORTBbits.RB4;
    return TRUE;
}


/*************************************************************************
 * Function:        CheckForNewAttach
 *
 * Preconditions:   None
 *
 * Input:           None
 *
 * Output:          deviceAddress (global)
 *                  Updates the device address when an attach is found.
 *
 * Returns:         TRUE if a new device has been attached.  FALSE,
 *                  otherwise.
 *
 * Side Effects:    Prints attach message
 *
 * Overview:        This routine checks to see if a new device has been
 *                  attached.  If it has, it records the address.
 *************************************************************************/

BOOL CheckForNewAttach ( void )
{
    // Try to get the device address, if we don't have one.
    if (deviceAddress == 0)
    {
        GENERIC_DEVICE_ID DevID;

        DevID.vid   = 0x04D8;
        DevID.pid   = 0x000C;
        #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
            DevID.serialNumberLength = 0;
            DevID.serialNumber = NULL;
        #endif

        if (USBHostGenericGetDeviceAddress(&DevID))
        {
            deviceAddress = DevID.deviceAddress;
            printf( "Generic demo device attached - polled, deviceAddress=%d\r\n", deviceAddress );
            return TRUE;
        }
    }

    return FALSE;

} // CheckForNewAttach


/*************************************************************************
 * Function:        ManageDemoState
 *
 * Preconditions:   The DemoState global variable must be initialized to
 *                  DEMO_STATE_IDLE (0).  (This occurs on reset.)
 *
 * Input:           DemoState (global)
 *                  Actions selected based value of DemoState on function
 *                  entry.
 *
 *                  deviceAddress (global)
 *                  May use device address to access device, depending on
 *                  state.
 *
 *                  DataPacket (global)
 *                  May read data from packet buffer, depending on state.
 *
 * Output:          DemoState (global)
 *                  Updates demo state as appropriate.
 *
 *                  DataPacket (global)
 *                  May cause data in the packet buffer to be updated,
 *                  depending on state.
 *
 * Returns:         None
 *
 * Side Effects:    Depend on state transition
 *
 * Overview:        This routine maintains the state of the application,
 *                  updateing global data and taking actions as necessary
 *                  to maintain the custom demo operations.
 *************************************************************************/
void ManageDemoState ( void )
{
    BYTE RetVal;

    if (USBHostGenericDeviceDetached(deviceAddress) && deviceAddress != 0)
    {
        printf( "Generic demo device detached - polled\r\n" );
        DemoState = BT_INITIALIZE;
		HciState=HCI_CMD_RESET;
        deviceAddress   = 0;
    }

    switch (DemoState)
    {
    case BT_INITIALIZE:
        DemoState = BT_STATE_IDLE;
        break;

    /** Idle State:  Loops here until attach **/
    case BT_STATE_IDLE:
        if (CheckForNewAttach())
        {
			DemoState = BT_STATE_PROCESS;
			HciState = HCI_CMD_RESET;
        }
        break;

    case BT_STATE_PROCESS:
		switch (HciState)
		{
//HCI layer***********************************************************************
		case HCI_CMD_RESET:
			buf1[0]=0x03;
			buf1[1]=0x0c;
			buf1[2]=0;
			data_size=3;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_RESET_END;
		    break;

		case HCI_CMD_RESET_END:
			end_num=0x0e;// When buf1[0]==end_num, reading process ends. See BT_STATE_READ_EP1
			strcpy(message,"HCI_CMD_RESET: ");//message for BT_STATE_READ_EP1
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_READ_BD_ADDR;
		    break;

//********************************************************************************
		case HCI_CMD_READ_BD_ADDR:
			buf1[0]=0x09;
			buf1[1]=0x10;
			buf1[2]=0;
			data_size=3;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_READ_BD_ADDR_END;
			break;

		case HCI_CMD_READ_BD_ADDR_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_READ_BD_ADDR: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_LOCAL_NAME;
		    break;

//********************************************************************************
		case HCI_CMD_LOCAL_NAME:
//local_bd_addr[6];
			buf1[0]=0x13;
			buf1[1]=0x0c;
			buf1[2]=0x04;
			buf1[3]='k';
			buf1[4]='e';
			buf1[5]='y';
			buf1[6]=0x00;
			data_size=7;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_LOCAL_NAME_END;
			break;

		case HCI_CMD_LOCAL_NAME_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_LOCAL_NAME: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_CLASS_DEVICE;
		    break;

//********************************************************************************
		case HCI_CMD_CLASS_DEVICE:
			buf1[0]=0x24;
			buf1[1]=0x0c;
			buf1[2]=0x03;
			buf1[3]=0x04;//joystick
			buf1[4]=0x05;//joystick
			buf1[5]=0x00;

			data_size=6;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_CLASS_DEVICE_WRITE_END;
			break;

		case HCI_CMD_CLASS_DEVICE_WRITE_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_CLASS_DEVICE: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_SCAN_ENABLE;
		    break;

//********************************************************************************
		case HCI_CMD_SCAN_ENABLE:
			buf1[0]=0x1a;
			buf1[1]=0x0c;
			buf1[2]=0x01;
			buf1[3]=0x03;
			data_size=4;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_SCAN_ENABLE_WRITE_END;
			break;

		case HCI_CMD_SCAN_ENABLE_WRITE_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_SCAN_ENABLE: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = PROGRAM_MEMORY_READ1;
		    break;


//********************************************************************************
//********************************************************************************
// SECOND OR LATER CONNECTION STAGE
//********************************************************************************
//********************************************************************************

		case PROGRAM_MEMORY_READ1:
			if(ConnectSwitch!=1) {HciState = HCI_CMD_WAIT_CONNECTION;break;} //GOTO FIRST CONNECTION
			//get storde BD_address of PC
			TBLPAG=((PROG_ADDR1 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR1 & 0x00ffff);

			asm("tblrdl.b [%1], %0": "=r"(buf1[3]) : "r"(addrOffset));// Read low byte
			asm("tblrdl.b [%1], %0": "=r"(buf1[4]) : "r"(addrOffset+1));// Read middle byte
			asm("tblrdh.b [%1], %0": "=r"(buf1[5]) : "r"(addrOffset));// Read high byte
			HciState = PROGRAM_MEMORY_READ2;
			break;			

		case PROGRAM_MEMORY_READ2:
			TBLPAG=((PROG_ADDR2 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR2 & 0x00ffff);

			asm("tblrdl.b [%1], %0": "=r"(buf1[6]) : "r"(addrOffset));// Read low byte
			asm("tblrdl.b [%1], %0": "=r"(buf1[7]) : "r"(addrOffset+1));// Read middle byte
			asm("tblrdh.b [%1], %0": "=r"(buf1[8]) : "r"(addrOffset));// Read high byte
			HciState = HCI_CMD_CREATE_CONNECTION;
			break;	

		case HCI_CMD_CREATE_CONNECTION:

			buf1[0]=0x05;
			buf1[1]=0x04;
			buf1[2]=0x0d;
//			buf1[3]=0x0a;//******************************************
//			buf1[4]=0x6f;//bd_address (6 octets) of the PIC bluetooth
//			buf1[5]=0xfe;//
//			buf1[6]=0x33;//
//			buf1[7]=0x24;//
//			buf1[8]=0x00;//******************************************
			buf1[9]=0x10;
			buf1[10]=0x00;
			buf1[11]=0x00;
			buf1[12]=0x00;
			buf1[13]=0xc2;
			buf1[14]=0xb0;
			buf1[15]=0x00;
			data_size=16;


			#ifdef DEBUG_MODE
			for(data_num=0;data_num<16;data_num++)
            {printf( "%02X ", buf1[data_num]);}
			printf( "\r\n" );
			#endif
			DemoState = BT_STATE_WRITE_CLASS;

			HciState = HCI_CMD_CONNECTION_COMP;
			break;

		case HCI_CMD_CONNECTION_COMP:
			end_num=0x03;strcpy(message,"HCI_CMD_CONNECTION_COMP: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = L2CAP_CON_REQ_TO;
		    break;

//L2CAP layer*********************************************************************

		case  L2CAP_CON_REQ_TO:
			handle[0]=buf1[3];handle[1]=buf1[4]+0x20;

			buf[0]=handle[0];
			buf[1]=handle[1];
			buf[2]=0x0c;
			buf[3]=0x00;			
			buf[4]=0x08;
			buf[5]=0x00;
			buf[6]=0x01;
			buf[7]=0x00;
			buf[8]=0x02;
			if(hid_flag==3) buf[9]=4; else buf[9]=0x01;
			buf[10]=0x04;
			buf[11]=0x00;
			if(hid_flag==3) buf[12]=0x13; else buf[12]=0x11;
			buf[13]=0x00;
			if(hid_flag==3) buf[14]=0x41; else buf[14]=0x40;
			buf[15]=0x00;
			data_size=16;

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  L2CAP_CON_RESP_FROM;

			break;

//********************************************************************************
		case  L2CAP_CON_RESP_FROM:
			strcpy(message,"L2CAP_CON_RESP_FROM \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = L2CAP_CON_RESP1_FROM;
			break;

		case  L2CAP_CON_RESP1_FROM:
			// sometimes received extra packet 2A 10 07 00 03 00 41 00 A2 00 00
			if(buf[6]!=0x01){ HciState =L2CAP_CON_RESP_FROM; break;}

			strcpy(message,"L2CAP_CONFIG_REQ1 \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState =L2CAP_CONFIG_REQ_FROM;
			break;

		case  L2CAP_CONFIG_REQ_FROM:
			src_cid[0]=buf[14];
			src_cid[1]=buf[15];		
			dst_cid[0]=buf[12];
			dst_cid[1]=buf[13];

			strcpy(message,"L2CAP_CONFIG_REQ_FROM \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = L2CAP_CONFIG_RESP_TO;
			break;

		case  L2CAP_CONFIG_RESP_TO:

			buf[2]=0x0e;
			buf[4]=0x0a;
			buf[8]=0x05;
			buf[10]=0x06;
			buf[12]=dst_cid[0];//=src_cid[0];
			buf[13]=dst_cid[1];//src_cid[1];
			buf[14]=0x00;//result
			buf[15]=0x00;//
			buf[16]=0x00;//
			buf[17]=0x00;//
			data_size=18;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = L2CAP_CONFIG_REQ_HOST_TO;

			break;

//********************************************************************************
		case  L2CAP_CONFIG_REQ_HOST_TO:

			buf[2]=0x0c;
			buf[4]=0x08;
			buf[8]=0x04;
			buf[9]=0x02;
			buf[10]=0x04;
			buf[12]=dst_cid[0];
			buf[13]=dst_cid[1];
			buf[14]=0x00;//result
			buf[15]=0x00;//
			data_size=16;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = L2CAP_CONFIG_RESP_HOST_FROM;
			break;

//********************************************************************************
		case  L2CAP_CONFIG_RESP_HOST_FROM:

			strcpy(message,"L2CAP_CONFIG_RESP_HOST_FROM \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			if(hid_flag==3) HciState = HID_START;
			else {HciState = L2CAP_CON_REQ_TO;hid_flag=3;}
			break;




//********************************************************************************
//********************************************************************************
// FIRST CONNECTION STAGE
//********************************************************************************
//********************************************************************************
		case HCI_CMD_WAIT_CONNECTION:

			end_num=0x04;strcpy(message,"HCI_CMD_WAIT_CONNECTION: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = PROGRAM_MEMORY_CLEAR;
			break;

//********************************************************************************
		case PROGRAM_MEMORY_CLEAR:
			NVMCON=0x4042;

			TBLPAG=((PROG_ADDR1 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR1 & 0x00ffff);

			__builtin_tblwtl(addrOffset,0x0000);

			asm("DISI #5");
			__builtin_write_NVM();
			HciState = PROGRAM_MEMORY_WRITE1;
			break;


		case PROGRAM_MEMORY_WRITE1:
			//store BD_address of PC in program memory (memory addresses are PROG_ADDR1 and PROG_ADDR2 )
			NVMCON=0x4003;

			TBLPAG=((PROG_ADDR1 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR1 & 0x00ffff);

			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[2]));// Write low byte
			addrOffset++;
			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[3]));// Write middle byte
			addrOffset--;
			asm("tblwth.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[4]));// Write high byte
			asm("DISI #5");
			__builtin_write_NVM();

			HciState = PROGRAM_MEMORY_WRITE2;
			break;

		case PROGRAM_MEMORY_WRITE2:
			NVMCON=0x4003;

			TBLPAG=((PROG_ADDR2 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR2 & 0x00ffff);

			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[5]));// Write low byte
			addrOffset++;
			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[6]));// Write middle byte
			addrOffset--;
			asm("tblwth.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[7]));// Write high byte
			asm("DISI #5");
			__builtin_write_NVM();

			HciState = HCI_CMD_CONNECTION_ACCEPT;
			break;


		case HCI_CMD_CONNECTION_ACCEPT:

			buf1[8]=buf1[7];//copy address
			buf1[7]=buf1[6];
			buf1[6]=buf1[5];
			buf1[5]=buf1[4];
			buf1[4]=buf1[3];
			buf1[3]=buf1[2];
			buf1[0]=0x09;
			buf1[1]=0x04;
			buf1[2]=0x07;
			buf1[9]=0x01;//role
			data_size=10;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_CONNECTION_ACCEPT_WRITE_END;
			break;

		case HCI_CMD_CONNECTION_ACCEPT_WRITE_END:
			end_num=0x03;strcpy(message,"HCI_CMD_CONNECTION_ACCEPT: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = L2CAP_CON_REQ;
		    break;

//L2CAP layer*********************************************************************
		case  L2CAP_CON_REQ:
			handle[0]=buf1[3];handle[1]=buf1[4]+0x20;

			strcpy(message,"L2CAP_CON_REQ \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = L2CAP_CON_RESP;
			break;

//********************************************************************************
		case  L2CAP_CON_RESP:

			if(buf[12]==0x11) hid_flag=2;
			buf[2]=0x10;
			buf[4]=0x0c;
			buf[8]=0x03;//
			buf[10]=0x08;
			buf[12]=buf[14]+4;//dst_cid (PIC)
			buf[13]=buf[15];//
			buf[16]=0x00;//result
			buf[17]=0x00;//
			buf[18]=0x00;//statis
			buf[19]=0x00;//
			data_size=20;
			src_cid[0]=buf[14];
			src_cid[1]=buf[15];		
			dst_cid[0]=buf[12];
			dst_cid[1]=buf[13];

			if(hid_flag==2){
				src_cid1[0]=buf[14];
				src_cid1[1]=buf[15];		
				dst_cid1[0]=buf[12];
				dst_cid1[1]=buf[13];
			}			

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  L2CAP_CONFIG_REQ;

			break;

//********************************************************************************
		case  L2CAP_CONFIG_REQ:
			strcpy(message,"L2CAP_CONFIG_REQ \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = L2CAP_CONFIG_RESP;
			break;

		case  L2CAP_CONFIG_RESP:
			buf[18]=0x01;//buf[16];//option
			buf[19]=0x02;//buf[17];//
			buf[20]=0x00;//buf[18];//
			buf[21]=0x10;//buf[19];//
			buf[2]=0x12;
			buf[4]=0x0e;
			buf[8]=0x05;//
			buf[10]=0x0a;
			buf[12]=src_cid[0];
			buf[13]=src_cid[1];
			buf[16]=0x00;//result
			buf[17]=0x00;//
			data_size=22;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = L2CAP_CONFIG_REQ_HOST;
			break;

//********************************************************************************
		case  L2CAP_CONFIG_REQ_HOST:
			buf[16]=0x01;//buf[18];//option
			buf[17]=0x02;//buf[19];//
			buf[18]=0x00;//buf[20];//
			buf[19]=0x10;//buf[21];//
			buf[2]=0x10;
			buf[4]=0x0c;
			buf[8]=0x04;//
			buf[10]=0x08;
			buf[12]=src_cid[0];
			buf[13]=src_cid[1];
			buf[14]=0x00;//result
			buf[15]=0x00;//
			data_size=20;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = L2CAP_CONFIG_RESP_HOST;
			break;

//********************************************************************************
		case  L2CAP_CONFIG_RESP_HOST:
			strcpy(message,"L2CAP_CONFIG_REQ_HOST \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = L2CAP_CONFIG_REQ_HOST_READ_END;
			break;

		case L2CAP_CONFIG_REQ_HOST_READ_END:
			if(hid_flag==3 && soleil_flag==0) HciState = HID_START;    //connection complete for Microsoft
			else if(hid_flag==3 && soleil_flag==1) HciState = HID_WRITE_DATA;    //connection complete for Bluesoleil
			else if(hid_flag==2) {HciState = L2CAP_CON_REQ; hid_flag=3;} //GO TO final stage for L2CAP connection
			else HciState = SDP_SEARCH_REQ;//GO TO first or second stage for L2CAP connection
		    break;

//SDP layer **********************************************************************
		case  SDP_SEARCH_REQ:
			strcpy(message,"SDP_SEARCH_REQ \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_SERCH_RESP;
			break;

//********************************************************************************
		case  SDP_SERCH_RESP:
			if(buf[8]==0x06){HciState = SDP_SERCH_RESP_MICRO; break;} //for Microsoft 

		soleil_flag=1;//for Bluesoleil
		if(hid_flag==1) {//second stage of L2CAP
			buf[2]=0x12;
			buf[4]=0x0e;

			buf[6]=src_cid[0];
			buf[7]=src_cid[1];
			buf[8]=0x03;

			buf[12]=0x09;
			buf[13]=0x00;//total service record
			buf[14]=0x01;
			buf[15]=0x00;//current service record
			buf[16]=0x01;
			buf[17]=0x00;//serevice record hadle list
			buf[18]=0x01;
			buf[19]=0x00;
			buf[20]=0x00;
			buf[21]=0x00;//continuation state

			data_size=22;
		}

		else{

			buf[2]=0x16;
			buf[4]=0x12;
			buf[6]=src_cid[0];
			buf[7]=src_cid[1];
			buf[8]=0x03;

			buf[12]=0x0d;
			buf[13]=0x00;//total service record
			buf[14]=0x02;
			buf[15]=0x00;//current service record
			buf[16]=0x02;
			buf[17]=0x00;//serevice record hadle list
			buf[18]=0x01;
			buf[19]=0x00;
			buf[20]=0x00;
			buf[21]=0x00;//serevice record hadle list
			buf[22]=0x01;
			buf[23]=0x00;
			buf[24]=0x01;
			buf[25]=0x00;//continuation state

			data_size=26;
		}
			DemoState = BT_STATE_WRITE_ACL;
			HciState = SDP_ATTR_REQ;
			break;

//********************************************************************************
		case  SDP_ATTR_REQ:
			strcpy(message,"SDP_ATTR_REQ \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_REQ_READ_END;
			break;

		case SDP_ATTR_REQ_READ_END:
//Initialization for SDP attribute data
//copy CID to SDP data
			for(step=0;step<4;step++){
				att3[step][0]=buf[0];
			}
			for(step=0;step<5;step++){
			 	att2a[step][0]=buf[0];
				att2b[step][0]=buf[0];
			}
				att2c[0][0]=buf[0];att2c[1][0]=buf[0];

//copy src_cid to SDP data		
			att2a[0][6]=src_cid[0];	att2a[0][7]=src_cid[1];
			att2b[0][6]=src_cid[0];	att2b[0][7]=src_cid[1];
			att2c[0][6]=src_cid[0];	att2c[0][7]=src_cid[1];
			att3[0][6]=src_cid[0];	att3[0][7]=src_cid[1];
			step=0;
//copy ID to SDP data
			att2a[0][9]=buf[9];att2a[0][10]=buf[10];
			HciState =SDP_ATTR_RESP2a;
		    break;


//********************************************************************************
		case  SDP_ATTR_RESP2a:
			if(step==5) {HciState = SDP_ATTR_RESP2a_READ;step=0;break;}
			data_size=att2a[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) buf[pdata]=att2a[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;

//********************************************************************************
		case  SDP_ATTR_RESP2a_READ:
			strcpy(message,"2a_SDP_ATTR_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP2a_READ_END;
			break;

		case SDP_ATTR_RESP2a_READ_END:
			step=0;
//copy ID to SDP data
			att2b[0][9]=buf[9];att2b[0][10]=buf[10];
			HciState = SDP_ATTR_RESP2b;
		    break;

//********************************************************************************
		case  SDP_ATTR_RESP2b:
			if(step==5) {HciState = SDP_ATTR_RESP2b_READ;step=0;break;}	
			data_size=att2b[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) 	buf[pdata]=att2b[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;

//********************************************************************************
		case  SDP_ATTR_RESP2b_READ:
			strcpy(message,"2b_SDP_ATTR_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP2b_READ_END;
			break;

		case SDP_ATTR_RESP2b_READ_END:
			step=0;
//copy ID to SDP data
			att2c[0][9]=buf[9];att2c[0][10]=buf[10];
			HciState =SDP_ATTR_RESP2c;
		    break;

//********************************************************************************
		case  SDP_ATTR_RESP2c:
			if(step==2) {HciState = SDP_ATTR_RESP2c_READ;step=0;break;}	
			data_size=att2c[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) 	buf[pdata]=att2c[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;

//********************************************************************************
		case  SDP_ATTR_RESP2c_READ:
			strcpy(message,"2c_SDP_ATTR_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP2d_READ_END;
			break;

		case SDP_ATTR_RESP2d_READ_END:
			step=0;
//copy ID to SDP data
			att3[0][9]=buf[9];att3[0][10]=buf[10];
			HciState =SDP_ATTR_RESP3;
			if(hid_flag==1) HciState =SDP_ATTR_END;
		    break;

//********************************************************************************
		case  SDP_ATTR_RESP3:
			if(step==4) {HciState = SDP_ATTR_RESP3_READ;step=0;break;}	
			data_size=att3[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) 	buf[pdata]=att3[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;

//********************************************************************************
		case  SDP_ATTR_RESP3_READ:
			strcpy(message,"3_SDP_ATTR_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP3_READ_END;
			break;

		case SDP_ATTR_RESP3_READ_END:
			step=0;
//copy ID to SDP data
			att2c[0][9]=buf[9];att2c[0][10]=buf[10];
			HciState =SDP_ATTR_END;
		    break;

//********************************************************************************
//********************************************************************************
		case SDP_SERCH_RESP_MICRO:

//Initialization for SDP attribute data
//copy CID to SDP data
			for(step=0;step<5;step++){
			 	att4a[step][0]=buf[0];
				att4b[step][0]=buf[0];
			}
			att4c[0][0]=buf[0];att4c[1][0]=buf[0];

			for(step=0;step<4;step++){
			 	att5[step][0]=buf[0];
			}
//copy src_cid to SDP data		
			att4a[0][6]=src_cid[0];	att4a[0][7]=src_cid[1];
			att4b[0][6]=src_cid[0];	att4b[0][7]=src_cid[1];
			att4c[0][6]=src_cid[0];	att4c[0][7]=src_cid[1];
			att5[0][6]=src_cid[0];	att5[0][7]=src_cid[1];
			step=0;
//copy ID to SDP data
			att4a[0][9]=buf[9];att4a[0][10]=buf[10];
			HciState =SDP_ATTR_RESP4a;
		    break;

//********************************************************************************
		case  SDP_ATTR_RESP4a:
			if(step==5) {HciState = SDP_ATTR_RESP4a_READ;step=0;break;}
			data_size=att4a[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) buf[pdata]=att4a[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;

//********************************************************************************
		case  SDP_ATTR_RESP4a_READ:
			strcpy(message,"SDP_ATTR_RESP4a_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP4a_READ_END;
			break;

		case SDP_ATTR_RESP4a_READ_END:
			step=0;
//copy ID to SDP data
			att4b[0][9]=buf[9];att4b[0][10]=buf[10];
			HciState = SDP_ATTR_RESP4b;
		    break;

//********************************************************************************
		case  SDP_ATTR_RESP4b:
			if(step==5) {HciState = SDP_ATTR_RESP4b_READ;step=0;break;}
			data_size=att4b[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) 	buf[pdata]=att4b[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;

//********************************************************************************
		case  SDP_ATTR_RESP4b_READ:
			strcpy(message,"SDP_ATTR_RESP4b_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP4b_READ_END;
			break;

		case SDP_ATTR_RESP4b_READ_END:
			step=0;
//copy ID to SDP data
			att4c[0][9]=buf[9];att4c[0][10]=buf[10];
			HciState =SDP_ATTR_RESP4c;
		    break;

//********************************************************************************
		case  SDP_ATTR_RESP4c:
			if(step==2) {HciState = SDP_ATTR_RESP4c_READ;step=0;break;}	
			data_size=att4c[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) 	buf[pdata]=att4c[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;
//********************************************************************************
		case  SDP_ATTR_RESP4c_READ:
			strcpy(message,"SDP_ATTR_RESP4d_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP4c_READ_END;
			break;


		case SDP_ATTR_RESP4c_READ_END:
			step=0;
//copy ID to SDP data
			att5[0][9]=buf[9];att5[0][10]=buf[10];
			HciState =SDP_ATTR_RESP5;

		    break;
//********************************************************************************
		case  SDP_ATTR_RESP5:
			if(step==4) {HciState = SDP_ATTR_RESP5_READ;step=0;break;}	
			data_size=att5[step][2]+4;
			for(pdata=0;pdata<data_size;pdata++) 	buf[pdata]=att5[step][pdata];
			step++; 
			DemoState = BT_STATE_WRITE_ACL;
			break;
//********************************************************************************
		case  SDP_ATTR_RESP5_READ:
			strcpy(message,"SDP_ATTR_RESP5_RESP \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SDP_ATTR_RESP5_READ_END;
			break;

		case SDP_ATTR_RESP5_READ_END:
			step=0;
//copy ID to SDP data
			att5[0][9]=buf[9];att5[0][10]=buf[10];
			HciState =SDP_ATTR_END;
		    break;

//********************************************************************************
		case  SDP_ATTR_END:
			//no disconnection command, go back to SDP (VISTA & 7)
			if(buf[6]>=0x40) {HciState = SDP_SERCH_RESP_MICRO; break;}
			else if(buf[8]==0x02) {HciState = L2CAP_CON_RESP; break;}
			else{//disconnection
	            buf[8]=0x07;
    	        data_size=16;
				hid_flag++;
				DemoState = BT_STATE_WRITE_ACL;
				HciState = L2CAP_CON_REQ;
				break;
			}

//********************************************************************************
		case L2CAP_DISCONNECT_REQ:
			buf1[0]=0x06;
			buf1[1]=0x04;
			buf1[2]=0x03;
			buf1[3]=handle[0];
			buf1[4]=handle[1]-0x20;
			buf1[5]=0x13;

			DemoState = BT_STATE_WRITE_CLASS;
			HciState = L2CAP_DISCONNECT_REQ_END;
			break;

		case L2CAP_DISCONNECT_REQ_END:
			#ifdef DEBUG_MODE
            printf( "HCI_DISCONNECT !\r\n" );
			#endif

			HciState = HCI_CMD_WAIT_CONNECTION;
		break;

//HID*****************************************************************************
		case  HID_START:
			strcpy(message,"HID START \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = HID_START_RESP;
			break;

		case  HID_START_RESP:
			buf[2]=0x08;
			buf[3]=0x00;
			buf[4]=0x04;
			buf[5]=0x00;
			buf[6]=src_cid[0];//0x41;
			buf[7]=src_cid[1];//0x00;
			buf[8]=0xa1;
			buf[9]=0x30;
			buf[10]=0x00;
			buf[11]=0x00;
			data_size=12;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = HID_HANDSHAKE_ERR;
		    break;

		case  HID_HANDSHAKE_ERR:
			buf[2]=0x05;
			buf[3]=0x00;
			buf[4]=0x01;
			buf[5]=0x00;
			buf[6]=src_cid1[0];//0x40;
			buf[7]=src_cid1[1];//0x00;
			buf[8]=0x03;
			data_size=9;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = HID_READ_LED;
		    break;


		case  HID_READ_LED:
			strcpy(message,"HID READ_LED \r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState =  HCI_CMD_SCAN_DISABLE;
			break;

		case HCI_CMD_SCAN_DISABLE:
			if(buf[8]!=0xa2) {HciState = HID_READ_LED;	break;}
			Led=buf[10];	//bit0:NumLock bit1:CapsLock bit2:ScrollLock

			#ifdef DEBUG_MODE
			printf( "LED: %02X\r\n", Led );
			#endif

			buf1[0]=0x1a;
			buf1[1]=0x0c;
			buf1[2]=0x01;
			buf1[3]=0x00;
			data_size=4;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_SCAN_DISABLE_WRITE_END;
			break;

		case HCI_CMD_SCAN_DISABLE_WRITE_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_SCAN_DISABLE: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HID_WRITE_DATA;
		    break;

		case  HID_WRITE_DATA://write

			/*
				set characters (USB HID codes: see an attached file "translate.pdf")
				in buffers (from buf[9] to buf[16])
					
				set following modiferkey in buf[9]
				ModiferKeyTable
					0x01 ; L CTRL
					0x02 ; L SHFT
					0x04 ; L ALT
					0x08 ; L GUI
					0x10 ; R CTRL
					0x20 ; R SHFT
					0x40 ; R ALT
					0x80 ; R GUI
			*/

			do{// to check a key-push
			data[0]=PORTAbits.RA0;
			data[1]=PORTAbits.RA1;
			data[2]=PORTBbits.RB2;
			data[3]=PORTBbits.RB3;
			}
			while(3!=data[0]+ data[1]+ data[2]+ data[3]);

			//to notify the key-push to PC
			if(data[0]==0x00) {buf[9]=0x00;buf[11]=4;}//HID Usage ID 'a' 
			else if(data[1]==0x00) {buf[9]=0x00;buf[11]=5;}//HID Usage ID 'b'
			else if(data[2]==0x00) {buf[9]=0x02;buf[11]=4;}//HID Usage ID 'A'
			else if(data[3]==0x00) {buf[9]=0x02;buf[11]=5;}//HID Usage ID 'B'

			buf[0]=handle[0];
			buf[1]=handle[1];
			buf[2]=0x0d;
			buf[3]=0x00;
			buf[4]=0x09;
			buf[5]=0x00;
			buf[6]=src_cid[0];//0x41;
			buf[7]=src_cid[1];//0x00;
			buf[8]=0xa1;
//			buf[9]=0;
			buf[10]= 0;
//			buf[11]= 0;
			buf[12]= 0;
			buf[13]= 0;
			buf[14]= 0;
			buf[15]= 0;
			buf[16]= 0;
			data_size=17;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = HID_WRITE_DATA1;
		    break;

		case  HID_WRITE_DATA1:
			DemoState = BT_STATE_READ_HCI;
			HciState = HID_WRITE_DATA2;
		    break;

		case  HID_WRITE_DATA2://write
			//to notify the key-release to PC

			buf[0]=handle[0];
			buf[1]=handle[1];
			buf[2]=0x0d;
			buf[3]=0x00;
			buf[4]=0x09;
			buf[5]=0x00;
			buf[6]=src_cid[0];//0x41;
			buf[7]=src_cid[1];//0x00;
			buf[8]=0xa1;
			buf[9]=0;
			buf[10]= 0;
			buf[11]= 0;
			buf[12]= 0;
			buf[13]= 0;
			buf[14]= 0;
			buf[15]= 0;
			buf[16]= 0;
			data_size=17;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = HID_WRITE_DATA3;
		    break;

		case  HID_WRITE_DATA3:
            DelayMs(50); // 50ms delay
			do{//to check the key-release
			data[0]=PORTAbits.RA0;
			data[1]=PORTAbits.RA1;
			data[2]=PORTBbits.RB2;
			data[3]=PORTBbits.RB3;
			}
			while(4!=data[0]+ data[1]+ data[2]+ data[3]);

			DemoState = BT_STATE_READ_HCI;
			HciState = HID_WRITE_DATA;
		    break;

		case  PROG_END:
			break;

		}
		break;

//********************************************************************************
//　READ-WRITE FUNCTIONS
//********************************************************************************
//WRITE ENDPOINT 0
    case BT_STATE_WRITE_CLASS:
        if (!USBHostGenericTxIsBusy(deviceAddress))
		{
            if ( (RetVal=USBHostGenericClassRequest( deviceAddress, buf1, data_size )) == USB_SUCCESS )
            {
			//printf( "HCI COMMAND SENT\r\n" );	
            DemoState = BT_STATE_PROCESS;
            }
            else
            {
			printf( "Write Class Error !\r\n" );	
            }
        }
        break;


//READ ENDPOINT 1
    case BT_STATE_READ_EP1:
        if (!USBHostGenericRx1IsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericRead(deviceAddress, buf1, DATA_PACKET_LENGTH)) == USB_SUCCESS )
            {
                DemoState = BT_STATE_READ_CLASS_WAITING;
                //printf( "READ EP1\r\n" );
            }
            else
            {
                printf( "Device Read Error !\r\n" );
            }
        }
        break;

    case BT_STATE_READ_CLASS_WAITING:
        if (!USBHostGenericRx1IsBusy(deviceAddress)){

			if(buf1[0]!=end_num){ DemoState =BT_STATE_READ_EP1; break;}

			#ifdef DEBUG_MODE
			printf(message);
			for(data_num=0;data_num<buf1[1]+2;data_num++)
            {printf( "%02X ", buf1[data_num] );}
			printf( "\r\n" );
			#endif

           DemoState = BT_STATE_PROCESS;
		}
        break;


//WRITE ENDPOINT 2
    case BT_STATE_WRITE_ACL:

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<buf[2]+4;data_num++)
	        {printf( "%02X ", buf[data_num]);}
			printf( "\r\n" );
			#endif

        if (!USBHostGenericTxIsBusy(deviceAddress))
		{
            if ( (RetVal=USBHostGenericAclWrite( deviceAddress, buf, data_size )) == USB_SUCCESS )
            {
			//printf( "HCI COMMAND SENT\r\n" );	
            DemoState = BT_STATE_PROCESS;
            }
            else
            {
			printf( "Write Acl Error !\r\n" );	
            }
        }
        break;

//READ ENDPOINT 2 & 1 
    case BT_STATE_READ_ACL_HCI:
		if(EP2busy_flag==1) {DemoState = BT_STATE_READ_ACL_WAITING;break;}
        if (!USBHostGenericRx2IsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericAclRead(deviceAddress, buf, DATA_PACKET_LENGTH)) == USB_SUCCESS )
            {
                DemoState = BT_STATE_READ_ACL_WAITING;
            }
            else
            {
                printf( "Read Acl Error !\r\n" );
            }
        }
        break;

    case BT_STATE_READ_ACL_WAITING:
        if (!USBHostGenericRx2IsBusy(deviceAddress)){

			#ifdef DEBUG_MODE
			printf(message);
			for(data_num=0;data_num<buf[2]+4;data_num++)
            {printf( "%02X ", buf[data_num]);}
			printf( "\r\n" );
			#endif
			EP2busy_flag=0;			
           DemoState = BT_STATE_READ_HCI;
		}
		else if(hid_flag!=3){
			EP2busy_flag=1;
			DemoState = BT_STATE_READ_HCI;
		}
        break;

	case BT_STATE_READ_HCI:
		if(EP2busy_flag==0) buf1[0]=0xff;
        if (!USBHostGenericRx1IsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericRead(deviceAddress, buf1, DATA_PACKET_LENGTH)) == USB_SUCCESS )
            {
                DemoState = BT_STATE_READ_HCI_WAITING;
                //printf( "READ EP1\r\n" );
            }
            else
            {
                printf( "Device Read Error !\r\n" );
            }
        }
        break;

    case BT_STATE_READ_HCI_WAITING:
        if (!USBHostGenericRx1IsBusy(deviceAddress)){

			#ifdef DEBUG_MODE
			printf( "HCI: " );
			for(data_num=0;data_num<buf1[1]+2;data_num++)
            {printf( "%02X ", buf1[data_num]);}
			printf( "\r\n" );
			#endif

			if(EP2busy_flag==0){
				if(buf1[0]==0xff) {DemoState = BT_STATE_PROCESS;}
				else {DemoState = BT_STATE_READ_HCI;}
			}
			else{
				if(buf1[0]==0x05){HciState = L2CAP_DISCONNECT_REQ;DemoState = BT_STATE_PROCESS;}
				else {DemoState = BT_STATE_READ_ACL_WAITING;}
			}
		}
        break;

//********************************************************************************

    /** Error state:  Hold here until detached **/
    case BT_STATE_ERROR:                          // To Do: Flash LEDs
        break;

    default:
        DemoState = BT_INITIALIZE;
        break;
    }

    DelayMs(1); // 1ms delay

} // ManageDemoState


//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

/*************************************************************************
 * Function:        USB_ApplicationEventHandler
 *
 * Preconditions:   The USB must be initialized.
 *
 * Input:           event       Identifies the bus event that occured
 *
 *                  data        Pointer to event-specific data
 *
 *                  size        Size of the event-specific data
 *
 * Output:          deviceAddress (global)
 *                  Updates device address when an attach or detach occurs.
 *
 *                  DemoState (global)
 *                  Updates the demo state as appropriate when events occur.
 *
 * Returns:         TRUE if the event was handled, FALSE if not
 *
 * Side Effects:    Event-specific actions have been taken.
 *
 * Overview:        This routine is called by the Host layer or client
 *                  driver to notify the application of events that occur.
 *                  If the event is recognized, it is handled and the
 *                  routine returns TRUE.  Otherwise, it is ignored (or
 *                  just "sniffed" and the routine returns FALSE.
 *************************************************************************/

BOOL USB_ApplicationEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
        BYTE i;
    #endif

    // Handle specific events.
    switch (event)
    {
        case EVENT_GENERIC_ATTACH:
            if (size == sizeof(GENERIC_DEVICE_ID))
            {
                deviceAddress   = ((GENERIC_DEVICE_ID *)data)->deviceAddress;
                DemoState = BT_STATE_PROCESS; HciState=HCI_CMD_RESET;//YTS !!!!!!!!!!!!!!!!!
                printf( "Generic demo device attached - event, deviceAddress=%d\r\n", deviceAddress );
                #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
                    for (i=1; i<((GENERIC_DEVICE_ID *)data)->serialNumberLength; i++)
                    {
                        printf( "%c", ((GENERIC_DEVICE_ID *)data)->serialNumber[i] );
                    }
                    printf( "\r\n" );
                #endif
                return TRUE;
            }
            break;

        case EVENT_GENERIC_DETACH:
            deviceAddress   = 0;
            DemoState = BT_INITIALIZE;	//YTS
            printf( "Generic demo device detached - event\r\n" );
            return TRUE;

        case EVENT_GENERIC_TX_DONE:           // The main state machine will poll the driver.
        case EVENT_GENERIC_RX1_DONE:	//YTS
        case EVENT_GENERIC_RX2_DONE:	//YTS
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




////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// END USB TEST
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


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
    printf( "\r\n _T1Interrupt %u\r\n", count++ );
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

    printf( "\r\nProtoType1.\r\n");
    Uart1Flush();

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

            if( Uart1SendQueueSize() == 0 && Uart1ReceiveQueueSize() == 0 ) {
                Idle();
            }
        }
    }
    return 0;
}
