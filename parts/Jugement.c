//#include "main.h"

///**********************************全局变量*************************************/
//Jugement_t Juge;
//CustomData_t CustomData;
//ext_client_draw_t	ClientDrawData;

///**********************************引用外部全局变量*****************************/
//extern short Juge_DisConnect;
//extern unsigned char  usart5_tx_buff[70];
//extern Chassis_typedef F105;
//extern Shoot_typedef BigShoot;
//extern unsigned char superCapAutoFlag;
//extern unsigned char BigFrictFlag;
//extern unsigned char Gyro_Flag;
//extern unsigned char Gimbal_Flag;
//extern unsigned char Juge_Flag;
//extern unsigned char RC_Flag;

///**********************************内部静态变量**********************************/

////测试数据
//float Test_Bullet42Spd_Last=0;
//short Test_Update_Bullet42=0;
//float Log_Bullet42Spd[50]= {0};

//float Test_Bullet17Spd_Last=0;
//short Test_Update_Bullet17=0;
//float Log_Bullet17Spd[50]= {0};
////
//unsigned char SaveBuffer[60];
//uint16_t DataLen;
//uint16_t cmd_id;
//short test_cntt=0;

///**
//  * @brief  裁判系统数据接收
//  * @param
//  * @retval  id号高位在后，低位在后
//  */
//void JugementRecv(unsigned char JugeBuff[])
//{
//    short PackPoint;
//    memcpy(&SaveBuffer[30],&JugeBuff[0],30);
//    for(PackPoint=0; PackPoint<30; PackPoint++) //最大数组30
//    {
//        if(SaveBuffer[PackPoint]==0xA5)//帧头
//        {
//            if((Verify_CRC8_Check_Sum(&SaveBuffer[PackPoint],5)==1))
//            {   //帧头的第5位置
//                cmd_id=(SaveBuffer[PackPoint+6])&0xff;//命令码2字节
//                cmd_id=(cmd_id<<8)|SaveBuffer[PackPoint+5];
////
//                DataLen=SaveBuffer[PackPoint+2]&0xff;//帧头第2位置,2字节
//                DataLen=(DataLen<<8)|SaveBuffer[PackPoint+1];

//                if((Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))
//                {   //第5位置校验和成功
//                    Juge_DisConnect = 0;//在线
//                    switch(cmd_id)
//                    {   //加7是比赛数据
//                    case 0x0001://比赛状态  0x0f    0000 1111
//                        Juge.GameSt.Progress=SaveBuffer[PackPoint+7+0]&0x0f;//取低四位
//                        Juge.GameSt.Type    =(SaveBuffer[PackPoint+7+0]>>4)&0x0f;//取高四位
//                        Juge.GameSt.Remain_Time=(SaveBuffer[PackPoint+7+1]<<8)|(SaveBuffer[PackPoint+7+2]);//2字节
//                        break;
//                    case 0x0002://比赛结果
//                        Juge.Winner=SaveBuffer[PackPoint+7+0];
//                        break;
//                    case 0x0003://机器人血量数据
//                        Juge.HeroSt.Remain_HP = SaveBuffer[PackPoint+7+0];
//                        break;
//                    case 0x0101://场地事件
//                        Juge.Area.Apron=SaveBuffer[PackPoint+7+0]&0x03;//取2位
//                        Juge.Area.Supply_No1=(SaveBuffer[PackPoint+7+0]>>2)&0x01;//补给站1号
//                        Juge.Area.Supply_No2=(SaveBuffer[PackPoint+7+0]>>3)&0x01;
//                        Juge.Area.Supply_No3=(SaveBuffer[PackPoint+7+0]>>4)&0x01;
//                        Juge.Area.Large_energy=(SaveBuffer[PackPoint+7+0]>>5)&0x07;//3位能量机关
//                        Juge.Area.Juncture=(SaveBuffer[PackPoint+7+0]>>8)&0x01;//关口
//                        Juge.Area.Bunker=(SaveBuffer[PackPoint+7+0]>>9)&0x01;//碉堡
//                        Juge.Area.Resource_Island=(SaveBuffer[PackPoint+7+0]>>10)&0x01;//资源岛
//                        Juge.Area.Base_Defense=(SaveBuffer[PackPoint+7+0]>>11)&0x03;//护盾状态
//                        break;
//                    case 0x0102://补给站动作
//                        Juge.Supply_Station.Id=SaveBuffer[PackPoint+7+0];
//                        Juge.Supply_Station.Robot_Id=SaveBuffer[PackPoint+7+1];
//                        Juge.Supply_Station.Outlet_St=SaveBuffer[PackPoint+7+2];
//                        Juge.Supply_Station.Bullet_Num=SaveBuffer[PackPoint+7+3];
//                        break;

