/*
 * =====================================================================================
 *
 *       Filename:  USART_Middle_Module.c
 *
 *    Description:  Freescale Middle Module 
 *
 *        Version:  0.1
 *        Created:  2013/11/23 16时19分15秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio 
 *
 * =====================================================================================
 */

#include "common.h"
#include <stdlib.h>

#include "OSMessage.h"
#include "USART_Middle_Module.h"
#include "SDCard_Middle_Module.h"
#include "Angle_Controller.h"
#include "Speed_Controller.h"
//INT32S my_atoi(char* pstr);
extern OS_CPU_SR cpu_sr;
/* Local Functions */
INT8U calcFCS(INT8U *pBuf, INT8U len);
void USART_Driver_Init(void);
void USART_Rx_IRQ_Handler(void);
void ACK_TARGET_RESET_Response(void);
void ACK_PARAMETERS_SET_Response(INT8U *dat);
void Tx_Send(INT8U *dat, INT16U cnt);
void USART_Tx_IRQ_Handler(void);
void USART_Rx_IRQ_Handler(void);
void Trans_Stat(INT8U Type, double value);
void STAT_Submit(void );
void PARAM_Submit (void  );
void ACK_REP_STOP_Response(void);
void ACK_REP_START_Response(void);
/* Local Variable*/
OS_EVENT *USART_Controller_MBox;
OS_EVENT *Tx_Buffer_OSem;
OS_EVENT *Rx_Buffer_OSem;
INT8U     Rx_Buffer[RX_BUFFER_SIZE];
INT8U     Tx_Buffer[TX_BUFFER_SIZE];
INT8U     Rx_Buffer_Cnt = 0   ;
INT8U     Tx_Buffer_Ptr = 0   ;
INT8U     Tx_Buffer_Cnt = 0   ;
INT16U    USART_Stat    = 0   ;
float32_t *SPEED_P      = NULL;
float32_t *SPEED_I      = NULL;
float32_t *SPPED_D      = NULL;
float32_t *DIR_P        = NULL;
float32_t *DIR_I        = NULL;
float32_t *DIR_D        = NULL;
float32_t *ANGLE_P      = NULL;
float32_t *ANGLE_I      = NULL;
float32_t *ANGLE_D      = NULL;
INT32U    *VALUE_SPEED  = NULL;
float32_t *VALUE_ANGLE  = NULL;
INT32U    *VALUE_DIREC  = NULL;
INT32U    *VALUE_LMOTOR = NULL;
INT32U    *VALUE_RMOTOR = NULL;
float32_t *VALUE_GYRO   = NULL;
float32_t *VALUE_ACCER  = NULL;
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  USART_Driver_Task
 *  Description:  USART_MIDDLE_MODULE Task 
 * =====================================================================================
 */
