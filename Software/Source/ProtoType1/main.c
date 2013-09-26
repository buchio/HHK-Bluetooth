/*
 * File:   main.c
 * Author: buchio
 *
 * Created on 2013/09/23, 5:23
 */


#include <xc.h>
#include <PIC24F_plib.h>

#include "../modules.h"
#include "../Microchip/Include/uart2.h"

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL &
         PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

void _ISR _IC1Interrupt( void ) {
}

int main(void) {
    IsResetFromDeepSleep();
    PLL_INITIALIZE();

    // Configure U2RX - put on pin 17 (RP8)
    RPINR19bits.U2RXR = 8;
    // Configure U2TX - put on pin 16 (RP7)
    RPOR3bits.RP7R = 5;
    UART2Init();
 
    while (1) {
        UART2PrintString("\r\n\r\nTest String.\r\n\r\n");
    }

    
    
    return 0;
}
