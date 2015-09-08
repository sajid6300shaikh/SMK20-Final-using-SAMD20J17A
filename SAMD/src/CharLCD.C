/*
20X4 LCD library routines created for Atmel SAMD Series controllers.

By:Sajid Shaikh

Contains all low level and high level routines for Full four line display, Single line display,
Many cursor options, Custom Character Building, etc

*/

#include "CharLCD.h"
#include "Delay.h"
#include "HardwareDefs.h"

	
void LCD_Init(void)
{
//First Setup All port pins properly
//Setup port as output and pin LCD_D7 as output with input read enable.
pinMode(LCD_EN, OUTPUT);
PinClr(LCD_EN);
pinMode(LCD_D4, OUTPUT);		//D4
pinMode(LCD_D5, OUTPUT);		//D5
pinMode(LCD_D6, OUTPUT);		//D6
pinMode(LCD_D7, OUTPUT);		//D7, will be changed when we need to read back busy flag

pinMode(LCD_RS, OUTPUT);
pinMode(LCD_RWb, OUTPUT);

//------------------------------------------------------------------------

 _data_pins[0] = LCD_D4;
 _data_pins[1] = LCD_D5;
 _data_pins[2] = LCD_D6;
 _data_pins[3] = LCD_D7;
 
	
	// Select the Command Register by pulling RS pin LOW
	PinClr(LCD_RS);
	// Select the Write Operation  by pulling RW pin LOW
	PinClr(LCD_RWb);
	

//LCD commands start here

	delay_ms(100);		//w8 for power rail to stabilize
	
		LCD_CmdWrite(0x08);		//Turn off display, very important for OLED
		checkbusy();
		
	write4bits(0x03);	//put LCD in 8 bit mode
	delay_ms(100);
	
	write4bits(0x03);	//again
	delay_ms(100);
	
	write4bits(0x03);	//again
	delay_ms(100);
	
	write4bits(0x02);  //Initialize the LCD in 4bit Mode
	delay_ms(100);
			
	LCD_CmdWrite(0x28);	//LCD in 4 bit, 2 lines, 5x8 dots 
	checkbusy();

	//  [7/25/2015 sajid]
	LCD_CmdWrite(0x08);		//Turn off display, very important for OLED
	checkbusy();	
	//  [7/25/2015 sajid]
	//LCD_PowerOff();
	
	LCD_CmdWrite(0x06);		//Entry mode:Increment, No display shift
	checkbusy();
	
	LCD_CmdWrite(0x0E);		// Display ON, cursor ON, Blink Off
	checkbusy();  
	
	LCD_CmdWrite(0x01);
	checkbusy();			// Clear the LCD
	
	LCD_CmdWrite(0x80);
	checkbusy();			// Move the Cursor to First line First Position
	LCD_PowerOn();
}

void PulseLCD_EN(void){
	PinSet(LCD_EN);
	delay_us(ENPulse);
	PinClr(LCD_EN);
}

void LCD_Setcursor(char Row, char Column)	
//pass row and column no to this function
//for eg. LCD_Setcursor(1,15), for row 1st and column 15 on LCD, since numbering here starts from 0.
{
	switch(Row)		
	{
			case 1:
			LCD_CmdWrite(0x80+Column-1);
			break;
			
			case 2:
			LCD_CmdWrite(0xC0+Column-1);
			break;
			
			case 3:
			LCD_CmdWrite(0x94+Column-1);
			break;
			
			case 4:
			LCD_CmdWrite(0xD4+Column-1);
			break;
	}
}

void write4bits(uint8_t value)
{
	for (int i = 0; i < 4; i++)
	{
		pinMode(_data_pins[i], OUTPUT);
		digitalWrite(_data_pins[i], (value >> i) & 0x01);
	}
	delay_us(5);
	PulseLCD_EN();
}

void LCD_DataWrite(uint8_t Data){
	// Select the Data Register by pulling RS High
	PinSet(LCD_RS);
	// Select the Write Operation  by pulling RW LOW
	PinClr(LCD_RWb);
	  
	  write4bits(Data>>4);
	  write4bits(Data);
	  checkbusy();
}

void LCD_Putchar(char Data){
	// Select the Data Register by pulling RS High
	PinSet(LCD_RS);
	// Select the Write Operation  by pulling RW LOW
	PinClr(LCD_RWb);
	
	write4bits(Data>>4);
	write4bits(Data);
	checkbusy();
}