//                    case 0x0104://警告数据
//                        Juge.HeroSt.Robot_level=SaveBuffer[PackPoint+7+0];//警告等级
//                        Juge.HeroSt.Foul_robot_id=SaveBuffer[PackPoint+7+1];
//                        break;
//                    case 0x201://机器人状态
//                        Juge.HeroSt.Id=SaveBuffer[PackPoint+7+0];
//                        Juge.HeroSt.Level=SaveBuffer[PackPoint+7+1];
//                        Juge.HeroSt.Remain_HP=(SaveBuffer[PackPoint+7+3]<<8)|SaveBuffer[PackPoint+7+2];
//                        Juge.HeroSt.Max_HP=(SaveBuffer[PackPoint+7+5]<<8)|SaveBuffer[PackPoint+7+4];
//                        Juge.HeroSt.Cool_Rate17=(SaveBuffer[PackPoint+7+7]<<8)|SaveBuffer[PackPoint+7+6];
//                        Juge.HeroSt.Heat_Limit17=(SaveBuffer[PackPoint+7+9]<<8)|SaveBuffer[PackPoint+7+8];

//                        Juge.HeroSt.Cool_Rate42=(SaveBuffer[PackPoint+7+11]<<8)|SaveBuffer[PackPoint+7+10];
//                        Juge.HeroSt.Heat_Limit42=(SaveBuffer[PackPoint+7+13]<<8)|SaveBuffer[PackPoint+7+12];
//                        break;

//                    case 0x202://功率和枪管能量
//                        //Juge.Power_Heat.Chassis_Volt=(SaveBuffer[PackPoint+7+1]<<8)|SaveBuffer[PackPoint+7+0];
//                        //Juge.Power_Heat.Chassis_Current=(SaveBuffer[PackPoint+7+2]<<8)|SaveBuffer[PackPoint+7+3];

//                        memcpy(&Juge.Power_Heat.Chassis_Volt,&SaveBuffer[PackPoint+7+0],2);
//                        memcpy(&Juge.Power_Heat.Chassis_Current,&SaveBuffer[PackPoint+7+2],2);
//                        memcpy(&Juge.Power_Heat.Chassis_Power,&SaveBuffer[PackPoint+7+4],4);
//                        memcpy(&Juge.Power_Heat.Chassis_Remain_Power,&SaveBuffer[PackPoint+7+8],2);
//                        memcpy(&Juge.Power_Heat.Shoot_Heat17,&SaveBuffer[PackPoint+7+10],2);
//                        memcpy(&Juge.Power_Heat.Shoot_Heat42,&SaveBuffer[PackPoint+7+12],2);

