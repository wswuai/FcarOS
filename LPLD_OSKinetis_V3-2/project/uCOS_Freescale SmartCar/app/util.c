/*
 * =====================================================================================
 *
 *       Filename:  util.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/12/22 20时34分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  
 *
 * =====================================================================================
 */

/*-----------------------------------------------------------------------------
 *  Convert String to integer
 *-----------------------------------------------------------------------------*/
INT32U atoi(char *str)
{
    int ret=0;
    while(*str!='\0')
    {
        ret = ret*10 + (*str++)-48;
    }
    return ret;
/* 
* name:xif 
* coder:xifan@2010@yahoo.cn 
* time:08.20.2012 
* file_name:my_atoi.c 
* function:int my_atoi(char* pstr) 
*/  
  
INT32S my_atoi(char* pstr)  
{  
    int Ret_Integer = 0;  
    int Integer_sign = 1;  
      
    /* 
    * 判断指针是否为空 
    */  
    if(pstr == NULL)  
    {  
        printf("Pointer is NULL\n");  
        return 0;  
    }  
      
    /* 
    * 跳过前面的空格字符 
    */  
    while(isspace(*pstr) == 0)  
    {  
        pstr++;  
    }  
      
    /* 
    * 判断正负号 
    * 如果是正号，指针指向下一个字符 
    * 如果是符号，把符号标记为Integer_sign置-1，然后再把指针指向下一个字符 
    */  
    if(*pstr == '-')  
    {  
        Integer_sign = -1;  
    }  
    if(*pstr == '-' || *pstr == '+')  
    {  
        pstr++;  
    }  
      
    /* 
    * 把数字字符串逐个转换成整数，并把最后转换好的整数赋给Ret_Integer 
    */  
    while(*pstr >= '0' && *pstr <= '9')  
    {  
        Ret_Integer = Ret_Integer * 10 + *pstr - '0';  
        pstr++;  
    }  
    Ret_Integer = Integer_sign * Ret_Integer;  
      
    return Ret_Integer;  
}  }
