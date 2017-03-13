#include "my_stm32f4_discovery.h"
#include "main.h"

  GPIO_InitTypeDef  GPIO_InitStructure;

  int main(void)
  {
    GPIO_Configuration();
    SysTick_Config(SystemCoreClock / 1000);
    while (1)
    {
      GPIO_ToggleBits(GPIOD ,GPIO_Pin_12 | GPIO_Pin_14);
      Delay(1000000);
    }
  }
  
  void GPIO_Configuration(void)
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD |RCC_AHB1Periph_GPIOA , ENABLE);
      /* Configure PB0 PB1 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* Configure PA0 in input mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  }
  void Delay(__IO uint32_t nCount)
  {
    while(nCount--)
    {
    }
  }
  
  