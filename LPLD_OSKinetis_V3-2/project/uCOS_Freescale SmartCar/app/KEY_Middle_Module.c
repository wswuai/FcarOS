/*
 * =====================================================================================
 *
 *       Filename:  KEY_Middle_Module.c
 *
 *    Description:  KEY Process Middle Module.
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时37分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "common.h"
#include "KEY_Middle_Module.h"
#include "DEV_SSD1306.h"
#include "SDCard_Middle_Module.h"
#include "OSMessage.h"
static GPIO_InitTypeDef Keys;
KEY_CALLBACK_FUNC Key_CB[3]= {NULL,NULL,NULL};

void Key_Init(void);

void Key_Init(void)
{
    Keys.GPIO_PTx = PTA;
    Keys.GPIO_Pins = GPIO_Pin15 | GPIO_Pin16 | GPIO_Pin17 ;
    Keys.GPIO_Dir = DIR_INPUT;
    Keys.GPIO_PinControl = INPUT_PULL_UP;
    LPLD_GPIO_Init(Keys);
}
void Key_Task(void *p_arg) // Key Task .Called By OS.
{
    Key_Init();
    (void)p_arg;
    while(TRUE)
    {
        OSTimeDlyHMSM(0,0,0,50);
        if(LPLD_GPIO_Input_b(PTA,15) == 0)
        {
            _BEEP_ON();
            OSTimeDlyHMSM(0,0,0,100);
            _BEEP_OFF();
            if(Key_CB[0]!=NULL)
                Key_CB[0]();
        }
        if(LPLD_GPIO_Input_b(PTA,16) == 0)
        {
            _BEEP_ON();
            OSTimeDlyHMSM(0,0,0,100);
            _BEEP_OFF();
            
            if(Key_CB[1]!=NULL)
                Key_CB[1]();
        }
        if(LPLD_GPIO_Input_b(PTA,17) == 0)
        {
            _BEEP_ON();
            OSTimeDlyHMSM(0,0,0,100);
            _BEEP_OFF();
            if(Key_CB[2]!=NULL)
                Key_CB[2]();         
        }
    }
}
void Key_CB_Invoke(INT8U Key_Type, KEY_CALLBACK_FUNC p)
{
    if( Key_Type <= KEY_C)
    {
        Key_CB[Key_Type] = p;
    }
}
