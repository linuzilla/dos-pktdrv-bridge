#include <dos.h>
#include "pktdrv.h"
#include "bridge.h"

#pragma warn -parm

void interrupt far   recif0  (unsigned bp, unsigned di, unsigned si,
                              unsigned ds, unsigned es, unsigned dx,
                              unsigned cx, unsigned bx, unsigned ax)
{
    if (! ax) {                    /*   AX == 0  (request a buffer)   */
        if (! buf_full) {
            es = FP_SEG(&pktr);
            di = FP_OFF(&pktr);
            pktrlen = cx;
            buf_full = 1;
            pkt_received++;
        } else {
            es = di = 0;
            pkt_dropped++;
        }
    } else {                        /*   AX == 1  (copy completed)     */
        buf_ready = INTERFACE0;
    }
}

void interrupt far   recif1  (unsigned bp, unsigned di, unsigned si,
                              unsigned ds, unsigned es, unsigned dx,
                              unsigned cx, unsigned bx, unsigned ax)
{
    if (! ax) {                    /*   AX == 0  (request a buffer)   */
        if (! buf_full) {
            es = FP_SEG(&pktr);
            di = FP_OFF(&pktr);
            pktrlen = cx;
            buf_full = 1;
            pkt_received++;
        } else {
            es = di = 0;
            pkt_dropped++;
        }
    } else {                        /*   AX == 1  (copy completed)     */
        buf_ready = INTERFACE1;
    }
}
