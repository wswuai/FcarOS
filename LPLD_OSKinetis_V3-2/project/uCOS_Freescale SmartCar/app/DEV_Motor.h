/*
 * =====================================================================================
 *
 *       Filename:  DEV_Motor.h
 *
 *    Description:  DEV Motor Header.
 *                  This Module use the peripheral Device:
 *                  FTM0, 
 *                  This Module use the pin :
 *                  PTC1, PTC2, PTC3, PTC4
 *        Version:  1.0
 *        Created:  2013/11/25 21时32分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */
//Public Defines:

#define MOTOR_LEFT  0u
#define MOTOR_RIGHT 1u
#define EN_LEFT_MOTOR 
#define EN_RIGHT_MOTOR
//Public functions:

void Motor_Init(void ); // Called by Starter Task. Called Once.
void Motor_Set(INT32S pwm,INT8U dir); // Called by Strategy Controller . Called Routine.
INT32U Motor_Get(void ); // Called by Debug interface. Called Random.
