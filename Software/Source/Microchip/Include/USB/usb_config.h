/*
********************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright ｩ 2007-2008 Microchip Technology Inc.  All rights reserved.           
                                                                                
Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital signal controller  
that is integrated into your product or third party product (pursuant to the    
sublicense terms in the accompanying license agreement).                        
                                                                                
You should refer to the license agreement accompanying this Software for        
additional information regarding your rights and obligations.                   
                                                                                
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,   
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF        
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.  
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER       
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR    
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES         
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR     
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF        
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES          
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.     
                                                                                
********************************************************************************
*/

// Created by the Microchip USBConfig Utility, Version 2.7.1.0, 12/23/2011, 12:25:38

#ifndef _usb_config_h_
#define _usb_config_h_

#include "../Compiler.h"

#define _USB_CONFIG_VERSION_MAJOR 0
#define _USB_CONFIG_VERSION_MINOR 0
#define _USB_CONFIG_VERSION_DOT   1
#define _USB_CONFIG_VERSION_BUILD 0

// Supported USB Configurations

#define USB_SUPPORT_HOST

// Hardware Configuration

#define USB_PING_PONG_MODE  USB_PING_PONG__FULL_PING_PONG

// Host Configuration

#define NUM_TPL_ENTRIES 1

#define USB_ENABLE_TRANSFER_EVENT

#define USB_MAX_GENERIC_DEVICES 1
#define USB_NUM_CONTROL_NAKS 200
#define USB_SUPPORT_INTERRUPT_TRANSFERS
#define USB_NUM_INTERRUPT_NAKS 3
#define USB_SUPPORT_BULK_TRANSFERS
#define USB_SUPPORT_ISOCHRONOUS_TRANSFERS
#define USB_NUM_BULK_NAKS 20000
#define USB_INITIAL_VBUS_CURRENT (100/2)
#define USB_INSERT_TIME (250+1)
#define USB_HOST_APP_EVENT_HANDLER USB_ApplicationEventHandler

#define USBTasks()                  \
    {                               \
        USBHostTasks();             \
    }

#define USBInitialize(x)            \
    {                               \
        USBHostInit(x);             \
    }

#endif //_usb_config_h_
