#include "sirc12_decode.h"

__IO StatusYesOrNo IRFrameReceived = NO;   /*!< IR frame status */
  
/* IR bit definitions*/
uint16_t IROnTimeMin = 0; 
uint16_t IROnTimeMax = 0; 
uint16_t IRValueStep = 0;
uint16_t IRValueMargin = 0;
uint16_t IRValue00 = 0;

/* Header time definitions*/
uint16_t IRHeaderLowMin = 0;
uint16_t IRHeaderLowMax = 0;
uint16_t IRHeaderWholeMin = 0;
uint16_t IRHeaderWholeMax = 0;
tSIRC_packet IRTmpPacket; /*!< IR packet*/

uint16_t IRTimeOut = 0;
static uint32_t TIMCLKValueKHz = 0;/*!< Timer clock */

 extern __IO uint8_t RFDemoStatus;

/** @defgroup SIRC_DECODE_Private_FunctionPrototypes
  * @{
  */
 
static uint8_t SIRC_DecodeHeader(uint32_t lowPulseLength, uint32_t wholePulseLength);
static uint8_t SIRC_DecodeBit(uint32_t lowPulseLength, uint32_t wholePulseLength);
static uint32_t TIM_GetCounterCLKValue(void);
static uint32_t reversebit(uint32_t data);

/**
  * @brief  De-initializes the peripherals (RCC,GPIO, TIM)       
  * @param  None
  * @retval None
  */
void SIRC_DeInit(void)
{ 
  TIM_DeInit(IR_TIM);
  GPIO_DeInit(IR_GPIO_PORT);
}

/**
  * @brief  Initialize the IR bit time range.
  * @param  None
  * @retval None
  */
void SIRC_Init(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  
  /*  Clock Configuration for TIMER */
  RCC_APB1PeriphClockCmd(IR_TIM_CLK, ENABLE);

  /* Enable Button GPIO clock */
  RCC_AHB1PeriphClockCmd(IR_GPIO_PORT_CLK, ENABLE);
  
  /* Pin configuration: input floating */
  GPIO_InitStructure.GPIO_Pin = IR_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(IR_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(IR_GPIO_PORT, IR_GPIO_SOURCE, GPIO_AF_TIM2);
     
  /* Enable the TIM global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = IR_TIM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* TIMER frequency input */
  TIM_PrescalerConfig(IR_TIM, TIM_PRESCALER, TIM_PSCReloadMode_Immediate);

  /* TIM configuration */
  TIM_ICInitStructure.TIM_Channel = IR_TIM_Channel;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(IR_TIM, &TIM_ICInitStructure); 

  /* Timer Clock */
  TIMCLKValueKHz = TIM_GetCounterCLKValue()/1000; 

  /* Select the TIM Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(IR_TIM, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(IR_TIM, TIM_SlaveMode_Reset);

  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(IR_TIM, TIM_MasterSlaveMode_Enable);

  /* Configures the TIM Update Request Interrupt source: counter overflow */
  TIM_UpdateRequestConfig(IR_TIM,  TIM_UpdateSource_Regular);
   
  IRTimeOut = TIMCLKValueKHz * SIRC_TIME_OUT_US/1000;

  /* Set the TIM auto-reload register for each IR protocol */
  IR_TIM->ARR = IRTimeOut;
  
  /* Clear update flag */
  TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);

  /* Enable TIM Update Event Interrupt Request */
  TIM_ITConfig(IR_TIM, TIM_IT_Update, ENABLE);

  /* Enable the CC2/CC1 Interrupt Request */
  TIM_ITConfig(IR_TIM, TIM_IT_CC2, ENABLE);
  TIM_ITConfig(IR_TIM, TIM_IT_CC1, ENABLE);

  /* Enable the timer */
  TIM_Cmd(IR_TIM, ENABLE);
  
  /* Header */	
  IRHeaderLowMin = TIMCLKValueKHz * SIRC_HEADER_LOW_MIN_US/1000;
  IRHeaderLowMax = TIMCLKValueKHz * SIRC_HEADER_LOW_MAX_US/1000;
  IRHeaderWholeMin = TIMCLKValueKHz * SIRC_HEADER_WHOLE_MIN_US/1000;
  IRHeaderWholeMax = TIMCLKValueKHz * SIRC_HEADER_WHOLE_MAX_US/1000;

  /* Bit time range*/
  IROnTimeMax = TIMCLKValueKHz * SIRC_ONTIME_MAX_US /1000;
  IROnTimeMin = TIMCLKValueKHz * SIRC_ONTIME_MIN_US/1000; 
  IRValueStep = TIMCLKValueKHz * SIRC_VALUE_STEP_US/1000;
  IRValueMargin = TIMCLKValueKHz * SIRC_VALUE_MARGIN_US/1000;
  IRValue00 = TIMCLKValueKHz * SIRC_VALUE_00_US/1000;

  /* Default state */
  SIRC_ResetPacket();
}

