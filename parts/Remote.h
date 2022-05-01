#ifndef _REMOTE_H
#define _REMOTE_H

//#include "main.h"
#define  RemoteReceiveSize 18

typedef struct {
    struct
    {
        unsigned short ch0;
        unsigned short ch1;
        unsigned short ch2;
        unsigned short ch3;
        unsigned char s1;
        unsigned char s2;
    } rc;
    struct
    {
        short x;
        short y;
        short z;
        unsigned char press_l;
        unsigned char press_r; //ÓÒ»÷

    } mouse;
    struct
    {
        unsigned short w,s,a,d,q,e,r,f,g,z,x,c,v,b,shift,ctrl;
    } key;


    volatile char recvd,DisconnectCnt;
} RC_Ctl_t;


/******************************* º¯ÊýÉùÃ÷ ****************************************/
void RemoteReceive(volatile unsigned char buff[]);
void RemoteInit(RC_Ctl_t *RC_Ctl);
void RC_Rst(void);
/**********************************************************************************/

#endif



