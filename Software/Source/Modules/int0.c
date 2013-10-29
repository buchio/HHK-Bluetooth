#define USE_AND_OR
#include <xc.h>
#include <ports.h>

#define DEBUG_OUTPUT
#include "../modules.h"

///
/// INT0割り込み
///
void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void)
{
    LedEvent_INT0();
    Int0_Clear_Intr_Status_Bit;
}

void Int0Init( void ) 
{
    CNPU2=0x80; //pin#16 pull-up CN23
    ConfigINT0(INT_ENABLE | FALLING_EDGE_INT | INT_PRI_1);
}

