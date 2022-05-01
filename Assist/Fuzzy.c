/**
  ******************************************************************************
  * @file     
  * @author  
  * @version
  * @date    
  * @brief    
  *
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "main.h"

#define NL   -3
#define NM	 -2
#define NS	 -1
#define ZE	 0
#define PS	 1
#define PM	 2
#define PL	 3

float FzExtend[2]={-3,3};
//RULER 

typedef struct 
{
	float MemberShip[7];
	float QuatiVal;
	float RealExtend[2];
	float QuatiExtend[2];
}FuzzyVal_t;
/*****************************************************/
static const float fuzzyRuleKp[7][7]={
	PL,	PL,	PM,	PM,	PS,	PS,	ZE,
	PL,	PL,	PM,	PM,	PS,	ZE,	ZE,
	PM,	PM,	PM,	PS,	ZE,	NS,	NM,
	PM,	PS,	PS,	ZE,	NS,	NM,	NM,
	PS,	PS,	ZE,	NS,	NS,	NM,	NM,
	ZE,	ZE,	NS,	NM,	NM,	NM,	NL,
	ZE,	NS,	NS,	NM,	NM,	NL,	NL
};
static const float fuzzyRuleKi[7][7]={
	NL,	NL,	NL,	NM,	NM,	ZE,	ZE,
	NL,	NL,	NM,	NM,	NS,	ZE,	ZE,
	NM,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	ZE,	PS,	PS,	PM,
	NS,	NS,	ZE,	PS,	PS,	PM,	PM,
	ZE,	ZE,	PS,	PM,	PM,	PL,	PL,
	ZE,	ZE,	PS,	PM,	PL,	PL,	PL
};
static const float fuzzyRuleKd[7][7]={
	PS,	PS,	ZE,	ZE,	ZE,	PL,	PL,
	NS,	NS,	NS,	NS,	ZE,	NS,	PM,
	NL,	NL,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	NS,	ZE,	PS,	PS,
	PS,	ZE,	ZE,	ZE,	ZE,	PL,	PL
};

FuzzyVal_t FzErr,FzDerr;
/**
  * @brief  �Է�ΧΪVal_Extend��val��������������������ΧQuati_Extend
  * @param   
  * @retval  
  */
static float Linear_Quantization(float value,float Val_Extend[],float Quati_Extend[])
{

}
/**
  * @brief  �Է�ΧΪQuati_Extend����������QuatiVal����ȥ��������ΧVal_Extend
  * @param   
  * @retval  
  */
static float Linear_Inverse_Quantization(float QuatiVal,float Val_Extend[],float Quati_Extend[])
{
	
}
/**
  * @brief  ����ģ������ �������� �е� ������
  * @param   
  * @retval  
  */
void CalcMembership(FuzzyVal_t *FuzzyVal)
{
	
}
/**
  * @brief  �����Ƶ� �Լ� ���ķ� ȥģ����
  * @param   
  * @retval  
  */
float Defuzzification(FuzzyVal_t FzErr,FuzzyVal_t FzDerr,const float RULER[7][7])
{
	short i=0,j=0;
	float Defz_Val=0,SumMembership=0,Temp=0;
	
	for(i=0;i<7;i++)//
	{
     for(j=0;j<7;j++)
     {
			  SumMembership+=FzErr.MemberShip[i]*FzDerr.MemberShip[j];
			  Temp+=FzErr.MemberShip[i]*FzDerr.MemberShip[j]*RULER[i][j];
     }	
	}
	Defz_Val=Temp/SumMembership;//
	return Defz_Val;
}

float Extend_DeltaKp[2]={0};
float FuzzyPID(Pid_Typedef *PID)
{	
	 float deltaKp,deltaKi,deltaKd;
	 float QdeltaKp,QdeltaKi,QdeltaKd;	
/**/	
		PID->PreError = PID->SetPoint-PID->ActPoint;     //
		PID->dError = PID->PreError - PID->LastError;
	
		PID->SumError += PID->PreError;
		PID->LastError = PID->PreError;
		
		if(PID->SumError >= PID->IMax)
			PID->SumError = PID->IMax;
		else if(PID->SumError <= -PID->IMax)
			PID->SumError = -PID->IMax;	
/*��������*/	
	 FzErr.QuatiVal=Linear_Quantization(PID->dError,FzErr.RealExtend,FzErr.QuatiExtend);
	 FzDerr.QuatiVal= Linear_Quantization(PID->PreError,FzDerr.RealExtend,FzDerr.QuatiExtend);
/*����������*/		
		CalcMembership(&FzErr);
		CalcMembership(&FzDerr);
/*�����Ƶ� �Լ� ���ķ�ȥģ����*/	
    QdeltaKp=Defuzzification(FzErr,FzDerr,fuzzyRuleKp);
//		QdeltaKi=Defuzzification(FzErr,FzDerr,fuzzyRuleKi);
//		QdeltaKd=Defuzzification(FzErr,FzDerr,fuzzyRuleKd);
/*����ȥ����*/		
    deltaKp=Linear_Inverse_Quantization(QdeltaKp,Extend_DeltaKp,FzExtend);		
		
		PID->POut=(PID->P+deltaKp)*PID->PreError;
		PID->IOut=(PID->I+deltaKi)*PID->SumError;
		PID->DOut=(PID->D+deltaKd)*PID->dError;
		
		return PID->POut+PID->IOut+PID->DOut;
}

