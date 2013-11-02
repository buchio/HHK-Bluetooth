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

#include <stdio.h>

#define USE_AND_OR
#include <xc.h>
#include <ports.h>

#define DEBUG_OUTPUT
#include "../modules.h"

ledState_t ledState = LED_blink0;

const struct ledStateCounts_t {
    const unsigned int on;
    const unsigned int off;
    const ledState_t nextState;
} ledStateCounts[] = {
    {   0, 100, LED_off    },  // LED_off
    { 100,   0, LED_on     },  // LED_on
    { 100, 100, LED_blink0 },  // LED_blink0
    {  10,  90, LED_blink1 },  // LED_blink1
    {  90,  10, LED_blink2 },  // LED_blink2
    {  10,  10, LED_blink3_1 },// LED_blink3
    { 0xffff, 0xffff },        // LED_STATE_END
    {  10,  10, LED_blink3_2 },// LED_blink3_1
    {  10,  50, LED_blink3 },  // LED_blink3_2
    { 0xffff, 0xffff }         // LED_ENUM_END
};


void LedEvent_T1( void )
{
    static ledState_t recentLedState = LED_ENUM_END;
    static volatile unsigned int ledCount = 0;
    static unsigned int ledOn = 0;

    if( recentLedState != ledState ) {
        ledCount = 0;
        recentLedState = ledState;
    }
    
    if( ledCount < ledStateCounts[ledState].on && ledStateCounts[ledState].on != 0 && ledOn == 0 ) {
        LATB &= ~0b1000000000000000; //LED 点灯
        ledOn = 1;
    } else if( ledStateCounts[ledState].off != 0 && ledOn == 1 ) {
        LATB |=  0b1000000000000000; //LED 消灯
        ledOn = 0;
    }
    ledCount += 1;

    if( ledCount > ledStateCounts[ledState].on + ledStateCounts[ledState].off ) {
        ledCount = 0;
        ledState = ledStateCounts[ledState].nextState;
    }
}

void LedEvent_INT0( void )
{
    printf( "{%d}", PORTBbits.RB7 );
    if( ledState < (LED_STATE_END-1) ) {
        ledState ++;
    } else {
        ledState = LED_off;
    }
}

void LedInit( void )
{
    TRISB = 0b0111111111111111;// PORTB bit6 OUTPUT for LED
    TimerAddCallback( LedEvent_T1, 10, 0 );
    Int0AddCallback( LedEvent_INT0 );
}


