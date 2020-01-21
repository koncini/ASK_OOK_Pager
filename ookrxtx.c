/**********************************************************
Company: Dinamica y Desarrollo								
Designed by: Juan Sebastian Rincón Manrique
Date: 10/10/2017
Description: This code includes the necesary functions to 
manage the ASK/OOk transmitter and reciever
from synoxo allows to send and recieve a 32
data block.
Dependencies: ookrxtx.h , Extevents.h
*********************************************************/

//////////////////////////////////////////////////////////////
//Libraries
//////////////////////////////////////////////////////////////

#include "ookrxtx.h"
#include "eeprom.h"
#include "Tim4.h"
#include "AWU.h"
#include "SSD1306.h"

//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//Exclusive ussage function variables
//////////////////////////////////////////////////////////////

unsigned int cap_val = 0;
unsigned char transmit_count = 0;
unsigned char dig_rec_count = 0;
unsigned char _state = 0;
unsigned char data_ok = 0;
uint8_t _width = 0; 
uint8_t antiTheftCount = 0;
uint8_t eraseScreenCount = 4;
uint32_t id_buffer = 0xFFFFFFFF;
uint32_t beat_id = 0x0000FFFF;
uint32_t mask = 0x80000000; 

volatile uint16_t pg;

//////////////////////////////////////////////////////////////

//Functions: The following are avalaible functions
/* 
InitialiseSystemClock(void);
TIM1_ICAP_Init(void);
GPIOTX_Init(void);
GPIORX_Init(void);
TXZero(void);
TXOne(void);
TIM4_Init(void);
TXSendData(uint32_t data);
TXSendDataChain(uint32_t data);
RXRecieveData(uint32_t data);
RXAlert(void);
*/

/************************************************************
** Timers interrupt handlers
************************************************************/

//Interrupt handler for external interrupt PC7 to wake up mcu 
#pragma vector=EXTI2_vector   
__interrupt void EXTI_PORTC_IRQHandler(void)
{
  Exti2flag = 1;
  PB_CR2_C20 = 1;
  PD_ODR_ODR6 = 1;
}

//Interrupt handler for external interrupt PB0 to decode the received data
#pragma vector=EXTI1_vector    
__interrupt void EXTI_PORTB_IRQHandler(void)
{
  TIM4_CR1_CEN = 1;
  TIM4_IER_UIE = 1;
  Exti1flag = 1;
}

/*---------------------------Start init functions------------------------------*/

void GPIORX_Init(void)
{
  /*Initializes the ports for the reciever
  pin PC3 as an input (TLI 20pin) (TLI PD7 32 pin)     Page 111
  Enable Interrupts                                    Page 112*/
  OOK_RX_INIT_MACRO;
  PE_DDR_DDR5 = 1;
  PE_CR1_C15 = 1;
  PD_DDR = 0x60;
  PD_CR1 = 0x60;
  PB_DDR = 0x80;
  PB_CR1 = 0x80;
  PB_CR2 = 0x40;
  BEEP_CSR_BEEPEN = 0;
  EXTI_CR1_PCIS = 0x0;          // Falling edge and low level to wake up
  EXTI_CR1_PBIS = 0x1;          // Rising edge for decoder
  pg = (eepromReadLong(4) >> 4) & 0x0FFF ;
 }

/*void GPIOTX_Init(void)
{
  /*Initializes the ports for the transmitter
  pin PA3 as an output                  Page 111
  output Push-pull                      Page 111
  switching speed - up to 10 MHz        Page 112
  OOK_TX_INIT_MACRO;
  BEEP_CSR_BEEPDIV = 0X03;
}*/

/*----------------------------End init functions-------------------------------*/


/*-------------------------------Start functions-------------------------------*/

/*-----------------------These functions are for Tx

//Toggles for generate a chain for zero manchester encoding
void TXZero(void)
{
  OOK_TX_PIN = 1;
  for (unsigned int i = 2300;i>0;i--){asm("nop"); }//delay
  OOK_TX_PIN = 0;
  for (unsigned int i = 500;i>0;i--){asm("nop"); }//delay
}

//Toggles for generate a chain for one manchester encoding
void TXOne(void)
{   
  OOK_TX_PIN = 1;
  for (unsigned int i = 500;i>0;i--){asm("nop"); }//delay
  OOK_TX_PIN = 0;
  for (unsigned int i = 2300;i>0;i--){asm("nop"); }//delay
}

void TXSendData(uint32_t data)
{
  //Sends an specified word whith ID and information to communicate
  if (transmitFlag == 1)
  {                   //Flag for trigger the sending chain
    transmit_count++;
    for (unsigned int i = 10000;i>0;i--){asm("nop"); }//delay
    transmitFlag=0;
    for (unsigned i = 7 ; i <= 32 ; i++)        //Buffer the data
    {   
      uint32_t mask = 0x80000000;           //Data mask
      uint32_t masked = data & ( mask >> i); 
      if (masked == 0)
      {
        TXZero();                           //Generates Zero chain
      }
      else
      {
        TXOne();                            //Generates One chain
      }
    }
  }
}

void TXSendDataChain(uint32_t data) //Sends a modulated datachain each period of time 20 times
{
  BEEP_CSR_BEEPEN = 1;
  BEEP_CSR_BEEPDIV = 0x00;
  OOK_TX_PIN = 1;                //Toggle for stabilizing RX and as an preamble
  for (unsigned int i = 15000;i>0;i--){asm("nop"); }//delay
  OOK_TX_PIN = 0;                //Toggle for stabilizing RX and as an preamble
  for (unsigned int i = 10000;i>0;i--){asm("nop"); }//delay
  OOK_TX_PIN = 1;                //Toggle for stabilizing RX and as an preamble
  for (unsigned int i = 5000;i>0;i--){asm("nop"); }//delay
  OOK_TX_PIN = 0;                //Toggle for stabilizing RX and as an preamble
  for (unsigned int i = 500;i>0;i--){asm("nop"); }//delay
  TIM4_IER_UIE = 1;                    //Update interrupt enable                Page 253
  while(transmit_count<30)
  {
    TXSendData(data);                  //Sends data just once
  }
  transmit_count = 0;                 //Clears the the counter
  TIM4_IER_UIE = 0;                  //Update interrupt disable               Page 253
  BEEP_CSR_BEEPDIV = 0x03;
  BEEP_CSR_BEEPEN = 0;
}

void HeartBeat()
{
  if (AWUflag==1)
  {
    beat_id = (beat_id & eepromReadData(2)) << 16;
    AWUflag = 0;
    TXSendData(beat_id);
  }
} 

*/

