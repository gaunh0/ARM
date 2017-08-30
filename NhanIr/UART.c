 /*
 ******************************************************************************
  * @file    UART.c
  * @author  Teddy
  * @version V1.0
  * @date    July 4, 2007
  ******************************************************************************
	*/

#include "UART.h"

GPIO_TypeDef* UART_PORT[UARTn] = {UART1_PORT,UART2_PORT,
																	UART3_PORT};
const uint16_t UART_PIN_TX[UARTn] ={UART1_PIN_TX,UART2_PIN_TX
																	,UART3_PIN_TX};
const uint16_t UART_PIN_RX[UARTn] ={UART1_PIN_RX,UART2_PIN_RX
																	,UART3_PIN_RX};
const uint32_t UART_UART_CLK[UARTn] = {UART1_UART_CLK,UART2_UART_CLK,UART3_UART_CLK};
const uint32_t UART_GPIO_CLK[UARTn] = {UART1_GPIO_CLK,UART2_GPIO_CLK,UART3_GPIO_CLK};
const uint16_t UART_IRQ[UARTn] = {UART1_IRQ,UART2_IRQ,UART3_IRQ};
USART_TypeDef* UART[UARTn] ={USART1,USART2,USART3};

void UART_Init(UART_TypeDef UARTx, int BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(UART_GPIO_CLK[UARTx] , ENABLE);

	if(UARTx == 0)
	{
		RCC_APB2PeriphClockCmd(UART_UART_CLK[UARTx], ENABLE); 
	}
	else
	{
		RCC_APB1PeriphClockCmd(UART_UART_CLK[UARTx], ENABLE); 
	}
		/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = UART_PIN_TX[UARTx];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(UART_PORT[UARTx], &GPIO_InitStructure);
		
		/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = UART_PIN_RX[UARTx];
	GPIO_Init(UART_PORT[UARTx], &GPIO_InitStructure);
		
	USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART[UARTx], &USART_InitStructure);
	
	USART_ITConfig(UART[UARTx], USART_IT_RXNE, ENABLE); // enable the USARTx receive interrupt 
	
	NVIC_InitStructure.NVIC_IRQChannel = UART_IRQ[UARTx];		
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);		
	
  USART_Cmd(UART[UARTx], ENABLE);
}
void SendUSART(USART_TypeDef* USARTx,uint16_t ch)
{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){;}
		USART_SendData(USARTx, (uint16_t)ch);
}

int GetUSART(USART_TypeDef* USARTx)
{
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
	{}
	return USART_ReceiveData(USARTx);
}
void SendString(USART_TypeDef* USARTx,char* str)
{
	while(*str != '\0')
	{
		SendUSART(USARTx,*str++);
	}
}

GETCHAR_PROTOTYPE
{
	return GetUSART(USART2);
}

PUTCHAR_PROTOTYPE
{
	 USART_SendData(USART2, (uint8_t) ch);
	 /* Loop until the end of transmission */
	 while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){} 
		 
	return ch;
}
