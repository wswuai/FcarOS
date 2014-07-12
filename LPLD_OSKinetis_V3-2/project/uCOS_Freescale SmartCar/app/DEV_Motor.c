/*
 * =====================================================================================
 *
 *       Filename:  DEV_Motor.c
 *
 *    Description:  Motor Dev Driver
 *
 *        Version:  1.0
 *        Created:  2013/11/25 21时32分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */
#include "common.h"
#include "DEV_Motor.h"
static FTM_InitTypeDef ftm_init_struct;
void Motor_Init(void)
{
    ftm_init_struct.FTM_Ftmx = FTM0;	
    ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	
    ftm_init_struct.FTM_PwmFreq = 7500 ;
    LPLD_FTM_Init(ftm_init_struct);
    LPLD_FTM_PWM_Enable(FTM0, 
            FTM_Ch0, 
            0, 
            PTC1, 
            ALIGN_LEFT        
            );    
    LPLD_FTM_PWM_Enable(FTM0, 
            FTM_Ch1, 
            0, 
            PTC2, 
            ALIGN_LEFT        
            );    
    LPLD_FTM_PWM_Enable(FTM0, 
            FTM_Ch2, 
            0, 
            PTC3, 
            ALIGN_LEFT        
            );    
    LPLD_FTM_PWM_Enable(FTM0, 
            FTM_Ch3, 
            0, 
            PTC4, 
            ALIGN_LEFT        
            );    
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Motor_Set
 *  Description:  Set PWM to Motor
 * =====================================================================================
 */
void Motor_Set(INT32S pwm,INT8U dir)// Called by Strategy Controller . Called Routine.
{
  
    //Dead Area fixed
    if(pwm > 0 && dir == MOTOR_LEFT ) pwm +=900;
    if(pwm < 0 && dir == MOTOR_LEFT ) pwm -=850;
    if(pwm > 0 && dir == MOTOR_RIGHT) pwm +=750;
    if(pwm < 0 && dir == MOTOR_RIGHT) pwm -=700;
  
  
    
    if(pwm >=10000) pwm = 10000;
    if(pwm <= -10000) pwm = -10000;
    
    if(MOTOR_LEFT == dir)
    {
        if(pwm>0)
        {
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch0, 
                    pwm, 
                    PTC1, 
                    ALIGN_LEFT        
                    );    
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch1, 
                    0, 
                    PTC2, 
                    ALIGN_LEFT        
                    );    
        }
        else
        {
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch0, 
                    0, 
                    PTC1, 
                    ALIGN_LEFT        
                    );    
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch1, 
                    -pwm, 
                    PTC2, 
                    ALIGN_LEFT        
                    );    
        }
    }
    else
    {
        if(pwm>0)
        {
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch2, 
                    0, 
                    PTC3, 
                    ALIGN_LEFT        
                    );    
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch3, 
                    pwm, 
                    PTC4, 
                    ALIGN_LEFT        
                    );    
        }
        else
        {
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch2, 
                    -pwm, 
                    PTC3, 
                    ALIGN_LEFT        
                    );    
            LPLD_FTM_PWM_Enable(FTM0, 
                    FTM_Ch3, 
                    0, 
                    PTC4, 
                    ALIGN_LEFT        
                    );    
        }
    }
}
