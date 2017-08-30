#ifndef _SIRC12_ENCODE_H__
#define _SIRC12_ENCODE_H__

#include "stm32f4xx_conf.h"

/* to avoid collusion, this value can be updated for thin adjust*/
#define  SIRC12_COLLUSION_AVOID_AJUST   ((uint8_t)0)

/**
  * @brief Definition of the SIRC12 devices adresses.
  */
   
typedef enum
   {
     SIRC12_Address_0          = ((uint8_t)0)
   }SIRC12_Address_TypeDef;
   
/**
  * @brief Definition of the SIRC12 devices instructions.
  */        
typedef enum
   {
     /* Device general Instruction Code*/
     SIRC12_Instruction_0      = ((uint8_t)0)
   }SIRC12_Instruction_TypeDef;
	 
/* Asserts Definition */

#define  SIRC12_ADDRESS_LIMIT        ((uint8_t ) 0x1F)   /* Address limit definition*/
#define  SIRC12_INSTRUCTION_LIMIT    ((uint8_t ) 0x7F)   /* Data limit definition*/

#define IS_SIRC12_ADDRESS_IN_RANGE(ADDRESS) ((ADDRESS) <= SIRC12_ADDRESS_LIMIT )
#define IS_SIRC12_INSTRUCTION_IN_RANGE(INSTRUCTION) ((INSTRUCTION) <= SIRC12_INSTRUCTION_LIMIT   )


void SIRC12_Init(void);
void SIRC12_Encode_SendFrame(uint8_t SIRC12_Address, uint8_t SIRC12_Instruction);
void SIRC12_Encode_SignalGenerate(uint32_t SIRC12_FramePulseWidthFormat[]);


#endif
