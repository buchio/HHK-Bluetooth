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

/**
 * \file
 * \author Yukio Obuchi 
 * \date 2013/10/06
 *
 * エントリモジュール
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

#define USE_AND_OR
#include <xc.h>
#include <ports.h>
#include <timer.h>
#include <dpslp.h>

#define DEBUG_OUTPUT
#include "../modules.h"


_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF &
         GCP_OFF & JTAGEN_OFF);
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD &
         FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON);
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS &
         WPEND_WPENDMEM);
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF &
         DSWDTEN_OFF);

#include "GenericTypeDefs.h"
#include "../Microchip/HardwareProfile.h"
#include "../Microchip/Include/timer.h"

#include "../Modules/Bluetooth/dongle.h"
#include "../Modules/Bluetooth/bluetooth.h"

/**
 * main関数
 *
 */
int main(void)
{
    IsResetFromDeepSleep();

    PllInit();
    TimerInit();
    Int0Init();

    Uart1Init();
    LedInit();

    BTInit();

    LedStateChange( LED_blink0 );
    while( 1 ) {
        BTTask();
        DelayMs(1); // 1ms delay

        int c = Uart1GetCh();

        // ここの記述次第では起動時に暴走してしまう。
        // ネストの深さが関係している？
        if( c == 'A' )        {
            printf("LED_off\r\n");
            LedStateChange( LED_off );
        } else if( c == 'B' ) {
            printf("LED_on\r\n");
            LedStateChange( LED_on );
        } else if( c == 'C' ) {
            printf("LED_blink0\r\n");
            LedStateChange( LED_blink0 );
        } else if( c == 'D' ) {
            printf("LED_blink1\r\n");
            LedStateChange( LED_blink1 );
        } else if( c == 'E' ) {
            printf("LED_blink2\r\n");
            LedStateChange( LED_blink2 );
        } else if( c == 'F' ) {
            printf("LED_blink3\r\n");
            LedStateChange( LED_blink3 );
        } else if( c != -1 ) {
            if ( c < ' ' ) {
                printf("[0x%02X]", c);
            } else {
                printf("[%c]", c);
            }
        }

        if( Uart1SendQueueSize() == 0 && Uart1ReceiveQueueSize() == 0 ) {
            Uart1Flush();
            Idle();
        }
    }
    return 0;
}
