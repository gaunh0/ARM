#include "main.h"

/* 
- Init 2 timer, 40hz & 600us
*/

int main(void)
{	
	SIRC_Frame_TypeDef SIRC_FRAME;
	Systick_Configuration();
	LED_init(LED_Green);
	LED_init(LED_Red);
  SIRC_Init();
	while(1)
	{
		SIRC_Decode(&SIRC_FRAME);
		if(SIRC_FRAME.Command)
		{
					LED_Togg(LED_Green);
		}
		LED_Togg(LED_Red);
		Delay(1000);
	}	
}
