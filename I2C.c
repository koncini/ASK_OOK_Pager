/**********************************************************
Company: Dinamica y Desarrollo								
Designed by: Juan Sebastian Rincón Manrique
Date: 07/03/2018
Description: This code manages the I2C interface as a 
master in writing mode to perform operations over another
MCU.
Dependencies: I2C.h 
*********************************************************/

//////////////////////////////////////////////////////////////
//                        Libraries                         //
//////////////////////////////////////////////////////////////

#include <I2C.h>

//////////////////////////////////////////////////////////////
//          Exclusive ussage function variables             //
//////////////////////////////////////////////////////////////

//
//  Define some pins to output diagnostic data.
//
#define PIN_BIT_BANG_DATA       PD_ODR_ODR4
#define PIN_BIT_BANG_CLOCK      PD_ODR_ODR5
#define PIN_ERROR               PD_ODR_ODR6

//
//  Buffer to hold the I2C data.
//
unsigned char _buffer[2];
int _nextByte = 0;

struct i2c_s 
{
	uint8_t	        SlaveAddress;
	uint8_t*	masterBuffer;
	uint8_t	        masterTransactionLength;
	uint8_t	        buffer_index;
	uint8_t         reg;
	uint8_t 	masterMode;
	uint8_t	        readwrite;
	uint8_t*	slaveBuffer;
	uint8_t 	slaveTransactionLength;
}i2c;

/*---------------------------Start init functions------------------------------*/

void I2C_Init()
{
    I2C_CR1_PE = 0;                     //  Disable I2C before configuration starts.
    //
    //  Setting up the clock information.
    //
    I2C_FREQR = 16;                     //  Set the internal clock frequency (16 MHz).
    I2C_CCRH_F_S = 1;                   //  I2C running is standard mode.
    I2C_CCRL = 0x0D;                    //  SCL clock speed is 400 KHz.
    I2C_CCRH_CCR = 0x00;
    //
    //  Set the address of this device.
    //
    I2C_OARH_ADDMODE = 0;               //  7 bit address mode.
    I2C_OARH_ADDCONF = 1;               //  This bit must be set by software
    //
    //  Setting up the bus characteristics.
    //
    I2C_TRISER = 17;
    //
    //  Turn on the interrupts.
    //
    I2C_ITR_ITBUFEN = 1;                //  Buffer interrupt enabled.
    I2C_ITR_ITEVTEN = 1;                //  Event interrupt enabled.
    I2C_ITR_ITERREN = 1;
    //
    //  Configuration complete so turn the peripheral on.
    //
    I2C_CR1_PE = 1;
}

/*----------------------------End init functions-------------------------------*/

/*----------------------------Interrupt handler--------------------------------*/

//SR1: 	TXE(Tx Empty) RXNE(Rx Not Empty) STOPF(Stop detection, slave mode) 
//		ADD10(10bit header sent, master mode) BTF(Byte Transfer Ffinished) 
//		ADDR(Address sent, master mode / matched in slave mode) SB(Start bit, master mode)

//SR2: 	WUFH(wake up from halt, slave/master) OVR(Overrun underrun) AF(Acknowledge Failure)
//		ARLO(Arbitration lost, master mode) BERR(Bus Error misplaced start or stop)

//SR3: 	DUALF(reserved ?) GENCALL(General call if used) TRA(Transmitted not received as of R/W address bit)
//		BUSY(Bus busy updated even if PE=0) MSL(Master mode set after Start Bit, cleared after Stop or on Arbitration Lost)

#pragma vector = I2C_RXNE_vector
__interrupt void I2C_IRQHandler()
{
   
  unsigned char reg;

    if (I2C_SR1_SB)
    {                         //(SB) The Start Byte has been sent
        //
        //  Master mode, send the address of the peripheral we
        //  are talking to.  Reading SR1 clears the start condition.
        //
        reg = I2C_SR1;
        //  Send the slave address and the write bit.
        I2C_DR = (DEVICE_ADDRESS << 1) | I2C_WRITE;
    }
    else if (I2C_SR1_ADDR)
    {
        //
        //  In master mode, the address has been sent to the slave.
        //  Clear the status registers and wait for some data from the salve.
        //
      reg = I2C_SR1;
      reg = I2C_SR3;
    }
    else if(I2C_SR3_TRA)        // (TRA) Data bytes transmitted
    {                    
      if (I2C_SR1_TXE)
      {
        I2C_DR = i2c.masterBuffer[i2c.buffer_index++]; //Send data Byte
        if(i2c.buffer_index == i2c.masterTransactionLength)
        {
          I2C_CR2_STOP = 1;	//Generate Stop condition
          I2Cflag = 0;
        }
      }
      else if(!I2C_SR1_STOPF)   //could only be a Stop Event then...
      {
        PIN_ERROR = 1;
        __no_operation();
        PIN_ERROR = 0;
      }
    }
}


/*-------------------------------Start functions-------------------------------*/

void I2C_Transaction(uint8_t read,uint8_t slaveAddress, uint8_t* buffer,uint8_t count)
{
  i2c.readwrite = read;
  i2c.SlaveAddress = slaveAddress;
  i2c.buffer_index = 0;
  i2c.masterBuffer = buffer;
  i2c.masterTransactionLength = count;
  i2c.masterMode = 1;
  
  I2Cflag = 1; 
  
  //wait for the Bus to get Free to avoid collisions
  while(I2C_SR3_BUSY);
  
  I2C_CR2_ACK = 1;	//Acknowledge Enable : Acknowledge returned after a byte is received (matched address or data)
  I2C_CR2_START = 1;	//Launch the process
}

void I2C_Write(uint8_t slaveAddress, uint8_t* buffer,uint8_t count)
{
  I2C_Transaction(0x00,slaveAddress,buffer,count);
}

//  Bit bang data on the diagnostic pins.

void BitBang(unsigned char byte)
{
    for (short bit = 7; bit >= 0; bit--)
    {
        if (byte & (1 << bit))
        {
            PIN_BIT_BANG_DATA = 1;
        }
        else
        {
            PIN_BIT_BANG_DATA = 0;
        }
        PIN_BIT_BANG_CLOCK = 1;
        __no_operation();
        PIN_BIT_BANG_CLOCK = 0;
    }
    PIN_BIT_BANG_DATA = 0;
}
