#include "Trajector_Equ.h"
Trajector hero_traj; //弹道方程
void Trajector_Equ(float distance,float dandao_speed,float dandao_high) //这个参数可能有点问题
{
    hero_traj.dandao_flag=1;
    hero_traj.dandao_speed = 19.1f;
    hero_traj.distance = 5.0f;
    hero_traj.dandao_high = -0.5f;//1.4d 1.3d 1.2d 1.1d 1.0d(与实际相差0.1m)
    hero_traj.Init_GimbalPitchPos = atan(hero_traj.dandao_high/hero_traj.distance);
    for(int num=0; num<5; num++)
    {
        hero_traj.Fly_time = hero_traj.distance/(hero_traj.dandao_speed*cos(hero_traj.Init_GimbalPitchPos));
        hero_traj.dandao_estimate_high = hero_traj.dandao_speed*sin(hero_traj.Init_GimbalPitchPos)*hero_traj.Fly_time-0.5*9.8*hero_traj.Fly_time*hero_traj.Fly_time;
        hero_traj.delta_theta = (hero_traj.dandao_estimate_high - hero_traj.dandao_high)*0.1f;
        hero_traj.Init_GimbalPitchPos = hero_traj.Init_GimbalPitchPos - hero_traj.delta_theta;
    }
    hero_traj.Add_GimbalPitchPos = hero_traj.Init_GimbalPitchPos*180/3.14f;
    hero_traj.Add_GimbalPitchPos = - hero_traj.Add_GimbalPitchPos;
    hero_traj.dandao_flag = 1;
	
}
