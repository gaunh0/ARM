/* Includes ------------------------------------------------------------------*/
#include "ILI9325.h"
#include "XPT2046.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
Matrix        matrix;
Coordinate    display;
Coordinate    ScreenSample[3];
Coordinate    DisplaySample[3] = { {30, 45}, {290, 45}, {160, 210} };

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2

void XPT2046_Init(void) 
{ 
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Configure GPIOs ---------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* Configure SPI3 pins: SCK, MISO and MOSI ---------------------------------*/ 
	GPIO_InitStruct.GPIO_Pin = XPT2046_SCK_PIN | XPT2046_MISO_PIN | XPT2046_MOSI_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);

  /* TP_CS */
  GPIO_InitStruct.GPIO_Pin = XPT2046_CS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);

  /* TP_IRQ */
  GPIO_InitStruct.GPIO_Pin =  XPT2046_IRQ_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);

  GPIO_PinAFConfig(XPT2046_PORT, GPIO_PinSource10, GPIO_AF_SPI3);
  GPIO_PinAFConfig(XPT2046_PORT, GPIO_PinSource11, GPIO_AF_SPI3);
  GPIO_PinAFConfig(XPT2046_PORT, GPIO_PinSource12, GPIO_AF_SPI3);
  
  set_CS(); 

  /* Configure SPI -----------------------------------------------------------*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  /* DISABLE SPI3 */ 
  SPI_Cmd(SPI3, DISABLE); 
  /* SPI3 Config -------------------------------------------------------------*/ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial = 7; 
  SPI_Init(SPI3, &SPI_InitStructure); 
  /* Enable SPI3 */ 
  SPI_Cmd(SPI3, ENABLE); 
} 

uint16_t Read_X(void)  
{  
  uint16_t curr_X;

  reset_CS(); 
  Delay(1); 
  WR_CMD(CHX); 
  Delay(1); 
  curr_X=RD_AD(); 
  set_CS(); 
  return curr_X;    
} 

uint16_t Read_Y(void)  
{  
  uint16_t curr_Y; 

  reset_CS(); 
  Delay(1); 
  WR_CMD(CHY); 
  Delay(1); 
  curr_Y=RD_AD(); 
  set_CS(); 
  return curr_Y;     
} 

void XPT2046_GetAdXY(int *x,int *y)  
{ 
  uint16_t adx,ady; 
  adx=Read_X(); 
  Delay(1); 
  ady=Read_Y(); 
  *x=adx; 
  *y=ady; 
} 
	
Coordinate *Read_XPT2046(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};
  
  do
  {		   
    XPT2046_GetAdXY(TP_X,TP_Y);  
	  buffer[0][count]=TP_X[0];  
	  buffer[1][count]=TP_Y[0];
	  count++;  
  }
  while(!read_IRQ()&& count<9);  /* TP_INT_IN  */
  if(count==9)   /* Average X Y  */ 
  {
	/* Average X  */
  temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
	temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
	temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];

	m0=m0>0?m0:(-m0);
  m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);

	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

	/* Average Y  */
  temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
	temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
	temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	m0=m0>0?m0:(-m0);
	m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
    }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;

	return &screen;
  }  
  return 0; 
}
	 
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;
  /* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

FunctionalState getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  FunctionalState retTHRESHOLD =ENABLE ;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = DISABLE;
  }
  return(retTHRESHOLD);
} 

void TouchPanel_Calibrate(void)
{
  unsigned char i;
  Coordinate * Ptr;

  for(i=0;i<3;i++)
  {
   //SSD1289_Clear(Black);
   //SSD1289_CleanText(44,10,"Touch crosshair to calibrate",Red);
   Delay(250);
   //SSD1289_DrawCross(DisplaySample[i].x,DisplaySample[i].y, Red, RGB565CONVERT(184,158,131));
   do
   {
     Ptr = Read_XPT2046();
   }
   while( Ptr == (void*)0 );
   ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
  }
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
} 

static void set_CS(void)
{
  GPIO_SetBits(XPT2046_PORT, XPT2046_CS_PIN);
}

static void reset_CS(void)
{
  GPIO_ResetBits(XPT2046_PORT, XPT2046_CS_PIN);
}

static uint16_t read_IRQ(void)
{
  return GPIO_ReadInputDataBit(XPT2046_PORT, XPT2046_IRQ_PIN);
}

uint16_t XPT2046_Press(void)
{
  return read_IRQ();
}

static void WR_CMD (uint16_t cmd)  
{ 
  /* Wait for SPI3 Tx buffer empty */ 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send SPI3 data */ 
  SPI_I2S_SendData(SPI3,cmd); 
  /* Wait for SPI3 data reception */ 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read SPI3 received data */ 
  SPI_I2S_ReceiveData(SPI3); 
} 

static uint16_t RD_AD(void)  
{ 
  uint16_t buf, temp; 
  /* Wait for SPI3 Tx buffer empty */ 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send SPI3 data */ 
  SPI_I2S_SendData(SPI3,0x0000); 
  /* Wait for SPI3 data reception */ 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read SPI3 received data */ 
  temp=SPI_I2S_ReceiveData(SPI3); 
  buf=temp<<8; 
  Delay(1); 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send SPI3 data */ 
  SPI_I2S_SendData(SPI3,0x0000); 
  /* Wait for SPI3 data reception */ 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read SPI3 received data */ 
  temp=SPI_I2S_ReceiveData(SPI3); 
  buf |= temp; 
  buf>>=3; 
  buf&=0xfff; 
  return buf; 
}
