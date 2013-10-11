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
 * \ingroup Uart
 * \file
 * 
 * Uart1送受信モジュール
 * 
 * 以下の要件を満すため、Microchip社提供のものを利用せず、スクラッチで記述しています。
 * \li デバッグ用の通信に特化するため、CPU負荷を小さくすることを最優先としています。例えば、キュー溢れなどに対しては単にデータを捨ててしまいます。
 * \li 送信時はできるだけ特殊なAPIを利用せず、printfやputcなどをそのまま利用できるようにしています。
 * \li 逆に、受信は完全にこのシステムに特化し、専用APIでのアクセスのみを想定しています。
 * 
 * \author Yukio Obuchi 
 * \date 2013/10/06
 */

#include <xc.h>
#include <uart.h>

#include <stdint.h>

#include "../modules.h"

#include "queue.h"

#include "../Microchip/HardwareProfile.h"

QUEUE_INIT(UART_TX, 9, unsigned char ); ///< 送信キュー サイズ：512バイト
QUEUE_INIT(UART_RX, 7, unsigned char ); ///< 受信キュー サイズ：128バイト

/// 
/// UART1送信割り込み
/// 
/// 送信キューにデータが存在し、且つ送信バッファがフルになるまでの間ルー
/// プします。ISR中のループとなりますが、送信バッファは4バイトしかないので気
/// にしていません。
/// 
void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;
    while( U1STAbits.UTXBF != 1 && !QUEUE_ISEMPTY( UART_TX ) ) {
        QUEUE_OUT( UART_TX, U1TXREG );
    }
}

/// 
/// UART1受信割り込み
/// 
/// 受信バッファから受信キューにデータをコピーします。
/// 受信キューが一杯のときは単に読み捨てます。
/// 
/// 
void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;

    if(U1STAbits.OERR) {
        U1STAbits.OERR = 0;
    }
    if( !QUEUE_ISFULL( UART_RX ) ) {
        QUEUE_IN( UART_RX, U1RXREG );
    } else {
        U1RXREG;// キューが一杯のときは読み捨て
    }
}

///
/// UART1初期化
///
/// 組込むハードウェアに合わせて変更することを想定し、パラメータは無し
/// 設定の詳細はダブルメンテナンスを避けるためここには記述していません。
/// ソースコードを参照して下さい。
///
void Uart1Init()
{
    RPINR18bits.U1RXR = 8;// UART1 RX input = RP8
    RPOR4bits.RP9R = 3;   // UART TX output = RP9

    U1BRG = (((GetSystemClock()/2)+(BRG_DIV2/2*BAUDRATE2))/BRG_DIV2/BAUDRATE2-1);

    IPC3bits.U1TXIP2=1; // Set Uart TX Interrupt Priority
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

///
/// UART1に文字を送信する
///
/// \param c 文字
///
/// 送信キューと送信バッファが共に空の場合には送信レジスタにデータをコピーします。
/// そうでない場合には送信キューに文字を格納し、実際の送信は送信割り込みで実施します。
/// 送信キューが一杯のときは単に読み捨てます。
///
void Uart1Putc( const char c )
{
    IEC0bits.U1TXIE=0;  // Disable Transmit Interrupt
    if( QUEUE_ISEMPTY( UART_TX ) ) {
        if( U1STAbits.UTXBF != 1 ) {
            U1TXREG = c;
        } else {
            QUEUE_IN( UART_TX, c );
        }
    } else {
        if( !QUEUE_ISFULL( UART_TX ) ) {
            QUEUE_IN( UART_TX, c );
        } else {
            // キューが一杯のときは無視
        }
    }
    IEC0bits.U1TXIE=1;  // Enable Transmit Interrupt
}


///
/// UART1に文字列を送信する
///
/// \param str '\0'でターミネートされた文字列
///
void Uart1Puts( const char *str )
{
    char c = 0;
    
    // 送信バッファに文字列を格納
    while( (c = *str++) != 0 ) {
        Uart1Putc( c );
    }

}

///
/// UART1にバイト列を送信する
///
/// param dat    バイト列
/// param szbyte 送信するバイト数
///
/// 送信にUart1Putcを用いているので、キューが溢れた場合にはデータが抜けます。 
///
int Uart1Write(char *dat, int szbyte)
{
    int count = 0;
    while( szbyte--  && !QUEUE_ISFULL( UART_TX ) ) {
        Uart1Putc( *(dat+count) );
        count ++;
    }
    return count;
}


#define STDIN   0
#define STDOUT  1
#define STDERR  2

/// 
/// スタンダードIOを利用するためのwrite置き換え
/// 
/// \param handle ファイルハンドル STDOUTもしくはSTDERRのみ対応
/// \param buffer 送信データを格納したバッファ
/// \param len    送信するバイト数
/// 
/// weakシンボルを用いてwriteを置き換え、printf, puts等を利用できるようにします。
/// 
int __attribute__((__weak__, __section__(".libc"))) write(int handle, void * buffer, unsigned int len)
{
    switch (handle) {
        case STDOUT:
        case STDERR:
            Uart1Write( buffer, len );
    }
    return (len);  // number of characters written
}


///
/// Uart1の送信バッファが空になるまで待つ
///
/// 割り込み禁止コンテキストでの使用はできません。
///
void Uart1Flush( void )
{
    while( !QUEUE_ISEMPTY( UART_TX ) ) {
    }
}

///
/// Uart1から一文字受信する
///
/// \return 受信文字 キューに文字が存在しなかった場合には-1
///
///
int Uart1GetCh()
{
    int c = -1;

    IEC0bits.U1RXIE=0;  // Disable Receive Interrupt

    if( !QUEUE_ISEMPTY( UART_RX ) ) {
        QUEUE_OUT( UART_RX, c );
    }
    IEC0bits.U1RXIE=1;  // Enable Receive Interrupt

    return c;
}

///
/// Uart1からの受信キューのサイズを確認
///
/// \return キューに入っているデータサイズ
///
int Uart1SendQueueSize()
{
    return QUEUE_STATUS( UART_TX );
}

///
/// Uart1からの受信キューのサイズを確認
///
/// \return キューに入っているデータサイズ
///
int Uart1ReceiveQueueSize()
{
    return QUEUE_STATUS( UART_RX );
}

