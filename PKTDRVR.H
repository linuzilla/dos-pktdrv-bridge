#ifndef __PKT_DRVR__H_
#define __PKT_DRVR__H_

#include "pktdrv.h"

class CPacketType {
protected:
    int       handle;
    int       iclass;
    int       itype;
    int       inum;
    int       rmode;
    RECEIVER  receiver;
};

class CPacketDriver {
protected:
    const int    vec = 0x19;
    int          vector;
    char         ip[4];
    char         eth[6];
    CPacketType  *pkt;
public:
    CPacketDriver(

};

#endif
