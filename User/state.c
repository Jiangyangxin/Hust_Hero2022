#include "main.h"
#include "remote.h"
#include "pid.h"
#include "can2.h"

/**
  * @brief  ¶ÏÏß¼ì²â
  * @param  Discnect_Flag:¶ÏÏß½á¹¹Ìå
  * @retval ·µ»Ø¶ÏÏßid
  */
u32 Offline_Detect(Disconnect_Flag *Discnect_Flag)
{
    u32 Discon_Flag;
    /**************CAN1¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Can1_Connect == 0)
    {
        Discnect_Flag->Can1_Cnt ++;
        Discon_Flag &= 0xfffe;//ÊµÊ±¶ÏÏß±êÖ¾Î»ÇåÁã
    }
    if(Discnect_Flag->Can1_Cnt > MS)
    {
        Discnect_Flag->Can1_Disonnect_Ever = 1;
        Discnect_Flag->Can1_Connect = 1;
        Discon_Flag |= 0x0001;
    }
    /**************CAN2¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Can2_Connect == 0)
    {
        Discnect_Flag->Can2_Cnt ++;
        Discon_Flag &= 0xfffd;
    }
    if(Discnect_Flag->Can2_Cnt > MS)
    {
        Discnect_Flag->Can2_Disonnect_Ever = 1;
        Discnect_Flag->Can2_Connect = 1;
        Discon_Flag |= 0x0002;
    }
    /**************Ò£¿ØÆ÷¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Rc_Connect == 0)
    {
        Discnect_Flag->Rc_Cnt ++;
        Discon_Flag &= 0xfffb;
    }
    if(Discnect_Flag->Rc_Cnt > MS)
    {
        Discnect_Flag->Rc_Disonnect_Ever = 1;
        Discnect_Flag->Rc_Connect = 1;
        Discon_Flag |= 0x0004;
    }
    /**************ÍÓÂÝÒÇ¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Gyro_Connect == 0)
    {
        Discnect_Flag->Gyro_Cnt ++;
        Discon_Flag &= 0xfff7;
    }
    if(Discnect_Flag->Gyro_Cnt > MS)
    {
        Discnect_Flag->Gyro_Disonnect_Ever = 1;
        Discnect_Flag->Gyro_Connect = 1;
        Discon_Flag |= 0x0008;
    }
    /**************ÔÆÌ¨¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Gimbal_Connect == 0)
    {
        Discnect_Flag->Gimbal_Cnt ++;
        Discon_Flag &= 0xffef;
    }
    if(Discnect_Flag->Gimbal_Cnt > MS)
    {
        Discnect_Flag->Gimbal_Disonnect_Ever = 1;
        Discnect_Flag->Gimbal_Connect = 1;
        Discon_Flag |= 0x0010;
    }
    /**************µ×ÅÌ¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Chassis_Connect == 0)
    {
        Discnect_Flag->Chassis_Cnt ++;
        Discon_Flag &= 0xffdf;
    }
    if(Discnect_Flag->Chassis_Cnt > MS)
    {
        Discnect_Flag->Chassis_Disonnect_Ever = 1;
        Discnect_Flag->Chassis_Connect = 1;
        Discon_Flag |= 0x0020;
    }
    /**************PC¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Pc_Connect == 0)
    {
        Discnect_Flag->Pc_Cnt ++;
        Discon_Flag &= 0xffbf;
    }
    if(Discnect_Flag->Pc_Cnt > MS)
    {
        Discnect_Flag->Pc_Disonnect_Ever = 1;
        Discnect_Flag->Pc_Connect = 1;
        Discon_Flag |= 0x0040;
    }
    /**************Usart1¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Usart1_Connect == 0)
    {
        Discnect_Flag->Usart1_Cnt ++;
        Discon_Flag &= 0xff7f;
    }
    if(Discnect_Flag->Usart1_Cnt > MS)
    {
        Discnect_Flag->Usart1_Disonnect_Ever = 1;
        Discnect_Flag->Usart1_Connect = 1;
        Discon_Flag |= 0x0080;
    }
    /**************Usart2¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Usart2_Connect == 0)
    {
        Discnect_Flag->Usart2_Cnt ++;
        Discon_Flag &= 0xfeff;
    }
    if(Discnect_Flag->Usart2_Cnt > MS)
    {
        Discnect_Flag->Usart2_Disonnect_Ever = 1;
        Discnect_Flag->Usart2_Connect = 1;
        Discon_Flag |= 0x0100;
    }
    /**************Usart3¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Usart3_Connect == 0)
    {
        Discnect_Flag->Usart3_Cnt ++;
        Discon_Flag &= 0xfdff;
    }
    if(Discnect_Flag->Usart3_Cnt > MS)
    {
        Discnect_Flag->Usart3_Disonnect_Ever = 1;
        Discnect_Flag->Usart3_Connect = 1;
        Discon_Flag |= 0x0200;
    }
    /**************Uart4¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Uart4_Connect == 0)
    {
        Discnect_Flag->Uart4_Cnt ++;
        Discon_Flag &= 0xfbff;
    }
    if(Discnect_Flag->Uart4_Cnt > MS)
    {
        Discnect_Flag->Uart4_Disonnect_Ever = 1;
        Discnect_Flag->Uart4_Connect = 1;
        Discon_Flag |= 0x0400;
    }
    /**************Uart5¶ÏÏß¼ì²â*******************/
    if(Discnect_Flag->Uart5_Connect == 0)
    {
        Discnect_Flag->Uart5_Cnt ++;
        Discon_Flag &= 0xf7ff;
    }
    if(Discnect_Flag->Uart5_Cnt > MS)
    {
        Discnect_Flag->Uart5_Disonnect_Ever = 1;
        Discnect_Flag->Uart5_Connect = 1;
        Discon_Flag |= 0x0800;
    }
    /***********************************************/
    return Discon_Flag;
}