void  USART_Param_Invoke_int32u(INT8U PARAM, INT32U *addr)
{
    if(PARAM == STAT_VALUE_SPEED ) VALUE_SPEED  =addr;
    if(PARAM == STAT_VALUE_DIREC ) VALUE_DIREC  =addr;
    if(PARAM == STAT_VALUE_LMOTOR) VALUE_LMOTOR =addr;
    if(PARAM == STAT_VALUE_RMOTOR) VALUE_RMOTOR =addr;



}
void  USART_Param_Invoke_float32(INT8U PARAM, float32_t *addr)
{
    if(PARAM == STAT_VALUE_ACCER ) VALUE_ACCER  =addr;
    if(PARAM == STAT_VALUE_GYRO  ) VALUE_GYRO   =addr;
    if(PARAM == STAT_SPEED_P     ) SPEED_P      =addr;
    if(PARAM == STAT_SPEED_I     ) SPEED_I      =addr;
    if(PARAM == STAT_SPEED_D     ) SPPED_D      =addr;
    if(PARAM == STAT_DIR_P       ) DIR_P        =addr;
    if(PARAM == STAT_DIR_I       ) DIR_I        =addr;
    if(PARAM == STAT_DIR_D       ) DIR_D        =addr;
    if(PARAM == STAT_ANGLE_P     ) ANGLE_P      =addr;
    if(PARAM == STAT_ANGLE_I     ) ANGLE_I      =addr;
    if(PARAM == STAT_ANGLE_D     ) ANGLE_D      =addr;
    if(PARAM == STAT_VALUE_ANGLE ) VALUE_ANGLE  =addr;

}
void  USART_Driver_Task (void *p_arg)
{
    (void)p_arg;
    UsartMsg_TypeDef *msg;
    INT8U err=0;
    INT16U i=0;
    INT8U dat_area[RX_BUFFER_SIZE];
    // Initialization of usart mode, isr sem and message queue
    //TODO
    USART_Driver_Init();
    printf("Initializating Uart Driver .... OK!\n");

    while(TRUE)                                   /* Task Circle */
    {//TODO here
        msg = (UsartMsg_TypeDef *)OSMboxPend(USART_Controller_MBox,0,&err);
        if(msg->Usart_Operation == CONTROLLER_DO)
        {
            STAT_Submit();
        }
        if(msg->Usart_Operation == USART_PROTOCOL_PROCESS )
        {
            USART_Stat += 1;
            /* Memory Copy */
            OSSemPend(Rx_Buffer_OSem,0,&err);
            for(i=0;i<Rx_Buffer[USART_LEN];i++)
            {
                dat_area[i] = Rx_Buffer[USART_DAT+i];
            }
            Rx_Buffer_Cnt=0;
            OSSemPost(Rx_Buffer_OSem);

            /* Verify the end of frame. */   
            if((Rx_Buffer[Rx_Buffer[USART_LEN]-2] == 0xFE ) &&
                    (Rx_Buffer[Rx_Buffer[USART_LEN]-1] == 0x01))
            {                                   
                if(dat_area[USART_OPT] == CONN_REQ) 
                    PARAM_Submit();
                if(dat_area[USART_OPT] == TARGET_RESET_REQ) 
                    ACK_TARGET_RESET_Response();
                if(dat_area[USART_OPT] == PARAMETERS_SET_REQ)
                {
                    ACK_PARAMETERS_SET_Response(dat_area);
                }
                if(dat_area[USART_OPT] == REP_START_REQ) 
                  ACK_REP_START_Response();      //TODO
                if(dat_area[USART_OPT] == REP_STOP_REQ)        //TODO
                    ACK_REP_STOP_Response(); 
                if(dat_area[USART_OPT] == REP_CAM_START) ;      //TODO

                if(dat_area[USART_OPT] == REP_CAM_STOP) ;       //TODO
                if(dat_area[USART_OPT] == STAT_REQ)             
                {
                    STAT_Submit();
                }
                if(dat_area[USART_OPT] == ACK_CAM) ;            //TODO
            }
        }
    }
}
#define ACK_FRAME_LEN 7
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ACK_PARAMETERS_SET_Response
 *  Description:  Response to PARAMETERS_SET_REQ.
 * =====================================================================================
 */
