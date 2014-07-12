/*
 * =====================================================================================
 *
 *       Filename:  USART_Middle_Module.h
 *
 *    Description:  USART_MIDDLE_MODULE header file
 *                  This Module use the pin : PTE8 PTE9
 *
 *        Version:  0.1
 *        Created:  2013/11/23 16时20分11秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio
 *
 * =====================================================================================
 */
#ifndef USART_MIDDLE_MODULE_H
#define USART_MIDDLE_MODULE_H

#define USART_MIDDLE_MODULE_PORT UART5
#define USART_BAUD_RATE 115200

//Export Function:
extern OS_EVENT *USART_Controller_MBox;
void USART_Driver_Task(void *p_arg);
void USART_Param_Invoke_int32u(INT8U PARAM, INT32U *addr);
void  USART_Param_Invoke_float32(INT8U PARAM, float32_t *addr);
void Tx_Send(INT8U *dat, INT16U cnt);
extern INT16U USART_Stat;
//Transform Frame of USART Protocol:
/*
 |USART_SOF| USART_SOF2|USART_LEN|USART_DAT|FCS|EOF |EOF2|
 |   0x58  |   0x5A    |    ~    |    ~    | ~ |0xFE|0x01|
 */
#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 256
// Communication protocol : Character Position
#define USART_SOF           0x00
#define USART_SOF2          0x01
#define USART_LEN           0x02
#define USART_DAT           0x03
// Communication Protocol : Offset : Data Frame
#define USART_OPT           0x00
// Communication protocol : (Up->Down) Value : USART_OPT
#define CONN_REQ            0x00
#define TARGET_RESET_REQ    0x01
#define PARAMETERS_SET_REQ  0x02
#define REP_START_REQ       0x03
#define REP_STOP_REQ        0x04
#define REP_CAM_START       0x05
#define REP_CAM_STOP        0x06
#define STAT_REQ            0x07
#define ACK_CAM             0x10
// Communication protocol :(Down->Up) Value : USART_OPT
// Name     : Operation Type
// Location : USART_DAT + 0
#define ACK_CONN            0x00
#define ACK_TARGET_RESET    0x01
#define ACK_PARAMETERS_SET  0x02
#define ACK_REP_START       0x03
#define ACK_REP_STOP        0x04
#define ACK_REP_CAM_START   0x05
#define ACK_REP_CAM_STOP    0x06
#define TRANS_STAT          0x07
#define TRANS_CAM           0x08

// Communication Protocol Sample When Trans STAT_DAT./Parameters_set
// |EOF |EOF2|Len|               DATA AREA                                | EOF | EOF2|
// |EOF |EOF2|Len| TRANS_STAT | ValueType (STAT_SPEED_P) | '/' 12345  '/' | EOF | EOF2|
// |0x58|0x5A| ~ |    0x07    | 0x02                     |   Char Stream  | 0xFE| 0x01|
// |0x58|0x5A| ~ |    0x02    | 0x02                     | '/' 12345  '/' | 0xFE| 0x01A
// Communication Protocol : (Down->Up)Value : (USART_DAT+1) WHEN USART_OPT == TRANS_STAT 
// Name     : Value                                                        == PARAMETERS_SET
// Location : USART_DAT +1
#define CAR_TYPE                   0x01
#define STAT_SPEED_P               0x02
#define STAT_SPEED_I               0x03
#define STAT_SPEED_D               0x04
#define STAT_DIR_P                 0x05
#define STAT_DIR_I                 0x06
#define STAT_DIR_D                 0x07
#define STAT_ANGLE_P               0x08
#define STAT_ANGLE_I               0x09
#define STAT_ANGLE_D               0x0a
#define STAT_VALUE_SPEED           0x0b
#define STAT_VALUE_ANGLE           0x0c
#define STAT_VALUE_DIREC           0x0d
#define STAT_VALUE_LMOTOR          0x0e
#define STAT_VALUE_RMOTOR          0x0f
#define STAT_VALUE_GYRO            0x11
#define STAT_VALUE_ACCER           0x13
//Communication Protocol : Offset Of USART_DAT WHEN USART_OPT == TRANS_STAT
// Name     : Value (String Stream )
// Location :( USART_DAT +3 )~ ( USART_DAT + 3 + Strlen(Value) +1)
#define STAT_VALUE_START    ((USART_DAT)+ 0x02)
#define STAT_VALUE          ((USART_DAT)+ 0x03)
#endif
// |0x58|0x5A| ~ |    0x02    | 0x02                     | '/' 12345  '/' | 0xFE| 0x01|
