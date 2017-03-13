#include "stm32f4xx_conf.h"
#include "main.h"
/* Ngat ngoai:
Khi nhan Button, den 12 se doi trang thai lien tuc
Thu tu chinh : GPIO -> EXTI -> NVIC

*/
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
EXTI_InitTypeDef  EXTI_InitStructure;

int main(void)
{	
	GPIO_Configuration();
	Ngat_cofig();
	while(1)
	{
		GPIO_ToggleBits(GPIOD,GPIO_Pin_All);
		Delay(10000000);
	}
	
}
	void GPIO_Configuration(void) // config den
	{
		/*
		- Cac led duoc dat o thu thu cac chan la D12 -> D15
		- Cac buoc cau hinh GPIO: pin -> mode -> Otype-> speed -> Pull 	
		*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); // cap xung chip GPIO D
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;// (GPIO_Pin_All)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD,&GPIO_InitStructure);
		
	}
void Ngat_cofig(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); // cap xung chip GPIO A
		RCC_AHB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); // cap xung cho khoi ngat 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;// (GPIO_Pin_0)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	// config Line -> Mode -> Trigger -> LineCMD
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	// chinh do uu tien
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void Delay(__IO uint32_t nCount)
 {
   while(nCount--)
    {
    }
  }
 