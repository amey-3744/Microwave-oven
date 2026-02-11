/* 
 * File:   isr.c
 */

#include <xc.h>
#include "main.h"

unsigned int count; 
extern unsigned char sec, min; 
extern unsigned char pre_heat_time; 
void __interrupt() isr(void)
{
    if (TMR2IF) 
    {
        if (++count == 20000) // 250, 200ns 8 bit timer for i sec 
        {
            count = 0;
            
            //for every 1 sec, decrement swc
            if (sec != 0)
                sec--;
            else if (sec == 0 && min != 0) 
            {
                min = 0;
                sec = 60;
            }
            //if sec is 0, decrement min
            else if(pre_heat_time != 0)
            {
                pre_heat_time--;
            }
        }
    
        TMR2IF = 0;
    }
}