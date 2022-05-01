#ifndef        _JUGEMENT_H
#define        _JUGEMENT_H

/*********************************头文件********************************************/
#include "main.h"

#define JudgeBufBiggestSize 30
/*********************************数据类型定义**************************************/
typedef struct
{
    struct
    {
        float X;
        float Y;
        float Z;
        float Yaw;
    } Locate;

    struct
    {
        uint8_t  Type : 4;
        uint8_t  Progress : 4;
        uint16_t Remain_Time;
    } GameSt; //这个是什么
    struct
    {
        uint8_t Id;
        uint8_t Level;
        uint8_t Robot_level;//警告等级
        uint8_t Foul_robot_id;//新增的,警告ID
        uint16_t Remain_HP;
        uint16_t Max_HP;
        uint16_t Cool_Rate17;
        uint16_t Heat_Limit17;
        uint16_t Cool_Rate42;
        uint16_t Heat_Limit42;
    } HeroSt;

    struct
    {
        uint16_t Chassis_Volt;
        uint16_t Chassis_Current;
        float    Chassis_Power;
        uint16_t Chassis_Remain_Power;
        uint16_t Shoot_Heat17;
        uint16_t Shoot_Heat42;

        short update;
    } Power_Heat;

    struct
    {
        uint8_t Blood_Up;
        uint8_t CoolRate_Up;
        uint8_t Defense_Up;
        uint8_t Attack_Up;
    } Buff;

    struct
    {
        uint8_t Armor_Id;
        uint8_t Type;
    } Hurt;

    struct
    {
        uint8_t Id;
        uint8_t Robot_Id;
        uint8_t Outlet_St;
        uint8_t Bullet_Num;
    } Supply_Station ;

    struct
    {
        uint8_t	Apron ;
        uint8_t  Supply_No1;
        uint8_t  Supply_No2;
        uint8_t Supply_No3;

        uint8_t Large_energy;
        uint8_t Juncture;

        uint8_t Bunker;
        uint8_t Resource_Island;

        uint8_t Base_Defense;
    } Area;

    struct
    {
        uint8_t Bullet_Type;
        uint8_t Bullet17_Freq;
        float Bullet17_Speed;

        uint8_t Bullet42_Freq;
        float Bullet42_Speed;

        short update17;
        short update42;

    } Shoot;

    uint8_t 	Winner;
    uint16_t  Shoot17_Remain_Heat;
    uint16_t	Shoot42_Remain_Heat;
} Jugement_t;

typedef struct
{
    float Data[3];
    uint8_t Mask;
} CustomData_t;		//客户端数据

typedef struct
{
    uint8_t operate_tpye;
    uint8_t graphic_tpye;
    uint8_t graphic_name[5];
    uint8_t layer;
    uint8_t color;
    uint8_t width;
    uint16_t start_x;
    uint16_t start_y;
    uint16_t radius;
    uint16_t end_x;
    uint16_t end_y;
    int16_t start_angle;
    int16_t end_angle;
    uint8_t text_lenght;
    uint8_t text[30];
} ext_client_draw_t;

void JugementRecv(unsigned char JugeBuff[]);
void Juge_SendData(void);
void ClientDrawSend(void);

#endif

