/*
 * File:   LCD.c
 * Author: Dina
 *
 * Created on February 12, 2021, 4:06 PM
 */

/*
 * REFERENCIA: https://simple-circuit.com/pic-microcontroller-mplab-xc8-lcd/
 * https://electrosome.com/lcd-pic-mplab-xc8/
 */
 
//librerias existentes
#include <xc.h>
#include <stdint.h>
//librería creada
#include "LCD.h"

char a;

#define LCD_FIRST_ROW          0x80
#define LCD_SECOND_ROW         0xC0
#define LCD_THIRD_ROW          0x94
#define LCD_FOURTH_ROW         0xD4
#define LCD_CLEAR              0x01
#define LCD_RETURN_HOME        0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_CURSOR_OFF         0x0C
#define LCD_UNDERLINE_ON       0x0E
#define LCD_BLINK_CURSOR_ON    0x0F
#define LCD_MOVE_CURSOR_LEFT   0x10
#define LCD_MOVE_CURSOR_RIGHT  0x14
#define LCD_TURN_ON            0x0C
#define LCD_TURN_OFF           0x08
#define LCD_SHIFT_LEFT         0x18
#define LCD_SHIFT_RIGHT        0x1E

//LCD module connections
#define LCD_RS   RE0    
#define LCD_EN   RE1
#define LCD_RW   RE2
#define LCD_D0   RD0
#define LCD_D1   RD1 
#define LCD_D2   RD2 
#define LCD_D3   RD3 
#define LCD_D4   RD4 
#define LCD_D5   RD5 
#define LCD_D6   RD6 
#define LCD_D7   RD7 
#define LCD_RS_DIR  TRISE0 
#define LCD_EN_DIR  TRISE1 
#define LCD_RW_DIR  TRISE2 
#define LCD_D0_DIR  TRISD0   
#define LCD_D1_DIR  TRISD1
#define LCD_D2_DIR  TRISD2
#define LCD_D3_DIR  TRISD3
#define LCD_D4_DIR  TRISD4 
#define LCD_D5_DIR  TRISD5 
#define LCD_D6_DIR  TRISD6 
#define LCD_D7_DIR  TRISD7
//End LCD module connections
#define _XTAL_FREQ 8000000


void LCDinit(void);
void LCDGoto(uint8_t col, uint8_t row);
void LCDWriteChar(char a);        //PUTS THE CHAR
void LCDPrintSTRING(char *a);     //PRINT
void LCDPort(char a);              //PORT
void LCDCmd(uint8_t Command);      //COMMAND

void LCDPORT(char a){
    PORTD = a;
}

void LCDinit(void){
  LCDCmd(0x38);
  LCDCmd(0x0c);
  LCDCmd(0x06);
  LCDCmd(0x80);
}

void LCDGoto(uint8_t col, uint8_t row){
switch(row)
  {
    case 2:
      LCDCmd(LCD_SECOND_ROW + col - 1);
      break;
    case 3:
      LCDCmd(LCD_THIRD_ROW  + col - 1);
      break;
    case 4:
      LCDCmd(LCD_FOURTH_ROW + col - 1);
    break;
    default:      // case 1:
      LCDCmd(LCD_FIRST_ROW  + col - 1);
  }    
} //VOID FINISHES

void LCDWriteChar(char LCDChar){
  LCDRS = 1;
  LCDPORT(LCDChar);
  LCD_EN = 1;
  __delay_us(45);
  LCD_EN = 0;
  LCD_RS = 0;
}

void LCDPrintString(char *LCDS){
    int i;
	for(i=0; LCDS[i]!='\0'; i++) //CICLO 
	   LCDwriteChar(LCDS[i]);
}

void LCDCmd(uint8_t Command){
  LCDPORT(Command);
  LCD_RS = 0;
  LCD_EN = 1;
  __delay_ms(6);
  LCD_EN = 0;
}

