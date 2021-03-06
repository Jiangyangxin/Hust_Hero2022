#ifndef _MINIPC_H
#define _MINIPC_H

#include "main.h"

#define PCReceiveSize 8
typedef struct
{
    float RCPitch;
    float RCYaw;
    short PCDisconnect;
    short Recvd;

    short Cmd;

} PC_Recv_t;

void PCReceive(unsigned char PCReceivebuffer[]);
void MiniPC_SendThread(void);
#endif
