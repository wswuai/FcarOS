/*
 * =====================================================================================
 *
 *       Filename:  Direction_Controller.c
 *
 *    Description:  Direction control module for smart car.
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
//Global Variables:
OS_EVENT *Direction_Controller_MBox;
//Controller_DescTypeDef Direction_Desc;
extern OS_CPU_SR cpu_sr;

//Local Variables:
INT32U DirectionValueGet(void);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Direction_Controller_Task
 *  Description:  Task Body of Direction Controller
                  Created by Starter Task.
 * =====================================================================================
 */
//Local Functions
INT8U Direction_Controller_Init();

void Direction_Control_Task (void *p_arg  )
{
    //Private Variables of task.
    INT32U DirectionValue=0;
    ControllerMsg_TypeDef *Direction_Controller_IN_Msg;
    ControllerMsg_TypeDef Direction_Controller_OUT_Msg;
    INT8U err;
    Direction_Controller_OUT_Msg.Controller_Msg_Type = NULL;
    (void)DirectionValue;
    (void)Direction_Controller_OUT_Msg;
    printf("Direction Controller Init...");
    if(Direction_Controller_Init()==SUCCESS)
        printf("OK\n");
    else
        printf("FAILED\n");
    (void)p_arg;

    for(;;)
    {//TODO
        // Type Switch from (void *) - > (ControllerMsg_TypeDef *)
        // Pend Msg from MBox.
        Direction_Controller_IN_Msg =(ControllerMsg_TypeDef *)(
            OSMboxPend(Direction_Controller_MBox,0,&err));
        if(err == OS_ERR_NONE)
        {
            if(Direction_Controller_IN_Msg->Controller_Msg_Type == CONTROLLER_DO)
            {
                OS_ENTER_CRITICAL();
                DirectionValue = DirectionValueGet();
                OS_EXIT_CRITICAL();
                //TODO, Send Controller Active to Strategycontroller.
                //Direction_Control_PID_Result = PIDCalculate(DirectionValue,PIDController); 
                //Direction_Controller_OUT_Msg.Controller_Msg_Type = ANGEL_CONTROLLER_REPORT;
                //Direction_Controller_OUT_Msg.msg = (void *) &Direction_Control_PID_Result;
                //OSMBoxPost(StrategyController,(void *) Direction_Controller_Msg);
            }
        }
        else
        {
            printf("Direction Controller MBox ERR, ERR NO:%d",err);
        }
    }
}		/* -----  end of function Direction_Control_Task  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DirectionValueGet
 *  Description:  Get Current Direction Value From Accerlerator & Gyro.
 Called by Direction Controller Task.
 * =====================================================================================
 */
INT32U DirectionValueGet(void)
{//TODO
    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Direction_Controller_Init
 *  Description:  Initialize Direction_controller MBox and variables. 
 * =====================================================================================
 */
INT8U Direction_Controller_Init (void  )
{//TODO
    Direction_Controller_MBox = OSMboxCreate((void *)0);    /* Initialize MBox of this module */
    return SUCCESS;
}		/* -----  end of function Direction_Controller_Init  ----- */
