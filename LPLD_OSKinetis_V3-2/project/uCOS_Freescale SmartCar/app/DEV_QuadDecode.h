/* =====================================================================================
 *
 *       Filename:  DEV_Encoder.h
 *
 *    Description:  Header file of encoder.used to sensor speed.
 *                  This Module use the Periph Device:
 *                  PIT1 , FTM1
 *                  This Module use the Pins :
 *                  PTB0 PTB1 (For QuadDecoder)
 *        Version:  1.0
 *        Created:  2013/11/25 21时35分29秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio.
 *
 * =====================================================================================
 */
//Global Functions:
void QuadDecode_Init(void ); // Called by Starter Task.
INT32U getSpeedNow(void ); // Called by Speed Controller . 
INT16S *Get_Speed(void);