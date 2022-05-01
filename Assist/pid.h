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
		float SetPoint;			//设定目标值
	  float ActPoint;      //实际值
		
		float P;						//比例常数
		float I;						//积分常数
		float D;						//微分常数
		
		float LastError;		//前次误差
		float PreError;			//当前误差
		float SumError;			//积分误差
		float dError;
	
		float IMax;					//积分限制
		
		float POut;					//比例输出
		float IOut;					//积分输出
		float DOut;					//微分输出
	
	   

    Pid_Mode_e    Mode;   //PID 模式
	
	
}Pid_Typedef;

typedef struct
{
	
		float SetPoint;			//设定目标值
	  float ActPoint;      //实际值
		float LastError;		//前次误差
		float PreError;			//当前误差
		float SumError;			//积分误差
		float dError;
	
		float IMax;					//积分限制
		
		float POut;					//比例输出
		float IOut;					//积分输出
		float DOut;					//微分输出
	
	float Kp;
	float Ki;
	float Kd;
}Fuzzle_Typedef;

float PID_Calc(Pid_Typedef * P);
float FuzzyPID_Calc(Fuzzle_Typedef *pid);
void fuzzy(Fuzzle_Typedef *pid,float e,float ec);

#endif
