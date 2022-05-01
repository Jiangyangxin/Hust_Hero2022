#ifndef __PID_H__
#define __PID_H__
#include "main.h"
typedef enum
{
	Icr  =0x01,       
	Pos  =0x00
}Pid_Mode_e;

typedef struct  
{
		float SetPoint;			//�趨Ŀ��ֵ
	  float ActPoint;      //ʵ��ֵ
		
		float P;						//��������
		float I;						//���ֳ���
		float D;						//΢�ֳ���
		
		float LastError;		//ǰ�����
		float PreError;			//��ǰ���
		float SumError;			//�������
		float dError;
	
		float IMax;					//��������
		
		float POut;					//�������
		float IOut;					//�������
		float DOut;					//΢�����
	
	   

    Pid_Mode_e    Mode;   //PID ģʽ
	
	
}Pid_Typedef;

typedef struct
{
	
		float SetPoint;			//�趨Ŀ��ֵ
	  float ActPoint;      //ʵ��ֵ
		float LastError;		//ǰ�����
		float PreError;			//��ǰ���
		float SumError;			//�������
		float dError;
	
		float IMax;					//��������
		
		float POut;					//�������
		float IOut;					//�������
		float DOut;					//΢�����
	
	float Kp;
	float Ki;
	float Kd;
}Fuzzle_Typedef;

float PID_Calc(Pid_Typedef * P);
float FuzzyPID_Calc(Fuzzle_Typedef *pid);
void fuzzy(Fuzzle_Typedef *pid,float e,float ec);

#endif
