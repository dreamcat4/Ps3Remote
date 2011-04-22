
// PS3 BD Remote, controlled via MCP23S17 SPI 16-bit IO expander

#ifndef Ps3RemoteKeys_h
#define Ps3RemoteKeys_h

#ifndef ARDUINO
#include <stdint.h>
#include "binary.h"
#else
#include "WProgram.h"
#endif

class PS3REMOTEKEYS
{
  protected:
    // Pin assignments MCP23S17 <-> Sony BD Remote 28-pin FFC connector
    // These mappings / assignment are specific to the circuit used

    // GPIO port, bits 0-2
    const static uint8_t PS3_FFC_01 = 0x00;
    const static uint8_t PS3_FFC_02 = 0x01;
    const static uint8_t PS3_FFC_03 = 0x02;
    const static uint8_t PS3_FFC_04 = 0x03;
    const static uint8_t PS3_FFC_05 = 0x04;
    const static uint8_t PS3_FFC_06 = 0x05;
    const static uint8_t PS3_FFC_07 = 0x06;
    const static uint8_t PS3_FFC_08 = 0x07;

    // GPIO port, bits 4-7
    const static uint8_t CD4067_START_BIT = 4;
    const static uint8_t PS3_FFC_09 = 0x00 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_10 = 0x01 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_11 = 0x02 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_12 = 0x03 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_13 = 0x04 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_14 = 0x05 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_15 = 0x06 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_16 = 0x07 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_17 = 0x08 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_18 = 0x09 << CD4067_START_BIT;
    const static uint8_t PS3_FFC_19 = 0x0a << CD4067_START_BIT;
    const static uint8_t PS3_FFC_20 = 0x0b << CD4067_START_BIT;
    const static uint8_t PS3_FFC_21 = 0x0c << CD4067_START_BIT;
    const static uint8_t PS3_FFC_22 = 0x0d << CD4067_START_BIT;
    const static uint8_t PS3_FFC_23 = 0x0e << CD4067_START_BIT;
    const static uint8_t PS3_FFC_24 = 0x0f << CD4067_START_BIT;

    // GPIO port, bit 3 + bits 4-7
    const static uint8_t CD4053_SELECT_4067B = B1000; 
    const static uint8_t PS3_FFC_25 = CD4053_SELECT_4067B | ( 0x00 << CD4067_START_BIT );
    const static uint8_t PS3_FFC_26 = CD4053_SELECT_4067B | ( 0x01 << CD4067_START_BIT );
    const static uint8_t PS3_FFC_27 = CD4053_SELECT_4067B | ( 0x02 << CD4067_START_BIT );
    const static uint8_t PS3_FFC_28 = CD4053_SELECT_4067B | ( 0x03 << CD4067_START_BIT );

    // not connected *isnt* 0x00 !
    const static uint8_t PS3_FFC_NC = CD4053_SELECT_4067B | ( 0x04 << CD4067_START_BIT );

    // these are over on GPIO port B
    const static uint16_t GPIO_OUTPUT  = 0x0000;
    const static uint8_t  GPIO_PORT_B_START_BIT = 8; 
    const static uint16_t PS3_KEEPALIVE = B0001 << GPIO_PORT_B_START_BIT;
    const static uint16_t PS3_ALT_START = B0010 << GPIO_PORT_B_START_BIT;

    // CD4053 inhibit is pulled high (and GPIO B2 to remain an input) until kPS3_NC is set
    const static uint16_t INH_CD4053    = B0100 << GPIO_PORT_B_START_BIT;


  public:

#ifndef ARDUINO
    // Lookup table for the host-side

    struct keys_pair \
    {
      const char * str;
      // const string str;
      // uint16_t sym;
      uint8_t sym;
    };

    static const keys_pair keys[];
    uint16_t str2sym(const char * str);

#endif // !ARDUINO

    const static uint8_t NUM3        = PS3_FFC_01 | PS3_FFC_09;
    const static uint8_t NUM5        = PS3_FFC_02 | PS3_FFC_09;
    const static uint8_t NUM7        = PS3_FFC_03 | PS3_FFC_09;
    const static uint8_t SUBTITLE    = PS3_FFC_04 | PS3_FFC_09;
    const static uint8_t RETURN      = PS3_FFC_05 | PS3_FFC_09;
    const static uint8_t PLAY        = PS3_FFC_06 | PS3_FFC_09;
    const static uint8_t PREV        = PS3_FFC_07 | PS3_FFC_09;

