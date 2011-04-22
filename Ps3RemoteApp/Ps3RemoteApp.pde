// This example demonstrates CmdMessenger's callback  & attach methods
// For Arduino Uno and Arduino Duemilanove board (may work with other)

// Download these into your Sketches/libraries/ folder...

#include <CmdMessenger.h>
#include <Base64.h>
#include <Streaming.h>
#include <Spi.h>
#include <Mcp23s17.h>
#include <Ps3Remote.h>

// Mustnt conflict / collide with our message payload data. Fine if we use base64 library ^^ above
char field_separator = ',';
char command_separator = ';';

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

// We must instantiate in setup(), any earlier the SPI won't work
PS3REMOTE Ps3Remote;

// Then choose any other free pin as the Slave Select (pin 10 if the default but doesnt have to be)
#define MCP23S17_SLAVE_SELECT_PIN  9 //arduino   <->   SPI Slave Select           -> CS  (Pin 11 on MCP23S17 DIP)

// ------------------ C M D  L I S T I N G ( T X / R X ) ---------------------

// We can define up to a default of 50 cmds total, including both directions (send + recieve)
// and including also the first 4 default command codes for the generic error handling.
// If you run out of message slots, then just increase the value of MAXCALLBACKS in CmdMessenger.h

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised

  // Now we can define many more 'send' commands, coming from the arduino -> the PC, eg
  // kICE_CREAM_READY,
  // kICE_CREAM_PRICE,
  // For the above commands, we just call cmdMessenger.sendCmd() anywhere we want in our Arduino program.

  kSEND_CMDS_END, // Mustnt delete this line
};

// Commands we send from the PC and want to recieve on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below vv.
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  ps3remote_pair,       // 004
  ps3remote_hold,       // 005
  ps3remote_release,    // 006
  ps3remote_press,      // 007
  NULL
};
// Its also possible (above ^^) to implement some symetric commands, when both the Arduino and
// PC / host are using each other's same command numbers. However we recommend only to do this if you
// really have the exact same messages going in both directions. Then specify the integers (with '=')


// ------------------ C A L L B A C K  M E T H O D S -------------------------

void ps3remote_pair()
{
}

void ps3remote_hold()
{
}

void ps3remote_release()
{
}

void ps3remote_press()
{
  // cmdMessenger.sendCmd(kACK,"ps3remote_press() cmd recieved");

  char buf[350] = { '\0' };

  while ( cmdMessenger.available() )
  {

    char buf[350] = { '\0' };
    cmdMessenger.copyString(buf, 350);
    if(buf[0])
    {
      char decode_buf[350] = { '\0' };
      base64_decode(decode_buf, buf, 350);

      decode_buf[0];
      // print back the hex of this
      // sprintf(decode_buf,"0x%.2x",decode_buf[0]);
      
      uint8_t button = (uint8_t)(decode_buf[0]);
      sprintf(decode_buf,"ps3remote_press() cmd - 0x%.2x recieved", button );
      Ps3Remote.press(button);
      cmdMessenger.sendCmd(kACK, decode_buf);
    }
  }

}



// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------



// ------------------ S E T U P ----------------------------------------------

void attach_callbacks(messengerCallbackFunction* callbacks)
{
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setup_ps3remote()
{
  // Instantiate Mcp23s17 object
  MCP23S17 Mcp23s17 = MCP23S17( MCP23S17_SLAVE_SELECT_PIN );

  // Instantiate Ps3Remote object
  Ps3Remote = PS3REMOTE( Mcp23s17 );

  // Ps3Remote.press( Ps3Remote.LEFT );
  // delay(500);
  // 
  // Ps3Remote.press( Ps3Remote.RIGHT );
  // delay(500);
  // 
  // Ps3Remote.press( Ps3Remote.UP );
  // delay(500);
  // 
  // Ps3Remote.press( Ps3Remote.DOWN );
  // delay(500);

}

void setup() 
{
  // Initialize ps3 remote before readying serial rx
  setup_ps3remote();

  // Listen on serial connection for messages from the pc
  // Serial.begin(57600);  // Arduino Duemilanove, FTDI Serial
  Serial.begin(115200); // Arduino Uno, Mega, with AT8u2 USB

  // cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor
  
  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  attach_callbacks(messengerCallbacks);

  arduino_ready();

}


// ------------------ M A I N ( ) --------------------------------------------

// This stops our Ps3Remote from going to sleep after the usual 5 min inactivity
// to remain in active state. This avoids occasional bluetooth wake-up delay
long timeoutInterval = 1000 * (60*5 - 30);
long previousMillis = 0;

void loop()
{
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();

  if (  millis() - previousMillis > timeoutInterval )
  {
    // Ps3Remote.keep_alive();
    previousMillis = millis();
  }
  // Loop.
}




