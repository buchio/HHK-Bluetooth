#include <xc.h>
#include <PIC24F_plib.h>

#include "../../Microchip/Include/struct_queue.h"
#include "../uart.h"

// キュー
#define UART_QUEUE_SIZE 128
    
typedef struct _uart_queue {
    int head;
    int tail;
    int count;
    char buffer[ UART_QUEUE_SIZE ];
} UART_QUEUE;

UART_QUEUE tx_queue;
UART_QUEUE rx_queue;

#define BAUD_RATE       38400
#define F_CPU           16000000UL  // メインクロック周波数 16MHz

/* This is UART1 transmit ISR */
void __attribute__((__interrupt__)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;
    if( StructQueueIsNotEmpty( &tx_queue, UART_QUEUE_SIZE ) ) {
        U1TXREG = *StructQueueRemove( &tx_queue, UART_QUEUE_SIZE );
    }
}

/* This is UART1 receive ISR */
void __attribute__((__interrupt__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;

    if( StructQueueIsNotFull( &rx_queue, UART_QUEUE_SIZE ) ) {
        char *c = StructQueueAdd( &rx_queue, UART_QUEUE_SIZE );
        *c = U1RXREG;
    } else {
        U1RXREG;// キューが一杯のときは読み捨て
    }
}

// UART1初期化
void Uart1Init()
{
    RPINR18bits.U1RXR = 7;// UART1 RX input = RP7
    RPOR4bits.RP8R = 3;   // UART TX output = RP8

    StructQueueInit( &rx_queue, UART_QUEUE_SIZE );
    StructQueueInit( &tx_queue, UART_QUEUE_SIZE );

    U1BRG = F_CPU/16/BAUD_RATE-1;// bps

    IPC3bits.U1TXIP2=1;// Set Uart TX Interrupt Priority
    IPC3bits.U1TXIP1=0;
    IPC3bits.U1TXIP0=0;

    U1STA=0;
    U1MODE=0x8000;// Enable Uart for 8-bit data no parity,
                  // 1 STOP bit, Flow制御なし,LOOP BACKなし,WAKEUPなし,ABAUDなし
                  // RX IDLE=1,低速ボーレート
    U1STAbits.UTXEN=1;  // Enable Transmit
    U1STAbits.UTXISEL0 = 1; // 送信バッファが空になったときに割り込み発生
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1RXIE=1;  // Enable Receive Interrupt
    IEC0bits.U1TXIE=1;  // Enable Transmit Interrupt
}

// UART1に文字を送信する
void Uart1Putc( const char c )
{
    IEC0bits.U1TXIE=0;  // Disable Transmit Interrupt
    if( StructQueueIsEmpty( &tx_queue, UART_QUEUE_SIZE ) ) {
        if( U1STAbits.UTXBF != 1 ) {
            U1TXREG = c;
        } else {
            char *p = StructQueueAdd( &tx_queue, UART_QUEUE_SIZE );
            *p = c;
        }
    } else {
        if( StructQueueIsNotFull( &tx_queue, UART_QUEUE_SIZE ) ) {
            char *p = StructQueueAdd( &tx_queue, UART_QUEUE_SIZE );
            *p = c;
        } else {
            // キューが一杯のときは無視
        }
    }
    IEC0bits.U1TXIE=1;  // Enable Transmit Interrupt
}


// UART1に文字列を送信する
void Uart1Puts( const char *str )
{
    char c = 0;
    
    // 送信バッファに文字列を格納
    while( (c = *str++) != 0 ) {
        Uart1Putc( c );
    }

}

// UART1にバイト列を送信する
int Uart1Write(char *dat, int szbyte)
{
    int count = 0;
    while( szbyte--  && StructQueueIsNotFull( &tx_queue, UART_QUEUE_SIZE ) ) {
        Uart1Putc( *(dat+count) );
        count ++;
    }
    return count;
}

// Uart1から一文字受信する
int Uart1GetCh()
{
    int c = -1;

    IEC0bits.U1RXIE=0;  // Disable Receive Interrupt
    if( StructQueueIsNotEmpty( &rx_queue, UART_QUEUE_SIZE ) ) {
        c = *StructQueueRemove( &rx_queue, UART_QUEUE_SIZE );
    }
    IEC0bits.U1RXIE=1;  // Enable Receive Interrupt

    return c;
}

// Uart1からの受信キューのサイズを確認
int Uart1QueueSize()
{
    return StructQueueCount( &rx_queue, UART_QUEUE_SIZE );
}