//                        Juge.Shoot17_Remain_Heat = Juge.HeroSt.Heat_Limit17 - Juge.Power_Heat.Shoot_Heat17;
//                        Juge.Shoot42_Remain_Heat = Juge.HeroSt.Heat_Limit42 - Juge.Power_Heat.Shoot_Heat42;
//                        Juge.Power_Heat.update=1;
//                        //Juge.Power_Heat.Chassis_Power_Buffer=(SaveBuffer[PackPoint+7+8]<<8)|SaveBuffer[PackPoint+7+9];
//                        //Juge.Power_Heat.Shoot_Heat17=(SaveBuffer[PackPoint+7+10]<<8)|SaveBuffer[PackPoint+7+11];
//                        //Juge.Power_Heat.Shoot_Heat42=(SaveBuffer[PackPoint+7+12]<<8)|SaveBuffer[PackPoint+7+13];
//                        break;

//                    case 0x203://机器人位置
//                        memcpy(&Juge.Locate.X,&SaveBuffer[PackPoint+7+0],4);
//                        memcpy(&Juge.Locate.Y,&SaveBuffer[PackPoint+7+4],4);
//                        memcpy(&Juge.Locate.Z,&SaveBuffer[PackPoint+7+8],4);
//                        memcpy(&Juge.Locate.Yaw,&SaveBuffer[PackPoint+7+12],4);
//                        break;

//                    case 0x206://机器人受伤
//                        Juge.Hurt.Armor_Id=SaveBuffer[PackPoint+7+0]&0x0f;
//                        Juge.Hurt.Type=(SaveBuffer[PackPoint+7+0]>>4)&0x0f;
//                        break;

//                    case 0x207://子弹射速射频
//                        Juge.Shoot.Bullet_Type=SaveBuffer[PackPoint+7+0];
//                        if(Juge.Shoot.Bullet_Type==1)//17mm
//                        {
//                            Juge.Shoot.update17=1;
//                            Juge.Shoot.Bullet17_Freq= SaveBuffer[PackPoint+7+1];
//                            memcpy(&Juge.Shoot.Bullet17_Speed,&SaveBuffer[PackPoint+7+2],4);

//                            /******记录17mm弹丸速度值***************/
//                            if(Test_Bullet17Spd_Last!=Juge.Shoot.Bullet17_Speed)
//                            {
//                                Test_Update_Bullet17++;
//                                Log_Bullet17Spd[Test_Update_Bullet17]=Juge.Shoot.Bullet17_Speed;

//                                if(Test_Update_Bullet17>50)
//                                {
//                                    Test_Update_Bullet17=0;
//                                }
//                            }
//                            Test_Bullet42Spd_Last=Juge.Shoot.Bullet42_Speed;
//                        }
//                        else if(Juge.Shoot.Bullet_Type==2)
//                        {
//                            Juge.Shoot.update42 = 1;
//                            Juge.Shoot.Bullet42_Freq= SaveBuffer[PackPoint+7+1];
//                            memcpy(&Juge.Shoot.Bullet42_Speed,&SaveBuffer[PackPoint+7+2],4);
//                            /************记录42mm弹丸速度值*********/
//                            if(Test_Bullet42Spd_Last!=Juge.Shoot.Bullet42_Speed)
//                            {
//                                Test_Update_Bullet42++;
//                                Log_Bullet42Spd[Test_Update_Bullet42]=Juge.Shoot.Bullet42_Speed;

//                                if(Test_Update_Bullet42>50)
//                                {
//                                    Test_Update_Bullet42=0;
//                                }

//                            }
//                            Test_Bullet42Spd_Last=Juge.Shoot.Bullet42_Speed;
//                        }
//                        break;
//                    }
//                }
//            }
//        }

//    }
//    memcpy(&SaveBuffer[0],&SaveBuffer[30],30);
//}

///**
//  * @brief  客户端数据发送
//  * @param
//  * @retval
//  */
//void Juge_SendData(void)
//{
//    FloatlongType  UserMes;

//    CustomData.Data[0] = (float)F105.CapReaimPower;
//    CustomData.Data[1] = (float)BigShoot.RemainNum;
//    CustomData.Data[2] = (float)Juge.Shoot42_Remain_Heat;
//    CustomData.Mask = (superCapAutoFlag&0x01) | ((BigFrictFlag<<1)&0x02) | ((RC_Flag<<2)&0x04) | ((Juge_Flag<<3)&0x08) | 0x10 | 0x20;

