#ifndef __ROUTE_H_
#define __ROUTE_H_

#include "pktdrv.h"

#define INTERFACE0     (1)
#define INTERFACE1     (2)

extern int                  handle[2];
extern InPkt                pktr, pkts, pkts2, pkts_auto;
extern unsigned int         pktrlen, learned;
extern int                  buf_full, buf_ready;
extern unsigned long        pkt_received, pkt_dropped;

extern int  pktinit(int *vector, int *handle);
extern void pktrelease(int *handle);
extern void pktgetaddress(unsigned char *e1, unsigned char *e2);

extern void interrupt far   recif0  (unsigned bp, unsigned di, unsigned si,
                                     unsigned ds, unsigned es, unsigned dx,
                                     unsigned cx, unsigned bx, unsigned ax);
extern void interrupt far   recif1  (unsigned bp, unsigned di, unsigned si,
                                     unsigned ds, unsigned es, unsigned dx,
                                     unsigned cx, unsigned bx, unsigned ax);


#endif
