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
	ConfigINT0(INT_ENABLE | FALLING_EDGE_INT | INT_PRI_1); /*Enable inerrupt*/
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

