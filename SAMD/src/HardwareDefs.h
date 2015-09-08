#ifndef __Hardware_Pins_Defined__
#define __Hardware_Pins_Defined__
#include "S_PORT.h"
/************************************************************************/
/*		Matrix Key Definitions		                                    */
/************************************************************************/

//Hardware
#define	ROW1	PB10
#define	ROW2	PB11
#define	ROW3	PB12
#define	COL1	PB13
#define	COL2	PB14


/************************************************************************/
/*		LCD Pin connections                                             */
/************************************************************************/
//Any random pin can be assigned to any of signals below.
//It need not be consecutive pins, even pins of different port can be assigned
#define LCD_D7			PA19		//Busy flag
#define LCD_D6			PA18
#define LCD_D5			PA17
#define LCD_D4			PA16

#define LCD_RS			PA20
#define LCD_EN			PB16
#define LCD_RWb			PB17

#define LCD_Contrast_PWM	PA21	//used to generate ive bias for LCD, not needed when OLED is used

/************************************************************************/
/*      EEProm Connections                                              */
/************************************************************************/
#define SDA			PA12
#define SCL			PA13
#define WP			PA14

/************************************************************************/
/*		Buzzer pin			                                            */
/************************************************************************/
#define BuzzerPin	PB03

/************************************************************************/
/*		External ADC SPI Connection										*/
/************************************************************************/
//SPI Pins are configured in S_SPI.c routine according to selected Sercom No for SPI pin set is default
#define F350Ready PA07
#define SPIDataReg REG_SERCOM0_SPI_DATA


/****************************************************************************/
/*  External 32 KHz oscillator will be routed to this pin, can be checked
	on scope for diagnosis and accuracy checking		*/
/****************************************************************************/
#define XOSC32Out PB23

#endif