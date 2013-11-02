// -*- coding:utf-8 -*-
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

#define USE_AND_OR
#include <xc.h>
#include <ports.h>

#define DEBUG_OUTPUT
#include "../modules.h"

#define MAX_INT0_CALLBACK (10)
    
struct int0Callback {
    void (*callback)( void );
} int0Callbacks[ MAX_INT0_CALLBACK ];

void Int0Init( void )
{
    int i;
    for( i = 0; i < MAX_INT0_CALLBACK; i++ ) {
        int0Callbacks[i].callback = NULL;
    }
	CNPU2=0x80;	//pin#16 pull-up CN23
    CloseINT0();
	ConfigINT0(INT_ENABLE | FALLING_EDGE_INT | INT_PRI_7);
}

void Int0AddCallback( void (*callback)( void ) )
{
    int i;
    for( i = 0; i < MAX_INT0_CALLBACK; i++ ) {
        if( int0Callbacks[i].callback == NULL ) {
            int0Callbacks[i].callback   = callback;
            return;
        }
    }
}

void Int0RemoveCallback( void (*callback)( void ) )
{
    int i;
    for( i = 0; i < MAX_INT0_CALLBACK; i++ ) {
        if( int0Callbacks[i].callback == callback ) {
            int0Callbacks[i].callback   = NULL;
        }
    }
}

void _INT0Interrupt(void) __attribute__((interrupt,no_auto_psv));

/// \ingroup Int0
///
/// INT0割り込み
///
void _INT0Interrupt(void)
{
    int i;
    for( i = 0; i < MAX_INT0_CALLBACK; i++ ) {
        if( int0Callbacks[i].callback != NULL ) {
            (int0Callbacks[i].callback)();
        }
    }
	Int0_Clear_Intr_Status_Bit;
}

