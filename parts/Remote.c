#include "main.h"

/*--------------------------全局变量声明----------------------------*/
RC_Ctl_t RC_Ctl;
/*--------------------------外部变量引用----------------------------*/
extern HeroStatus_typedef	HeroStatus;
short RC_DisConnect;
/**
  * @brief
  * @param
  * @retval
  */
void RemoteReceive(volatile unsigned char buff[])
{
    RC_Ctl.rc.ch0 = (buff[0]| (buff[1] << 8)) & 0x07ff; //!< Channel 0
    RC_Ctl.rc.ch1 = ((buff[1] >> 3) | (buff[2] << 5)) & 0x07ff; //!< Channel 1
    RC_Ctl.rc.ch2 = ((buff[2] >> 6) | (buff[3] << 2) | (buff[4] << 10)) & 0x07ff;//!< Channel 2
    RC_Ctl.rc.ch3 = ((buff[4] >> 1) | (buff[5] << 7)) & 0x07ff; //!< Channel 3
    RC_Ctl.rc.s1 = ((buff[5] >> 4)& 0x0003); //!< Switch left
    RC_Ctl.rc.s2 = ((buff[5] >> 6)& 0x0003);

    RC_Ctl.mouse.x = buff[6] | (buff[7] << 8); //!< Mouse X axis
    RC_Ctl.mouse.y = buff[8] | (buff[9] << 8); //!< Mouse Y axis
    RC_Ctl.mouse.z = buff[10] | (buff[11] << 8); //!< Mouse Z axis
    RC_Ctl.mouse.press_l = buff[12]; //!< Mouse Left Is Press ?
    RC_Ctl.mouse.press_r = buff[13]; //!< Mouse Right Is Press ?

    RC_Ctl.key.w = buff[14]&0x01; // KeyBoard value
    RC_Ctl.key.s = (buff[14]>>1)&0x01;
    RC_Ctl.key.a = (buff[14]>>2)&0x01;
    RC_Ctl.key.d = (buff[14]>>3)&0x01;
    RC_Ctl.key.shift =(buff[14]>>4)&0x01;
    RC_Ctl.key.ctrl = (buff[14]>>5)&0x01;
    RC_Ctl.key.q = (buff[14]>>6)&0x01;
    RC_Ctl.key.e = (buff[14]>>7)&0x01;
    RC_Ctl.key.r = (buff[15])&0x01;
    RC_Ctl.key.f = (buff[15]>>1)&0x01;
    RC_Ctl.key.g = (buff[15]>>2)&0x01;
    RC_Ctl.key.z = (buff[15]>>3)&0x01;
    RC_Ctl.key.x = (buff[15]>>4)&0x01;
    RC_Ctl.key.c = (buff[15]>>5)&0x01;
    RC_Ctl.key.v = (buff[15]>>6)&0x01;
    RC_Ctl.key.b = (buff[15]>>7)&0x01;

    RC_Ctl.recvd = 1; 		//数据接收标志位
    				RC_DisConnect = 0;    //断线数据清零

    if((RC_Ctl.rc.ch0-1024<10)&&(RC_Ctl.rc.ch0-1024>-10)) RC_Ctl.rc.ch0=1024;
    if((RC_Ctl.rc.ch1-1024<10)&&(RC_Ctl.rc.ch1-1024>-10)) RC_Ctl.rc.ch1=1024;
    if((RC_Ctl.rc.ch2-1024<10)&&(RC_Ctl.rc.ch2-1024>-10)) RC_Ctl.rc.ch2=1024;
    if((RC_Ctl.rc.ch3-1024<10)&&(RC_Ctl.rc.ch3-1024>-10)) RC_Ctl.rc.ch3=1024;


}
/**
  * @brief
  * @param
  * @retval
  */
void RemoteInit(RC_Ctl_t *RC_Ctl)
{
    RC_Ctl->rc.ch0 = 1024;
    RC_Ctl->rc.ch1 = 1024;
    RC_Ctl->rc.ch2 = 1024;
    RC_Ctl->rc.ch3 = 1024;
    RC_Ctl->mouse.x = 0;
    RC_Ctl->mouse.y = 0;
    RC_Ctl->mouse.z = 0;
    RC_Ctl->mouse.press_l = 0;
    RC_Ctl->mouse.press_r = 0;

    RC_Ctl->key.w = 0;
    RC_Ctl->key.s = 0;
    RC_Ctl->key.a = 0;
    RC_Ctl->key.d = 0;
    RC_Ctl->key.q = 0;
    RC_Ctl->key.e = 0;
    RC_Ctl->key.r = 0;
    RC_Ctl->key.f = 0;
	  RC_Ctl->key.g = 0;
    RC_Ctl->key.z = 0;
    RC_Ctl->key.x = 0;
    RC_Ctl->key.c = 0;
    RC_Ctl->key.v = 0;
    RC_Ctl->key.b = 0;
    RC_Ctl->key.shift = 0;
    RC_Ctl->key.ctrl = 0;

    RC_Ctl->rc.s1=2;
    RC_Ctl->rc.s2=2;

    RC_Ctl->recvd = 0; 						//数据接收标志位
    RC_Ctl->DisconnectCnt = 0;    //断线数据清零
}

void RC_Rst(void)
{
		RC_Ctl.rc.ch0 = 1024;
		RC_Ctl.rc.ch1 = 1024;
		RC_Ctl.rc.ch2 = 1024;
		RC_Ctl.rc.ch3 = 1024;
		RC_Ctl.mouse.x = 0;
		RC_Ctl.mouse.y = 0;
		RC_Ctl.mouse.z = 0;
		RC_Ctl.mouse.press_l = 0;                                                
		RC_Ctl.mouse.press_r = 0;
	
		RC_Ctl.key.w = 0;
		RC_Ctl.key.s = 0;                            
		RC_Ctl.key.a = 0;
		RC_Ctl.key.d = 0;
		RC_Ctl.key.q = 0;
		RC_Ctl.key.e = 0;
		RC_Ctl.key.r = 0;
		RC_Ctl.key.f = 0;
		RC_Ctl.key.g = 0;
    RC_Ctl.key.z = 0;
    RC_Ctl.key.x = 0;
    RC_Ctl.key.c = 0;
    RC_Ctl.key.v = 0;
    RC_Ctl.key.b = 0;
		RC_Ctl.key.shift = 0;
		RC_Ctl.key.ctrl = 0;
	
	  RC_Ctl.rc.s1 = 2;
		RC_Ctl.rc.s2 = 2;
} 





