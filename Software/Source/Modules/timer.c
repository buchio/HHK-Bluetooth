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
#include <timer.h>

#include "../modules.h"

#include "../Modules/queue.h"

#define MAX_TIMER_CALLBACK (10)
    
struct timerCallback {
    void (*callback)( void );
    int expireCount;
    int timerCount;
    int isOneShot;
} timerCallbacks[ MAX_TIMER_CALLBACK ];

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Init Timer
////////////////////////////////////////////////////////////////////////////////////////////////////
void TimerInit( void )
{
    int i;
    for( i = 0; i < MAX_TIMER_CALLBACK; i++ ) {
        timerCallbacks[i].callback = NULL;
        timerCallbacks[i].expireCount = 0;
        timerCallbacks[i].timerCount = 0;
    }
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_1);
    OpenTimer1(T1_ON | T1_PS_1_64, 250); // 10ms
    IEC0bits.T1IE = 1;

}

void TimerAddCallback( void (*callback)( void ), int expireCount, int isOneShot )
{
    int i;
    for( i = 0; i < MAX_TIMER_CALLBACK; i++ ) {
        if( timerCallbacks[i].callback == NULL ) {
            timerCallbacks[i].callback   = callback;
            timerCallbacks[i].expireCount = expireCount;
            timerCallbacks[i].timerCount = 0;
            timerCallbacks[i].isOneShot  = isOneShot;
            return;
        }
    }
}

void TimerRemoveCallback( void (*callback)( void ) )
{
    int i;
    for( i = 0; i < MAX_TIMER_CALLBACK; i++ ) {
        if( timerCallbacks[i].callback == callback ) {
            timerCallbacks[i].callback   = NULL;
            timerCallbacks[i].expireCount = 0;
            timerCallbacks[i].timerCount = 0;
        }
    }
}

void _T1Interrupt (void) __attribute__ ((interrupt,no_auto_psv));


/// \ingroup Timer
///
/// タイマ1割り込み
/// 
void _T1Interrupt ( void )
{
    int i;
    for( i = 0; i < MAX_TIMER_CALLBACK; i++ ) {
        if( timerCallbacks[i].callback != NULL ) {
            timerCallbacks[i].timerCount ++;
            if( timerCallbacks[i].timerCount > timerCallbacks[i].expireCount ) {
                (timerCallbacks[i].callback)();
                timerCallbacks[i].timerCount = 0;
                if( timerCallbacks[i].isOneShot ) {
                    timerCallbacks[i].callback   = NULL;
                    timerCallbacks[i].expireCount = 0;
                }
            }
        }
    }
    T1_Clear_Intr_Status_Bit;
}

