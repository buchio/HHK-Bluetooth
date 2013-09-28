/*
 * File:   main.c
 * Author: buchio
 *
 * Created on 2013/09/23, 5:23
 */


#include <xc.h>
#include <PIC24F_plib.h>

#include "../modules.h"

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL &
         PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
            

int main(void) {
            
    IsResetFromDeepSleep();
    PLL_INITIALIZE();

    TRISB = 0b1111111110111111;// PORTB bit6 OUTPUT for LED

            
    Uart1Init();
    Uart1Puts( "Hello\r\n" );

    while (1) {
        static int c = 0;
        volatile static int len = 0;

        LATB &= ~0b0000000001000000;//LED 点灯
        while( 1 ) {
            c = Uart1GetCh(); // リングバッファから読み込む
            if(c == -1) {
                break;
            }
            Uart1Putc( '[' );
            Uart1Putc( c );
            Uart1Putc( ']' );
            
        }

        LATB |= 0b0000000001000000;//LED 消灯
    }

    
    
    return 0;
}
