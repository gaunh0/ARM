#include "main.h"

/* 
- Init 2 timer, 40hz & 600us
*/
SIRC_Frame_TypeDef SIRC_FRAME;

int main(void)
{	
	Systick_Configuration();
	LED_init(LED_Green);
	LED_init(LED_Red);
	SIRC_Init();
	while(1)
	{
		SIRC_Decode(&SIRC_FRAME);
		LED_Togg(LED_Red);
		Delay(1000);
	}	
}
