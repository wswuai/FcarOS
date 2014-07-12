/*
 * =====================================================================================
 *
 *       Filename:  DEV_Encoder.c
 *
 *    Description:  ENCoder driver. used to sensor the speed.
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时34分55秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio.
 *
 * =====================================================================================
 */
#include "common.h"
void qd_init(void);
void pit_init(void);
void pit_isr(void);
void delay(uint16);
static FTM_InitTypeDef ftm_init_struct;
PIT_InitTypeDef pit_init_struct;
INT16S qd_result=0;
void QuadDecode_Init(void)
{
    qd_init();
    pit_init();
}
void qd_init(void)
{

  ftm_init_struct.FTM_Ftmx = FTM1;              
  ftm_init_struct.FTM_Mode = FTM_MODE_QD;       
  ftm_init_struct.FTM_QdMode = QD_MODE_PHAB;    
  LPLD_FTM_Init(ftm_init_struct);
  //PTB0引脚接A相输入、PTB1引脚接B相输入
  LPLD_FTM_QD_Enable(FTM1, PTB0, PTB1);
}

void pit_init(void)
{
  pit_init_struct.PIT_Pitx = PIT1;
  pit_init_struct.PIT_PeriodMs = 10;    
  pit_init_struct.PIT_Isr = pit_isr;
  LPLD_PIT_Init(pit_init_struct);
  LPLD_PIT_EnableIrq(pit_init_struct);
}

void pit_isr(void)
{
  qd_result = LPLD_FTM_GetCounter(FTM1);
  LPLD_FTM_ClearCounter(FTM1);
}
INT16S *Get_Speed(void)
{
  return &qd_result;
}
