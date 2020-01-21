/********************************************************
Company: Dinamica y Desarrollo								
Designed by: Juan Sebastian Rincón Manrique
Date: 10/11/2017
Description: This code includes the necesary functions to 
manage the ADC module just for read any values with single
convertion modes.
Dependencies: ADC.h 
*********************************************************/

//////////////////////////////////////////////////////////////
//Libraries
//////////////////////////////////////////////////////////////

#include <iostm8s003k3.h>
#include "ADC.h"

/*---------------------------Start init functions------------------------------*/

void Set_Otuput_Port()
{ //Set otpuc control pin for ADC management
  PB_ODR_ODR1 = 0;
  PB_DDR_DDR1 = 1;
  PB_CR1_C11 = 1;
}

void ADC_Init()
{ //using ADC in single conversion mode
  Set_Otuput_Port();
  ADC_CR1_ADON = 1;       //  Turn ADC on, note a second set is required to start the conversion.
  ADC_CR1_CONT = 0;
  ADC_CSR_CH = 0x03;
  ADC_CR3_DBUF = 0;
  ADC_CR2_ALIGN = 1;      //  Data is right aligned.
  ADC_CSR_EOCIE = 1;      //  Enable the interrupt after conversion completed.
}

/*----------------------------End init functions-------------------------------*/


/*---------------------------------IRQ Handler---------------------------------*/

#pragma vector = ADC1_EOC_vector
__interrupt void ADC1_EOC_IRQHandler()
{
  unsigned char low, high;
  int reading;
  
  if(PB_ODR_ODR1)
  {
    
    ADC_CR1_ADON = 0;           //Disable the ADC.
    ADC_CSR_EOC = 0;		//Indicate that ADC conversion is complete.
    
    low = ADC_DRL;		//Extract the ADC reading.
    high = ADC_DRH;
    reading = 1023 - ((high * 256) + low); //431 para 5V con divisor a 2.9 V
    //
    //	Calculate the values for the capture compare register and restart Timer 1.
    //
        
    PB_ODR_ODR1 = 0;
  }
}