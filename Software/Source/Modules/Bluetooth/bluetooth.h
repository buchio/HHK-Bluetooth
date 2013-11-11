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
} recvHciEventParam_t;

extern sendParam_t sendHciCommandParam;
extern recvHciEventParam_t recvHciEventParam;
extern sendParam_t sendAclDataParam;
extern unsigned char recvAclBuf[DATA_PACKET_LENGTH];
extern unsigned char recvHciBuf[DATA_PACKET_LENGTH];
extern BT_STATE     btState;

void ManageBluetoothState ( void );

#endif //H_DONGLE_H_131023082113_
