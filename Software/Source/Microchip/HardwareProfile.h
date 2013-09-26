
// Various clock values
#define GetSystemClock()            32000000UL
#define GetPeripheralClock()        (GetSystemClock())
#define GetInstructionClock()       (GetSystemClock()/2)

#define BAUDRATE2       115200//57600 //19200
#define BRG_DIV2        4 //16
#define BRGH2           1 //0
