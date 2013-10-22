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

#include "hci_defs.h"

static const char * const hciErrorCode[] = {
    "Success",                                                  // 0x00
    "Unknown HCI Command",                                      // 0x01
    "Unknown Connection Identifier",                            // 0x02
    "Hardware Failure",                                         // 0x03
    "Page Timeout",                                             // 0x04
    "Authentication Failure",                                   // 0x05
    "PIN or Key Missing",                                       // 0x06
    "Memory Capacity Exceeded",                                 // 0x07
    "Connection Timeout",                                       // 0x08
    "Connection Limit Exceeded",                                // 0x09
    "Synchronous Connection Limit To A Device Exceeded",        // 0x0A
    "ACL Connection Already Exists",                            // 0x0B
    "Command Disallowed",                                       // 0x0C
    "Connection Rejected due to Limited Resources",             // 0x0D
    "Connection Rejected Due To Security Reasons",              // 0x0E
    "Connection Rejected due to Unacceptable BD_ADDR",          // 0x0F
    "Connection Accept Timeout Exceeded",                       // 0x10
    "Unsupported Feature or Parameter Value",                   // 0x11
    "Invalid HCI Command Parameters",                           // 0x12
    "Remote User Terminated Connection",                        // 0x13
    "Remote Device Terminated Connection due to Low Resources", // 0x14
    "Remote Device Terminated Connection due to Power Off",     // 0x15
    "Connection Terminated By Local Host",                      // 0x16
    "Repeated Attempts",                                        // 0x17
    "Pairing Not Allowed",                                      // 0x18
    "Unknown LMP PDU",                                          // 0x19
    "Unsupported Remote Feature / Unsupported LMP Feature",     // 0x1A
    "SCO Offset Rejected",                                      // 0x1B
    "SCO Interval Rejected",                                    // 0x1C
    "SCO Air Mode Rejected",                                    // 0x1D
    "Invalid LMP Parameters",                                   // 0x1E
    "Unspecified Error",                                        // 0x1F
    "Unsupported LMP Parameter Value",                          // 0x20
    "Role Change Not Allowed",                                  // 0x21
    "LMP Response Timeout / LL Response Timeout",               // 0x22
    "LMP Error Transaction Collision",                          // 0x23
    "LMP PDU Not Allowed",                                      // 0x24
    "Encryption Mode Not Acceptable",                           // 0x25
    "Link Key cannot be Changed",                               // 0x26
    "Requested QoS Not Supported",                              // 0x27
    "Instant Passed",                                           // 0x28
    "Pairing With Unit Key Not Supported",                      // 0x29
    "Different Transaction Collision",                          // 0x2A
    "Reserved",                                                 // 0x2B
    "QoS Unacceptable Parameter",                               // 0x2C
    "QoS Rejected",                                             // 0x2D
    "Channel Classification Not Supported",                     // 0x2E
    "Insufficient Security",                                    // 0x2F
    "Parameter Out Of Mandatory Range",                         // 0x30
    "Reserved",                                                 // 0x31
    "Role Switch Pending",                                      // 0x32
    "Reserved",                                                 // 0x33
    "Reserved Slot Violation",                                  // 0x34
    "Role Switch Failed",                                       // 0x35
    "Extended Inquiry Response Too Large",                      // 0x36
    "Secure Simple Pairing Not Supported By Host.",             // 0x37
    "Host Busy - Pairing",                                      // 0x38
    "Connection Rejected due to No Suitable Channel Found",     // 0x39
    "Controller Busy",                                          // 0x3A
    "Unacceptable Connection Interval",                         // 0x3B
    "Directed Advertising Timeout",                             // 0x3C
    "Connection Terminated due to MIC Failure",                 // 0x3D
    "Connection Failed to be Established",                      // 0x3E
    "MAC Connection Failed"                                     // 0x3F
};

const char *hciErrorCodeToText( unsigned char code )
{
    if( code < sizeof( hciErrorCode )/ sizeof( const char * ) ) {
        return hciErrorCode[code];
    }
    return "Unknown Error";
}

#ifdef __RUN_MODULE_TEST__
#include <stdio.h>

int main( int argc, char **argv)
{
    unsigned char code;
    for( code = 0; code < 0xff; code++ ) {
        printf( "Error code 0x%02X [%s]\n", code, hciErrorCodeToText( code ) );
    }
    
    return 0;
}

#endif //__RUN_MODULE_TEST__

/*
 * Local variables:
 *  compile-command: "gcc -Wall -g  -I . -D__RUN_MODULE_TEST__ hci_defs.c -o hci_defs && ./hci_defs"
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  tab-width: 8
 * End:
 */

