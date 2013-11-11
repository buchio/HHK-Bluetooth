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


#define YTS_SAMPLE

#ifdef YTS_SAMPLE

#else //YTS_SAMPLE

#include <stdio.h>
#include <stdarg.h>

#define USE_AND_OR
#include <xc.h>
#include <ports.h>

#include "../../debug.h"
#include "../bluetooth.h"
#include "../hcisequence.h"

#include "../../../Microchip/Include/DEE_Emulation_16-bit.h"

HCI_SEQUENCE hciSequence;         ///< Current sequence position of HCI connection.
unsigned char hidState = 0;       ///< HID接続状態
unsigned char srcCid[2];          ///< HID interrupt (data) after hidState == 2
unsigned char dstCid[2];          ///< 
unsigned char srcCid1[2];         ///< HID control after hidState == 3
unsigned char dstCid1[2];         ///< 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static unsigned char ConnectSwitch;       ///< 接続スイッチの状態
static unsigned char aclHandle[2];        ///< Handle for ACL 
static unsigned char soleilFlag = 0;


// data for SDP

// for Bluesoleil bluetooth stack
static unsigned char att2a[5][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x80, 0x00, 0xFF, 0xFF, 0x05, 0xFF, 0xFF, 0x00, 0x7b, 0x00, 0x76, 0x36,
      0x00, 0xfd, 0x09, 0x00, 0x00, 0x0a, 0x00, 0x01, 0x00, 0x00, 0x09, 0x00, 0x01, 0x35, 0x03 }, 
    { 0x26, 0x10, 0x1b, 0x00, 0x19, 0x11, 0x24, 0x09, 0x00, 0x04, 0x35, 0x0d, 0x35, 0x06, 0x19, 0x01,
      0x00, 0x09, 0x00, 0x11, 0x35, 0x03, 0x19, 0x00, 0x11, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19 }, 
    { 0x26, 0x10, 0x1b, 0x00, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65, 0x6e, 0x09, 0x00,
      0x6a, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x24, 0x09 }, 
    { 0x26, 0x10, 0x1b, 0x00, 0x01, 0x00, 0x09, 0x00, 0x0d, 0x35, 0x0f, 0x35, 0x0d, 0x35, 0x06, 0x19,
      0x01, 0x00, 0x09, 0x00, 0x13, 0x35, 0x03, 0x19, 0x00, 0x11, 0x09, 0x01, 0x00, 0x25, 0x03 }, 
    { 0x26, 0x10, 0x18, 0x00, 0x6b, 0x65, 0x79, 0x09, 0x02, 0x00, 0x09, 0x01, 0x00, 0x09, 0x02, 0x01,
      0x09, 0x01, 0x11, 0x09, 0x02, 0x02, 0x08, 0x04, 0x09, 0x02, 0x00, 0x76, 0xff, 0xff, 0xff }
};

static unsigned char att2b[5][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x80, 0x00, 0xFF, 0xFF, 0x05, 0xFF, 0xFF, 0x00, 0x7b, 0x00, 0x76, 0x02,
      0x03, 0x08, 0x33, 0x09, 0x02, 0x04, 0x28, 0x00, 0x09, 0x02, 0x05, 0x28, 0x01, 0x09, 0x02 },
    { 0x26, 0x10, 0x1b, 0x00, 0x06, 0x35, 0x45, 0x35, 0x43, 0x08, 0x22, 0x25, 0x3f, 0x05, 0x01, 0x09,
      0x06, 0xa1, 0x01, 0x05, 0x07, 0x19, 0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01 },
    { 0x26, 0x10, 0x1b, 0x00, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08, 0x81, 0x01, 0x95, 0x05,
      0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91 },
    { 0x26, 0x10, 0x1b, 0x00, 0x01, 0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x25, 0x65, 0x05, 0x07, 0x19,
      0x00, 0x29, 0x65, 0x81, 0x00, 0xc0, 0x09, 0x02, 0x07, 0x35, 0x08, 0x35, 0x06, 0x09, 0x04 },
    { 0x26, 0x10, 0x18, 0x00, 0x09, 0x09, 0x01, 0x00, 0x09, 0x02, 0x08, 0x28, 0x00, 0x09, 0x02, 0x09,
      0x28, 0x01, 0x09, 0x02, 0x0a, 0x28, 0x01, 0x09, 0x02, 0x02, 0x00, 0xec, 0xff, 0xff, 0xff }
};

