
// This example demonstrates control over PS3 DB Remote Control, via Microchip MCP23S17 16-bit IO expander
// and custom demultiplexer circuitry and FFC Connector interface. This example is designed to drive the
// controller circuit and not perform any kind of Arduino emulation without the Sony BD Remote Hardware.

// Download these into your Sketches/libraries/ folder...
#include <Spi.h>
#include <Mcp23s17.h>
#include <Ps3Remote.h>

// Then choose any other free pin as the Slave Select (pin 10 if the default but doesnt have to be)
#define MCP23S17_SLAVE_SELECT_PIN  9 //arduino   <->   SPI Slave Select           -> CS  (Pin 11 on MCP23S17 DIP)

// We must instantiate in setup(), any earlier the SPI won't work
PS3REMOTE Ps3Remote;

void setup()
{
  // Instantiate Mcp23s17 object
  MCP23S17 Mcp23s17 = MCP23S17( MCP23S17_SLAVE_SELECT_PIN );

  // Instantiate Ps3Remote object
  Ps3Remote = PS3REMOTE( Mcp23s17 );


  // Example usage

  Ps3Remote.press( Ps3Remote.LEFT );
  delay(500);

  Ps3Remote.press( Ps3Remote.RIGHT );
  delay(500);

  Ps3Remote.press( Ps3Remote.UP );
  delay(500);

  Ps3Remote.press( Ps3Remote.DOWN );
  delay(500);

}

// This stops our Ps3Remote from going to sleep after the usual 5 min inactivity
// to remain in active state. This avoids occasional bluetooth wake-up delay
long timeoutInterval = 1000 * (60*5 - 30);
long previousMillis = 0;

void loop()
{
  if (  millis() - previousMillis > timeoutInterval )
  {
    Ps3Remote.keep_alive();
    previousMillis = millis();
  }
  // Loop.
}


