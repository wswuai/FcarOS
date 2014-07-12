/*
 * =====================================================================================
 *
 *       Filename:  DEV_Camera_OV7620.h
 *
 *    Description:  this module use :
 *                          DMA_CH2 , PTD0~7 , PTC7 , PTC8 , PTC10
 *                                  , (Data) , (      ISR of PTC  )
 *        Version:  1.0
 *        Created:  2014/03/22 22时53分04秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio.
 *
 * =====================================================================================
 */

//Defines:
#define H 320
#define V 240
#define PHOTO_SIZE H*V
//Public Functions:
void Camera_Task(void);


