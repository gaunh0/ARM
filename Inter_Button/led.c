 /*
 ******************************************************************************
  * @file    led.c
  * @author  Teddy
  * @version V1.0
  * @date    July 4, 2007
  ******************************************************************************
	*/
#include "led.h"

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_PORT,LED2_PORT};
const uint16_t GPIO_PIN[LEDn] ={LED1_PIN,LED2_PIN};
const uint16_t GPIO_CLK[LEDn] ={LED1_CLK,LED2_CLK};

void LED_init(LED_Typedef LEDx)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(GPIO_CLK[LEDx], ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN[LEDx];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIO_PORT[LEDx], &GPIO_InitStructure);
}
//
uint8_t LED_Status(LED_Typedef LEDx)
{
	return GPIO_ReadOutputDataBit(GPIO_PORT[LEDx] , GPIO_PIN[LEDx]); 
}
//
void LED_Off(LED_Typedef LEDx)
{
	GPIO_ResetBits(GPIO_PORT[LEDx] , GPIO_PIN[LEDx]);
}
//
void LED_On(LED_Typedef LEDx)
{
	GPIO_SetBits(GPIO_PORT[LEDx] , GPIO_PIN[LEDx]);
}
//
void LED_Togg(LED_Typedef LEDx)
{
	GPIO_ToggleBits(GPIO_PORT[LEDx] , GPIO_PIN[LEDx]);
}
