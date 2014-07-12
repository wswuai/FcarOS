/*
 * =====================================================================================
 *
 *       Filename:  Speed_Controller.c
 *
 *    Description:  Speed control module for smart car.
 *
 *        Version:  0.1
 *        Created:  2013/11/24 13时58分39秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio. 
 *
 * =====================================================================================
 */

#include "common.h"
#include "OSMessage.h"
#include "DEV_QuadDecode.h"
#include "PIDCalculator.h"
#include "Angle_Controller.h"
#include "DEV_QuadDecode.h"
#include "USART_Middle_Module.h"
//Global Variables:
OS_EVENT *Speed_Controller_MBox;

float32_t Speed_PID_P=SPEEDPID_PROPORTION;
float32_t Speed_PID_I=SPEEDPID_INTEGRAL;
float32_t Speed_PID_D=SPEEDPID_DERIVATIVE;
float32_t Speed_PID_S=SPEEDPID_CARSPEEDSET;
arm_pid_instance_f32 Speed_PID;
static INT16S *p_Speed;
extern OS_CPU_SR cpu_sr;
double SpeedControllerOut;
//Local Functions:
INT32U SpeedValueGet(void);
void Speed_Controller_Reset(void );
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Speed_Controller_Task
 *  Description:  Task Body of Speed Controller
                  Created by Starter Task.
 * =====================================================================================
 */
//Local Functions
void Speed_Controller_Init();
float32_t Speed_Control_PID_Result;
void Speed_Control_Task (void *p_arg  )
{
    //Private Variables of task.
    INT16S SpeedValue;
    INT8U err=0;
    ControllerMsg_TypeDef *Speed_Controller_IN_Msg;
    float32_t SpeedValueERR;
    (void)p_arg;
    Speed_Controller_Init();
    p_Speed = Get_Speed();

    for(;;)
    {
        Speed_Controller_IN_Msg = (ControllerMsg_TypeDef *)
            OSMboxPend(Speed_Controller_MBox,0,&err);
        if(err == OS_ERR_NONE)
        {
            if(Speed_Controller_IN_Msg->Controller_Msg_Type == CONTROLLER_DO)
            {
                SpeedValue = -*p_Speed;
                
                SpeedValueERR = (float32_t)SpeedValue - Speed_PID_S;
                
                Speed_Control_PID_Result = arm_pid_f32(&Speed_PID,SpeedValueERR);
            }
            if(Speed_Controller_IN_Msg->Controller_Msg_Type == CONTROLLER_CHANGE)
            {
                Speed_Controller_Reset();
            }
        }
    }
}		/* -----  end of function Speed_Control_Task  ----- */

void Speed_Controller_Reset(void )
{
    arm_pid_init_f32(&Speed_PID,TRUE);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Speed_Controller_Init
 *  Description:  Initialize Speed_controller MBox and variables. 
 * =====================================================================================
 */
void Speed_Controller_Init (void  )
{//TODO
    Speed_Controller_MBox = OSMboxCreate((void *)0);    /* Initialize MBox of this module */
    QuadDecode_Init();
    Speed_PID.Kp = Speed_PID_P;
    Speed_PID.Ki = Speed_PID_I;
    Speed_PID.Kd = Speed_PID_D;
    arm_pid_init_f32(&Speed_PID,TRUE);
/*    USART_Param_Invoke_float32(STAT_SPEED_P,&Speed_PID_P);
    USART_Param_Invoke_float32(STAT_SPEED_I,&Speed_PID_I);
    USART_Param_Invoke_float32(STAT_SPPED_D,&Speed_PID_D);
    USART_Param_Invoke_float32(STAT_DIR_I,  &Speed_PID_S);
    */
}		/* -----  end of function Speed_Controller_Init  ----- */
