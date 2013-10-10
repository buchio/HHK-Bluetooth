
// Various clock values
#define GetSystemClock()            32000000UL
#define GetPeripheralClock()        (GetSystemClock())
#define GetInstructionClock()       (GetSystemClock()/2)

#define BAUDRATE2       38400
#define BRG_DIV2        16
#define BRGH2           0

#define DEMO_TIMEOUT_LIMIT  0xF000

#include <p24fxxxx.h>

