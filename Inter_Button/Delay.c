 /*
 ******************************************************************************
  * @file    led.c
  * @author  Teddy
  * @version V1.0
  * @date    July 4, 2007
  ******************************************************************************
	*/

#include "Delay.h"

static __IO uint32_t TimingDelay;

void Systick_Configuration(void)
{
	if(SysTick_Config(SystemCoreClock/1000)) // 1 tick 1 ms
	{
		while(1); // if fail
	}
}

void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
 
  while(TimingDelay != 0);
}
void SysTick_Handler(void)
{
    if(TimingDelay)  TimingDelay--; // TimingDelay giam di 1
}
