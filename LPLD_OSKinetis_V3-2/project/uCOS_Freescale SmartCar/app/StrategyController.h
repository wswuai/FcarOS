/*
 * =====================================================================================
 *
 *       Filename:  StrategyController.h
 *
 *    Description:  Header of Strategycontroller.
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时33分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef  __STRATEGY_CONTROLLER_H
#define  __STRATEGY_CONTROLLER_H
//Public Variables.
extern OS_EVENT *Strategy_Controller_MBox;       
extern  INT8U MotorStat;
//Public Functions.
void Strategy_Control_Task (void *p_arg);// Strategycontroller Routine Task. Called By OS.
#define MOTOR_ON     1u
#define MOTOR_OFF    0u

#endif
