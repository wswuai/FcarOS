/*
 * =====================================================================================
 *
 *       Filename:  OSMessage.h
 *
 *    Description:  OSMessage Defines hader file. 
 *
 *        Version:  0.1
 *        Created:  2013/11/24 15时03分16秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio 
 *
 * =====================================================================================
 */

//Parameters Defines:
#define SPEEDPID_PROPORTION              0.1
#define SPEEDPID_DERIVATIVE              0.1
#define SPEEDPID_INTEGRAL                0.0
#define SPEEDPID_CARSPEEDSET             0.0
#define ANGLEPID_PROPORTION              90.0
#define ANGLEPID_DERIVATIVE              4.00
#define ANGLEPID_INTEGRAL                0.00
#define ANGLEPID_ANGLESET                2120
#define DIRPID_PROPORTION                1.00
#define DIRPID_DERIVATIVE                0.01
#define DIRPID_INTEGRAL                  1.00
#define DIRPID_DIRSET                    1.00

//Angle Controller Parameters :
#define GRAVITY_ANGLE_RATIO              2.0
#define GYRO_ANGLE_RATIO                 0.15
#define GRAVITY_ADJUST_TIME_CONSTANT     30
#define GYRO_INTEGRAL_FREQUENCY          5
#define GRAVITY_OFFSET                   200

//OSMESSAGE Defines:
#define CONTROLLER_DO                    1u 
#define CONTROLLER_CHANGE                3u
#define CONTROLLER_SHUTDOWN              4u
#define SDCARD_PTR_INJECTION             5u
#define SDCARD_TASK_PAUSE                6u
#define SDCARD_TASK_STOP                 7u

#define CONTROLLER_INTEGRATE             8u
#define SDCARD_REC_REQ                   9u
#define USART_PROTOCOL_PROCESS           20u

#define SUCCESS                          0u
#define FAILED                           1u
//Hardware Operation :
#define _BEEP_ON()    LPLD_GPIO_Output_b(PTD,13,1)
#define _BEEP_OFF()   LPLD_GPIO_Output_b(PTD,13,0)
typedef struct
{
    INT32U Controller_Msg_Type;
    void *msg;
}ControllerMsg_TypeDef;

typedef struct
{
    INT32U Usart_Operation;
    void *msg;
}UsartMsg_TypeDef;
typedef struct
{
    INT32U width;
    INT32U height;
    INT8U* ptr;
}sdcard_Injection_TypeDef;