void SIRC_Decode(SIRC_Frame_TypeDef *ir_frame)
{
  if(IRFrameReceived != NO) 
  {
    IRTmpPacket.data = reversebit(IRTmpPacket.data);

    ir_frame->Command = (IRTmpPacket.data >> 20)& 0x7F;
    ir_frame->Address = (IRTmpPacket.data >> 27) & 0x1F;

    /* Default state */
    IRFrameReceived = NO; 
    SIRC_ResetPacket();

  }
}
void SIRC_ResetPacket(void)
{
  IRTmpPacket.count = 0;
  IRTmpPacket.status = INITIAL_STATUS;
  IRTmpPacket.data = 0;
}
void SIRC_DataSampling(uint32_t lowPulseLength, uint32_t wholePulseLength)
{
  uint8_t  tmpBit = 0;
  
  /* If the pulse timing is correct */
  if ((IRTmpPacket.status == SIRC_STATUS_RX)) 
  {
    /* Convert raw pulse timing to data value */
    tmpBit = SIRC_DecodeBit(lowPulseLength, wholePulseLength); 
    if (tmpBit != SIRC_BIT_ERROR) /* If the pulse timing is correct */
    {
      /* This operation fills in the incoming bit to the correct position in
      32 bit word*/
      IRTmpPacket.data |= tmpBit;
      IRTmpPacket.data <<= 1;
      /* Increment the bit count  */
      IRTmpPacket.count++;
    }
    /* If all bits identified */
    if (IRTmpPacket.count == SIRC_TOTAL_BITS_COUNT)
    {
      /* Frame received*/
      IRFrameReceived = YES;
    }
    /* Bit 15:the idle time between IR message and the inverted one */
    else if (IRTmpPacket.count == SIRC_BITS_COUNT)
    {
      IRTmpPacket.status = SIRC_STATUS_HEADER; 
    }
  }
  else if ((IRTmpPacket.status == SIRC_STATUS_HEADER))
  {
    /* Convert raw pulse timing to data value */
    tmpBit = SIRC_DecodeHeader(lowPulseLength, wholePulseLength);
    
    /* If the Header timing is correct */
    if ( tmpBit!= SIRC_HEADER_ERROR)
    { 
      /* Reception Status for the inverted message */
      IRTmpPacket.status = SIRC_STATUS_RX; 
    }
    else
    {
      /* Wrong header time */
      SIRC_ResetPacket();  
    }
  }
}

/**
  * @brief  Check the header pulse if it has correct pulse time.
  * @param  lowPulseLength: low pulse header duration.
  * @param  wholePulseLength: whole pulse header duration.
  * @retval SIRC_HEADER_OK or SIRC_HEADER_ERROR
  */
static uint8_t SIRC_DecodeHeader(uint32_t lowPulseLength, uint32_t wholePulseLength)
{
  /* First check if low pulse time is according to the specs */
  if ((lowPulseLength >= IRHeaderLowMin) && (lowPulseLength <= IRHeaderLowMax)) 
  {
    /* Check if the high pulse is OK */
    if ((wholePulseLength >= IRHeaderWholeMin) && (wholePulseLength <= IRHeaderWholeMax))
    {
      return SIRC_HEADER_OK; /* Valid Header */
    }
  }	
  return SIRC_HEADER_ERROR;  /* Wrong Header */
}

/**
  * @brief  Convert raw time to data value.
  * @param  lowPulseLength: low pulse bit duration.
  * @param  wholePulseLength: whole pulse bit duration.
  * @retval BitValue( data0 or data1) or SIRC_BIT_ERROR
  */
static uint8_t SIRC_DecodeBit(uint32_t lowPulseLength , uint32_t wholePulseLength)
{
  uint8_t i = 0;
  
  /* First check if low pulse time is according to the specs */
  if ((lowPulseLength >= IROnTimeMin) && (lowPulseLength <= IROnTimeMax))
  {
    for (i = 0; i < 2; i++) /* There are data0 to data1 */
    {
      /* Check if the length is in given range */
      if ((wholePulseLength >= IRValue00 + (IRValueStep * i) - IRValueMargin) 
          && (wholePulseLength <= IRValue00 + (IRValueStep * i) + IRValueMargin))
        return i; /* Return bit value */
    }
  }
  return SIRC_BIT_ERROR; /* No correct pulse length was found */
}

/**
  * @brief  Identify TIM clock
  * @param  None
  * @retval Timer clock
  */
static uint32_t TIM_GetCounterCLKValue(void)
{
  uint32_t apbprescaler = 0, apbfrequency = 0;
  uint32_t timprescaler = 0;
  __IO RCC_ClocksTypeDef RCC_ClockFreq;
  
  /* This function fills the RCC_ClockFreq structure with the current
  frequencies of different on chip clocks */
  RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_ClockFreq);
  
   /* Get the clock prescaler of APB1 */
    apbprescaler = ((RCC->CFGR >> 8) & 0x7);
    apbfrequency = RCC_ClockFreq.PCLK1_Frequency; 
    timprescaler = TIM_PRESCALER;

  
  /* If APBx clock div >= 4 */
  if (apbprescaler >= 4)
  {
    return ((apbfrequency * 2)/(timprescaler + 1));
  }
  else
  {
    return (apbfrequency/(timprescaler+ 1));
  }
}
/**
  * @brief  Reverse bit       
  * @param  data: data to be inversed
  * @retval None
  */
static uint32_t reversebit(uint32_t data) 
{
  uint32_t i = 0;
  uint32_t j = 0;
  
  for(j = i = 0; i < 32; i++) 
  {
    j = (j << 1) + (data & 1); 
    data >>= 1; 
  }
  
  return j;
}
