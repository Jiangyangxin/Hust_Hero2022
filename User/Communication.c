
#include "Communication.h"
/*��Ҫ��ȫ�ֱ�־��ͨ�ţ���24����־λ����
��Ϊ�����32λ�洢������Ҫ�ӵ͵�ַ��ʼ����
ͨ��xxEventGroupSetBits ���ñ�־λ
ͨ��xEventGroupWaitBits �����־λ

λ0 ���̵��� λ1 ң�������� λ2 ��̨���� λ3 ����ϵͳ����
λ4 ������ģʽ��ʼ��־λ  λ5 ����ģʽ��־λ
λ6 ���������Զ�����ģʽ���� 
λ7 Ħ����ת����־λ λ8 �����ǿ���
λ9 ��̨���� λ10 ����ϵͳ����
λ11 ң��������
*/
EventGroupHandle_t global_flag = NULL;
QueueHandle_t      global_array = NULL;
void communicate_init(void) {
	global_array = xQueueCreate(4 , 4);//4���ֽڣ�4����Ԫ
    global_flag = xEventGroupCreate();
    xEventGroupSetBits(global_flag,1 << 0);//ֻ��Ϊ1��ֵ�ŶԱ�־����Ӱ��
    xEventGroupSetBits(global_flag,1 << 1);
    xEventGroupSetBits(global_flag,1 << 2);
    xEventGroupSetBits(global_flag,1 << 3);
    xEventGroupSetBits(global_flag,1 << 4);
    xEventGroupSetBits(global_flag,1 << 5);
    xEventGroupSetBits(global_flag,1 << 6);
    xEventGroupSetBits(global_flag,1 << 7);
    xEventGroupSetBits(global_flag,1 << 8);
	xEventGroupWaitBits(global_flag, 1 << 0 ,pdTRUE,pdTRUE,10);//�ȴ�10��tick,ֻ��Ҫ�޸��������ָ��
	

}

