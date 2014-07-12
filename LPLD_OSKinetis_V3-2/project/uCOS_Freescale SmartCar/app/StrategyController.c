/*
 * =====================================================================================
 *
 *       Filename:  StrategyController.c
 *
 *    Description:  Strategycontroller. Main Controller of SmartCar.
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时33分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */
#define  DEBUG_DEADAREA 0 
#define  DEBUG_NORMAL 1
#include "common.h"
#include "OSMessage.h"
#include "StrategyController.h"
#include "Angle_Controller.h"
#include "Speed_Controller.h"
#include "DEV_Motor.h"
#include "USART_Middle_Module.h"
#include "Key_Middle_Module.h"
#include "DEV_SSD1306.h"
#include "DEV_QuadDecode.h"
INT8U   MotorStat = MOTOR_OFF;
void KEY_A_Down(void);
void KEY_B_Down(void);
//  float32_t Angle_Control_PID_Result;
//  float32_t Speed_Control_PID_Result;
//  float32_t Angle_Offset=0;
float32_t DeadArea_Left_P = 0.0;
float32_t DeadArea_Left_N = 0.0;
float32_t DeadArea_Right_P = 0.0;
float32_t DeadArea_Right_N = 0.0;
static INT16S *p_Speed;
static int cnt=0;
void Strategy_Control_Task (void  *p_arg)// Strategycontroller Routine Task. Called By OS.
{
    p_Speed = Get_Speed();
    Key_CB_Invoke(KEY_A,KEY_A_Down);
    Key_CB_Invoke(KEY_B,KEY_B_Down);
    USART_Param_Invoke_float32(STAT_VALUE_ACCER,&Angle_Control_PID_Result);
    OSTimeDlyHMSM(0,0,5,0);
    _BEEP_ON();
    OSTimeDlyHMSM(0,0,0,100);
    _BEEP_OFF();
#if DEBUG_DEADAREA 
    USART_Param_Invoke_float32(STAT_SPEED_P,&DeadArea_Left_P);
    USART_Param_Invoke_float32(STAT_SPEED_I,&DeadArea_Left_N);
    USART_Param_Invoke_float32(STAT_SPEED_D,&DeadArea_Right_P);
    USART_Param_Invoke_float32(STAT_DIR_P,&DeadArea_Right_N);
#endif
    while(1)
    {
        OSTimeDlyHMSM(0,0,0,5);
        if(cnt++%100 == 0) printf("\n%d,%d,%d,%d\n",(INT32S)DeadArea_Left_P,(INT32S)DeadArea_Left_N,(INT32S)DeadArea_Right_P,(INT32S)DeadArea_Right_N);
        //Angle_Offset = -Speed_Control_PID_Result;
        
        if(MotorStat == MOTOR_ON )
        {
#if DEBUG_DEADAREA   //DEAD AREA DEBUG            
            if(DeadArea_Left_P > 0 && DeadArea_Left_N == 0)
            {
              Motor_Set((INT32S)DeadArea_Left_P,MOTOR_LEFT);
              Motor_Set(0,MOTOR_RIGHT);
            }
            if(DeadArea_Left_N > 0 && DeadArea_Left_P == 0)
            {
              Motor_Set((INT32S)-DeadArea_Left_N,MOTOR_LEFT);
              Motor_Set(0,MOTOR_RIGHT);
            } 
            if(DeadArea_Right_P > 0 && DeadArea_Right_N == 0)
            {
              Motor_Set((INT32S)DeadArea_Right_P,MOTOR_RIGHT);
              Motor_Set(0,MOTOR_LEFT);
            }
            if(DeadArea_Right_N > 0 && DeadArea_Right_P == 0)
            {
              Motor_Set((INT32S)-DeadArea_Right_N,MOTOR_RIGHT);
              Motor_Set(0,MOTOR_LEFT);
            }
#endif
#if DEBUG_NORMAL
            Motor_Set((INT32S)       //Cannot confirm + OR - !!!!!!!
                    (Angle_Control_PID_Result +Speed_Control_PID_Result)
                    ,MOTOR_LEFT);
            Motor_Set((INT32S)
                    (Angle_Control_PID_Result +Speed_Control_PID_Result)
                    ,MOTOR_RIGHT);
#endif 
        }
        else
        {
            Motor_Set(0,MOTOR_LEFT);
            Motor_Set(0,MOTOR_RIGHT);
        }
#if DEBUG_NORMAL

        if(*p_Speed > 1000)                  MotorStat = MOTOR_OFF;
        if(Angle_Control_PID_Result > 15000) MotorStat = MOTOR_OFF;

#endif
    }
}
static ControllerMsg_TypeDef Msg_Reset;
void KEY_A_Down(void)
{
  MotorStat = MOTOR_ON;
  LCD_P6x8Str(1,6,"  -MOTOR  ON -");
  Msg_Reset.Controller_Msg_Type = CONTROLLER_CHANGE;
  OSMboxPost(Angle_Controller_MBox,(void *)&Msg_Reset);
  OSMboxPost(Speed_Controller_MBox,(void *)&Msg_Reset);
  
#if DEBUG_DEADAREA
  DeadArea_Left_P = 0.0;
  DeadArea_Left_N = 0.0;
  DeadArea_Right_P = 0.0;
  DeadArea_Right_N = 0.0;
  Motor_Set(0,MOTOR_RIGHT);
  Motor_Set(0,MOTOR_LEFT);
#endif
}
void KEY_B_Down(void)
{
  MotorStat = MOTOR_OFF;
  LCD_P6x8Str(1,6,"  -MOTOR OFF -");
}
