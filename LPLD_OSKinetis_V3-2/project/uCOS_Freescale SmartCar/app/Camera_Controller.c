#include "common.h"
#include "OSMessage.h"
#include "DEV_SCCB.h"
#include "KEY_Middle_Module.h"
#include "SDCard_Middle_Module.h"
#define H 320   
#define V 240
#define PHOTO_SIZE H*V
extern OS_CPU_SR cpu_sr;
void porta_isr(void);
void portb_isr(void);
void gpio_init(void);
void dma_init(void);

uint8 ov7670_init();
void delay(void);
void SyncIRQ(void);
GPIO_InitTypeDef VSyncPin_InitStructure;
GPIO_InitTypeDef HSyncPin_InitStructure;
GPIO_InitTypeDef PCLK_InitStructure;
GPIO_InitTypeDef ptd_init;
DMA_InitTypeDef DMACH2_InitStructure;

uint8 Pix_Data[V][H];       
uint32 V_Cnt=0;                     
OS_EVENT *CapFinished;


ControllerMsg_TypeDef mmessage_to_sdcard;
ControllerMsg_TypeDef rec_message;
sdcard_Injection_TypeDef inject;
//debug !

void Camera_Task(void *p_arg) //Camera Task
{
    INT8U err;
    inject.width = 320;
    inject.height = 240;
    inject.ptr = &Pix_Data[0][0];
    

    OSTimeDlyHMSM(0,0,0,50);
    rec_message.Controller_Msg_Type = CONTROLLER_DO;
    
    if(ov7670_init() == SUCCESS)
    {
        printf("Camera Init successful!\r\n");;
        LPLD_GPIO_Output_b(PTD,12,0);
        V_Cnt=0;          
        CapFinished = OSSemCreate(1);
        gpio_init();
        dma_init();
        enable_irq(PORTC_IRQn);
        LPLD_GPIO_Output_b(PTD,12,0);
        mmessage_to_sdcard.Controller_Msg_Type = SDCARD_PTR_INJECTION;
    
        mmessage_to_sdcard.msg = (void *)&inject;
    
        OSMboxPost(SDCard_MBox,(void *)&mmessage_to_sdcard);
    
    
    }
    else
    {
        while(1)
        {
            OSTimeDlyHMSM(0,1,0,0);
            printf("Camera Init Failed!\n");
        }
    }

    while(TRUE)
    {
        OSSemPend(CapFinished,0,&err);
        disable_irq(PORTC_IRQn);
        LPLD_GPIO_ClearIntFlag(PORTC);
        enable_irq(PORTC_IRQn);
        LPLD_GPIO_Toggle_b(PTD,12);
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
    PCLK_InitStructure.GPIO_PinControl = IRQC_DMARI | INPUT_PULL_DIS ;
    LPLD_GPIO_Init(PCLK_InitStructure); 

}

void dma_init(void)
{

    DMACH2_InitStructure.DMA_CHx = DMA_CH2;    
    DMACH2_InitStructure.DMA_Req = PORTC_DMAREQ;
    DMACH2_InitStructure.DMA_MajorLoopCnt = H; 
    DMACH2_InitStructure.DMA_MinorByteCnt = 1; 
    DMACH2_InitStructure.DMA_SourceAddr = (uint32)&PTD->PDIR;        
    DMACH2_InitStructure.DMA_DestAddr = (uint32)Pix_Data;      
    DMACH2_InitStructure.DMA_DestAddrOffset = 1;       
    DMACH2_InitStructure.DMA_AutoDisableReq = TRUE;    
    LPLD_DMA_Init(DMACH2_InitStructure);
}

INT8U ImageCapturing = 0;

void SyncIRQ(void)
{
    if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin8))
    {
        if(ImageCapturing == 1)
        {
            if(V_Cnt<V-1 ){

                LPLD_DMA_EnableReq(DMA_CH2);  
                rec_message.msg = (void *)V_Cnt++; 
                
            }
            else
            {
                OSSemPost(CapFinished);
                V_Cnt=0;
                ImageCapturing = 0;
                OSMboxPost(SDCard_MBox,(void *)&rec_message);
            }
        }
    }

    if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin10))
    {

        LPLD_DMA_LoadDstAddr(DMA_CH2, Pix_Data);
        LPLD_GPIO_ClearIntFlag(PORTC);
        ImageCapturing = 1;
        V_Cnt=0;
    }
}


uint8 ov7670_init(void)
{
    uint8 device_id = 0;

    LPLD_SCCB_Init();

    delay();


    if( 0 == LPLD_SCCB_ReadReg(0x1C, &device_id, 1)) 
    {
        printf("Camera Init Failed: Read ID Failed!\n");
        return FAILED;
    }


    if(device_id == 0x7F) //Confirm Device Configuration.
    {  
        if( 0 == LPLD_SCCB_WriteReg(0x11, 0x03) );	
        if( 0 == LPLD_SCCB_WriteReg(0x14, 0x24) );	
        if( 0 == LPLD_SCCB_WriteReg(0x28, 0x20) );	
    }
    else
    {

        return FAILED;
    }

    return SUCCESS;
}
