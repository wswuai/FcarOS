/*
 * =====================================================================================
 *
 *       Filename:  SDCard_Middle_Module.h
 *
 *    Description:  SDCARD_MIDDLE_MODULE.
 *
 *        Version:  0.1
 *        Created:  2013/11/24 22时06分48秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio
 *
 * =====================================================================================
 */
#ifndef SDCARD_MIDDLE_MODULE_H
#define SDCARD_MIDDLE_MODULE_H
//Global Variables
extern OS_EVENT *SDCard_MBox;
//Public Functions
void SDCard_Driver_Task( void * );

#endif

