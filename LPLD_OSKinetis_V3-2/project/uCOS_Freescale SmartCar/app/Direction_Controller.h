/*
 * =====================================================================================
 *
 *       Filename:  Direction_Controller.h
 *
 *    Description:  Direction_controller header 
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
#ifndef DIRECTION_CONTROLLER_H
#define DIRECTION_CONTROLLER_H

//External Function declarations
void Direction_Control_Task (void* p_arg  );
//Angel Controller Message Box. Send OS_EVENT to this.
extern OS_EVENT *Direction_Controller_MBox;
#endif
