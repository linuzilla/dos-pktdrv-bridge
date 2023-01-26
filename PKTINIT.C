#include <stdio.h>
#include "pktdrv.h"
#include "bridge.h"

int pktinit(int *vector, int *handle)
{
    if (((vector[0] = initial_pktdrv()) == 0) ||
        ((vector[1] = initial_pktdrv()) == 0)) {
        return 0;
    }

    select_driver(1);

    if ((handle[0] = access_type(1, 0xFFFF, 0, NULL, 0, (RECEIVER) recif0)) == 0)
        return 0;
    set_rcv_mode(handle[0], RCV_PROMISCUOUS);

    select_driver(2);

    if ((handle[1] = access_type(1, 0xFFFF, 0, NULL, 0, (RECEIVER) recif1)) == 0)
        return 0;
    set_rcv_mode(handle[1], RCV_PROMISCUOUS);

    return 1;
}

void pktgetaddress(unsigned char *ether0, unsigned char *ether1)
{
    select_driver(1);
    get_address(handle[0], ether0, 6);
    select_driver(2);
    get_address(handle[1], ether1, 6);
}

void pktrelease(int *handle)
{
    select_driver(1);
    release_type(handle[0]);
    select_driver(2);
    release_type(handle[1]);
}
