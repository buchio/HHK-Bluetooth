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

void LedInit( void )
{
    TRISB = 0b0111111111111111;// PORTB bit6 OUTPUT for LED
}

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


