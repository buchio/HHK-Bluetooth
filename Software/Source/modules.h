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
 * \date 2013/09/24, 13:49
 */

#ifndef COMMON_H
#define	COMMON_H

/// \defgroup Modules ソフトウェアモジュール
/// @{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup DEBUG Debug用モジュール
/// @{

#ifdef DEBUG_OUTPUT
#define DEBUG_PRINTF( ... ) printf( __VA_ARGS__ )
#define DEBUG_DUMP( xName, xBuf, xSize ) {              \
        int i;                                          \
        printf( xName );                                \
        for( i = 0; i < xSize; i++ ) {                  \
            printf( " %02X", *( (BYTE*)xBuf + i ) );    \
        }                                               \
        printf( "\r\n" );                               \
    }
#else
#define DEBUG_PRINTF( ... ) {}
#define DEBUG_DUMP( xName, xBuf, xSize ) {}
#endif

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup Timer Timer用モジュール
/// @{

void TimerInit( void );
void TimerAddCallback( void (*callback)( void ), int expireCount, int isOneShot );
void TimerRemoveCallback( void (*callback)( void ) );

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup LED LED用モジュール
/// @{


/// 
/// LED点滅状態
/// 
typedef enum {
    LED_off,       ///< 常時消灯
    LED_on,        ///< 常時点灯
    LED_blink0,    ///< ****----
    LED_blink1,    ///< *---*---
    LED_blink2,    ///< ***-***-
    LED_blink3,    ///< *-*-*---
    LED_STATE_END, ///< ユーザーが指定する状態はここまで
    LED_blink3_1,  ///< 複雑な点滅を実現するためのサブ状態
    LED_blink3_2,  ///< 複雑な点滅を実現するためのサブ状態
    LED_ENUM_END   ///< enumの最後
} ledState_t;

extern ledState_t ledState;
void LedInit( void );
void LedEvent_T1( void );
void LedEvent_INT0( void );

/// @}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup PLL PLL操作
/// @{
#define PllInit() {                             \
        unsigned int pll_startup_counter = 600; \
        CLKDIVbits.PLLEN = 1;                   \
        while( pll_startup_counter -- ) {       \
        }                                       \
    }
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup Uart UART入出力モジュールAPI
/// @{

void Uart1Init();
void Uart1Putc( const char c );
void Uart1Puts( const char *str );
void Uart1Flush( void );
int Uart1Write( char *dat, int szbyte );
int Uart1GetCh( void );
int Uart1SendQueueSize( void );
int Uart1ReceiveQueueSize( void );

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup Bluetooth Bluetooth関連モジュール
/// @{

/// Bluetooth初期化
void BTInit( void );

/// Bluetoothタスク
void BTTask( void );

/// @}

/// @}

#endif	/* COMMON_H */

