/*
 * =====================================================================================
 *
 *       Filename:  DEV_Camera_OV7725.c
 *
 *    Description:  Camera Driver of OV7725
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时34分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio.
 *
 * =====================================================================================
 */

#include "common.h"
#include "DEV_SCCB.h"

//Private Functions:

uint8 ov7670_init();
void porta_isr(void);
void portb_isr(void);
void gpio_init(void);
void dma_init(void);
void SyncIRQ(void);

//Private Variables:
GPIO_InitTypeDef VSyncPin_InitStructure;
GPIO_InitTypeDef HSyncPin_InitStructure;
GPIO_InitTypeDef PCLK_InitStructure;
DMA_InitTypeDef dma_init_struct;

//Global Variables:
uint8 Pix_Data[H][V];       
uint32 V_Cnt=0;                     
uint8 CapFinished=0;               

void Camera_Task(void *p_arg) //Camera Task
{
    OS_ENTER_CRITICAL();
    if(ov7670_init() == 1)
        printf("Camera Init successful!\n");
    else
        printf("Camera Init Failed!\r\n");

    V_Cnt=0;          
    CapFinished=0;   
    gpio_init();
    dma_init();
    enable_irq(PORTC_IRQn);
    OS_EXIT_CRITICAL();

    while(TRUE)
    {

        if(CapFinished==1)
        {   
            CapFinished = 0;
            disable_irq(PORTC_IRQn);
            LPLD_GPIO_ClearIntFlag(PORTC);
            enable_irq(PORTC_IRQn);
        }
    } 
}

void gpio_init()
{

    ptd_init.GPIO_PTx = PTD;
    ptd_init.GPIO_Dir = DIR_INPUT;
    ptd_init.GPIO_Pins = GPIO_Pin0_7;
    ptd_init.GPIO_PinControl = IRQC_DIS | INPUT_PULL_DIS;
    LPLD_GPIO_Init(ptd_init);


    HSyncPin_InitStructure.GPIO_PTx = PTC;
    HSyncPin_InitStructure.GPIO_Dir = DIR_INPUT;
    HSyncPin_InitStructure.GPIO_Pins = GPIO_Pin8;
    HSyncPin_InitStructure.GPIO_PinControl = IRQC_RI|INPUT_PULL_DOWN;
    HSyncPin_InitStructure.GPIO_Isr = SyncIRQ;
    LPLD_GPIO_Init(HSyncPin_InitStructure); 


    VSyncPin_InitStructure.GPIO_PTx = PTC;
    VSyncPin_InitStructure.GPIO_Dir = DIR_INPUT;
    VSyncPin_InitStructure.GPIO_Pins = GPIO_Pin10;
    VSyncPin_InitStructure.GPIO_PinControl = IRQC_RI|INPUT_PULL_DOWN;
    VSyncPin_InitStructure.GPIO_Isr = SyncIRQ;
    LPLD_GPIO_Init(VSyncPin_InitStructure); 


    PCLK_InitStructure.GPIO_PTx = PTC;
    PCLK_InitStructure.GPIO_Pins = GPIO_Pin7;
    PCLK_InitStructure.GPIO_Dir = DIR_INPUT;
    PCLK_InitStructure.GPIO_PinControl = IRQC_DMARI | INPUT_PULL_DIS;
    LPLD_GPIO_Init(PCLK_InitStructure); 

    PCLK_InitStructure.GPIO_PTx = PTD;
    PCLK_InitStructure.GPIO_Pins = GPIO_Pin8;
    PCLK_InitStructure.GPIO_Dir = DIR_OUTPUT;
    PCLK_InitStructure.GPIO_PinControl = PORT_PCR_DSE_MASK;
    LPLD_GPIO_Init(PCLK_InitStructure); 


}

void dma_init(void)
{

    dma_init_struct.DMA_CHx = DMA_CH1;    
    dma_init_struct.DMA_Req = PORTC_DMAREQ;       
    dma_init_struct.DMA_MajorLoopCnt = H; 
    dma_init_struct.DMA_MinorByteCnt = 1; 
    dma_init_struct.DMA_SourceAddr = (uint32)&PTD->PDIR;        
    dma_init_struct.DMA_DestAddr = (uint32)Pix_Data;      
    dma_init_struct.DMA_DestAddrOffset = 1;       
    dma_init_struct.DMA_AutoDisableReq = TRUE;    

    LPLD_DMA_Init(dma_init_struct);
}

INT8U ImageCapturing = 0;
void SyncIRQ(void)
{
    if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin8))
    {

        if(ImageCapturing == 1)
        {
            if(V_Cnt<V-1 ){
                LPLD_DMA_EnableReq(DMA_CH1);  
                V_Cnt++; 
            }

            else
            {
                CapFinished = 1;
                V_Cnt=0;
                LPLD_GPIO_Output_b(PTD,8,1);
                ImageCapturing = 0;
            }
        }
    }

    if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin10))
    {

        LPLD_DMA_LoadDstAddr(DMA_CH1, Pix_Data);

        LPLD_GPIO_ClearIntFlag(PORTC);

        LPLD_GPIO_Output_b(PTD,8,0);
        ImageCapturing = 1;
        V_Cnt=0;
    }



}


uint8 ov7670_init(void)
{
    uint16 i = 0;
    uint8 device_id = 0;

    LPLD_SCCB_Init();

    delay();


    while( 0 == LPLD_SCCB_ReadReg(0x1C, &device_id, 1)) printf("Read ID Failed!\n");


    if(device_id == 0x7F)
    {  
        while( 0 == LPLD_SCCB_WriteReg(0x11, 0x03) );	
        while( 0 == LPLD_SCCB_WriteReg(0x14, 0x24) );	
        while( 0 == LPLD_SCCB_WriteReg(0x28, 0x20) );	
    }
    else
    {
        return 0;
    }

    return 1;
}