void LCD_CmdWrite(uint8_t Command){
	// Select the Command Register by pulling RS pin LOW
	PinClr(LCD_RS);
	// Select the Write Operation  by pulling RW pin LOW
	PinClr(LCD_RWb);
	
	write4bits(Command>>4);
	write4bits(Command);
	checkbusy();
}


void LCD_Print(const char *str)	//usage: LCD_disp("Hello World");
{
	while(*str) 			//Till string ends
	LCD_DataWrite(*str++); 	//Send characters one by one
}

void LCD_OneLine(uint8_t Row, const char *str)	//usage: LCD_disp(2, "Hello World"); to display on 2nd line
{
	LCD_Setcursor(Row,1);
	while(*str) 			//Till string ends
	LCD_DataWrite(*str++); 	//Send characters one by one
}


void LCD_DispAscii(char a){	//adds 48 offset to data before sending to LCD, can be used to print single digit no on LCD
	LCD_DataWrite(a+48);
}

void LCD_FullDisp(const char *str0,const char *str1,const char *str2,const char *str3 )	//pass a string for each row	
{			
	
//Fills all four lines of LCD with the passed strings msg.
	LCD_Setcursor(1,1);
	LCD_Print(str0);

	LCD_Setcursor(2,1);
	LCD_Print(str1);

	LCD_Setcursor(3,1);
	LCD_Print(str2);

	LCD_Setcursor(4,1);
	LCD_Print(str3);
	
	
}


void checkbusy()		//waits until LCD is busy
{
#ifndef BypassLCD
//delay_ms(2);
	volatile bool p27=1;			// temporary boolean bit var.

	pinMode(LCD_D7, INPUT);		//configure lcd_d7 as input
	PinClr(LCD_RS);
	PinSet(LCD_RWb);	//RWb=1 indicates read operation.
	
	while(p27) 
	{
	PinClr(LCD_EN);
	delay_us(ENPulse);
	PinSet(LCD_EN);

		p27=PinRead(LCD_D7);
		
	PinClr(LCD_EN);
	delay_us(ENPulse);
	PinSet(LCD_EN);
	}
	PinClr(LCD_EN);
	PinClr(LCD_RWb);
	pinMode(LCD_D7,OUT);	//reconfigure LCD_D7 as output
#endif
}

void LCD_build(uint8_t location, uint8_t *ptr){		//character should be built before writing anything to lcd.
	uint8_t i;
	if(location<8){
		LCD_CmdWrite(0x40+(location*8));	//since each symbol eats up 8 bytes, location is multiplied by 8
		for(i=0;i<8;i++)
		LCD_DataWrite(ptr[ i ]);
	}
	
}


void LCD_build2(){			//polltech Symbol
	LCD_CmdWrite(0x41); //Load the location where we want to store
	LCD_DataWrite(0x10); //Load row 1 data
	LCD_DataWrite(0x00); //Load row 2 data
	LCD_DataWrite(0x1e); //Load row 3 data
	LCD_DataWrite(0x11); //Load row 4 data
	LCD_DataWrite(0x1e); //Load row 5 data
	LCD_DataWrite(0x10); //Load row 6 data
	LCD_DataWrite(0x10); //Load row 7 data
	LCD_DataWrite(0x00); //Load row 8 data
}

void LCD_Frame1(){
	LCD_Setcursor(1,1);
	LCD_DataWrite(255);
	LCD_Print("==================");
	LCD_Setcursor(1,20);
	LCD_DataWrite(255);		//block symbol
	
	LCD_Setcursor(4,1);
	LCD_DataWrite(255);
	LCD_Print("==================");
	LCD_Setcursor(4,20);
	LCD_DataWrite(255);
	
	LCD_Setcursor(2,1);
	LCD_DataWrite(0);		// '||' symbol
	
	LCD_Setcursor(2,20);
	LCD_DataWrite(0);
	LCD_Setcursor(3,1);
	LCD_DataWrite(0);
	LCD_Setcursor(3,20);
	LCD_DataWrite(0);
}

