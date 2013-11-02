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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup DEBUG Debug出力
/// @{

#ifndef H_DEBUG_H_131103071917_
#define H_DEBUG_H_131103071917_

#define DEBUG_OUTPUT
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


#endif // H_DEBUG_H_131103071917_
