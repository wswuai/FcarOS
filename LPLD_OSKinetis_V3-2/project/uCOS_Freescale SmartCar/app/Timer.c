/*
 * =====================================================================================
 *
 *       Filename:  Timer.c
 *
 *    Description:  Time Support For Smartcar System.
 *
 *        Version:  1.0
 *        Created:  2013/12/17 09时20分14秒
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
#include "Timer.h"

/*-----------------------------------------------------------------------------
 *  Private Structure Defines.
 *-----------------------------------------------------------------------------*/
typedef struct
{
    INT8U valid;
    INT32U resi_ms;
    void *Msg;
    OS_EVENT *Mbox;
}Msg_Node;

/*-----------------------------------------------------------------------------
 *  Create Memory for Wait List.
 *-----------------------------------------------------------------------------*/
Msg_Node Wait_List[TIMER_LENGTH];
INT32U Wait_List_Length=0;
INT32U Next_Valid_Node=0;
INT8U Send_Msg_After_MS(OS_EVENT *mBox,(void *)msg,INT32U ms)
{
    if(Wait_List_Length >= TIMER_LENGTH)
        return FAILED;
    else
    {
        OS_ENTER_CRITICAL();                    /* Start Operate Global Variables */
        if(Next_Valid_Node!=0&&Wait_List[Next_Valid_Node].valid==0)
        {
            //TODO
            Wait_List[Next_Valid_Node].resi_ms = ms;
            Wait_List[Next_Valid_Node].Msg = msg;
            Wait_List[Next_Valid_Node].Mbox = mBox;
            Wait_List[Next_Valid_Node].valid = 1;
        }
        OS_EXIT_CRITICAL();
    }

}  
void Timer_Task((void *)p_arg)
{

    (void)p_arg;
    int i;
    if(Wait_List_Length != 0)
    {
        
        for ( i = 0; i < Wait_List_Length; i += 1 )
        {
            if((Wait_List[i].resi_ms--)!=0)
                continue;
            else
            {
                OS_ENTER_CRITICAL();
                OSMboxPost(Wait_List[i].Mbox,Wait_List[i].Msg);
            }

        }

    }
    else
        return;

}
