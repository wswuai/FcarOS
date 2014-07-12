/* =====================================================================================
 *
 *       Filename:  DEV_Gyro.h
 *
 *    Description:  Header file of GYRO Driver.
 *                  This Module use 
 *                                  Peripherial Device:
 *                                      ADC0 , ADC1 , DMA_CH0, DMA_CH1 , PDB, PIT3
 *                                  Pins:
 *                                      ADC0_SE16   , ADC1_SE16  
 *        Version:  1.0
 *        Created:  2013/11/25 21时36分21秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef DEV_GYRO_H
#define DEV_GYRO_H
// Function Declaration:
void Gyro_Init(void );
INT32U *Get_GYRO_X(void );
INT32U *Get_Accer(void );
// *********************
#endif