static unsigned char att2c[5][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x1c, 0x00, 0xFF, 0xFF, 0x05, 0xFF, 0xFF, 0x00, 0x17, 0x00, 0x14, 0x0b,
      0x09, 0x01, 0x00, 0x09, 0x02, 0x0c, 0x09, 0x0c, 0x80, 0x09, 0x02, 0x0d, 0x28, 0x00, 0x09 },
    { 0x26, 0x10, 0x05, 0x00, 0x02, 0x0e, 0x28, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

static unsigned char att3[4][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x65, 0x00, 0xFF, 0xFF, 0x05, 0xFF, 0xFF, 0x00, 0x60, 0x00, 0x5d, 0x36,
      0x00, 0x5a, 0x09, 0x00, 0x00, 0x0a, 0x00, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x35, 0x03 },
    { 0x26, 0x10, 0x1b, 0x00, 0x19, 0x12, 0x00, 0x09, 0x00, 0x04, 0x35, 0x0d, 0x35, 0x06, 0x19, 0x01,
      0x00, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x00, 0x01, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19 },
    { 0x26, 0x10, 0x1b, 0x00, 0x10, 0x02, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x12, 0x00,
      0x09, 0x01, 0x00, 0x09, 0x02, 0x00, 0x09, 0x01, 0x00, 0x09, 0x02, 0x01, 0x09, 0x05, 0x7e },
    { 0x26, 0x10, 0x18, 0x00, 0x09, 0x02, 0x02, 0x09, 0x03, 0x09, 0x09, 0x02, 0x03, 0x09, 0x06, 0x00,
      0x09, 0x02, 0x04, 0x28, 0x01, 0x09, 0x02, 0x05, 0x09, 0x00, 0x02, 0x00, 0xff, 0xff, 0xff }
};

// for Microchip bluetooth stack
static unsigned char att4a[5][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x80, 0x00, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x7b, 0x00, 0x76, 0x36,
      0x01, 0x00, 0x36, 0x00, 0xfd, 0x09, 0x00, 0x00, 0x0a, 0x00, 0x01, 0x00, 0x00, 0x09, 0x00 },
    { 0x26, 0x10, 0x1b, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x24, 0x09, 0x00, 0x04, 0x35, 0x0d, 0x35,
      0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x11, 0x35, 0x03, 0x19, 0x00, 0x11, 0x09, 0x00, 0x05 },
    { 0x26, 0x10, 0x1b, 0x00, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65,
      0x6e, 0x09, 0x00, 0x6a, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19 },
    { 0x26, 0x10, 0x1b, 0x00, 0x11, 0x24, 0x09, 0x01, 0x00, 0x09, 0x00, 0x0d, 0x35, 0x0f, 0x35, 0x0d,
      0x35, 0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x13, 0x35, 0x03, 0x19, 0x00, 0x11, 0x09, 0x01 },
    { 0x26, 0x10, 0x18, 0x00, 0x00, 0x25, 0x03, 0x6b, 0x65, 0x79, 0x09, 0x02, 0x00, 0x09, 0x01, 0x00,
      0x09, 0x02, 0x01, 0x09, 0x01, 0x11, 0x09, 0x02, 0x02, 0x02, 0x00, 0x76, 0xff, 0xff, 0xff }
};

static unsigned char att4b[5][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x80, 0x00, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x7b, 0x00, 0x76, 0x08,
      0x04, 0x09, 0x02, 0x03, 0x08, 0x33, 0x09, 0x02, 0x04, 0x28, 0x00, 0x09, 0x02, 0x05, 0x28 },
    { 0x26, 0x10, 0x1b, 0x00, 0x01, 0x09, 0x02, 0x06, 0x35, 0x45, 0x35, 0x43, 0x08, 0x22, 0x25, 0x3f,
      0x05, 0x01, 0x09, 0x06, 0xa1, 0x01, 0x05, 0x07, 0x19, 0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25 },
    { 0x26, 0x10, 0x1b, 0x00, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08, 0x81,
      0x01, 0x95, 0x05, 0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02, 0x95, 0x01 },
    { 0x26, 0x10, 0x1b, 0x00, 0x75, 0x03, 0x91, 0x01, 0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x25, 0x65,
      0x05, 0x07, 0x19, 0x00, 0x29, 0x65, 0x81, 0x00, 0xc0, 0x09, 0x02, 0x07, 0x35, 0x08, 0x35 },
    { 0x26, 0x10, 0x18, 0x00, 0x06, 0x09, 0x04, 0x09, 0x09, 0x01, 0x00, 0x09, 0x02, 0x08, 0x28, 0x00,
      0x09, 0x02, 0x09, 0x28, 0x01, 0x09, 0x02, 0x0a, 0x28, 0x02, 0x00, 0xec, 0xff, 0xff, 0xff }
};

