/*
	USARTx       Tx  				Rx
	-----------------------------
	USART 1      PA9				PA10
	USART 2			 PA2				PA3
	USART 3			 PB10				PA11
*/

#ifndef __UART_H
#define __UART_H
	
#include "stm32f4xx_conf.h"
#include "stdio.h"
#include "main.h"
	
#ifdef __GNUC__
		/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
		set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
	#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
	#endif /* __GNUC__ */


typedef enum
{
	UART1 = 0,
	UART2 = 1,
	UART3 = 2
}	UART_TypeDef;

#define UARTn 							3

#define UART1_PIN_TX				GPIO_Pin_9
#define UART1_PIN_RX				GPIO_Pin_10
#define UART1_PORT					GPIOA
#define UART1_UART_CLK			RCC_APB2Periph_USART1
#define UART1_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define UART1_IRQ						USART1_IRQn

#define UART2_PIN_TX				GPIO_Pin_2
#define UART2_PIN_RX				GPIO_Pin_3
#define UART2_PORT					GPIOA
#define UART2_UART_CLK			RCC_APB1Periph_USART2
#define UART2_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define UART2_IRQ						USART2_IRQn

#define UART3_PIN_TX				GPIO_Pin_10
#define UART3_PIN_RX				GPIO_Pin_11
#define UART3_PORT					GPIOB
#define UART3_UART_CLK			RCC_APB1Periph_USART3
#define UART3_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define UART3_IRQ						USART3_IRQn

void UART_Init(UART_TypeDef UARTx, int BaudRate);
void SendUSART(USART_TypeDef* USARTx,uint16_t ch);
int GetUSART(USART_TypeDef* USARTx);
void SendString(USART_TypeDef* USARTx,char* str);
#endif