ControllerMsg_TypeDef Msg_Reset;
void ACK_PARAMETERS_SET_Response(INT8U *dat)
{
    INT16U ptr=0;
    INT8U str[30];
    double ret=0.0;
    (void)ret;
    if(dat[2] == '/')
    {
        while(dat[3+(ptr)]!='/' && ptr<20)
        {
            str[ptr] = dat[3+(ptr)];
            ptr++;
        }
        str[ptr+1]='\0';
        ret = atoi((char *)str);
        
    }
    ret /= 1000;
    if( (dat[1] == STAT_SPEED_P) &&  (SPEED_P!=NULL)  ) *SPEED_P = ret;
    if( (dat[1] == STAT_SPEED_I) &&  (SPEED_I!=NULL)  ) *SPEED_I = ret;
    if( (dat[1] == STAT_SPEED_D) &&  (SPPED_D!=NULL)  ) *SPPED_D = ret;
    if( (dat[1] == STAT_DIR_P  ) &&  (DIR_P  !=NULL)  ) *DIR_P   = ret;
    if( (dat[1] == STAT_DIR_I  ) &&  (DIR_I  !=NULL)  ) *DIR_I   = ret;
    if( (dat[1] == STAT_DIR_D  ) &&  (DIR_D  !=NULL)  ) *DIR_D   = ret;
    if( (dat[1] == STAT_ANGLE_P) &&  (ANGLE_P!=NULL)  ) *ANGLE_P = ret;
    if( (dat[1] == STAT_ANGLE_I) &&  (ANGLE_I!=NULL)  ) *ANGLE_I = ret;
    if( (dat[1] == STAT_ANGLE_D) &&  (ANGLE_D!=NULL)  ) *ANGLE_D = ret;
    Msg_Reset.Controller_Msg_Type = CONTROLLER_CHANGE;
    OSMboxPost(Angle_Controller_MBox,(void *)&Msg_Reset);
    OSMboxPost(Speed_Controller_MBox,(void *)&Msg_Reset);
    
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ACK_TARGET_RESET_Response
 *  Description:  Response to TARGET_RESET_REQ
 * =====================================================================================
 */
void ACK_TARGET_RESET_Response(void)
{
    INT8U buf[ACK_FRAME_LEN];
    buf[USART_SOF] = 'X';
    buf[USART_SOF2] = 'Z';
    buf[USART_LEN] = ACK_FRAME_LEN;
    buf[USART_DAT] = ACK_TARGET_RESET;
    buf[ACK_FRAME_LEN -3] = calcFCS(&buf[USART_DAT], 1);
    buf[ACK_FRAME_LEN -2] = 0xFE;
    buf[ACK_FRAME_LEN -1] = 0x01;
    Tx_Send(buf, ACK_FRAME_LEN);
}
void Trans_Stat(INT8U Type, double value)
{
    INT8U buf[40];
    INT32U len;
    buf[USART_SOF] = 'X';
    buf[USART_SOF2] = 'Z';
    buf[USART_DAT] = TRANS_STAT;
    buf[USART_DAT +1] = Type;
    len = sprintf((signed char *)&buf[USART_DAT + 2],"/%d/",(INT32U)(value*1000));
    buf[USART_LEN] = len + 7;
    buf[len+5] = 0xFE;
    buf[len+6] = 0x01;
    Tx_Send(buf,len+7);
}
ControllerMsg_TypeDef usart_message_to_sdcard;
void ACK_REP_START_Response(void)
{
    INT8U buf[ACK_FRAME_LEN];
    buf[USART_SOF] = 'X';
    buf[USART_SOF2] = 'Z';
    buf[USART_LEN] = ACK_FRAME_LEN;
    buf[USART_DAT] = ACK_REP_START;
    buf[ACK_FRAME_LEN -3] = calcFCS(&buf[USART_DAT], 1);
    buf[ACK_FRAME_LEN -2] = 0xFE;
    buf[ACK_FRAME_LEN -1] = 0x01;
    Tx_Send(buf, ACK_FRAME_LEN);
    usart_message_to_sdcard.Controller_Msg_Type = SDCARD_TASK_PAUSE;
    OSMboxPost(SDCard_MBox,&usart_message_to_sdcard);
}
void ACK_REP_STOP_Response(void)
{
    INT8U buf[ACK_FRAME_LEN];
    buf[USART_SOF] = 'X';
    buf[USART_SOF2] = 'Z';
    buf[USART_LEN] = ACK_FRAME_LEN;
    buf[USART_DAT] = ACK_REP_STOP;
    buf[ACK_FRAME_LEN -3] = calcFCS(&buf[USART_DAT], 1);
    buf[ACK_FRAME_LEN -2] = 0xFE;
    buf[ACK_FRAME_LEN -1] = 0x01;
    Tx_Send(buf, ACK_FRAME_LEN);
    usart_message_to_sdcard.Controller_Msg_Type = SDCARD_TASK_STOP;
    OSMboxPost(SDCard_MBox,&usart_message_to_sdcard);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Tx_Send
 *  Description:  Send character stream to USART Port. K60 Register Related.
 * =====================================================================================
 */
void Tx_Send(INT8U *dat, INT16U cnt)
{
    int i;
    INT8U err;
    if(cnt>0 && cnt <TX_BUFFER_SIZE)
    {
        OSSemPend(Tx_Buffer_OSem, 0 ,&err);     /* Get the TX Semaphore */

        for ( i = 0; i < cnt; i += 1 )
            Tx_Buffer[i] = dat[i];              /* Memory Copy */
        Tx_Buffer_Cnt = cnt;
        Tx_Buffer_Ptr = 0;
        USART_MIDDLE_MODULE_PORT -> C2 |= (UART_C2_TIE_MASK); /* Enable Send Interrupts. */

    } 
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  USART_Driver_Init
 *  Description:  initialization of usart  
 * =====================================================================================
 */
void USART_Driver_Init (void  )
{
    UART_InitTypeDef UART_InitStructure;
    Tx_Buffer_OSem = OSSemCreate(1);
    OSSemPost(Tx_Buffer_OSem);                       /* create tx semaphore and post */
    Rx_Buffer_OSem = OSSemCreate(1);
    OSSemPost(Rx_Buffer_OSem);                       /* create rx semaphore and post */
    USART_Controller_MBox = OSMboxCreate((void *)0); /* create USART Mbox */
    UART_InitStructure.UART_Uartx = USART_MIDDLE_MODULE_PORT ;
    UART_InitStructure.UART_BaudRate = USART_BAUD_RATE;
    UART_InitStructure.UART_RxPin = PTE9;
    UART_InitStructure.UART_TxPin = PTE8;
    UART_InitStructure.UART_RxIntEnable = TRUE;
    UART_InitStructure.UART_RxIsr = USART_Rx_IRQ_Handler;
    UART_InitStructure.UART_TxIsr = USART_Tx_IRQ_Handler;
    UART_InitStructure.UART_TxIntEnable = TRUE;
    LPLD_UART_Init(UART_InitStructure);
    LPLD_UART_EnableIrq(UART_InitStructure);
}		/* -----  end of function USART_Driver_Init  ----- */

void PARAM_Submit (void  )
{
    Msg_Reset.msg =(void *) CONTROLLER_CHANGE;
    if(SPEED_P     != NULL) Trans_Stat(STAT_SPEED_P     ,*SPEED_P     );
    if(SPEED_I     != NULL) Trans_Stat(STAT_SPEED_I     ,*SPEED_I     );
    if(SPPED_D     != NULL) Trans_Stat(STAT_SPEED_D     ,*SPPED_D     );
    if(DIR_P       != NULL) Trans_Stat(STAT_DIR_P       ,*DIR_P       );
    if(DIR_I       != NULL) Trans_Stat(STAT_DIR_I       ,*DIR_I       );
    if(DIR_D       != NULL) Trans_Stat(STAT_DIR_D       ,*DIR_D       );
    if(ANGLE_P     != NULL) Trans_Stat(STAT_ANGLE_P     ,*ANGLE_P     );
    if(ANGLE_I     != NULL) Trans_Stat(STAT_ANGLE_I     ,*ANGLE_I     );
    if(ANGLE_D     != NULL) Trans_Stat(STAT_ANGLE_D     ,*ANGLE_D     );

}		/* -----  end of function PARAM_Submit  ----- */
/*-----------------------------------------------------------------------------
 *  Functon:        STAT_Submit
 *  Description:    Submit all Parameters to Upper Computer.
 ALL Parameters should be INT32U and Invoked.
 *-----------------------------------------------------------------------------*/
void STAT_Submit (void  )
{
    
    if(VALUE_ANGLE  != NULL) Trans_Stat(STAT_VALUE_ANGLE ,(double)*VALUE_ANGLE );
    if(VALUE_SPEED  != NULL) Trans_Stat(STAT_VALUE_SPEED ,(double)*VALUE_SPEED );
    if(VALUE_DIREC  != NULL) Trans_Stat(STAT_VALUE_DIREC ,(double)*VALUE_DIREC );
    if(VALUE_LMOTOR != NULL) Trans_Stat(STAT_VALUE_LMOTOR,(double)*VALUE_LMOTOR);
    if(VALUE_RMOTOR != NULL) Trans_Stat(STAT_VALUE_RMOTOR,(double)*VALUE_RMOTOR);
    if(VALUE_GYRO   != NULL) Trans_Stat(STAT_VALUE_GYRO  ,(double)*VALUE_GYRO  );
    if(VALUE_ACCER  != NULL) Trans_Stat(STAT_VALUE_ACCER ,(double)*VALUE_ACCER );
}		/* -----  end of function STAT_Submit  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  USART_Rx_IRQ_Handler
 *  Description:    Interrupt Process Function.
 Process Frame, Save Received data to Rx_buffer, Send OSMessage.
 * =====================================================================================
 */
UsartMsg_TypeDef umsg;
void USART_Rx_IRQ_Handler (void  )
{
    if(OSSemAccept(Rx_Buffer_OSem) != 0)
    {
        Rx_Buffer[Rx_Buffer_Cnt++] = LPLD_UART_GetChar(USART_MIDDLE_MODULE_PORT);
        if(Rx_Buffer_Cnt >=2)  //Get the Frame Head.
        {
            if(Rx_Buffer[0]!='X' || Rx_Buffer[1]!='Z') /* not get header */
            { // Fixed header.

                Rx_Buffer[0] = Rx_Buffer[1];
                Rx_Buffer_Cnt = 1;
            }
        }
        if(Rx_Buffer_Cnt > USART_LEN+1 && (Rx_Buffer_Cnt >= Rx_Buffer[USART_LEN])) /* Get Enough Frame . */
        {
            umsg.Usart_Operation = USART_PROTOCOL_PROCESS;
            OSMboxPost(USART_Controller_MBox,(void *)&umsg);
            Rx_Buffer_Cnt = 0;
        }
        else
        {
            if((Rx_Buffer[Rx_Buffer_Cnt -2] == 0xFE) && (Rx_Buffer[Rx_Buffer_Cnt -1] ==0x01))
            {
                Rx_Buffer_Cnt = 0; //Clear Rx_Buffer . When found the frame end.
            }

        }
        OSSemPost(Rx_Buffer_OSem);
    }
}		/* -----  end of function USART_IRQ_Handler  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  USART_Tx_IRQ_Handler
 *  Description:  Send Serial Data via Interrupt Method.
 * =====================================================================================
 */

void USART_Tx_IRQ_Handler (void  )
{ 
    if(Tx_Buffer_Cnt !=0)
    {
        LPLD_UART_PutChar(USART_MIDDLE_MODULE_PORT, Tx_Buffer[Tx_Buffer_Ptr++]);
        Tx_Buffer_Cnt--;
    }
    else
    {
        OS_ENTER_CRITICAL();
        USART_MIDDLE_MODULE_PORT -> C2 &=~(UART_C2_TIE_MASK); /* Close Usart Send Interrupts */
        Tx_Buffer_Ptr = 0;
        OS_EXIT_CRITICAL();
        OSSemPost(Tx_Buffer_OSem);
    }

}		/* -----  end of function USART_Tx_IRQ_Handler  ----- */
static INT8U calcFCS(INT8U *pBuf, INT8U len)
{
    uint8_t rtrn = 0;

    while (len--)
    {
        rtrn ^= *pBuf++;
    }

    return rtrn;
}