//-----------------------These functions are for Rx

void RXReceiveData()
{
  if (Exti1flag)
  {
    Exti1flag = 0;
    while(PB_IDR_IDR0);
    TIM4_IER_UIE = 0;
    getPulseWidth();
    _width = pulseWidth;
    RXDecodeData();
  }
}

void RXDecodeData()
{
  if (_width <= 0x0E && _width >= 0x02)
  {
    id_buffer = id_buffer ^  (mask >> dig_rec_count);
  }
  else if (_width <= 0x14 && _width >= 0x20)
  {
    id_buffer = id_buffer | (mask >> dig_rec_count);
  }
  dig_rec_count++;
  if (dig_rec_count == 32)
  {
    dig_rec_count = 0;
    PB_CR2_C20 = 0;
    TIM4_IER_UIE = 0;
    TIM4_CR1_CEN = 0;
    RXAlert();
  }
}

void RXAlert()  //Generates the alert when RX recieves a call
{
  PD_ODR_ODR6 = 0;
  Exti2flag = 0;
  mask = 0x80000000;
  uint32_t id = ~id_buffer;
  id_buffer = 0xFFFFFFFF;
  if(id == eepromReadLong(4))
  {
    uint8_t index = 0;
    Mini_SSD1306_clearDisplay();
    Mini_SSD1306_putString(0x00 ,0x00, "   LLAMADA PEDIDO             # 1"); 
    Mini_SSD1306_display();
    while(I2Cflag);
    Mini_SSD1306_startscrollleft(0x00, 0x0F);
    while(I2Cflag);
    while(index < 5)
    {
      BEEP_CSR_BEEPEN = 1;
      PB_ODR_ODR7 = !PB_ODR_ODR7;
      for (unsigned int i = 0; i<33 ; i++)
      {
        BEEP_CSR = (_state<<6)+30;
        if(_state ^ 3) BEEP_CSR |= 32;
        for(unsigned long dummy = 0; dummy<70000;dummy++);
        _state = (_state+1)&3;
      }
      RXGlowLigths();
      BEEP_CSR_BEEPEN = 0;
      index++;
    }
  }
  else if(id == (eepromReadLong(4) & 0xFFFF0000))
  {
    AWUflag = 0;
    antiTheftCount = 0;
    eraseScreenCount--;
    if(eraseScreenCount == 0)
    {
      eraseScreenCount = 4;
      Mini_SSD1306_clearDisplay();
      Mini_SSD1306_putString(0x00 ,0x00, "  www.grupodyd.com  "); 
      Mini_SSD1306_display();
      while(I2Cflag);
      Mini_SSD1306_startscrollleft(0x00, 0x0F);
      while(I2Cflag);
    }
  }
  PB_ODR_ODR7 = 0;
  id = 0;
}

void antiTheft()
{
  if(AWUflag)
  {
    AWUflag = 0;
    antiTheftCount++;
    if(antiTheftCount == 15)
    {
      PE_ODR_ODR5 = 1;
      PB_ODR_ODR7 = 1;
      BEEP_CSR_BEEPEN = 1;
      Mini_SSD1306_clearDisplay();
      Mini_SSD1306_putString(0x00 ,0x00, "   FUERA DE RANGO  "); 
      Mini_SSD1306_display();
      while(I2Cflag);
      Mini_SSD1306_startscrollleft(0x00, 0x0F);
      while(I2Cflag);
    }
  }
}

void RXGlowLigths()
{
  PE_ODR_ODR5 = 1; 
  for (unsigned long i = 50000;i>0;i--){asm("nop"); }//delay for init display
  PD_ODR_ODR5 = 1;
  PE_ODR_ODR5 = 0;
  for (unsigned long i = 50000;i>0;i--){asm("nop"); }//delay for init display
  PD_ODR_ODR6 = 1;
  PD_ODR_ODR5 = 0;
  for (unsigned long i = 50000;i>0;i--){asm("nop"); }//delay for init display
  PD_ODR_ODR6 = 0;
}