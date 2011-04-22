
#ifndef ARDUINO

#include "Ps3RemoteApp.h"
#include <Base64.h>

// #include <stdio.h> 
// #include <stdlib.h>
// #include <cstdlib>
// using namespace std;

//---------- constructor ----------------------------------------------------

PS3REMOTEAPP::PS3REMOTEAPP(int argc, char * argv[])
{
  this->errno = EXIT_FAILURE;

  // These classes have no copy constructors
  CmdArgBool pair     ('p', "pair",     "Pair the remote (duration 3.3 seconds).");
  CmdArgBool hold     ('h', "hold",     "Button down (keydown)");
  CmdArgBool release  ('r', "release",  "Button up   (keyup)");

  CmdArgInt  duration ('d', "duration", "ms",              "Button down (keydown) duration");
  CmdArgStr  button   ('b', "button",   "identifier",      "The button to press");
  CmdArgStr  serial   ('s', "serial",   "/dev/serialport", "POSIX tty serial device");

  // Declare command object and its argument-iterator
  CmdLine  cmd(*argv, &pair, &hold, &release, &duration, &button, &serial, NULL);
  CmdArgvIter  arg_iter(--argc, ++argv);

  // Default values.
  pair = false;
  hold = false;
  release = false;
  duration = 0;

  button = "PS";
  serial = "/dev/tty.usbmodem3d11";

  // Parse arguments
  cmd.parse(arg_iter);

  // Store result
  this->button   = (const char *) button;
  this->serial   = (const char *) serial;
  this->duration = (int)          duration;

  if( (bool)pair )
    this->pair();

  if( (bool)hold )
    this->hold();

  if( (bool)release && !(int)duration)
    this->release();

  if( !(bool)hold && !(bool)release )
    this->press();

  run();
}

void PS3REMOTEAPP::pair()
{
  // send cmd pair
}

void PS3REMOTEAPP::hold()
{
  if(duration)
  {
    // send cmd button dur
  }
  else
  {
    // send cmd hold on
  }
}

void PS3REMOTEAPP::release()
{
  // send cmd release
}

void PS3REMOTEAPP::press()
{
  if(duration)
  {
    // send cmd button dur

    sendCmd("4;");
  }
  else
  {
    // send cmd press

    // uint8_t keycode = str2sym( (const char *)button );
    // char data[] = { keycode, '\0' };
    char data[] = { str2sym( (const char *)button ), '\0' };
    char base64_data[350] = { '\0' };
    base64_encode(base64_data, data, 1);

    char  cmd_string[350] = { '\0' };
    sprintf(cmd_string, "7,%s;", base64_data);
    cout << cmd_string << endl;
    sendCmd(cmd_string);
  }
}

char * PS3REMOTEAPP::itoa(long i)
{
  char * buf = (char *)malloc(sizeof(char)*25);
  sprintf(buf,"%li",i);
  return buf;
}

void PS3REMOTEAPP::sendCmd(const char* cmd)
{
  const char * args[] = { "sendcmd","-b",itoa(this->baud),"-p",serial,"-d300","-s",cmd,"-r" };
  Serial.serial( 9, (char **)args );
}

void PS3REMOTEAPP::run()
{
  // cmd;  
  
  // Ps3RemoteKeys.str2sym( (const char*)button );
  printf("0x%.2x\n",Ps3RemoteKeys.str2sym( (const char*)button ));

  // Print arguments
  // cout << button << ", symbol = 0x" << hex << Ps3RemoteKeys.str2sym( (const char*)button ) << endl;

  // cout << "hold = " << (bool)hold << endl;
  // cout.flush();

  // arduino-serial -b 115200 -p /dev/tty.usbmodem3d11 -r -s "4;" -r
  // const char * args[] = { "this_cmd", "-b115200","-p/dev/tty.usbmodem3d11","-r","-s4;","-r" };
  // Serial.serial( 6, (char **)args );

  const char * args[] = { "this_cmd", "-b115200","-p/dev/tty.usbmodem3d11","-s4;","-r" };
  Serial.serial( 5, (char **)args );

  // const char * args[] = { "this_cmd", "-b115200","-p/dev/tty.usbmodem3d11","-d50","-s4;","-r" };
  // Serial.serial( 6, (char **)args );

  // cout << button << ", symbol = " << Ps3RemoteKeys.str2sym( (const char*)button ) << endl;
  // cout << button << "symbol = " << setprecision(2) << width(2) << hex << 11 << endl;

  this->errno = EXIT_SUCCESS;
}

#endif // !ARDUINO


