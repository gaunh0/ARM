#ifndef _LED_H_
#define _LED_H_

#include "main.h"
#include "stm32f4xx_conf.h"

#define LED_Green  LED1
#define LED_Red LED2

#define LEDn 						2

#define LED1_PIN 		GPIO_Pin_13
#define LED1_PORT			  GPIOG
#define LED1_CLK				RCC_AHB1Periph_GPIOG

#define LED2_PIN 				GPIO_Pin_14
#define LED2_PORT			 	GPIOG
#define LED2_CLK				RCC_AHB1Periph_GPIOG

typedef enum {
   LED1 = 0 ,
	 LED2 = 1
} LED_Typedef;

void LED_init(LED_Typedef LEDx);
uint8_t LED_Status(LED_Typedef LEDx);
void LED_On(LED_Typedef LEDx);
void LED_Off(LED_Typedef LEDx);
void LED_Togg(LED_Typedef LEDx);

#endif
