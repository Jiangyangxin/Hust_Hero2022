#include "Start_Task.h"
#include "GimBalTask.h"
#include "ShootTask.h"
#include "ChassisTask.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

uint32_t CPU_high_water;

/*�������ȼ���ֵԽ�ͣ����ȼ�Խ��*/

#define CHASSIS_TASK_PRIO 19  //�������ȼ�
#define CHASSIS_STK_SIZE 256 //�����ջ
static TaskHandle_t CHASSISTask_Handler; //������

#define TX2_TASK_PRIO 16  //�������ȼ�
#define TX2_STK_SIZE 256 //�����ջ
static TaskHandle_t TX2Task_Handler; //������

#define GIMBAL_TASK_PRIO 20  //�������ȼ�
#define GIMBAL_STK_SIZE 256 //�����ջ
static TaskHandle_t GIMBALTask_Handler; //������

#define SHOOT_TASK_PRIO 18  //�������ȼ�
#define SHOOT_STK_SIZE 256 //�����ջ
static TaskHandle_t SHOOTTask_Handler; //������

#define CONTROL_TASK_PRIO 17  //�������ȼ�
#define CONTROL_STK_SIZE 256 //�����ջ
static TaskHandle_t CONTROLTask_Handler; //������

#define CPU_TASK_PRIO 2  //�������ȼ�
#define CPU_STK_SIZE 256 //�����ջ
static TaskHandle_t CPUTask_Handler; //������

#define START_TASK_PRIO 1  //�������ȼ�
#define START_STK_SIZE 128 //�����ջ
static TaskHandle_t StartTask_Handler; //������

#define OFFLINE_CHECK_TASK_PRIO 3  //�������ȼ�
#define OFFLINE_CHECK_STK_SIZE 128 //�����ջ
static TaskHandle_t OfflineCheckTask_Handler; //������

#define LED_TASK_PRIO 2  //�������ȼ�
#define LED_TASK_STK_SIZE 128 //�����ջ
static TaskHandle_t LEDTask_Handler; //������

/**********************************************************************************************************
*�� �� ��: start_task
*����˵��: ����������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    xTaskCreate((TaskFunction_t)GIMBAL_task,          //������
                (const char *)"GIMBAL_task",          //��������
                (uint16_t)GIMBAL_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)GIMBAL_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&GIMBALTask_Handler); //������

    xTaskCreate((TaskFunction_t)SHOOT_task,          //������
                (const char *)"SHOOT_task",          //��������
                (uint16_t)SHOOT_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)SHOOT_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&SHOOTTask_Handler); //������

    xTaskCreate((TaskFunction_t)CHASSIS_task,          //������
                (const char *)"CHASSIS_task",          //��������
                (uint16_t)CHASSIS_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)CHASSIS_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&CHASSISTask_Handler); //������

    xTaskCreate((TaskFunction_t)ActionControl_task,          //������
                (const char *)"ActionControl_task",          //��������
                (uint16_t)CONTROL_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)CONTROL_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&CONTROLTask_Handler); //������
    xTaskCreate((TaskFunction_t)CPU_task,          //������
                (const char *)"CPU_task",          //��������
                (uint16_t)CPU_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)CPU_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&CPUTask_Handler); //������

    xTaskCreate((TaskFunction_t)TX2_task,          //������
                (const char *)"TX2_task",          //��������
                (uint16_t)TX2_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)TX2_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&TX2Task_Handler); //������

    xTaskCreate((TaskFunction_t)Offline_Check_task,          //������
                (const char *)"Offline_Check_task",          //��������
                (uint16_t)OFFLINE_CHECK_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)OFFLINE_CHECK_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&OfflineCheckTask_Handler); //������

		 xTaskCreate((TaskFunction_t)LED_Task,          //������
                (const char *)"LED_Task",          //��������
                (uint16_t)LED_TASK_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)LED_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&LEDTask_Handler); //������
								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


/**********************************************************************************************************
*�� �� ��: CPU_task
*����˵��: CPU�����ʺ��������忴FreeRTOS �鼮CPU������ͳ�Ʋ��֣�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//��Ҫ������Ϊ�˵�����Ҫ�Լ����ؿͻ�������

void CPU_task(void *pvParameters)
{

    uint8_t CPU_RunInfo1[200]; //������������ʱ����Ϣ �ֱ��ǣ������� ����״̬ ���ȼ� ʣ��ջ �������
    uint8_t CPU_RunInfo2[200]; //������������ʱ����Ϣ �ֱ��ǣ������� ���м���  ʹ����

    while (1) {
//        memset(CPU_RunInfo1,0,200); //��Ϣ����������

//        vTaskList((char *)&CPU_RunInfo1); //��ȡ��������ʱ����Ϣ

//        memset(CPU_RunInfo2,0,200); //��Ϣ����������

//        vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
//        Juge_SendData();
//        ClientDrawSend();
        vTaskDelay(100); /* ��ʱ 100 �� tick */

#if INCLUDE_uxTaskGetStackHighWaterMark
        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
/**********************************************************************************************************
*�� �� ��: startTast
*����˵��: ��������������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}

