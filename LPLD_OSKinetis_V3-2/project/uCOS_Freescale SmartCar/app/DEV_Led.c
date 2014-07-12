/*
 * =====================================================================================
 *
 *       Filename:  DEV_Led.c
 *
 *    Description:  Control Functions for LED.
 *
 *        Version:  1.0
 *        Created:  2014/02/19 21时24分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "common.h"
#include "DEV_Led.h"
//Declaration for local variables:
INT8U   LED_Controls[LED_COUNT];
INT32U  LED_Blink_Times[LED_COUNT];
INT8U   LED_Change_Flag=0;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Init_Led
 *  Description:  Config GPIO 
 * =====================================================================================
 */

void Init_Led (void  )
{
    GPIO_InitTypeDef gpio_init_struct; 
    gpio_init_struct.GPIO_PTx = PTD;
    gpio_init_struct.GPIO_Pins = GPIO_Pin8_15;
    gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_init_struct.GPIO_Output = OUTPUT_H;
    gpio_init_struct.GPIO_PinControl = OUTPUT_DSH;
    LPLD_GPIO_Init(gpio_init_struct);
    
}		/* -----  end of function LED_Init  ----- */
