/*
 * =====================================================================================
 *
 *       Filename:  LCD_Controller.c
 *
 *    Description:  LCD Controller.
 *
 *        Version:  1.0
 *        Created:  2014/04/09 09时12分44秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio.
 *
 * =====================================================================================
 */
#include "DEV_SSD1306.h"
INT8U LCD_Screen[20][8];
void LCD_Write(INT8U line, (INT8U *) ch)
{

    INT8U ctr=0;
    assert(line >= 0 && line <= 8);
    while(ctr++ < 20 && ch != '\0')
    {

    }
   

}

