/*
 * =====================================================================================
 *
 *       Filename:  Angle_Controller.c
 *
 *    Description:  Angle control module for smart car.
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
#include "USART_Middle_Module.h"
#include "DEV_Gyro.h"
#include "DEV_SSD1306.h"
#include "DEV_Motor.h"
// Global Variables:
OS_EVENT *Angle_Controller_MBox;
extern    OS_CPU_SR cpu_sr;
float32_t Angle_PID_P=ANGLEPID_PROPORTION;
float32_t Angle_PID_I=ANGLEPID_INTEGRAL;
float32_t Angle_PID_D=ANGLEPID_DERIVATIVE;
float32_t Angle_PID_S=ANGLEPID_ANGLESET;
float32_t Angle_Offset = 0;
INT32U    Gyro_Middle_Value=0;
INT32U   *GYRO;
INT32U   *ACCER;
// Local Variables:
INT32U AngleValueGet(void);
arm_pid_instance_f32 Angle_PID;
float32_t Angle_Control_PID_Result;
float32_t GyroSpeed =0;
float32_t CarAngle =0;
float32_t GyroIntegral=1400;
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Angle_Controller_Task
 *  Description:  Task Body of Angle Controller
 *                Created by Starter Task.
 * =====================================================================================
 */
// Local Functions
INT8U Angle_Controller_Init();
void AngleValueCalculate(void);
INT32S Gyro_GetStable(INT32U *gyro,INT32U *accer);
void Angle_Controller_Reset(void );
float32_t Gravity_Offset = GRAVITY_OFFSET;
float32_t Gravity_Angle_Ratio = GRAVITY_ANGLE_RATIO;
float32_t Gravity_Adjust_Time_Constant = GRAVITY_ADJUST_TIME_CONSTANT;
float32_t Gyro_Angle_Ratio = GYRO_ANGLE_RATIO;
float32_t GravityAngle = 0;
float32_t AngleValueERR=0;
// =====================================================================================
// Function Implementation : 

void AngleValueCalculate(void)
{
    // Level 1 - Low Pass Filter 

    float32_t  Delta_Gravity_Gyro = 0;
    GravityAngle = (*ACCER - Gravity_Offset) * Gravity_Angle_Ratio;
    GyroSpeed = ((double)Gyro_Middle_Value  - (double)*GYRO) * Gyro_Angle_Ratio; 
    CarAngle = GyroIntegral;
    Delta_Gravity_Gyro = (GravityAngle - CarAngle) / Gravity_Adjust_Time_Constant;
    GyroIntegral += (GyroSpeed + Delta_Gravity_Gyro) / GYRO_INTEGRAL_FREQUENCY;

}
void Angle_Control_Task (void *p_arg  )
{
    //Private Variables of task.

    ControllerMsg_TypeDef *Angle_Controller_IN_Msg;
    INT8U err;
    float32_t Angle_Set;
    (void)p_arg; /* Suppress Warning*/
    if(Angle_Controller_Init()== SUCCESS)
      LPLD_GPIO_Output_b(PTD,14,0);


    for(;;)
    {
        // Type Cast from (void *) - > (ControllerMsg_TypeDef *)
        // Pend Msg from MBox.
        Angle_Controller_IN_Msg =(ControllerMsg_TypeDef *)(
                OSMboxPend(Angle_Controller_MBox,0,&err));
        if(err == OS_ERR_NONE)
        {
            if(Angle_Controller_IN_Msg->Controller_Msg_Type == CONTROLLER_DO)
            {

                AngleValueCalculate();
                Angle_Set = Angle_PID_S +  Angle_Offset;
                AngleValueERR= AngleValueGet() - Angle_Set;
                Angle_Control_PID_Result = arm_pid_f32(&Angle_PID,AngleValueERR);
            }
            if(Angle_Controller_IN_Msg->Controller_Msg_Type == CONTROLLER_CHANGE)
            {
                Angle_Controller_Reset();
            }
        }
    }
}		/* -----  end of function Angle_Control_Task  ----- */
float32_t *Get_Angle_Result(void)
{
    return &Angle_Control_PID_Result;
}
arm_pid_instance_f32 *Angle_PID_Get(void)
{
    return &Angle_PID;
}
void Angle_Controller_Reset(void )
{  
    Angle_PID.Kp = Angle_PID_P;
    Angle_PID.Ki = Angle_PID_I;
    Angle_PID.Kd = Angle_PID_D;
    arm_pid_init_f32(&Angle_PID,TRUE);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  AngleValueGet
 *  Description:  Get Current Angle Value From Accerlerator & Gyro.
 *                Called by Angle Controller Task.
 * =====================================================================================
 */
INT32U AngleValueGet(void)
{
    return (INT32U)CarAngle;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Angle_Controller_Init
 *  Description:  Initialize Angle_controller MBox and variables. 
 * =====================================================================================
 */
INT8U Angle_Controller_Init (void  )
{//TODO
    char tmp[30];
    INT32S off=0;
    (void)tmp;
    Angle_Controller_MBox = OSMboxCreate((void *)0);    /* Initialize MBox of this module */
    Angle_PID.Kp = Angle_PID_P;
    Angle_PID.Ki = Angle_PID_I;
    Angle_PID.Kd = Angle_PID_D;
    arm_pid_init_f32(&Angle_PID,TRUE);
    GYRO = Get_GYRO_X();
    ACCER = Get_Accer();
    USART_Param_Invoke_float32(STAT_VALUE_GYRO,&GravityAngle);

    USART_Param_Invoke_float32(STAT_ANGLE_P,&Angle_PID_P);
    USART_Param_Invoke_float32(STAT_ANGLE_I,&Angle_PID_I);
    USART_Param_Invoke_float32(STAT_ANGLE_D,&Angle_PID_D);
    USART_Param_Invoke_float32(STAT_DIR_P,&Angle_PID_S);

    USART_Param_Invoke_float32(STAT_VALUE_ANGLE,&CarAngle);

    OSTimeDlyHMSM(0,0,0,50);

    do{
        off = Gyro_GetStable(GYRO,ACCER);
    }while(off>20);

    AngleValueCalculate();
    CarAngle=GyroIntegral=GravityAngle;

    LCD_P6x8Str(0,4,"  -Gyro Aligned -   ");

    return SUCCESS;
}		/* -----  end of function Angle_Controller_Init  ----- */
INT32S Gyro_GetStable(INT32U *gyro , INT32U *accer)
{
    INT32U i;
    INT32U stable = *gyro;
    INT32U max=0,min=10000;
    OSTimeDlyHMSM(0,0,0,10);
    for(i=0;i<100;i++)
    {
        stable += *gyro;
        stable /= 2;
        if(*gyro > max) max = *gyro;
        if(*gyro < min) min = *gyro;
        OSTimeDlyHMSM(0,0,0,15);
    }
    Gyro_Middle_Value = stable;
    return (max-min);
}
