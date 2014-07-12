/*
 * =====================================================================================
 *
 *       Filename:  SDCard_Middle_Module.c
 *
 *    Description:  SDCARD_MIDDLE_MODULE
 *
 *        Version:  0.1
 *        Created:  2013/11/24 22时06分34秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio
 *
 * =====================================================================================

 */
#include "common.h"
#include "OSMessage.h"
#include "SDCard_Middle_Module.h"
#include "USART_Middle_Module.h"
#include "ff.h"
//Local Defines:
#define SDCARD_NOINIT   0
#define SDCARD_READY    1
#define SDCARD_RECING   2
#define SDCARD_ERR      3
#define SDCARD_NOINJECT 4
//Global Variables
OS_EVENT *SDCard_MBox;
//Local Variables
FRESULT rc;			
FATFS fatfs;			
FIL fil;			
UINT bw, br;
BYTE buff[128];
INT8U taskStat=SDCARD_NOINIT;
INT8U *Image_Pointer;
INT32U Image_Width=0;
INT32U Image_Height=0;

//Local Functions:
INT32U SDCard_Driver_Init(void);
void die(FRESULT rc)
{
  printf("err code =%u.\n", rc);
  for (;;) ;
}
DWORD get_fattime (void)
{
  return ((DWORD)(2013 - 1980) << 25)	
       | ((DWORD)3 << 21)              
       | ((DWORD)15 << 16)            
       | ((DWORD)0 << 11)
       | ((DWORD)0 << 5)
       | ((DWORD)0 >> 1);
}
INT8U *ptr;
void SDCard_Driver_Task (void  *p_arg)
{

    INT8U err=0;
    ControllerMsg_TypeDef *msg;
    printf("initializating SDCard...");
    if( SUCCESS == SDCard_Driver_Init())
    {
        printf("OK\n");
        LPLD_GPIO_Output_b(PTD,15,0);
    }
    else
    {
        taskStat = SDCARD_ERR;
        printf("Failed");
        return ;
    }
    for(;;)
    {//TODO
        msg = (ControllerMsg_TypeDef *)OSMboxPend(SDCard_MBox,0,&err);
        if(msg->Controller_Msg_Type == CONTROLLER_DO)
        {
            if(taskStat == SDCARD_RECING)
            {
                ptr=Image_Pointer;
                rc = f_write(&fil, "\xff", 1, &bw);
                rc = f_write(&fil, ptr, 76800, &bw);
                LPLD_GPIO_Toggle_b(PTD,10);
            }
    }
        
        if(msg->Controller_Msg_Type == SDCARD_PTR_INJECTION)
        {
            Image_Width = ((sdcard_Injection_TypeDef *)(msg->msg))->width;
            Image_Height = ((sdcard_Injection_TypeDef *)(msg->msg))->height;
            Image_Pointer = ((sdcard_Injection_TypeDef *)(msg->msg))->ptr;
            taskStat = SDCARD_READY;
            printf("PTR injected!\r\n");
        }
        if(msg->Controller_Msg_Type == SDCARD_TASK_STOP)
        {
            rc = f_close(&fil);
            taskStat = SDCARD_NOINIT;
            printf("Stop recing!");
        }
        if(msg->Controller_Msg_Type == SDCARD_TASK_PAUSE)
        {
            printf("pause request!\r\n");
            if(taskStat == SDCARD_READY)
            { 
              taskStat = SDCARD_RECING;
              printf("start recing!\n");
            }
        }
    }
}		/* -----  end of function SDCard_Driver_Task  ----- */

void SDCard_Close(void)
{
    rc = f_close(&fil);
    if(rc) printf("Close SDCard Failed, ERROR Code:  %u \n",rc);

}


INT32U SDCard_Driver_Init (void  )
{
    SDCard_MBox = OSMboxCreate(0);
    rc = f_mount(0, &fatfs);		
    if(rc) printf("mount failed, err code:%d\n",rc);
    rc = f_open(&fil, "0:/record.dat", FA_WRITE | FA_CREATE_ALWAYS);
    if (rc)
    { printf("File Open Failed...ERROR Code :%u\n",rc);return rc;}
    return SUCCESS;
}		/* -----  end of function SDCard_Driver_Init  ----- */