#define frmcornr	'-'
void LCD_Frame2(){				//ANOTHER FRAME FOR GENERAL PURPOSE
	LCD_Setcursor(1,1);
	LCD_Print("--------------------");
	
	LCD_Setcursor(4,1);
	LCD_Print("____________________");
	
	//LCD_Setcursor(2,1);
	//LCD_DataWrite('|');		// '|' symbol
	//LCD_Setcursor(2,20);
	//LCD_DataWrite('|');
	//LCD_Setcursor(3,1);
	//LCD_DataWrite('|');
	//LCD_Setcursor(3,20);
	//LCD_DataWrite('|');
}

void LCD_CursorOn(){
	LCD_CmdWrite(0x0E);
}

void LCD_CursorOff(){
	LCD_CmdWrite(0x0C);
}

void LCD_CursorBlink(){
	LCD_CmdWrite(0x0F);
}

void LCD_Clrscr(){
	LCD_CmdWrite(0x01);
	LCD_Setcursor(1,1);
}
void LCD_PowerOff(){
//  [7/25/2015 sajid]
	LCD_CmdWrite(0x13);	//sets internal power off
	checkbusy();  
//---
}
void LCD_PowerOn(){
	LCD_CmdWrite(0x17);	//sets internal power ON
	checkbusy();
}


/************************************************************************/
/* OLD LCD write functions                                             */
/************************************************************************/
//void LCD_DataWrite(uint8_t Data)	//same as cmd functn, only RS pin is high in this case.
//{
	//volatile uint32_t lcddata =0;
	//// GET the Higher Nibble of the command in lcddata buffer
	//lcddata = ( 0xF << LCD_Shift) & (Data << (LCD_Shift-4));
	//
	//// Select the Data Register by pulling RS High
	//PinSet(LCD_RS);
	//// Select the Write Operation  by pulling RW LOW
	//PinClr(LCD_RWb);
	//
	////put lcddata buffer's data on output port.
	//LCD_BUS =LCD_BUS & ~LCD_BUS_Mask;
	//LCD_BUS =LCD_BUS | lcddata;
	//delay_us(2);
	//// Send a enable pulse to lcd
	//PinSet(LCD_EN);
	//delay_us(ENPulse);
	//PinClr(LCD_EN);
//
	//delay_ms(1);                   // wait for some time
	//
	////Now you know the Drill
	//lcddata =0;
	//lcddata = ( 0xF << LCD_Shift) & (Data << LCD_Shift);
//
	//PinSet(LCD_RS);
	//PinClr(LCD_RWb);
	//
	//LCD_BUS =LCD_BUS & ~LCD_BUS_Mask;
	//LCD_BUS =LCD_BUS | lcddata;
	//delay_us(2);
	//PinSet(LCD_EN);
	//delay_us(ENPulse);
	//PinClr(LCD_EN);
	//checkbusy();
//}
//
//void LCD_CmdWrite(uint8_t Command)
//{
	//volatile uint32_t lcddata =0;	//temp var
	//// GET the Higher Nibble of the command in lcddata buffer
	//lcddata = ( 0xF << LCD_Shift) & (Command << (LCD_Shift-4));
	//// Select the Command Register by pulling RS pin LOW
	//PinClr(LCD_RS);
	//
	//// Select the Write Operation  by pulling RW pin LOW
	//PinClr(LCD_RWb);
	//
	////this instruction puts the nibble on o/p pins
	//LCD_BUS = LCD_BUS & ~LCD_BUS_Mask;		//put lcddata buffer's data on output port.
	//LCD_BUS = LCD_BUS | lcddata;
	//delay_us(2);
	//// Send a enable pulse to lcd
	//PinSet(LCD_EN);
	//delay_us(ENPulse);
	//PinClr(LCD_EN);
	//delay_ms(2);
	//
	//lcddata =0;
	//// GET the Lower Nibble of the command in lcddata buffer
	//lcddata = ( 0xF << LCD_Shift) & (Command << LCD_Shift);
	//
	//// Select the Command Register by pulling RS LOW
	//PinClr(LCD_RS);
	//
	//// Select the Write Operation  by pulling RW LOW
	//PinClr(LCD_RWb);
//
	//LCD_BUS =LCD_BUS & ~LCD_BUS_Mask;
	//LCD_BUS =LCD_BUS | lcddata;
	//delay_us(2);
	//// Send a enable pulse to lcd
	//PinSet(LCD_EN);
	//delay_us(ENPulse);
	//PinClr(LCD_EN);
	//checkbusy();
//}