//    DMA_Cmd(DMA1_Stream7,DISABLE);

//    unsigned char SOF = 0xA5;
//    usart5_tx_buff[0] =  SOF;
//    unsigned short DataLength = 19;
//    usart5_tx_buff[1] = (unsigned char)(DataLength&0xff);
//    usart5_tx_buff[2] = (unsigned char)((DataLength >> 8)&0xff);
//    static unsigned char Seq = 0;
//    usart5_tx_buff[3] =  Seq;
//    Append_CRC8_Check_Sum(usart5_tx_buff, 5);

//    unsigned short Cmd_id = 0x0301;
//    unsigned short DataId = 0xD180;
//    unsigned short SendId = Juge.HeroSt.Id;              //发送者ID（机器人id）
//    unsigned short ReceiveId;
//    if(Juge.HeroSt.Id==1)//0x0101 红1 0x0111 蓝11
//        ReceiveId= 0x0101;
//    else if(Juge.HeroSt.Id==11)
//        ReceiveId=0x0111;


//    usart5_tx_buff[5] = (unsigned char)(Cmd_id&0xff);
//    usart5_tx_buff[6] = (unsigned char)((Cmd_id >> 8)&0xff);

//    usart5_tx_buff[7] = (unsigned char)((DataId&0xff)&0xff);
//    usart5_tx_buff[8] = (unsigned char)((DataId >> 8)&0xff);

//    usart5_tx_buff[9] = (unsigned char)((SendId&0xff)&0xff);
//    usart5_tx_buff[10] = (unsigned char)((SendId >> 8)&0xff);

//    usart5_tx_buff[11] = (unsigned char)((ReceiveId&0xff)&0xff);
//    usart5_tx_buff[12] = (unsigned char)((ReceiveId >> 8)&0xff);

//    UserMes.fdata=CustomData.Data[0];								//cap剩余能量
//    usart5_tx_buff[16] = (UserMes.idata>>24)&0xff;
//    usart5_tx_buff[15] = (UserMes.idata>>16)&0xff;
//    usart5_tx_buff[14] = (UserMes.idata>>8)&0xff;
//    usart5_tx_buff[13] = UserMes.idata&0xff;

//    UserMes.fdata=CustomData.Data[1];								//枪管剩余子弹
//    usart5_tx_buff[20] = (UserMes.idata>>24)&0xff;
//    usart5_tx_buff[19] = (UserMes.idata>>16)&0xff;
//    usart5_tx_buff[18] = (UserMes.idata>>8)&0xff;
//    usart5_tx_buff[17] = UserMes.idata&0xff;

//    UserMes.fdata=CustomData.Data[2];								//剩余热量
//    usart5_tx_buff[24] = (UserMes.idata>>24)&0xff;
//    usart5_tx_buff[23] = (UserMes.idata>>16)&0xff;
//    usart5_tx_buff[22] = (UserMes.idata>>8)&0xff;
//    usart5_tx_buff[21] = UserMes.idata&0xff;

//    /*
//    	mask.1		大子弹数目
//    	mask.2
//    */
//    usart5_tx_buff[25] = CustomData.Mask;
//    Append_CRC16_Check_Sum(usart5_tx_buff, 28);

//    DMA_Cmd(DMA1_Stream7,ENABLE);
//}

///**
//  * @brief  客户端绘图
//  * @param
//  * @retval
//  */
//void ClientDrawSend(void)
//{
//    DMA_Cmd(DMA1_Stream7,DISABLE);

