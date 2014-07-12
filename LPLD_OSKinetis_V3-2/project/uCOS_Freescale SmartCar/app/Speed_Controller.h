/*
 * =====================================================================================
 *
 *       Filename:  Speed_Controller.h
 *
 *    Description:  Speed_controller header 
 *
 *        Version:  0.1
 *        Created:  2013/11/24 14时00分15秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 studio 
 *
 * =====================================================================================
 */
#ifndef SPEED_CONTROLLER_H
#define SPEED_CONTROLLER_H

//External Function declarations
void Speed_Control_Task (void  );
//Angel Controller Message Box. Send OS_EVENT to this.
extern OS_EVENT *Speed_Controller_MBox;
extern float32_t Speed_Control_PID_Result;
#endif
