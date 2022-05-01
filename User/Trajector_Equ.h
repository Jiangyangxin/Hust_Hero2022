#ifndef TRAJECTOR_EQU_H
#define TRAJECTOR_EQU_H
#include "main.h"
void Trajector_Equ(float distance,float dandao_speed,float dandao_high);
typedef struct {
    vu8 dandao_flag;
    float dandao_speed;
    float distance;
    float Init_GimbalPitchPos;
    float dandao_high;
    float Fly_time;
    float dandao_estimate_high;
    float delta_theta;
    float Add_GimbalPitchPos;
} Trajector;


#endif
