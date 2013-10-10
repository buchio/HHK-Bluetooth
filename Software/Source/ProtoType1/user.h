#ifndef H_USER_H_131008145849_
#define H_USER_H_131008145849_

/** D E F I N I T I O N S ****************************************************/
/* PICDEM FS USB Demo Version */
#define DEMO_FW_MINOR_VERSION   0x00    //Demo Version 1.00
#define DEMO_FW_MAJOR_VERSION   0x01

#define DATA_PACKET_LENGTH  8

#define POT_FULL_VALUE   10000

/* Commands */
#define READ_VERSION     0x00
#define CMD_INVALID      0xAA
#define ID_BOARD         0x31
#define UPDATE_LED       0x32
#define SET_TEMP_REAL    0x33
#define RD_TEMP          0x34
#define SET_TEMP_LOGGING 0x35
#define RD_TEMP_LOGGING  0x36
#define RD_POT           0x37
#define RESET            0xFF

#define SW3_STATE        0x08
#define SW6_STATE        0x40

/** S T R U C T U R E S ******************************************************/
typedef union DATA_PACKET
{
    BYTE _byte[DATA_PACKET_LENGTH];  //For byte access
    WORD _word[DATA_PACKET_LENGTH/2];//For word access(DATA_PACKET_LENGTH must be even)
    struct
    {
        BYTE CMD;
        BYTE len;
    };
    struct
    {
        BYTE res1;
        BYTE ID;
    };
    struct
    {
        BYTE res2;
        BYTE led_num;
        BYTE led_status;
    };
    struct
    {
        BYTE res3;
        WORD word_data;
    };
} DATA_PACKET;

#endif //H_USER_H_131008145849_
