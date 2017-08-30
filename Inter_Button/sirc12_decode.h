#ifndef __SIRC12_DECODE_H
#define __SIRC12_DECODE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_conf.h"

typedef enum { NO = 0, YES = !NO} StatusYesOrNo;	 
	 
/** 
  * @brief  SIRC frame structure  
  */
typedef struct
{  
  __IO uint8_t Command;   /*!< Command field */
  __IO uint8_t Address ;  /*!< Address field */
} SIRC_Frame_TypeDef;

/** 
  * @brief  SIRC packet structure  
  */
typedef struct
{
  uint8_t count;  /*!< Bit count */
  uint8_t status; /*!< Status */
  uint32_t data;  /*!< Data */
} tSIRC_packet;

#define SIRC_TIME_OUT_US                4050 

#define SIRC_STATUS_HEADER              1 << 1
#define SIRC_STATUS_RX                  1 << 0
#define INITIAL_STATUS                SIRC_STATUS_HEADER 

#define SIRC_BIT_ERROR                  0xFF
#define SIRC_HEADER_ERROR               0xFF
#define SIRC_HEADER_OK                  0x00

#define SIRC_BITS_COUNT                 11
#define SIRC_TOTAL_BITS_COUNT           11

#define SIRC_ONTIME_MIN_US              (600 - 60)
#define SIRC_ONTIME_MAX_US              (1200 + 60)

#define SIRC_HEADER_LOW_MIN_US          (2400 - 150)
#define SIRC_HEADER_LOW_MAX_US          (2400 + 150)
#define SIRC_HEADER_WHOLE_MIN_US        (2400 + 600 - 60)
#define SIRC_HEADER_WHOLE_MAX_US        (2400 + 600 + 60)

#define SIRC_VALUE_STEP_US              600
#define SIRC_VALUE_MARGIN_US            100
#define SIRC_VALUE_00_US                1200

#define IR_TIM                 TIM2                     /*!< Timer used for IR decoding */
#define TIM_PRESCALER          84                       /* !< TIM prescaler */
#define IR_TIM_CLK             RCC_APB1Periph_TIM2      /*!< Clock of the used timer */
#define IR_TIM_IRQn            TIM2_IRQn                /*!< IR TIM IRQ */
#define IR_TIM_Channel         TIM_Channel_2            /*!< IR TIM Channel */

#define IR_GPIO_PORT           GPIOB                    /*!< Port which IR output is connected */
#define IR_GPIO_PORT_CLK       RCC_AHB1Periph_GPIOB      /*!< IR pin GPIO Clock Port */
#define IR_GPIO_PIN            GPIO_Pin_3               /*!< Pin which IR is connected */
#define IR_GPIO_SOURCE         GPIO_PinSource3


void Menu_SIRCDecode_Func(void);
void SIRC_DeInit(void);
void SIRC_Init(void);
void SIRC_Decode(SIRC_Frame_TypeDef *ir_frame);
void SIRC_ResetPacket(void);
void SIRC_DataSampling(uint32_t lowPulseLength, uint32_t wholePulseLength);

#ifdef __cplusplus
}
#endif	 
	 
#endif
