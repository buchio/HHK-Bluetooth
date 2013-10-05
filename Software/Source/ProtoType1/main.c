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
 * に記述しているが、将来的には処理は全て\ref modules.h に定義されるモ
 * ジュールに記述し、このモジュールは、初期化後にメインループを起動する
 * だけの役割とする予定
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
 * で提供しますが、\ref Microchip にはMicrochip社製のファイルを含んでい
 * ます。それらのファイルについてはMicrochip社のライセンス条項に従うも
 * のとします。
 *
 * \li \ref modules.h
 * \li \ref queue.h
 *
 *
 */

#include <stdio.h>

#include <xc.h>
#include <PIC24F_plib.h>

#include "../modules.h"

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL &
         PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
            

/**
 * タイマ1割り込み
 *
 * \note 仕様理解中
 * メッセージを表示してLEDを点滅するだけ
 * 
 */
void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt (void)
{
    printf( "\r\n _T1Interrupt\r\n" );
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

    // タイマ1の初期化複数のパラメータを結合する際に「&」を用いるように
    // マクロが定義されているが、この場合、デフォルトがデバイスのデフォ
    // ルトとは異なり、0xFFFFになっているので、デフォルト値に期待せず全
    // てのビットを定義する必要がある。一般的なORルールではなく、ANDルー
    // ルになっているのは謎。PICの慣習かな？
    ConfigIntTimer1(T1_INT_ON & T1_INT_PRIOR_1);
    OpenTimer1(T1_ON &
               T1_IDLE_CON &
               T1_GATE_OFF &
               T1_PS_1_256 &
               T1_SYNC_EXT_OFF &
               T1_SOURCE_INT,
               0xf424);

    IEC0bits.T1IE = 1;
    
    while (1) {
        static int c = 0;
        static int num = 0;

        while( 1 ) {
            c = Uart1GetCh();
            if(c != -1) {
                if (c == 'A') LATB &= ~0b1000000000000000; //LED 点灯
                if (c == 'B') LATB |= 0b1000000000000000; //LED 点灯
                printf("[%c]", c);
            }

            // printf( "   Message Number %d\n", num++ );
            // 通信速度を越えて出力する場合にはFlushでキュー内の送信待
            // ちをしないととキューが溢れる。暴走はしないが、文字抜けす
            // るので表示がグチャグチャになる。
            // // Uart1Flush();

            
        }
    }
    return 0;
}
