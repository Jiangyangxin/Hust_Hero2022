#ifndef __USART5_H__
#define __USART5_H__
void TX2_task(void *pvParameters);
//void Tx2_Off_CheckAndSet(uint8_t* Buff);
void UART5_Configuration(void);
void USART5_SendtoPC(void);
#define GYRO_BUF_SIZE 18
#define PC_SENDBUF_SIZE 10
#define PC_RECVBUF_SIZE 9

#endif

