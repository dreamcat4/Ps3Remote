
// Serial send / recv

#ifndef Serial_h
#define Serial_h

#ifndef ARDUINO

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
// #include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ttycom.h> /* TTY ioctl functions */
#include <sys/ioctl.h>
#include <getopt.h>

class SERIAL
{
  protected:

    void usage(void);
    int serialport_init(const char* serialport, int baud);
    int serialport_writebyte(int fd, uint8_t b);
    int serialport_write(int fd, const char* str);
    int serialport_read_until(int fd, char* buf, char until);


  public:

    int serial(int argc, char *argv[]);
    // int serial(int argc, const char * argv[]);

};

#endif // !ARDUINO

#endif // Serial_h


