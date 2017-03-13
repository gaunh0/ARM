/* Includes ------------------------------------------------------------------*/

#include "main.h"

static __IO uint32_t TimingDelay;

int main(void)
{
  static FRESULT fr;   /* Result code */  
  static FATFS fatfs;  /* File system object */  
  static FIL fil;   /* File object */  
  static DIR dir;   /* Directory object */  
  static FILINFO fno;  /* File information object */  
	char line[82];
	
  /* Initialize Leds mounted on STM32F4-Discovery board */
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

		/* Turn on LED4 and LED5 */
 // STM_EVAL_LEDOn(LED3);
 // STM_EVAL_LEDOn(LED4);
	LCD_Init();
	XPT2046_Init();
	LCD_SetBackLight(100); // do sang mang hinh tu 1 den 100
	LCD_DisplayOn();
	LCD_SetTextColor(255, 255, 255);
	LCD_StringLine(5,5,"Test cmn SD card");	
	power();
	
	  /* Register work area to the default drive */
    f_mount(0, &fatfs);
		
		LCD_StringLine(0,30,"Tao vung lam viec thanh cong");
		    /* Open a text file */
    fr = f_open(&fil, "hello.txt",FA_READ);
    if (fr){
			LCD_Clear(255,255,255);
			LCD_StringLine(5,20,"Khong mo duoc");
		} 
		else{
		LCD_StringLine(5,50,"Mo thanh cong");
			}
		
			
		    /* Read all lines and display it */
		//while (f_gets(line, sizeof line, &fil))
		LCD_StringLine(5,20,line);
		
		    /* Close the file */
    f_close(&fil);
	
	while (1) {

	}
	

  
	
//		while(1) {
//		// Mau nen cua man hinh duoc chinh o ham LCD_Init
//		LCD_DisplayOn();// che do mo mang hinh
//		LCD_CharSize(16); // Chinh kich thuoc font chu cac gia tri truyen vao: 8,12,16,24.Mac dinh la 16
//		LCD_SetColors(0, 0, 0,0, 0,0);	 // set ma cho cac pixel duoc ve va mau nen chu
//		LCD_SetTextColor(255, 255, 255);// set mau cach pha tron 3 mau theo thu tu R,G,Blu
//		LCD_StringLine(5, 0, "Test");// Ham tham chieu gia tri x,y, text can in ra mang hinh
//		LCD_StringLine(5, 60, "ILI9325");// Ham tham chieu gia tri x,y, text can in ra mang hinh
//		LCD_StringLine(5, 90, "Touch the LCD:  ");// Ham tham chieu gia tri x,y, text can in ra mang hinh
//			while(XPT2046_Press());
//			while (1) 
//		{
//				LCD_Clear(255,255,255);
				//LCD_StringLine(5, 120, "OK" );// Ham tham chieu gia tri x,y, text can in ra mang hinh
				//LCD_Disp_Image(img02);
//		}	
//  }
}
			
	
		//LCD_SetBackColor(255,75,86); /// back sau cho chu
//		int k=100;
//		for(k;k<310;k++){
//		Pixel(10,k,236,236,236);// ham xuat 1 pixel ra man hinh, 2 thong so dau la toa do diem can xuat, va mau
//		Pixel(11,k,255,140,0);
//		Pixel(12,k,45,30,105);	
//		Delay_ms(100);
			//LCD_Cross(0,50, 220); // deo biet de lam gi
		//	LCD_DrawFullCircle(50,50,10); // ve full hinh tron
			//LCD_DrawSquare(50,50,50); // ve hinh vuong
		 //LCD_DrawRect(10, 10, 20, 30); // ve hinh chu nhat, thong so dang sau la kich thuoc
		//PutPixel(0x234, 0x2344); ve 1 diem
		//LCD_DisplayOff(); // tat man hinh
			//LCD_DrawCircle(50, 50, 10); // ve hinh tron (X,Y la toa do cua tam)
		//	LCD_DrawUniLine(0,0, 249, 319); // ve duong thang( toa do cua 2 diem)
			//LCD_GetType("2");
		//LCD_SetBackColor(0xff, 0,0);
		//LCD_Cross(100,20, 50);
		//LCD_DrawFullRect(50,50,100,50); // full hinh chu nhat
		
 				

/** 
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
