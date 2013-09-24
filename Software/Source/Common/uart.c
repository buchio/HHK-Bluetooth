#include "xc.h"
#include "uart.h"

void uart_initialize() {
    // U1RXをRP7に設定
    RPINR18bits.U1RXR = 7;
    // RP8にTxDを割り当てる
    RPOR4bits.RP8R = 3;  // U1TX

    U1MODE = 0;
}

