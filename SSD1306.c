/********************************************************
Company: Dinamica y Desarrollo								
Designed by: Juan Sebastian Rinc�n Manrique
Date: 06/12/2017
Description: This libary manages the SSD1306 functions.
Dependencies: I2C, Extevents, ClockSys, fonts
*********************************************************/

/**************************************************
* Libraries include
**************************************************/

#include "SSD1306.h"

/*---------------------------Start init functions------------------------------*/

static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = 
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xF0, 0xF8, 0xFC, 0x06, 0x07, 0x07, 0xE7, 0xE7, 0x67, 0x67, 0x67, 0x67,
0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x07, 0x07, 0xFF,
0xFF, 0x07, 0x07, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67, 0x67,
0x67, 0x67, 0x67, 0xE7, 0xE7, 0x07, 0x07, 0x06, 0xFC, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //End of page 0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C, 0x00, 0x7F, 0x41, 0x41, 
0x22, 0x1C, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0xFF,
0xFF, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //End of page 1 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x61, 0x51, 0x49, 
0x46, 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x36, 0x49, 0x49,
0x49, 0x36, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0xFF,
0xFF, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //End of page 2
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //End of page 3
};

static int8_t _i2caddr;
static int8_t _vccstate;

#define ssd1306_swap(a, b) { int16_t t = a; a = b; b = t; }
#define rotation	(0)
#define getRotation()	(0)

void ssd1306_command(uint8_t c);

uint8_t Data[2];
uint8_t bufferaux[17];

void Mini_SSD1306_gotoxy(uint8_t x_pos, uint8_t y_pos)
{
  ssd1306_command(SSD1306_SET_PAGE_START_ADDRESS + y_pos);
  ssd1306_command((x_pos & 0x0F) | SET_LOWER_COLUMN_START_ADDRESS);
  ssd1306_command((x_pos & 0x0F) >> 0x04 | SET_HIGHER_COLUMN_START_ADDRESS);
}

void Mini_SSD1306_drawPixel(int16_t x, int16_t y, uint8_t color) 
{
  switch (color)
  {
  case WHITE:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
  case BLACK:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break;
  case INVERSE: buffer[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break;
  }
}

void Mini_SSD1306_begin(uint8_t vccstate, uint8_t i2caddr, char reset)
{
  _vccstate = vccstate;
  _i2caddr = i2caddr;
  
  ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
  ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  ssd1306_command(0x80);                                  // the suggested ratio 0x80

  ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
  ssd1306_command(SSD1306_LCDHEIGHT - 1);

  ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  ssd1306_command(0x0);                                   // no offset
  ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x10); }
  else
    { ssd1306_command(0x14); }
  ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
  ssd1306_command(0x00);                                  // 0x0 act like ks0108
  ssd1306_command(SSD1306_SEGREMAP | 0x1);
  ssd1306_command(SSD1306_COMSCANDEC);                    // 0xC8
  
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x02);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  ssd1306_command(0x8F);
  
  ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xD9
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x22); }
  else
    { ssd1306_command(0xF1); }
  ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  ssd1306_command(0x40);
  ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6

  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel 0xAF
  
 // buffer = dyd_logo;
}

void Mini_SSD1306_invertDisplay(uint8_t i) 
{
  if (i) 
  {
    ssd1306_command(SSD1306_INVERTDISPLAY);
  } else 
  {
    ssd1306_command(SSD1306_NORMALDISPLAY);
  }
}

 void ssd1306_command(uint8_t c)
 {
  while(I2Cflag);
  Data[0] = CMD;         //Control Byte
  Data[1] = c;
  I2C_Write(_i2caddr,Data,3);
}

void Mini_SSD1306_startscrollright(uint8_t start, uint8_t stop)
{
  ssd1306_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X00);
  ssd1306_command(0XFF);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void Mini_SSD1306_startscrollleft(uint8_t start, uint8_t stop)
{
  ssd1306_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X00);
  ssd1306_command(0XFF);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void Mini_SSD1306_stopscroll(void)
{
  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
}

void fill_buffer_aux(uint8_t *buffer_source)
{
  bufferaux[0]=DAT;
  for (uint16_t i=1;i<17;i++){
    bufferaux[i]=buffer_source[i];
  }
}

void Mini_SSD1306_display(void)
{
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(0);   // Column start address (0 = reset)
  ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)
  
  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(0); // Page start address (0 = reset)
  
  ssd1306_command(3); // Page end address
    
  for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) 
  {
    while(I2Cflag); 
    fill_buffer_aux(&buffer[i]);
    I2C_Write(_i2caddr,bufferaux,18);
    i+=15;
  }
}

void Mini_SSD1306_clearDisplay(void) 
{
  memset(buffer, 0x00, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
}

void Mini_SSD1306_putChar(uint8_t x_pos, uint8_t y_pos, uint8_t ch)
{
  uint8_t s = 0x00;
  uint8_t chr = 0x00;
  
  chr = (ch - 0x20);
  
  for (s = 0x00; s < 0x06; s++)
  {
    buffer[x_pos+s] = font_regular[chr][s];
  }
}

void Mini_SSD1306_putString(uint8_t x_pos, uint8_t y_pos, char *ch)
{
  do
  {
    Mini_SSD1306_putChar(x_pos, y_pos, *ch++);
    x_pos += 0x06;
  }while((*ch >= 0x20) && (*ch <= 0x7F) && (*ch != '\n'));
}