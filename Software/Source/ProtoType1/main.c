/*
 * File:   main.c
 * Author: buchio
 *
 * Created on 2013/09/23, 5:23
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
            

void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt (void)
{
    printf( "\r\n _T1Interrupt\r\n" );
    LATBbits.LATB15=!LATBbits.LATB15;
    T1_Clear_Intr_Status_Bit;
}

int main(void) {
            
    IsResetFromDeepSleep();
    PLL_INITIALIZE();

    TRISB = 0b0111111111111111;// PORTB bit6 OUTPUT for LED

    Uart1Init();

    printf( "\r\nProtoType1.\r\n");

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

//            printf( "   Message Number %d\n", num++ );
            Uart1Flush();
        }
    }

    
    
    return 0;
}
