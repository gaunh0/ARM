/**
  ******************************************************************************
  * @file    SysTick/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "sirc12_encode.h"
#include "sirc12_decode.h"
#include "led.h"
/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t SIRC12_FramePulseWidthFormat[];
__IO uint8_t RFDemoStatus = 0x00;
uint32_t ICValue1 = 0;
uint32_t ICValue2 = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//  TimingDelay_Decrement();
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles TIM17 interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
  
  SIRC12_Encode_SignalGenerate(SIRC12_FramePulseWidthFormat);
	
  LED_Togg(LED_Red);
  /* Clear TIM16 update interrupt */
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
void TIM2_IRQHandler(void)
{
  /* Clear the TIM2 Update pending bit */
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
      
    /* IC1 Interrupt */
    if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC2) != RESET))
    {
      TIM_ClearFlag(IR_TIM, TIM_FLAG_CC2);
      /* Get the Input Capture value */
      ICValue2 = TIM_GetCapture2(IR_TIM);
      SIRC_DataSampling(ICValue1, ICValue2); 
    }  /* IC2 Interrupt*/   
    else  if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC1) != RESET))
    {
      TIM_ClearFlag(IR_TIM, TIM_FLAG_CC1);
      /* Get the Input Capture value */
      ICValue1 = TIM_GetCapture1(IR_TIM);
    } 
    /* Checks whether the IR_TIM flag is set or not. */
    else if ((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_Update) != RESET))
    { 
      /* Clears the IR_TIM's pending flags*/
      TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);
      
      SIRC_ResetPacket();
    }
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
