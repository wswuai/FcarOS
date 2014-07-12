/**
 * --------------£¨UCOS_SmartCar Freescale 2014£©-----------------
 * @file LPLD_uCosV292.c
 * @version 0.1
 * @date 2013-9-29
 * @author XiaoGo
 * All Rights Reserved By 202 Studio.
 */
#include "common.h"
#include "OSMessage.h"
#include "os_cpu.h"
#include "USART_Middle_Module.h"
#include "SDCard_Middle_Module.h"
#include "Angle_Controller.h"
#include "Speed_Controller.h"
#include "Direction_Controller.h"
#include "DEV_Led.h"
#include "DEV_Gyro.h"
#include "DEV_SSD1306.h"
#include "DEV_Motor.h"
#include "DEV_QuadDecode.h"
#include "Camera_Controller.h"
#include "Key_Middle_Module.h"
#include "StrategyController.h"
//CPU SR & Task Stack Declaration:
OS_STK App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];	
OS_STK Speed_Controller_Task_Stk[APP_CFG_TASK_START_STK_SIZE];
OS_STK Angle_Controller_Task_Stk[APP_CFG_TASK_START_STK_SIZE];
OS_STK USART_Middle_Module_Stk[APP_CFG_TASK_START_STK_SIZE];
OS_STK SDCard_Driver_Stk[APP_CFG_TASK_START_STK_SIZE];
OS_STK Camera_Task_Stk[APP_CFG_TASK_START_STK_SIZE];
OS_STK Key_Task_Stk[APP_CFG_TASK_START_STK_SIZE/2];
OS_STK Strategy_Task_Stk[APP_CFG_TASK_START_STK_SIZE];
void App_TaskStart(void  *p_arg);
void App_Task1 (void *p_arg);
OS_CPU_SR cpu_sr;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Main
 *  Description:  init os and create starter task. bring chip control to ucos shell.
 * =====================================================================================
 */
int  main (void)
{
    INT8U err =0;
    OSInit();      
    (void )err;
    OSTaskCreateExt((void (*)(void *)) App_TaskStart,         
            (void           *) 0,
            (OS_STK         *)&App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
            (INT8U           ) APP_CFG_TASK_START_PRIO,
            (INT16U          ) APP_CFG_TASK_START_PRIO,
            (OS_STK         *)&App_TaskStartStk[0],
            (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
            (void           *) 0,
            (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSStart();
}
ControllerMsg_TypeDef Message_To_Controller; 
INT32U *GYRO_X;
INT32U *ACCER_X;
void delay(void)
{
    int i,j;
    for(i=0;i<100;i++)
        for(j=0;j<33;j++);
}
INT16S *Speed_Value;
INT32U ctr=0;



void  App_TaskStart (void *p_arg)
{
    INT8U err = 0;
    int64_t cnt =0;
    (void)p_arg;
    (void)err;
    char tmp[20];
    SystemTickInit();
    Init_Led();
    _BEEP_ON();
    Gyro_Init();
    LCD_Init();
    Motor_Init();
    LCD_P6x8Str(0,0,"Freescale Camera Car");
    LCD_P6x8Str(0,1,"       By 202 STUDIO");
    LCD_P6x8Str(0,3,"System    Initing..."); 
    LCD_P6x8Str(0,4,"  -Gyro Aligning-   ");
    LCD_P6x8Str(0,5,"  -DO NOT  MOVE -   ");
    OSStatInit();
    QuadDecode_Init();
    Speed_Value = Get_Speed();
    OSTaskCreate((void (*)(void *)) Angle_Control_Task,
            (void *) 0,
            (OS_STK*)&Angle_Controller_Task_Stk[APP_CFG_TASK_START_STK_SIZE - 1],
            (INT8U) APP_ANGLE_CONTROLLER_PRIO
            ); 
    OSTaskCreate((void (*)(void *)) Key_Task,
            (void *) 0,
            (OS_STK*)&Camera_Task_Stk[APP_CFG_TASK_START_STK_SIZE/2 - 1],
            (INT8U) APP_TASK_KEY_PRIO
            );
    OSTaskCreate((void (*)(void *)) USART_Driver_Task,
            (void *) 0,
            (OS_STK*)&USART_Middle_Module_Stk[APP_CFG_TASK_START_STK_SIZE - 1],
            (INT8U) USART_MIDDLE_MODULE_PRIO
            );
    OSTaskCreate((void (*)(void *)) SDCard_Driver_Task,
            (void *) 0,
            (OS_STK*)&SDCard_Driver_Stk[APP_CFG_TASK_START_STK_SIZE - 1],
            (INT8U) SDCARD_DRIVER_PRIO
            );
    OSTaskCreate((void (*)(void *)) Camera_Task,
            (void *) 0,
            (OS_STK*)&Camera_Task_Stk[APP_CFG_TASK_START_STK_SIZE - 1],
            (INT8U) APP_CAMERA_CONTROLLER_PRIO
            );
    OSTaskCreate((void (*)(void *)) Strategy_Control_Task,
            (void *) 0,
            (OS_STK*)&Strategy_Task_Stk[APP_CFG_TASK_START_STK_SIZE - 1],
            (INT8U) APP_STRATEGY_TASK_PRIO
            );
    OSTaskCreate((void (*)(void *)) Speed_Control_Task,
            (void *) 0,
            (OS_STK*)&Speed_Controller_Task_Stk[APP_CFG_TASK_START_STK_SIZE - 1],
            (INT8U) APP_SPEED_CONTROLLER_PRIO
            );

    Message_To_Controller.Controller_Msg_Type = CONTROLLER_DO; 
    OSTimeDlyHMSM(0,0,0,50);
    _BEEP_OFF();
    while (TRUE)
    {
        OSTimeDlyHMSM(0, 0,0,5);
        cnt++;
        if((cnt%4)==0) {OSMboxPost(USART_Controller_MBox ,(void *)&Message_To_Controller);}
        OSMboxPost(Angle_Controller_MBox,(void *)&Message_To_Controller);
        OSMboxPost(Speed_Controller_MBox,(void *)&Message_To_Controller);
        if(cnt%10==0)
        {
            sprintf((signed char *)tmp,"  Speed:%d   ",*Speed_Value);
            LCD_P6x8Str(0,6,(INT8U *)tmp);  
            sprintf((signed char *)tmp,"  CPU  :%d%%  ",OSCPUUsage);
            LCD_P6x8Str(0,7,(INT8U *)tmp);  
        }
        if(cnt%15 == 0)
            LPLD_GPIO_Toggle_b(PTD,8);


        //        OSMboxPost(Direction_Controller_MBox,(void *)&Message_To_Controller);
        //        OSMboxPost(Speed_Controller_MBox,(void *)&Message_To_Controller);
    }
}

