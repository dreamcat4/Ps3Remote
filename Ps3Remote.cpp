
// PS3 BD Remote, controlled via MCP23S17 SPI 16-bit IO expander

#ifndef ARDUINO

// export BASE64=$HOME/Documents/Arduino/libraries/base64 ;
// g++ -o ps3remote  -I$BASE64 $BASE64/Base64.cpp -I$HOME/tmp/CmdLine/src/lib -L$HOME/tmp/CmdLine/src/lib -lcmdline *.cpp

#include "Ps3RemoteApp.h"

int main(int argc, char * argv[])
{
  PS3REMOTEAPP ps3_remote_app(argc,argv);
  return ps3_remote_app;
}

#else // ARDUINO

#include "Ps3Remote.h"

//---------- constructor ----------------------------------------------------

PS3REMOTE::PS3REMOTE()
{
  PS3REMOTE( MCP23S17() );
}

PS3REMOTE::PS3REMOTE(MCP23S17 Mcp23s17)
{
  // Initialize the GPIO port input / output
  setup_device(Mcp23s17);
  bool button_on_hold = false;
}

//------------------ protected -----------------------------------------------

void PS3REMOTE::setup_device(MCP23S17 Mcp23s17)
{
  // initialize the GPIO ports

  // Prepare the output value to PS3_FFC_NC
  Mcp23s17.port(PS3_FFC_NC);

  // Go avoid glitching, we first set all our 4051/4053/4067 Address pins
  // to PS3_FFC_NC (not connected), whilst keeping the inhibit held high
  Mcp23s17.ioDir( GPIO_OUTPUT | INH_CD4053 );

  // We should read the IODIR register first, and mask / avoid
  // touching those upper pins

  // Clear the inhibit pin (goes LOW, enables circuit)
  Mcp23s17.ioDir( GPIO_OUTPUT );

  // Ready
  this->Mcp23s17 = Mcp23s17;
  keep_alive();
}

//---------- public ----------------------------------------------------

void PS3REMOTE::pair()
{
  Mcp23s17.port( ENTER | PS3_ALT_START );
  delay(pairing_hold_millis);
  Mcp23s17.port( PS3_FFC_NC );  
}

void PS3REMOTE::keep_alive()
{
  if(!button_on_hold)
  {
    Mcp23s17.port( PS3_KEEPALIVE );
    delay(keepalive_millis);
    Mcp23s17.port( PS3_FFC_NC );  
  }
}

void PS3REMOTE::hold(uint8_t button)
{
  button_on_hold = true;
  Mcp23s17.port( button );
}

void PS3REMOTE::release(uint8_t button)
{
  Mcp23s17.port( PS3_FFC_NC );  
  button_on_hold = false;
}

void PS3REMOTE::press(uint8_t button)
{
  Mcp23s17.port( button );
  delay(press_millis);
  Mcp23s17.port( PS3_FFC_NC );  
}

#endif // ARDUINO


