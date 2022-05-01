
#include "Communication.h"
/*主要做全局标志的通信，有24个标志位可用
因为这个是32位存储，所以要从低地址开始访问
通过xxEventGroupSetBits 设置标志位
通过xEventGroupWaitBits 清除标志位

位0 底盘掉线 位1 遥控器掉线 位2 云台掉线 位3 裁判系统掉线
位4 卡尔曼模式开始标志位  位5 大波盘模式标志位
位6 超级电容自动加速模式开启 
位7 摩擦轮转动标志位 位8 陀螺仪开启
位9 云台开启 位10 裁判系统开启
位11 遥控器开启
*/
EventGroupHandle_t global_flag = NULL;
QueueHandle_t      global_array = NULL;
void communicate_init(void) {
	global_array = xQueueCreate(4 , 4);//4个字节，4个单元
    global_flag = xEventGroupCreate();
    xEventGroupSetBits(global_flag,1 << 0);//只有为1的值才对标志组有影响
    xEventGroupSetBits(global_flag,1 << 1);
    xEventGroupSetBits(global_flag,1 << 2);
    xEventGroupSetBits(global_flag,1 << 3);
    xEventGroupSetBits(global_flag,1 << 4);
    xEventGroupSetBits(global_flag,1 << 5);
    xEventGroupSetBits(global_flag,1 << 6);
    xEventGroupSetBits(global_flag,1 << 7);
    xEventGroupSetBits(global_flag,1 << 8);
	xEventGroupWaitBits(global_flag, 1 << 0 ,pdTRUE,pdTRUE,10);//等待10个tick,只需要修改这个左移指令
	

}

