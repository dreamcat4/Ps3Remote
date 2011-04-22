
// PS3 BD Remote, controlled via MCP23S17 SPI 16-bit IO expander

#ifndef Ps3RemoteApp_h
#define Ps3RemoteApp_h

#ifndef ARDUINO

#include "Ps3RemoteKeys.h"

#include <stdlib.h>
#include <cmdargs.h>

#include <iostream>
#include <iomanip>

#include "serial.h"

using namespace std;

class PS3REMOTEAPP : public PS3REMOTEKEYS
{
  protected:

  public:
    int errno;
    PS3REMOTEAPP(int argc, char * argv[]);
    operator int(void) const { return  errno; }

    static const int baud = 115200;
    char * itoa(long i);

    const char * button;
    const char * serial;
    int duration;

    PS3REMOTEKEYS Ps3RemoteKeys;
    SERIAL Serial;

    void sendCmd(const char * cmd);

    void pair();
    void hold();
    void release();
    void press();


    void run();
};
#endif // !ARDUINO

#endif // Ps3RemoteApp_h


