// -*- coding:utf-8 -*-
/*
 *  Copyright (c) 2013 Yukio Obuchi
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

#include <stdio.h>

#define USE_AND_OR
#include <xc.h>
#include <ports.h>
#include <timer.h>
#include <dpslp.h>

#include "GenericTypeDefs.h"
#include "../Microchip/HardwareProfile.h"
#include "../Microchip/Include/timer.h"

#include "pll.h"
#include "timer.h"
#include "int0.h"
#include "uart.h"
#include "shell.h"
#include "led.h"

#include "bluetooth.h"

void ModulesInit( void )
{
    PllInit();

    TimerInit();
    Int0Init();
    ShellInit();

    Uart1Init();
    LedInit();

    if( IsResetFromDeepSleep() ) {
        printf( "Reset From Deep Sleep\r\n" );
    }
    
    BTInit();

    LedStateChange( LED_blink0 );
}

void ModulesMainloop( void )
{
    BTTask();
    DelayMs(1); // 1ms delay
    Shell();
}


        