static unsigned char att4c[2][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x1f, 0x00, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x1a, 0x00, 0x17, 0x01,
      0x09, 0x02, 0x0b, 0x09, 0x01, 0x00, 0x09, 0x02, 0x0c, 0x09, 0x0c, 0x80, 0x09, 0x02, 0x0d },
    { 0x26, 0x10, 0x08, 0x00, 0x28, 0x00, 0x09, 0x02, 0x0e, 0x28, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

static unsigned char att5[4][31]= {
    { 0x26, 0x20, 0x1b, 0x00, 0x68, 0x00, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x63, 0x00, 0x60, 0x36,
      0x00, 0x5d, 0x36, 0x00, 0x5a, 0x09, 0x00, 0x00, 0x0a, 0x00, 0x01, 0x00, 0x01, 0x09, 0x00 },
    { 0x26, 0x10, 0x1b, 0x00, 0x01, 0x35, 0x03, 0x19, 0x12, 0x00, 0x09, 0x00, 0x04, 0x35, 0x0d, 0x35,
      0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x00, 0x01, 0x09, 0x00, 0x05 },
    { 0x26, 0x10, 0x1b, 0x00, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06,
      0x19, 0x12, 0x00, 0x09, 0x01, 0x00, 0x09, 0x02, 0x00, 0x09, 0x01, 0x00, 0x09, 0x02, 0x01 },
    { 0x26, 0x10, 0x1b, 0x00, 0x09, 0x05, 0x7e, 0x09, 0x02, 0x02, 0x09, 0x03, 0x09, 0x09, 0x02, 0x03,
      0x09, 0x06, 0x00, 0x09, 0x02, 0x04, 0x28, 0x01, 0x09, 0x02, 0x05, 0x09, 0x00, 0x02, 0x00 }
};


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
    recvHciEventParam.eventCode = endnum;
    btState  = BT_STATE_RECV_HCI_EVENT;
    hciSequence = nextSequence;
}

static void SendAclBuf( HCI_SEQUENCE nextSequence, size_t length, void *buf )
{
    size_t i = 0;
    for( i = 0; i < length; i++ ) {
        sendAclDataParam.buf[i] = *((unsigned char *)buf + i);
    }
    btState = BT_STATE_SEND_ACL;
    hciSequence = nextSequence;
}

static void SendAcl( HCI_SEQUENCE nextSequence, size_t length, ... )
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

