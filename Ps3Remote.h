
// PS3 BD Remote, controlled via MCP23S17 SPI 16-bit IO expander

#ifndef Ps3Remote_h
#define Ps3Remote_h

#include <Ps3RemoteKeys.h>

#ifdef ARDUINO

#include "WProgram.h"

#include <Mcp23s17.h>

class PS3REMOTE : public PS3REMOTEKEYS
{
  protected:
    MCP23S17 Mcp23s17;

    void setup_device(MCP23S17 Mcp23s17);

    // Button press delays
    const static uint16_t pairing_hold_millis = 3300;
    const static uint16_t keepalive_millis    =    5;
    const static uint16_t press_millis        =    5;

    // Skip keepalive whilst button pressed
    bool button_on_hold;

  public:
    PS3REMOTE();
    PS3REMOTE(MCP23S17 Mcp23s17);

    void pair();
    void keep_alive();
    void hold(uint8_t button);
    void release(uint8_t button);
    void press(uint8_t button);
};
#endif // ARDUINO

#endif // Ps3Remote_h


