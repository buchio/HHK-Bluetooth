/*
 * File:   main.c
 * Author: buchio
 *
 * Created on 2013/09/23, 5:23
 */


#include <xc.h>
#include "../Common/uart.h"

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

int main(void) {
    unsigned int pll_startup_counter = 600;
    CLKDIVbits.PLLEN = 1;
    while( pll_startup_counter -- ) {
    }

    // U2RXをRP7に設定
    RPINR19bits.U2RXR = 7;
    // RP8にTxDを割り当てる
    RPOR4bits.RP8R = 5;

    U2MODE = 0;
    uart_initialize();



    return 0;
}
