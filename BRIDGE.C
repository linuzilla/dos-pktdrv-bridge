#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <alloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BRIDGE
#define LEARNING

#include "pktdrv.h"
#include "bridge.h"
#include "asmtool.h"

int    vect[2];
int    handle[2];
unsigned char   ether[2][6];

#ifdef LEARNING

#define LEARNTABLESIZE   (100)
unsigned char  broadCast[6] = { '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF' };
unsigned char  learningTable[LEARNTABLESIZE][6];
int            learningTableIDX = 0;

#else

#define NUMOFMAC  (4)

unsigned char  MacAddr[][6] = {
             { '\x00', '\x80', '\xc8', '\x36', '\xa6', '\x5b' },  /* pc win95 */
             { '\x00', '\x80', '\xc8', '\x26', '\x5b', '\x4a' },  /* dlink local bridge */
             { '\x00', '\x00', '\x21', '\x55', '\x55', '\x26' },  /* pc */
             { '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF' },  /* broadcast */
             { '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' }
                               };

#endif

InPkt                pktr;
unsigned int         pktrlen;
int                  buf_full = 0, buf_ready = 0;
unsigned long        pkt_received = 0L, pkt_dropped = 0L;

static char *print_ether(unsigned char *buf);
static char *print_ip(unsigned char *buf);

int main(int argc, char *argv[])
{
    int      i;
#ifdef LEARNING
    int      found;
#endif


    if (! pktinit(vect, handle)) {
        printf("Bridge need 2 interface card\n");
        return 1;
    }

    pktgetaddress(ether[0], ether[1]);

    printf("Packet Driver found at 0x%02x and 0x%02x\n", vect[0], vect[1]);
    printf("  eth0: %s\n", print_ether(ether[0]));
    printf("  eth1: %s\n", print_ether(ether[1]));

    while (! kbhit() || getch() != 27) {
        if (buf_ready) {
            switch (buf_ready) {
            case INTERFACE0:
#ifdef LEARNING
                for (i = found = 0; i < learningTableIDX; i++) {
                    if (MacAddrCMP((char far *) learningTable[i], (char far *) pktr.sa)) {
                        found = 1;
                        break;
                    }
                }
                if (found) {
#else
                if (MacAddrCMP((char far *) MacAddr[0], (char far *) pktr.sa)) {
#endif
                    printf("eth0 sa: %s", print_ether(pktr.sa));
                    printf(" da: %s\n", print_ether(pktr.da));
                    select_driver(2);
                    send_pkt(&pktr, pktrlen);
                } else {
#ifdef BRIDGE
#ifdef LEARNING
                    if (MacAddrCMP((char far *) broadCast, (char far *) pktr.da)) {
                        select_driver(2);
                        send_pkt(&pktr, pktrlen);
                    } else {
                        for (i = 0; i < learningTableIDX; i++) {
                            if (MacAddrCMP((char far *) learningTable[i], (char far *) pktr.da)) {
                                select_driver(2);
                                send_pkt(&pktr, pktrlen);
                                break;
                            }
                        }
                    }
#else
                    for (i = 0; i < NUMOFMAC; i++) {
                        if (MacAddrCMP((char far *) MacAddr[i], (char far *) pktr.da)) {
                            select_driver(2);
                            send_pkt(&pktr, pktrlen);
                            break;
                        }
                    }
#endif
#else
                    select_driver(2);
                    send_pkt(&pktr, pktrlen);
#endif
                }
                break;
            case INTERFACE1:
#ifdef LEARNING
                for (i = found = 0; i < learningTableIDX; i++) {
                    if (MacAddrCMP((char far *) learningTable[i], (char far *) pktr.sa)) {
                        found = 1;
                        break;
                    }
                }

                if (! found && learningTableIDX < LEARNTABLESIZE) {
                    for (i = 0; i < 6; i++) {
                        learningTable[learningTableIDX][i] = pktr.sa[i];
                    }
                    learningTableIDX++;
                    printf("Ethernet Address: %s learned.\n", print_ether(pktr.sa));
                }
#endif
                select_driver(1);
                send_pkt(&pktr, pktrlen);
                break;
            }
            buf_ready = 0;
            buf_full = 0;
        }
    }

    printf("\n%lu packets received by filter\n", pkt_received);
    printf("%lu packets dropped by kernel\n", pkt_dropped);

    pktrelease(handle);
    return 0;
}

static char *print_ether(unsigned char *buf)
{
    static char  ether[20];
    sprintf(ether, "%02X:%02X:%02X:%02X:%02X:%02X",
            buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    return ether;
}

static char *print_ip(unsigned char *buf)
{
     static char  ip[16];
     sprintf(ip, "%u.%u.%u.%u", buf[0], buf[1], buf[2], buf[3]);
     return ip;
}
