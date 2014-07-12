/*
 * =====================================================================================
 *
 *       Filename:  DEV_Gyro.c
 *
 *    Description:  GYRO Driver, Used to sensor the angle .
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时35分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */
#include "common.h"
#include "OSMessage.h"
#include "DEV_Gyro.h"
#include "USART_Middle_Module.h"
INT32U Result_X=0;
INT32U Result_Y=0;
PDB_InitTypeDef pdb_init_struct;
ADC_InitTypeDef adc_init_struct;
PIT_InitTypeDef pit3_init_struct;
DMA_InitTypeDef dma_init_struct;
INT32S Gyro_GetStable(INT32U *gyro);
void PDB3_Isr_Integrate(void);

void Gyro_Init (void)
{
    adc_init_struct.ADC_Adcx = ADC0;     
    adc_init_struct.ADC_BitMode = SE_12BIT;      
    adc_init_struct.ADC_CalEnable = TRUE;       
    adc_init_struct.ADC_HwTrgCfg = HW_TRGA;    
    adc_init_struct.ADC_DmaEnable = TRUE;     
    adc_init_struct.ADC_HwAvgSel = HW_4AVG;
    LPLD_ADC_Init(adc_init_struct);   
    LPLD_ADC_Chn_Enable(ADC0, AD16);
    LPLD_ADC_EnableConversion(ADC0, AD16, 0, FALSE);
    dma_init_struct.DMA_CHx = DMA_CH0;   
    dma_init_struct.DMA_Req = ADC0_DMAREQ;        
    dma_init_struct.DMA_MajorLoopCnt = 10;       
    dma_init_struct.DMA_MinorByteCnt = 2; 
    dma_init_struct.DMA_SourceAddr = (uint32)&(ADC0->R[0]);       
    dma_init_struct.DMA_SourceDataSize = DMA_SRC_16BIT;   
    dma_init_struct.DMA_DestAddr = (uint32)&Result_X;       
    dma_init_struct.DMA_DestDataSize = DMA_DST_16BIT;     
    dma_init_struct.DMA_AutoDisableReq = FALSE;   
    LPLD_DMA_Init(dma_init_struct);
    LPLD_DMA_EnableReq(DMA_CH0);

    adc_init_struct.ADC_Adcx = ADC1;     
    adc_init_struct.ADC_BitMode = SE_12BIT;      
    adc_init_struct.ADC_CalEnable = TRUE;       
    adc_init_struct.ADC_HwTrgCfg = HW_TRGA;    
    adc_init_struct.ADC_DmaEnable = TRUE;     
    LPLD_ADC_Init(adc_init_struct);   
    LPLD_ADC_Chn_Enable(ADC1, AD16);
    LPLD_ADC_EnableConversion(ADC1, AD16, 0, FALSE);
    dma_init_struct.DMA_CHx = DMA_CH1;   
    dma_init_struct.DMA_Req = ADC1_DMAREQ;        
    dma_init_struct.DMA_MajorLoopCnt = 10;       
    dma_init_struct.DMA_MinorByteCnt = 2; 
    dma_init_struct.DMA_SourceAddr = (uint32)&(ADC1->R[0]);       
    dma_init_struct.DMA_SourceDataSize = DMA_SRC_16BIT;   
    dma_init_struct.DMA_DestAddr = (uint32)&Result_Y;       
    dma_init_struct.DMA_DestDataSize = DMA_DST_16BIT;     
    dma_init_struct.DMA_AutoDisableReq = FALSE;   
    LPLD_DMA_Init(dma_init_struct);
    LPLD_DMA_EnableReq(DMA_CH1);

    pdb_init_struct.PDB_CounterPeriodUs = 10000;   
    pdb_init_struct.PDB_LoadModeSel = LOADMODE_0; 
    pdb_init_struct.PDB_ContinuousModeEnable = FALSE;     
    pdb_init_struct.PDB_TriggerInputSourceSel = TRIGGER_PIT3;     
    LPLD_PDB_Init(pdb_init_struct);
    LPLD_PDB_AdcTriggerCfg(ADC0, PRETRIG_EN_A, 0);
    LPLD_PDB_AdcTriggerCfg(ADC1, PRETRIG_EN_A, 0);
    pit3_init_struct.PIT_Pitx = PIT3;     
    pit3_init_struct.PIT_PeriodUs = 3000;  
    LPLD_PIT_Init(pit3_init_struct);      
}       /* -----  end of function GYRO_INIT  ----- */
INT32U *Get_GYRO_X (void )
{
    return &Result_X;
}		/* -----  end of function Get_GYRO_X  ----- */
INT32U *Get_Accer (void )
{
    return &Result_Y;
}		/* -----  end of function Get_GYRO_Y  ----- */
