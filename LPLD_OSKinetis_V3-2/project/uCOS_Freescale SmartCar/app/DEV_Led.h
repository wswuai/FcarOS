/*
 * =====================================================================================
 *
 *       Filename:  DEV_Led.h
 *
 *    Description:  Header file of Led Dev Driver.
 *                  WARNING! LED Setting won't be immediately! Do not range time with it!
 *        Version:  1.0
 *        Created:  2013/11/25 21时48分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */

//Declaration of public functions:
void Init_Led (void  );
void Set_LED_OFF(void );
void Set_LED_ON(void );
//Public Defination:
#define LED_PTX         PTD
#define LED_HEAD        GPIO_Pin8
#define LED_PORT        (GPIO_Pin8|GPIO_Pin9|GPIO_Pin10|GPIO_Pin11|GPIO_Pin12|GPIO_Pin13|GPIO_Pin14|GPIO_Pin15)
#define LED_COUNT       8u
//defines for LED status
#define LED_OFF     0
#define LED_ON      1
#define LED_BLINK   2


