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

#define USE_AND_OR
#include <xc.h>
#include <ports.h>
#include <timer.h>
#include <dpslp.h>

#define DEBUG_OUTPUT
#include "../modules.h"


_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

#include "GenericTypeDefs.h"
#include "../Microchip/HardwareProfile.h"
#include "../Microchip/Include/timer.h"

#include "../Modules/Bluetooth/dongle.h"
#include "../Modules/Bluetooth/bluetooth.h"




typedef enum {
    LED_off,
    LED_on,
    LED_blink0,
    LED_blink1,
    LED_blink2,
    LED_blink3,
    LED_blink3_1,
    LED_blink3_2,
    LED_END
} ledState_t;

static ledState_t ledState = LED_blink0;
const struct ledStateCounts_t {
    const unsigned int on;
    const unsigned int off;
    const ledState_t nextState;
} ledStateCounts[] = {
    {   0, 100, LED_off    },  // LED_off
    { 100,   0, LED_on     },  // LED_on
    { 100, 100, LED_blink0 },  // LED_blink0
    {  10,  90, LED_blink1 },  // LED_blink1
    {  90,  10, LED_blink2 },  // LED_blink2
    {  10,  10, LED_blink3_1 },// LED_blink3
    {  10,  10, LED_blink3_2 },// LED_blink3_1
    {  10,  50, LED_blink3 },  // LED_blink3_2
    { 0xffff, 0xffff } // LED_END
};

static void T1_LedTask( void )
{
    static ledState_t recentLedState = LED_END;
    static volatile unsigned int ledCount = 0;

    if( recentLedState != ledState ) {
        ledCount = 0;
        recentLedState = ledState;
    }
    
    if( ledCount < ledStateCounts[ledState].on  && ledStateCounts[ledState].on != 0 ) {
        LATB &= ~0b1000000000000000; //LED 点灯
    } else if( ledStateCounts[ledState].off != 0 ) {
        LATB |=  0b1000000000000000; //LED 消灯
    }
    ledCount += 1;

    if( ledCount > ledStateCounts[ledState].on + ledStateCounts[ledState].off ) {
        ledCount = 0;
        ledState = ledStateCounts[ledState].nextState;
    }
}


/**
 * タイマ1割り込み
 *
 */
void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt (void)
{
    T1_LedTask();
    T1_Clear_Intr_Status_Bit;
}


/**
 * INT0割り込み
 * 
 * LEDStateを変更する
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void)
{
    printf( "{%d}", PORTBbits.RB7 );
    if( ledState < (LED_END-1) ) {
        ledState ++;
    } else {
        ledState = LED_off;
    }
    
	Int0_Clear_Intr_Status_Bit;
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

    BTInit();

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
    OpenTimer1(T1_ON | T1_PS_1_256, 0x0271); // 10ms
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
                if (c == 'A') ledState = LED_off;
                if (c == 'B') ledState = LED_on;
                if (c == 'C') ledState = LED_blink0;
                if (c == 'D') ledState = LED_blink1;
                if (c == 'E') ledState = LED_blink2;
                if (c == 'F') ledState = LED_blink3;
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
            BTTask();

            Uart1Flush();

#if 1
            if( Uart1SendQueueSize() == 0 && Uart1ReceiveQueueSize() == 0 ) {
                // printf( "Going into Idle.\n" );
                Uart1Flush();
                Idle();
            }
#endif

            DelayMs(1); // 1ms delay
    
        }
    }
    return 0;
}
