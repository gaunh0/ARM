#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx_conf.h"

void Systick_Configuration(void);
void SysTick_Handler(void);
void Delay(__IO uint32_t nTime);

#endif
