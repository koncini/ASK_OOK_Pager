#include <iostm8s003k3.h>
#include <string.h>
#include "ClockSys.h"
#include "DataType.h"
#include "ookrxtx.h"
#include "Extevents.h"
#include "ADC.h"
#include "I2C.h"
#include "SSD1306.h"
#include "Tim4.h"
#include "eeprom.h"

char str[12];
uint8_t u8message[80];
int main( void )
{
  __disable_interrupt();
  InitialiseSystemClock();
  GPIORX_Init();
  Tim4_Init(0x03,0x07);
  I2C_Init();
  AWU_Init();
  __enable_interrupt();
  
  pg = (eepromReadLong(4) >> 4) & 0x0FFF;
  char number = (char) pg;
  
  strcpy(u8message, "  www.grupodyd.com  ");
  strcat(u8message, &number);
    
  Mini_SSD1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS , 1);
  Mini_SSD1306_display();
  while(I2Cflag);
  for (unsigned long i = 5000000;i>0;i--){asm("nop"); }//delay for init display
  Mini_SSD1306_clearDisplay();
  Mini_SSD1306_putString(0x00 ,0x00, u8message); 
  Mini_SSD1306_display();
  while(I2Cflag);
  Mini_SSD1306_startscrollleft(0x00, 0x0F);
  while(I2Cflag);
  RXGlowLigths();
  
  while(1)
  {
    RXReceiveData();
    __halt();
  }
}

