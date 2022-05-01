#include "main.h"
#define PI 3.1415926f
/**
  * @brief	调试程序
  * @param
  * @retval
  */
extern Key_typedef key_status;
extern RC_Ctl_t RC_Ctl;
extern Shoot_typedef BigShoot;
extern Jugement_t Juge;
extern HeroStatus_typedef	HeroStatus;
extern Pid_Typedef PidSmlFrictSpeed[2],PidBigFrictSpeed[2],PidBigPullerPos;
extern short SmlFrictSpeed;
extern Pid_Typedef	PidBigPushPos,PidBigPushSpeed;
extern M2006Receive_Typedef		SmlPuller,BigPushRecv;
extern short BigFrictSpeed;
extern M3508Receive_Typedef		BigFrict[2],SmlFrict[2];
extern ext_client_draw_t	ClientDrawData;
extern PC_Recv_t PC_Recv;


short cnt;
short c = 0;
short DebugInitFlag = 0;
short debug_preRC_s1;
float watch_frict_l,watch_frict_r,watch_diff;
short debugFlag = 0;
float debug_bigPushPos = 0;
short debug_bigShootFlag = 0;
short debug_Priod = 1000;
short debug_cnt = 500;
int 	debug_increasePos = 8191*24;
short debug_setSpeed = 0;
short speed = 5000;
float debug_pitch;
int 	pitch_out;
short debug_pitch_angle;

float yaw_angle;
float pitch_angle;
float pc_yaw;
float pc_pitch;
float now_yaw;
float now_pitch;
void DebugTask(void)
{
    pc_yaw = PC_Recv.RCYaw;
    pc_pitch = PC_Recv.RCPitch;
    yaw_angle = (float)(tan((pc_yaw - now_yaw) / 180.0f * PI) / tan(61.5f / 180.0f * PI));
    pitch_angle =(float)(tan((pc_pitch - now_pitch) / 180.0f * PI) / tan(47.0f / 180.0f * PI));
    if(DebugInitFlag != 1)
    {
        ClientDrawData.operate_tpye = 2;
        ClientDrawData.graphic_tpye = 3;
        ClientDrawData.layer = 1;

        ClientDrawData.color = 0;
        ClientDrawData.width = 5;
        ClientDrawData.start_x = 960 - 960 * yaw_angle;
        ClientDrawData.start_y = 540 - 540 * pitch_angle;
        ClientDrawData.radius = 40;

        ClientDrawData.end_x = 0;
        ClientDrawData.end_y = 0;
        DebugInitFlag = 1;
    }


//	pitch_out = ZeroCheck_PitchOutPut();
//	debug_pitch = (((pitch_out - GIM_PIT_INIT_SCALE)%8192)/8192.0)*360;
//	debug_pitch_angle = (short)debug_pitch;
//发射机构测试
//	ZeroCheck_cal();
//	if(DebugInitFlag != 1)
//	{
//		PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();
//		PidBigPushPos.SetPoint = ZeroCheck_BigPushPosOutPut();
//		DebugInitFlag = 1;
//	}
//	HeroStatus.ShootMode = Shoot_Act_Mode;
//	PidBigFrictSpeed[0].SetPoint = debug_setSpeed;
//	PidBigFrictSpeed[1].SetPoint = -debug_setSpeed;
//	if(debugFlag == 1)
//	{
//		PidBigPullerPos.SetPoint += 8192;
//		PidBigPushPos.SetPoint += debug_increasePos;
//		debugFlag = 0;
//	}
//	ShootCur_Pid_Cal();
//	watch_frict_l = -BigFrict[0].RealSpeed;
//	watch_frict_r = BigFrict[1].RealSpeed;
//	watch_diff = watch_frict_l - watch_frict_r;


//摩擦轮供弹发射机构测试
//	ZeroCheck_cal();
//	HeroStatus.ShootMode = Shoot_Act_Mode;
//	if(debugFlag != 1)
//	{
//		debugFlag = 1;
//		PidBigPushPos.SetPoint = ZeroCheck_BigPushPosOutPut();
//	}
//
//	if(debug_bigShootFlag == 1)
//	{
//		PidBigPushPos.SetPoint += debug_bigPushPos;
//		debug_bigShootFlag = 0;
//	}
//	ShootCur_Pid_Cal();
//	if(RC_Ctl.rc.s2 == 2)
//		BigPushCur = 0;
//	BigPushCur = (short)LIMIT_MAX_MIN(BigPushCur,10000,-10000);
//	CAN2_BigPushSend(BigPushCur);

//	debug_flag = BulletFullFlag();
//	debug_preRC_s1 = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
}

/*
7.4:
大发射结构：
P：25
speed：5400
*/
