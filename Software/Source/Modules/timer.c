#define USE_AND_OR
#include <xc.h>
#include <ports.h>
#include <timer.h>

#include "../modules.h"

///
/// タイマ1割り込み
///
void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt (void)
{
    LedEvent_T1();
    T1_Clear_Intr_Status_Bit;
}

void TimerInit( void )
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Timer1
    // 「USE_AND_OR」を定義しておくことでORでパラメータを連結することができる
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_1);
    OpenTimer1(T1_ON | T1_PS_1_256, 0x0271); // 10ms
    IEC0bits.T1IE = 1;
}
