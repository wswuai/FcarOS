/*
 * =====================================================================================
 *
 *       Filename:  KEY_Middle_Module.h
 *
 *    Description:  Key_middle_module header file.
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时37分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */

#define KEY_A   0u
#define KEY_B   1u
#define KEY_C   2u

typedef void (*KEY_CALLBACK_FUNC)(void);
void Key_Task(void *p_arg); // Key Task .Called By OS.
void Key_CB_Invoke(INT8U Key_Type, KEY_CALLBACK_FUNC p);