static void RecvAclHci( HCI_SEQUENCE nextSequence )
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
    int i = 0; // Generic iterator.
    static HCI_SEQUENCE recentHciSequence = HCI_SEQUENCE_END;
    if( recentHciSequence != hciSequence ) {
        DEBUG_PRINTF( "hciSequence Changed %d -> %d\r\n", recentHciSequence, hciSequence );
        recentHciSequence = hciSequence;
    }
    
    switch (hciSequence) {
        HCI_EXEC_CMD( HCI_CMD_RESET,        HCI_CMD_READ_BD_ADDR, 3, 0x03, 0x0C, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_READ_BD_ADDR, HCI_CMD_LOCAL_NAME,   3, 0x09, 0x10, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_LOCAL_NAME,   HCI_CMD_CLASS_DEVICE, 7, 0x13, 0x0c, 0x04,
                      'H', 'H', 'K', 0x00 );
        HCI_EXEC_CMD( HCI_CMD_CLASS_DEVICE, HCI_CMD_SIMPLE_PAIR,  6, 0x24, 0x0c, 0x03,
                      0x40, 0x05, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_SIMPLE_PAIR,  HCI_CMD_EVENT_MASK,   4, 0x56, 0x0c, 0x01, 0x01 );
        HCI_EXEC_CMD( HCI_CMD_EVENT_MASK,   HCI_CMD_SCAN_ENABLE,  11, 0x01, 0x0c, 0x08,
                      0xff, 0xff, 0xff,
                      0xff, 0xff, 0x1f,
                      0xff, 0x00 );
        HCI_EXEC_CMD( HCI_CMD_SCAN_ENABLE,  HCI_CMD_CREATE_CONNECTION, 4, 0x1a, 0x0c, 0x01,
                      0x03 );
        
        case HCI_CMD_WAIT_CONNECTION:
            RecvHciEvent( HCI_CMD_CONNECTION_ACCEPT, 0x04 );
            break;

        case HCI_CMD_CONNECTION_ACCEPT:
            DataEEWrite( recvHciEventParam.buf[2], 1 );
            DataEEWrite( recvHciEventParam.buf[3], 2 );
            DataEEWrite( recvHciEventParam.buf[4], 3 );
            DataEEWrite( recvHciEventParam.buf[5], 4 );
            DataEEWrite( recvHciEventParam.buf[6], 5 );
            DataEEWrite( recvHciEventParam.buf[7], 6 );
            SendHciCommand( HCI_CMD_CONNECTION_ACCEPT_END, 10,
                            0x09, 0x04, 0x07,
                            DataEERead(1),DataEERead(2),DataEERead(3),
                            DataEERead(4),DataEERead(5),DataEERead(6),
                            0x01 );
            break;
                          
        case HCI_CMD_CONNECTION_ACCEPT_END:
            RecvHciEvent( L2CAP_CON_REQ, 0x03 );
            break;
            
        case HCI_CMD_CREATE_CONNECTION:
            if( ConnectSwitch != 1 ) { //GOTO FIRST CONNECTION
                hciSequence = HCI_CMD_WAIT_CONNECTION;
                break;
            }
            SendHciCommand( HCI_CMD_CONNECTION_END, 16,
                            0x05, 0x04, 0x0d,
                            DataEERead(1),DataEERead(2),DataEERead(3),
                            DataEERead(4),DataEERead(5),DataEERead(6),
                            0x10, 0x00, 0x00, 0x00, 0xc2, 0xb0, 0x00 );
            break;
        case HCI_CMD_CONNECTION_END:
            RecvHciEvent( HCI_AUTH_REQ, 0x03 );
            break;
            
        case HCI_AUTH_REQ:
            aclHandle[0] = recvHciEventParam.buf[3];
            aclHandle[1] = recvHciEventParam.buf[4] + 0x20;
            SendHciCommand( HCI_AUTH_REQ_END, 5, 0x11, 0x04, 0x02,
                            recvHciEventParam.buf[3], recvHciEventParam.buf[4] );
            break;
            
        case HCI_AUTH_REQ_END:
            RecvHciEvent( HCI_LINK_KEY_REP, 0x17 );
            break;
            
        HCI_EXEC_CMD( HCI_LINK_KEY_REP, HCI_SET_ENCRYPT, 25, 0x0b, 0x04, 0x16,
                      DataEERead(1),DataEERead(2),DataEERead(3),
                      DataEERead(4),DataEERead(5),DataEERead(6),
                      DataEERead(7),DataEERead(8),DataEERead(9),
                      DataEERead(10),DataEERead(11),DataEERead(12),
                      DataEERead(13),DataEERead(14),DataEERead(15),
                      DataEERead(16),DataEERead(17),DataEERead(18),
                      DataEERead(19),DataEERead(20),DataEERead(21),
                      DataEERead(22) );
            
        case HCI_SET_ENCRYPT:
            SendHciCommand( HCI_SET_ENCRYPT_END, 6, 0x13, 0x04, 0x03,
                            aclHandle[0], aclHandle[1] - 0x20, 0x01 );
            break;

        case HCI_SET_ENCRYPT_END:
            RecvHciEvent( L2CAP_CON_REQ_TO, 0x08 );
            break;

        case L2CAP_CON_REQ:
            aclHandle[0] = recvHciEventParam.buf[3];
            aclHandle[1] = recvHciEventParam.buf[4] + 0x20;
            RecvAclHci( L2CAP_CON_RESP );
            break;
            
        case L2CAP_CON_RESP:
            if( recvAclBuf[12] == 0x11 ) {
                hidState = 2;
            }

            dstCid[0] = recvAclBuf[14] + 4;
            dstCid[1] = recvAclBuf[15];
            srcCid[0] = recvAclBuf[14];
            srcCid[1] = recvAclBuf[15];

            if( hidState == 2 ) {
                dstCid1[0] = recvAclBuf[14] + 4;
                dstCid1[1] = recvAclBuf[15];
                srcCid1[0] = recvAclBuf[14];
                srcCid1[1] = recvAclBuf[15];
            }

            SendAcl( L2CAP_CONFIG_REQ, 20,
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
            RecvAclHci( L2CAP_CONFIG_RESP );
            break;

        case L2CAP_CONFIG_RESP:
            SendAcl( L2CAP_CONFIG_REQ_HOST, 22,
                     recvAclBuf[0],      // 0
                     recvAclBuf[1],      // 1
                     0x12,               // 2
                     recvAclBuf[3],      // 3
                     0x0e,               // 4
                     recvAclBuf[5],      // 5
                     recvAclBuf[6],      // 6
                     recvAclBuf[7],      // 7
                     0x05,               // 8
                     recvAclBuf[9],      // 9
                     0x0a,               // 10
                     recvAclBuf[11],     // 11
                     srcCid[0],          // 12
                     srcCid[1],          // 13
                     recvAclBuf[14],     // 14
                     recvAclBuf[15],     // 15
                     0x00,               // 16
                     0x00,               // 17
                     0x01,               // 18
                     0x02,               // 19
                     0x00,               // 20
                     0x10 );             // 21
            break;

        case L2CAP_CONFIG_REQ_HOST:
            SendAcl( L2CAP_CONFIG_RESP_HOST, 20,
                     recvAclBuf[0],      // 0
                     recvAclBuf[1],      // 1
                     0x10,               // 2
                     recvAclBuf[3],      // 3
                     0x0c,               // 4
                     recvAclBuf[5],      // 5
                     recvAclBuf[6],      // 6
                     recvAclBuf[7],      // 7
                     0x04,               // 8
                     recvAclBuf[9],      // 9
                     0x08,               // 10
                     recvAclBuf[11],     // 11
                     srcCid[0],          // 12
                     srcCid[1],          // 13
                     0x00,               // 14
                     0x00,               // 15
                     0x01,               // 16
                     0x02,               // 17
                     0x00,               // 18
                     0x10 );             // 19
            
        case L2CAP_CONFIG_RESP_HOST:
            RecvAclHci( L2CAP_CONFIG_REQ_HOST_READ_END );
            break;
            
        case L2CAP_CONFIG_REQ_HOST_READ_END:
            if( hidState==3 && soleilFlag==0 ) {
                // connection complete for Microsoft
                hciSequence = HID_START;
            } else if(hidState==3 && soleilFlag==1) {
                // connection complete for Bluesoleil
                hciSequence = HID_WRITE_DATA;
            } else if(hidState==2) {
                //GO TO final stage for L2CAP connection
                hciSequence = L2CAP_CON_REQ;
                hidState=3;
            } else {
                // GO TO first or second stage for L2CAP connection
                hciSequence = SDP_SEARCH_REQ;
            }
            break;

        case L2CAP_CON_REQ_TO:
            SendAcl( L2CAP_CON_RESP_FROM, 16,
                     aclHandle[0],                      // 0
                     aclHandle[1],                      // 1
                     0x0c,                              // 2
                     0x00,                              // 3
                     0x08,                              // 4
                     0x00,                              // 5
                     0x01,                              // 6
                     0x00,                              // 7
                     0x02,                              // 8
                     (hidState == 3 ? 0x04 : 0x01 ),    // 9
                     0x04,                              // 10
                     0x00,                              // 11
                     (hidState == 3 ? 0x13 : 0x11 ),    // 12
                     0x00,                              // 13
                     (hidState == 3 ? 0x41 : 0x40 ),    // 14
                     0x00 );                            // 15
            break;

        case L2CAP_CON_RESP_FROM:
            RecvAclHci( L2CAP_CON_RESP1_FROM );
            break;
            
        case L2CAP_CON_RESP1_FROM:
            // sometimes received extra packet 2A 10 07 00 03 00 41 00 A2 00 00
            if( recvAclBuf[6] != 0x01 ) {
                hciSequence = L2CAP_CON_RESP1_FROM;
                break;
            }
            RecvAclHci( L2CAP_CONFIG_REQ_FROM );
            break;

        case L2CAP_CONFIG_REQ_FROM:
            srcCid[0] = recvAclBuf[14];
            srcCid[1] = recvAclBuf[15];
            dstCid[0] = recvAclBuf[12];
            dstCid[1] = recvAclBuf[13];
            RecvAclHci( L2CAP_CONFIG_RESP_TO );
            break;
            
        case L2CAP_CONFIG_RESP_TO:
            SendAcl( L2CAP_CONFIG_REQ_HOST_TO, 18,
                     recvAclBuf[0],      // 0
                     recvAclBuf[1],      // 1
                     0x0e,               // 2
                     recvAclBuf[3],      // 3
                     0x0a,               // 4
                     recvAclBuf[5],      // 5
                     recvAclBuf[6],      // 6
                     recvAclBuf[7],      // 7
                     0x05,               // 8
                     recvAclBuf[9],      // 9
                     0x06,               // 10
                     recvAclBuf[11],     // 11
                     dstCid[0],          // 12
                     dstCid[1],          // 13
                     0x00,               // 14
                     0x00,               // 15
                     0x00,               // 16
                     0x00 );             // 17
            break;

        case L2CAP_CONFIG_REQ_HOST_TO:
            SendAcl( L2CAP_CONFIG_RESP_HOST_FROM, 16,
                     recvAclBuf[0],      // 0
                     recvAclBuf[1],      // 1
                     0x0c,               // 2
                     recvAclBuf[3],      // 3
                     0x08,               // 4
                     recvAclBuf[5],      // 5
                     recvAclBuf[6],      // 6
                     recvAclBuf[7],      // 7
                     0x04,               // 8
                     0x02,               // 9
                     0x04,               // 10
                     recvAclBuf[11],     // 11
                     dstCid[0],          // 12
                     dstCid[1],          // 13
                     0x00,               // 14
                     0x00 );             // 15
            break;

        case L2CAP_CONFIG_RESP_HOST_FROM:
            RecvAclHci( hidState == 3 ? HID_START : L2CAP_CON_REQ_TO );
            hidState = 3;
            break;

        case L2CAP_DISCONNECT_REQ:
            SendHciCommand( L2CAP_DISCONNECT_REQ_END, 6, 0x06, 0x04, 0x03,
                            aclHandle[0], aclHandle[1] - 0x20, 0x13 );
            break;

        case L2CAP_DISCONNECT_REQ_END:
            hciSequence = HCI_CMD_WAIT_CONNECTION;
            break;
            
        case HCI_IOC_REPLY:
            SendHciCommand( HCI_IOC_REPLY_END, 12, 0x2b, 0x04, 0x09,
                            DataEERead(1),DataEERead(2),DataEERead(3),
                            DataEERead(4),DataEERead(5),DataEERead(6),
                            0x03, 0x00, 0x00 );
            break;
            
        case HCI_IOC_REPLY_END:
            RecvHciEvent( HCI_CONF_REPLY, 0x33 );
            break;

        case HCI_CONF_REPLY:
            SendHciCommand( HCI_CONF_REPLY_END, 9, 0x2c, 0x04, 0x06,
                            DataEERead(1),DataEERead(2),DataEERead(3),
                            DataEERead(4),DataEERead(5),DataEERead(6) );
            break;
            
        case HCI_CONF_REPLY_END:
            RecvHciEvent( HCI_SAVE_LINK_KEY, 0x18 );
            break;
            
        case HCI_SAVE_LINK_KEY:
            for( i = 0; i < 16; i ++ ) {
                DataEEWrite( recvHciEventParam.buf[i+8], i+7 );
            }
            hciSequence = SDP_SEARCH_REQ;
            break;
            
        case SDP_SEARCH_REQ:
            RecvAclHci( SDP_SERCH_RESP );
            break;
            
        case SDP_SERCH_RESP:
            if( recvAclBuf[8] == 0x06 ) { //for Microsoft 
                hciSequence = SDP_SERCH_RESP_MICRO;
                break;
            }
            if( recvAclBuf[8] == 0x02 ) { //for SPP
                hciSequence = L2CAP_CON_RESP;
                break;
            }
            
            soleilFlag = 1; //for Bluesoleil
            if( hidState == 1 ) { //second stage of L2CAP
                SendAcl( SDP_ATTR_REQ, 22,
                         recvAclBuf[0],      // 0
                         recvAclBuf[1],      // 1
                         0x12,               // 2
                         recvAclBuf[3],      // 3
                         0x0e,               // 4
                         recvAclBuf[5],      // 5
                         srcCid[0],          // 6
                         srcCid[1],          // 7
                         0x03,               // 8
                         recvAclBuf[9],      // 9
                         recvAclBuf[10],     // 10
                         recvAclBuf[11],     // 11
                         0x09,               // 12
                         0x00,               // 13 total service record
                         0x01,               // 14
                         0x00,               // 15 current service record
                         0x01,               // 16
                         0x00,               // 17 serevice record hadle list
                         0x01,               // 18
                         0x00,               // 19
                         0x00,               // 20
                         0x00 );             // 21 continuation state
            } else {
                SendAcl( SDP_ATTR_REQ, 26,
                         recvAclBuf[0],      // 0
                         recvAclBuf[1],      // 1
                         0x12,               // 2
                         recvAclBuf[3],      // 3
                         0x0e,               // 4
                         recvAclBuf[5],      // 5
                         srcCid[0],          // 6
                         srcCid[1],          // 7
                         0x03,               // 8
                         recvAclBuf[9],      // 9
                         recvAclBuf[10],     // 10
                         recvAclBuf[11],     // 11
                         0x0d,               // 12
                         0x00,               // 13 total service record
                         0x02,               // 14
                         0x00,               // 15 current service record
                         0x02,               // 16
                         0x00,               // 17 serevice record hadle list
                         0x01,               // 18
                         0x00,               // 19
                         0x00,               // 20
                         0x00,               // 21 serevice record hadle list
                         0x01,               // 22
                         0x00,               // 23
                         0x01,               // 24
                         0x00 );             // 25 continuation state
            }
            break;

        case SDP_ATTR_REQ:
            RecvAclHci( SDP_ATTR_REQ_READ_END );
            break;

        case SDP_ATTR_REQ_READ_END:
            //Initialization for SDP attribute data
            //copy CID to SDP data
            for( i = 0; i < 4; i++ ) {
                att3[i][0] = recvAclBuf[0];
            }
            for( i = 0; i < 5; i++ ) {
                att2a[i][0] = recvAclBuf[0];
                att2b[i][0] = recvAclBuf[0];
            }
            att2c[0][0] = recvAclBuf[0];
            att2c[1][0] = recvAclBuf[0];
            
            //copy src_cid to SDP data      
            att2a[0][6] = srcCid[0];
            att2a[0][7] = srcCid[1];

            att2b[0][6] = srcCid[0];
            att2b[0][7] = srcCid[1];

            att2c[0][6] = srcCid[0];
            att2c[0][7] = srcCid[1];

            att3[0][6] = srcCid[0];
            att3[0][7] = srcCid[1];

            //copy ID to SDP data
            att2a[0][9] = recvAclBuf[9];
            att2a[0][10] = recvAclBuf[10];
            i = 0;
            hciSequence =SDP_ATTR_RESP2a;
            break;
            
        case SDP_ATTR_RESP2a:
            if( i == 5) {
                i = 0;
                hciSequence = SDP_ATTR_RESP2a_READ;
            } else {
                SendAclBuf( SDP_ATTR_RESP2a, att2a[i][2] + 4,  att2a[i] );
                i++;
            }
            break;

        case SDP_ATTR_RESP2a_READ:
            RecvAclHci( SDP_ATTR_RESP2a_READ_END );
            break;

        case SDP_ATTR_RESP2a_READ_END:
            //copy ID to SDP data
            att2b[0][9] = recvAclBuf[9];
            att2b[0][10] = recvAclBuf[10];
            i=0;
            hciSequence = SDP_ATTR_RESP2b;
            break;

        case SDP_ATTR_RESP2b:
            if( i == 5) {
                i = 0;
                hciSequence = SDP_ATTR_RESP2b_READ;
            } else {
                SendAclBuf( SDP_ATTR_RESP2b, att2b[i][2] + 4,  att2b[i] );
                i++;
            }
            break;

        case SDP_ATTR_RESP2b_READ:
            RecvAclHci( SDP_ATTR_RESP2b_READ_END );
            break;

        case SDP_ATTR_RESP2b_READ_END:
            //copy ID to SDP data
            att2c[0][9] = recvAclBuf[9];
            att2c[0][10] = recvAclBuf[10];
            i = 0;
            hciSequence = SDP_ATTR_RESP2c;
            break;

        case SDP_ATTR_RESP2c:
            if( i == 2) {
                i = 0;
                hciSequence = SDP_ATTR_RESP2c_READ;
            } else {
                SendAclBuf( SDP_ATTR_RESP2c, att2c[i][2] + 4,  att2c[i] );
                i++;
            }
            break;

        case SDP_ATTR_RESP2c_READ:
            RecvAclHci( SDP_ATTR_RESP2d_READ_END );
            break;

        case SDP_ATTR_RESP2d_READ_END:
            //copy ID to SDP data
            att3[0][9] = recvAclBuf[9];
            att3[0][10] = recvAclBuf[10];
            i = 0;
            hciSequence = SDP_ATTR_RESP3;
            break;

        case SDP_ATTR_RESP3:
            if( i == 4) {
                i = 0;
                hciSequence = SDP_ATTR_RESP3_READ;
            } else {
                SendAclBuf( SDP_ATTR_RESP3, att3[i][2] + 4,  att3[i] );
                i++;
            }
            break;

        case SDP_ATTR_RESP3_READ:
            RecvAclHci( SDP_ATTR_RESP3_READ_END );
            break;

        case SDP_ATTR_RESP3_READ_END:
            //copy ID to SDP data
            att2c[0][9] = recvAclBuf[9];
            att2c[0][10] = recvAclBuf[10];
            i = 0;
            hciSequence = SDP_ATTR_END;
            break;

        case SDP_SERCH_RESP_MICRO:
            //Initialization for SDP attribute data
            //copy CID to SDP data
            for(i = 0; i < 5; i++) {
                att4a[i][0]=recvAclBuf[0];
                att4b[i][0]=recvAclBuf[0];
            }
            att4c[0][0]=recvAclBuf[0];
            att4c[1][0]=recvAclBuf[0];

            for(i = 0; i < 4; i++) {
                att5[i][0]=recvAclBuf[0];
            }
            //copy srcCid to SDP data       
            att4a[0][6]=srcCid[0];
            att4a[0][7]=srcCid[1];
            att4b[0][6]=srcCid[0];
            att4b[0][7]=srcCid[1];
            att4c[0][6]=srcCid[0];
            att4c[0][7]=srcCid[1];
            att5[0][6]=srcCid[0];
            att5[0][7]=srcCid[1];
            i=0;
            //copy ID to SDP data
            att4a[0][9]=recvAclBuf[9];
            att4a[0][10]=recvAclBuf[10];
            hciSequence =SDP_ATTR_RESP4a;
            break;

        case SDP_ATTR_RESP4a:
            break;

        case SDP_ATTR_RESP4a_READ:
            break;

        case SDP_ATTR_RESP4a_READ_END:
            break;

        case SDP_ATTR_RESP4b:
            break;

        case SDP_ATTR_RESP4b_READ:
            break;

        case SDP_ATTR_RESP4b_READ_END:
            break;

        case SDP_ATTR_RESP4c:
            break;

        case SDP_ATTR_RESP4c_READ:
            break;

        case SDP_ATTR_RESP4c_READ_END:
            break;

        case SDP_ATTR_RESP5:
            break;

        case SDP_ATTR_RESP5_READ:
            break;

        case SDP_ATTR_RESP5_READ_END:
            break;

        case SDP_ATTR_END:
            break;

            
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

#endif //YTS_SAMPLE
