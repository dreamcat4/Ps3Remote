
#ifndef ARDUINO

/*
 * Arduino-serial
 * --------------
 * 
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC) 
 *
 *
 * Compile with something like:
 * gcc -o arduino-serial arduino-serial.c
 *
 * Created 5 December 2006
 * Copyleft (c) 2006, Tod E. Kurt, tod@todbot.com
 * http://todbot.com/blog/
 *
 * 
 * Updated 8 December 2006: 
 *  Justin McBride discoevered B14400 & B28800 aren't in Linux's termios.h.
 *  I've included his patch, but commented out for now.  One really needs a
 *  real make system when doing cross-platform C and I wanted to avoid that
 *  for this little program. Those baudrates aren't used much anyway. :)
 *
 * Updated 26 December 2007:
 *  Added ability to specify a delay (so you can wait for Arduino Diecimila)
 *  Added ability to send a binary byte number
 *
 * Update 31 August 2008:
 *  Added patch to clean up odd baudrates from Andy at hexapodia.org
 *
 */
 
#include "serial.h"

#include <cstddef>
#include <stdint.h>

#include <iostream>
#include <iomanip>
using namespace std;

void SERIAL::usage(void) {
    printf("Usage: arduino-serial -p <serialport> [OPTIONS]\n"
    "\n"
    "Options:\n"
    "  -h, --help                   Print this help message\n"
    "  -p, --port=serialport        Serial port Arduino is on\n"
    "  -b, --baud=baudrate          Baudrate (bps) of Arduino\n"
    "  -s, --send=data              Send data to Arduino\n"
    "  -r, --receive                Receive data from Arduino & print it out\n"
    "  -n  --num=num                Send a number as a single byte\n"
    "  -d  --delay=millis           Delay for specified milliseconds\n"
    "\n"
    "Note: Order is important. Set '-b' before doing '-p'. \n"
    "      Used to make series of actions:  '-d 2000 -s hello -d 100 -r' \n"
    "      means 'wait 2secs, send 'hello', wait 100msec, get reply'\n"
    "\n");
}

int SERIAL::serial(int argc, char *argv[]) 
// int SERIAL::serial(int argc, const char * argv[]) 
{
    int fd = 0;
    char serialport[256];
    int baudrate = B9600;  // default
    char buf[256];
    // char buf[256] = { '\0' };
    int rc,n;

    // cout << "point 01" << endl; cout.flush();

    if (argc==1) {
        usage();
        // exit(EXIT_SUCCESS);
    }
    // cout << "point 02" << endl; cout.flush();

    /* parse options */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",       no_argument,       0, 'h'},
        {"port",       required_argument, 0, 'p'},
        {"baud",       required_argument, 0, 'b'},
        {"send",       required_argument, 0, 's'},
        {"receive",    no_argument,       0, 'r'},
        {"num",        required_argument, 0, 'n'},
        {"delay",      required_argument, 0, 'd'}
    };
    // cout << "point 03" << endl; cout.flush();
    
    while(1) {
        // cout << "point 04" << endl; cout.flush();
        // opt = getopt_long (argc, (char * const *)argv, "hp:b:s:rn:d:",
        //                    loptions, &option_index);

         opt = getopt_long (argc, argv, "hp:b:s:rn:d:",
                            loptions, &option_index);

       // cout << "point 05" << endl; cout.flush();

        if (opt==-1) break;
        switch (opt) {
        case '0': break;
        case 'd':
            n = strtol(optarg,NULL,10);
            usleep(n * 1000 ); // sleep milliseconds
            break;
        case 'h':
            usage();
            break;
        case 'b':
            baudrate = strtol(optarg,NULL,10);
            // cout << "point -b" << endl; cout.flush();
            // cout << "bautrate = " << baudrate << endl; cout.flush();
            break;
        case 'p':
            // cout << "point -p" << endl; cout.flush();
            strcpy(serialport,optarg);
            fd = serialport_init(optarg, baudrate);
            if(fd==-1) return -1;
            break;
        case 'n':
            n = strtol(optarg, NULL, 10); // convert string to number
            rc = serialport_writebyte(fd, (uint8_t)n);
            if(rc==-1) return -1;
            break;
        case 's':
        // cout << "point -s" << endl; cout.flush();
            strcpy(buf,optarg);
            rc = serialport_write(fd, buf);
            if(rc==-1) return -1;
            break;
        case 'r':
        // cout << "point read_until" << endl; cout.flush();
            serialport_read_until(fd, buf, '\n');
            printf("read: %s\n",buf);
            break;
        }
    }

    // exit(EXIT_SUCCESS);    
} // end main
    
int SERIAL::serialport_writebyte( int fd, uint8_t b)
{
    int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;
}

int SERIAL::serialport_write(int fd, const char* str)
{
    int len = strlen(str);
    int n = write(fd, str, len);
    if( n!=len ) 
        return -1;
    return 0;
}

int SERIAL::serialport_read_until(int fd, char* buf, char until)
{
    float timeout = 5.0;
    float elapsed = 0.0;

    char b[1];
    int i=0;
    do { 
      // cout << "reading" << endl; cout.flush();

        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            elapsed += 0.01;
            continue;
        }
        buf[i] = b[0]; i++;
        elapsed = 0;

    } while( b[0] != until && elapsed < timeout );

    buf[i] = 0;  // null terminate the string
    return 0;
}

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
int SERIAL::serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
    
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    // brate=B115200;
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;

    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;

    toptions.c_cflag &= ~HUPCL;      // disable hang-up-on-close to avoid resetting next time
                                     //     - doesnt seem to work (Arduino UNO)

    toptions.c_cflag &= ~CDTR_IFLOW; // disable DTR flow control - doesnt seem to work (Arduino UNO)

    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    if(ioctl(fd, TIOCCDTR) == -1) // Clear Data Terminal Ready(DTR) - doesnt seems to work (Arduino UNO)
    {
      perror("init_serialport: Error clearing DTR");
      return -1;
    }

    return fd;
}

#endif // !ARDUINO
