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

#include "../../debug.h"
#include "../bluetooth.h"
#include "../hcisequence.h"

#include "../../../Microchip/HardwareProfile.h"
#include "../../../Microchip/Include/USB/usb_config.h"
#include "../../../Microchip/Include/USB/usb.h"
#include "../../../Microchip/Include/USB/usb_host_generic.h"

void BTInit( void )
{
    BluetoothInit();
    // Set Default Bluetooth dongle controler state
    btState = BT_INITIALIZE;
    
    if ( USBHostInit(0) == TRUE ) {
        printf( "\r\n\r\nHHK Bluetooth Adapter Initialized.\r\n\r\n" );
    } else {
        printf( "\r\n\r\nCould not initialize HHK Bluetooth Adapter.  Halting.\r\n\r\n" );
        while (1);
    }

}

void BTTask( void )
{
    BluetoothTask();
    USBHostTasks();
    ManageBluetoothState();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////////
sendParam_t sendHciCommandParam;      ///< BT_STATE_SEND_HCI_COMMAND で指定するパラメータ
recvHciEventParam_t recvHciEventParam;   ///< BE_STATE_RECV_HCI_EVENT で指定するパラメータ定義
sendParam_t sendAclDataParam;        ///< BT_STATE_SEND_ACL で指定するパラメータ
unsigned char recvAclBuf[DATA_PACKET_LENGTH];
unsigned char recvHciBuf[DATA_PACKET_LENGTH];

BT_STATE     btState;              ///< Current state of Bluetooth dongle controler.



////////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////////
static BYTE         sDeviceAddress;       ///< Address of the device on the USB
static unsigned char ep2BusyFlag = 0;     ///< エンドポイント2のビジーフラグ


static BOOL CheckForNewAttach ( void )
{
    // Try to get the device address, if we don't have one.
    if (sDeviceAddress == 0) {
        GENERIC_DEVICE_ID DevID;

        DevID.vid   = 0x04D8;
        DevID.pid   = 0x000C;

        if (USBHostGenericGetDeviceAddress(&DevID)) {
            sDeviceAddress = DevID.deviceAddress;
            DEBUG_PRINTF( "Generic demo device attached - polled, deviceAddress= %d\r\n",
                          sDeviceAddress );
            return TRUE;
        }
    }
    return FALSE;
}

void ManageBluetoothState ( void )
{
    BYTE RetVal;
    static BT_STATE recentBtState = BT_STATE_END;
    static BT_STATE recentRecentBtState = BT_STATE_END;
    if( recentBtState != btState && recentRecentBtState != btState ) {
        DEBUG_PRINTF( "btState Changed %d -> %d -> %d\r\n", recentRecentBtState,
                      recentBtState, btState );
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

        case BT_STATE_SEND_HCI_COMMAND:
            if (!USBHostGenericTxIsBusy(sDeviceAddress)) {
                if((RetVal = USBHostGenericClassRequest( sDeviceAddress,
                                                         (BYTE *)sendHciCommandParam.buf,
                                                         sendHciCommandParam.size )) == USB_SUCCESS ) {
                    DEBUG_DUMP( "Send CMD ", sendHciCommandParam.buf, sendHciCommandParam.size );
                    btState = BT_STATE_ATTACHED;
                } else {
                    DEBUG_PRINTF( "Write Error (%02X) !\r\n", RetVal );	
                    btState = BT_STATE_ERROR;
                }
            }
            break;
            
        case BT_STATE_RECV_HCI_EVENT:
            if (!USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                if ( (RetVal = USBHostGenericRead( sDeviceAddress,
                                                   recvHciEventParam.buf,
                                                   DATA_PACKET_LENGTH)) == USB_SUCCESS ) {
                    btState = BT_STATE_RECV_HCI_EVENT_WAITING;
                } else {
                    DEBUG_PRINTF( "Device Read Error !\r\n" );
                    btState = BT_STATE_ERROR;
                }
            }
            break;
            
        case BT_STATE_RECV_HCI_EVENT_WAITING:
            if ( !USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                if( recvHciEventParam.buf[0] != recvHciEventParam.eventCode) {
                    btState = BT_STATE_RECV_HCI_EVENT;
                } else {
                    DEBUG_DUMP( "Recv EVT ", recvHciEventParam.buf, recvHciEventParam.buf[1]+2 );
                    btState = BT_STATE_ATTACHED;
                }
            }
            break;
            
        case BT_STATE_SEND_ACL:
            if ( !USBHostGenericTxIsBusy(sDeviceAddress) ) {
                if ( (RetVal = USBHostGenericAclWrite( sDeviceAddress,
                                                       sendAclDataParam.buf,
                                                       sendAclDataParam.size )) == USB_SUCCESS ) {
                    DEBUG_DUMP( "Send ACL ", sendAclDataParam.buf, sendAclDataParam.size );
                    btState = BT_STATE_ATTACHED;
                } else {
                    DEBUG_PRINTF( "Write Acl Error !\r\n" );	
                }
            }
            break;
            
        case BT_STATE_RECV_ACL_HCI:
            if( ep2BusyFlag == 1 ) {
                btState = BT_STATE_RECV_ACL_WAITING;
            } else if ( !USBHostGenericRx2IsBusy( sDeviceAddress ) ) {
                if ( ( RetVal = USBHostGenericAclRead( sDeviceAddress,
                                                       recvAclBuf,
                                                       DATA_PACKET_LENGTH)) == USB_SUCCESS ) {
                    btState = BT_STATE_RECV_ACL_WAITING;
                } else {
                    DEBUG_PRINTF( "Read Acl Error !\r\n" );
                }
            }
            break;
            
        case BT_STATE_RECV_ACL_WAITING:
            if ( !USBHostGenericRx2IsBusy( sDeviceAddress) )  {
                DEBUG_DUMP( "Recv ACL ", recvAclBuf, recvAclBuf[2] + 4 );
                ep2BusyFlag=0;			
                btState = BT_STATE_RECV_HCI;
            } else if( hidState != 3 ) {
                ep2BusyFlag = 1;
                btState = BT_STATE_RECV_HCI;
            }
            break;
            
        case BT_STATE_RECV_HCI:
            if( ep2BusyFlag == 0 ) {
                recvHciBuf[0] = 0xff;
            }
            if ( !USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                if ( (RetVal = USBHostGenericRead( sDeviceAddress,
                                                   recvHciBuf,
                                                   DATA_PACKET_LENGTH ) ) == USB_SUCCESS ) {
                    btState = BT_STATE_RECV_HCI_WAITING;
                    DEBUG_PRINTF( "READ HCI\r\n" );
                } else {
                    DEBUG_PRINTF( "Device Read Error !\r\n" );
                }
            }
            break;

        case BT_STATE_RECV_HCI_WAITING:
            if( !USBHostGenericRx1IsBusy( sDeviceAddress ) ) {
                DEBUG_DUMP( "Recv HCI ", recvHciBuf, recvHciBuf[2] + 4 );
                if( ep2BusyFlag == 0 ) {
                    if(recvHciBuf[0] == 0xff ) {
                        btState = BT_STATE_ATTACHED;
                    } else if( recvHciBuf[0] == 0x31 ) {
                        hciSequence = HCI_IOC_REPLY;
                        btState = BT_STATE_ATTACHED;
                    } else {
                        btState = BT_STATE_RECV_HCI;
                    }
                } else {
                    if( recvHciBuf[0] == 0x05 ) {
                        hciSequence = L2CAP_DISCONNECT_REQ;
                        btState = BT_STATE_ATTACHED;
                    } else if( recvHciBuf[0] == 0x31 ) {
                        hciSequence = HCI_IOC_REPLY;
                        btState = BT_STATE_ATTACHED;
                    } else {
                        btState = BT_STATE_RECV_ACL_WAITING;}
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
}


/// \ingroup Bluetooth
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