    const static uint8_t NUM2        = PS3_FFC_01 | PS3_FFC_10;
    const static uint8_t NUM4        = PS3_FFC_02 | PS3_FFC_10;
    const static uint8_t ANGLE       = PS3_FFC_03 | PS3_FFC_10;
    const static uint8_t BLUE        = PS3_FFC_04 | PS3_FFC_10;
    const static uint8_t POPUP       = PS3_FFC_05 | PS3_FFC_10;
    const static uint8_t SCAN_BCK    = PS3_FFC_06 | PS3_FFC_10;
    const static uint8_t STEP_FWD    = PS3_FFC_07 | PS3_FFC_10;

    const static uint8_t NUM1        = PS3_FFC_01 | PS3_FFC_11;
    const static uint8_t AUDIO       = PS3_FFC_02 | PS3_FFC_11;
    const static uint8_t TIME        = PS3_FFC_03 | PS3_FFC_11;
    const static uint8_t YELLOW      = PS3_FFC_04 | PS3_FFC_11;
    const static uint8_t TOP_MENU    = PS3_FFC_05 | PS3_FFC_11;
    const static uint8_t R3          = PS3_FFC_06 | PS3_FFC_11;
    const static uint8_t PAUSE       = PS3_FFC_07 | PS3_FFC_11;

    const static uint8_t EJECT       = PS3_FFC_01 | PS3_FFC_12;
    const static uint8_t NUM9        = PS3_FFC_02 | PS3_FFC_12;
    const static uint8_t GREEN       = PS3_FFC_04 | PS3_FFC_12;

    const static uint8_t PS3_DISPLAY = PS3_FFC_05 | PS3_FFC_12;

    const static uint8_t NEXT        = PS3_FFC_06 | PS3_FFC_12;
    const static uint8_t START       = PS3_FFC_07 | PS3_FFC_12;

    const static uint8_t NUM6        = PS3_FFC_01 | PS3_FFC_13;
    const static uint8_t NUM8        = PS3_FFC_02 | PS3_FFC_13;
    const static uint8_t CLEAR       = PS3_FFC_03 | PS3_FFC_13;
    const static uint8_t RED         = PS3_FFC_04 | PS3_FFC_13;
    const static uint8_t SCAN_FWD    = PS3_FFC_05 | PS3_FFC_13;
    const static uint8_t STOP        = PS3_FFC_06 | PS3_FFC_13;
    const static uint8_t STEP_BCK    = PS3_FFC_07 | PS3_FFC_13;
    const static uint8_t L3          = PS3_FFC_08 | PS3_FFC_13;

    const static uint8_t TRIANGLE    = PS3_FFC_01 | PS3_FFC_14; //
    const static uint8_t CIRCLE      = PS3_FFC_02 | PS3_FFC_15; //
    const static uint8_t ENTER       = PS3_FFC_03 | PS3_FFC_16;
    const static uint8_t CROSS       = PS3_FFC_04 | PS3_FFC_17;
    const static uint8_t SQUARE      = PS3_FFC_05 | PS3_FFC_18;
    const static uint8_t PS          = PS3_FFC_06 | PS3_FFC_19;
    const static uint8_t R2          = PS3_FFC_07 | PS3_FFC_20;
    const static uint8_t SELECT      = PS3_FFC_08 | PS3_FFC_21;

    const static uint8_t UP          = PS3_FFC_01 | PS3_FFC_22;
    const static uint8_t LEFT        = PS3_FFC_02 | PS3_FFC_23; //
    const static uint8_t RIGHT       = PS3_FFC_03 | PS3_FFC_24; //
    const static uint8_t DOWN        = PS3_FFC_04 | PS3_FFC_25;
    const static uint8_t R1          = PS3_FFC_05 | PS3_FFC_26;
    const static uint8_t L1          = PS3_FFC_06 | PS3_FFC_27;
    const static uint8_t L2          = PS3_FFC_07 | PS3_FFC_28;

};

#endif // Ps3RemoteKeys_h


