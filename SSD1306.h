/************************************************
Company: Dinamica y Desarrollo
Designed by: Juan Sebastián Rincón Manrique
Date: 16/01/2018
Description: Contains definitions and headers to 
controll a SSD1306 oled screen
**************************************************/

#ifndef _SSD1306_H
#define _SSD1306_H

/**************************************************
* Libraries include
**************************************************/

#include <iostm8s003k3.h>
#include <intrinsics.h>
#include <string.h>
#include "dataType.h"
#include "Extevents.h"
#include "I2C.h"
#include "fonts.h"
#include "dydlogo.h"

/**************************************************
* SSD1306 settings
**************************************************/

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define DAT                                             0x40
#define CMD                                             0x00

#define SSD1306_I2C_ADDRESS                             0x3C
#define SSD1306_128_32

#define SSD1306_LCDWIDTH                                128
#define SSD1306_LCDHEIGHT                               32

#define WIDTH	SSD1306_LCDWIDTH
#define HEIGHT	SSD1306_LCDHEIGHT

#define SSD1306_SETCONTRAST                             0x81
#define SSD1306_DISPLAYALLON_RESUME                     0xA4
#define SSD1306_DISPLAYALLON                            0xA5
#define SSD1306_NORMALDISPLAY                           0xA6
#define SSD1306_INVERTDISPLAY                           0xA7
#define SSD1306_DISPLAYOFF                              0xAE
#define SSD1306_DISPLAYON                               0xAF
#define SSD1306_SETDISPLAYOFFSET                        0xD3
#define SSD1306_SETCOMPINS                              0xDA
#define SSD1306_SETVCOMDETECT                           0xDB
#define SSD1306_SETDISPLAYCLOCKDIV                      0xD5
#define SSD1306_SETPRECHARGE                            0xD9
#define SSD1306_SETMULTIPLEX                            0xA8
#define SSD1306_SETLOWCOLUMN                            0x00
#define SSD1306_SETHIGHCOLUMN                           0x10
#define SSD1306_SETSTARTLINE                            0x40
#define SSD1306_MEMORYMODE                              0x20
#define SSD1306_COLUMNADDR                              0x21
#define SSD1306_PAGEADDR                                0x22
#define SSD1306_COMSCANINC                              0xC0
#define SSD1306_COMSCANDEC                              0xC8
#define SSD1306_SEGREMAP                                0xA0
#define SSD1306_CHARGEPUMP                              0x8D
#define SSD1306_EXTERNALVCC                             0x1
#define SSD1306_SWITCHCAPVCC                            0x2

#define SET_LOWER_COLUMN_START_ADDRESS  		0x00
#define SET_HIGHER_COLUMN_START_ADDRESS       		0x10
#define SSD1306_SET_PAGE_START_ADDRESS			0xB0

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A

/**************************************************
* Function headers
**************************************************/

void Mini_SSD1306_init(int8_t reset);
void Mini_SSD1306_begin(uint8_t switchvcc, uint8_t i2caddr, char reset);
void Mini_SSD1306_clearDisplay(void);
void Mini_SSD1306_invertDisplay(uint8_t i);
void Mini_SSD1306_display();
void Mini_SSD1306_startscrollright(uint8_t start, uint8_t stop);
void Mini_SSD1306_startscrollleft(uint8_t start, uint8_t stop);
void Mini_SSD1306_startscrolldiagright(uint8_t start, uint8_t stop);
void Mini_SSD1306_startscrolldiagleft(uint8_t start, uint8_t stop);
void Mini_SSD1306_stopscroll(void);
void Mini_SSD1306_dim(char dim);
void Mini_SSD1306_drawPixel(int16_t x, int16_t y, uint8_t color);
void Mini_SSD1306_gotoxy(uint8_t x_pos, uint8_t y_pos);
void Mini_SSD1306_putChar(uint8_t x_pos, uint8_t y_pos, uint8_t ch);
void Mini_SSD1306_putString(uint8_t x_pos, uint8_t y_pos, char *ch);
#endif