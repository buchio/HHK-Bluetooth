/* 
 * File:   common.h
 * Author: 0000102095
 *
 * Created on 2013/09/24, 13:49
 */

#ifndef COMMON_H
#define	COMMON_H

#include "uart.h"

#define PLL_INITIALIZE() { \
    unsigned int pll_startup_counter = 600; \
    CLKDIVbits.PLLEN = 1;\
    while( pll_startup_counter -- ) {\
    } \
}

#endif	/* COMMON_H */

