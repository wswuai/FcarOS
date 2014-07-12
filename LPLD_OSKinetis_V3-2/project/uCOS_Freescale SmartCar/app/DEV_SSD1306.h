#ifndef _LQOLED_H
#define _LQOLED_H
#define     LCD_SCL_H()     LPLD_GPIO_Output_b(PTA,27,1)
#define     LCD_SCL_L()     LPLD_GPIO_Output_b(PTA,27,0)
#define     LCD_SDA_H()     LPLD_GPIO_Output_b(PTA,26,1)
#define     LCD_SDA_L()     LPLD_GPIO_Output_b(PTA,26,0)
#define     LCD_RST_H()     LPLD_GPIO_Output_b(PTA,25,1)
#define     LCD_RST_L()     LPLD_GPIO_Output_b(PTA,25,0)
#define     LCD_DC_H()      LPLD_GPIO_Output_b(PTA,24,1)
#define     LCD_DC_L()      LPLD_GPIO_Output_b(PTA,24,0)
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 


 extern INT8U longqiu96x64[768];
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(INT8U x,INT8U y,INT8U ch[]);
 void LCD_P8x16Str(INT8U x,INT8U y,INT8U ch[]);
 void LCD_P14x16Str(INT8U x,INT8U y,INT8U ch[]);
 void LCD_Print(INT8U x, INT8U y, INT8U ch[]);
 void LCD_PutPixel(INT8U x,INT8U y);
 void LCD_Rectangle(INT8U x1,INT8U y1,INT8U x2,INT8U y2,INT8U gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(INT8U x0,INT8U y0,INT8U x1,INT8U y1,INT8U bmp[]); 
 void LCD_Fill(INT8U dat);


#define X_WIDTH 128
#define Y_WIDTH 64

#endif
