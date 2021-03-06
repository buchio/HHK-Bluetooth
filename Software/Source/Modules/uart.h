// -*- coding:utf-8 -*-
/*
 *  Copyright (c) 2013 Yukio Obuchi
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

#ifndef H_UART_H_131103070420_
#define H_UART_H_131103070420_

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup Uart UART入出力
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

#endif // H_UART_H_131103070420_