//    unsigned char SOF = 0xA5;
//    usart5_tx_buff[0] =  SOF;
//    unsigned short DataLength = 61;
//    usart5_tx_buff[1] = (unsigned char)(DataLength&0xff);
//    usart5_tx_buff[2] = (unsigned char)((DataLength >> 8)&0xff);
//    static unsigned char Seq = 0;
//    usart5_tx_buff[3] =  Seq;
//    Append_CRC8_Check_Sum(usart5_tx_buff, 5);

//    unsigned short Cmd_id = 0x0301;
//    unsigned short DataId = 0x0100;
//    unsigned short SendId = Juge.HeroSt.Id;              //发送者ID（机器人id）
//    unsigned short ReceiveId;
//    if(Juge.HeroSt.Id==1)//0x0101 红1 0x0111 蓝11
//        ReceiveId= 0x0101;
//    else if(Juge.HeroSt.Id==11)
//        ReceiveId=0x0111;

//    usart5_tx_buff[5] = (unsigned char)(Cmd_id&0xff);
//    usart5_tx_buff[6] = (unsigned char)((Cmd_id >> 8)&0xff);

//    usart5_tx_buff[7] = (unsigned char)((DataId&0xff)&0xff);
//    usart5_tx_buff[8] = (unsigned char)((DataId >> 8)&0xff);

//    usart5_tx_buff[9] = (unsigned char)((SendId&0xff)&0xff);
//    usart5_tx_buff[10] = (unsigned char)((SendId >> 8)&0xff);

//    usart5_tx_buff[11] = (unsigned char)((ReceiveId&0xff)&0xff);
//    usart5_tx_buff[12] = (unsigned char)((ReceiveId >> 8)&0xff);

//    /***************客户端UI操作***********************/
//    usart5_tx_buff[7+6] = ClientDrawData.operate_tpye;
//    usart5_tx_buff[7+7] = ClientDrawData.graphic_tpye;

//    usart5_tx_buff[7+8] = ClientDrawData.graphic_name[0];
//    usart5_tx_buff[7+9] = ClientDrawData.graphic_name[1];
//    usart5_tx_buff[7+10] = ClientDrawData.graphic_name[2];
//    usart5_tx_buff[7+11] = ClientDrawData.graphic_name[3];
//    usart5_tx_buff[7+12] = ClientDrawData.graphic_name[4];

//    usart5_tx_buff[7+13] = ClientDrawData.layer;
//    usart5_tx_buff[7+14] = ClientDrawData.color;
//    usart5_tx_buff[7+15] = ClientDrawData.width;

//    usart5_tx_buff[7+16] = ClientDrawData.start_x & 0xff;
//    usart5_tx_buff[7+17] = (ClientDrawData.start_x>>8) & 0xff;

//    usart5_tx_buff[7+18] = ClientDrawData.start_y & 0xff;
//    usart5_tx_buff[7+19] = (ClientDrawData.start_y>>8)&0xff;

//    usart5_tx_buff[7+20] = ClientDrawData.radius & 0xff;
//    usart5_tx_buff[7+21] = (ClientDrawData.radius>>8) & 0xff;

//    usart5_tx_buff[7+22] = ClientDrawData.end_x & 0xff;
//    usart5_tx_buff[7+23] = (ClientDrawData.end_x>>8)&0xff;

//    usart5_tx_buff[7+24] = ClientDrawData.end_y & 0xff;
//    usart5_tx_buff[7+25] = (ClientDrawData.end_y>>8)&0xff;

//    usart5_tx_buff[7+26] = ClientDrawData.start_angle&0xff;
//    usart5_tx_buff[7+27] = (ClientDrawData.start_angle>>8)&0xff;

//    usart5_tx_buff[7+28] = ClientDrawData.end_angle&0xff;
//    usart5_tx_buff[7+29] = (ClientDrawData.end_angle>>8)&0xff;

//    usart5_tx_buff[7+30] = ClientDrawData.text_lenght;

//    Append_CRC16_Check_Sum(usart5_tx_buff, 70);

//    DMA_Cmd(DMA1_Stream7,ENABLE);
//}
