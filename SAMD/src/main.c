/************************************************************************/
/*	Project Name: STACK MONITORING KIT 
	STARTED ON: JUNE, 2015
	MODEL NAME: PEM-SMK 20
	Version: 1.0
	AUTHOR: SAJID SHAIKH
	COMPANY: POLLTECH INTRUMENTS PVT. LTD.,
	
			 MULUND(WEST), MUMBAI-400080 */
/**************	**********************************************************/


#include <asf.h>
#include <math.h>
#include "HardwareDefs.h"
#include "GlobalVarsnPrototypes.h"
#include "Delay.h"
#include "S_PORT.h"
#include "S_GCLOCK.h"
#include "CharLCD.h"
#include "EEProm.h"
#include "MatrixKeypad.h"
#include "ExtADC.h"
#include "LCDAdvanced.h"
#include "Computations.h"
#include "UserInterface.h"
#include "EEPmap.h"
#include "Calibration.h"
#include "S_SPI.h"




void config_timer(void);
void config_lcd(void);

uint8_t  bell[8]={4,14,14,14,31,0,4};
uint8_t  smiley[8]={0,0,10,0,17,14,0,0};
uint8_t  PILOGO[8]={24,24,0,31,25,31,24,24};
uint8_t  framelement[8]={10,10,10,10,10,10,10,10};
uint8_t	DegreeSym[8]={12, 18, 18, 12, 0, 0, 0, 0};
	
const char MSG[][LCDCOLS]=	{	{"1. Line in Lcd disp"}, 
								{"2. Line in lCd disp"}, 
								{"3. Line in lcD disp"},
								{"4. Line in LCD Disp"},
								{"5. Option.........."}	
							};
const char ms[]={"Hello world"};
		
/************************************************************************/
/* Sets up Timer to output PWM wave to drive LCD contrast charge pump*/
/************************************************************************/
void Config_Contrast_PWM(void){
	
	//Enable TC7 in power managers
	REG_PM_APBCMASK |= (1<<15);
	
	//Assign GCLK0 to Timer //GCLK0 runs at 48 MHz
	GCLKGen_Peripheral(GCLKGEN0 , GCLK_TC6_GCLK_TC7);
	
	//Reset Timer
	REG_TC7_CTRLA=(1<<0);
	
	//	Start Timer
	REG_TC7_CTRLBSET=(1<<6);
	//PWM output pin
	pinMux(LCD_Contrast_PWM,ETCmux);				
					//presync |prescale|wave mode|TC Mode| Enable
	REG_TC7_CTRLA = (0<<12) | (3<<8) | (2<<5) | (1<<2) | (1<<1);	
	//127 =50%, will need value from EEPROM
	REG_TC7_COUNT8_CC1= 127;	 //This register's not enable protected
}

/***************************************************************************************************/
/*	Contrast value will be between 0 to 5
	Hence corresponding PWM value will be between 0 to 130
	which gives 0 to 51% duty cycle, charge pump's o/p(-ve voltage) is highest with 50 % duty cycle
	So LCD contrast is highest with 50 % duty cycle */
/***************************************************************************************************/
void Set_LcdContrast(uint8_t ContrastValue){

	REG_TC7_COUNT8_CC1= 26*ContrastValue;
}

/************************************************************************/
/* Initializes LCD, Contrast, and Builds some required symbols          */
/************************************************************************/
void Config_LCD(void){
	Config_Contrast_PWM();	//not needed for OLED, since no contrast pin
	//LCD_Init();
	LCD_Init();
	LCD_CursorOff();
	//Build symbol in LCD's CGRAM to be used later on
	LCD_build(0, framelement);	//build '||' symbol at location 0, used to draw frame
	LCD_build(1, PILOGO);		//build Polltech LOGO at location 1
	LCD_build(2,DegreeSym);
}

void Disp_IntroLcdMsgs(void){
	LCD_PowerOff();
	LCD_Frame1();			//make good looking frame around LCD
	LCD_Setcursor(1,9);
	LCD_Print("   ");
	LCD_Setcursor(1,10);
	LCD_DataWrite(1);		//prints polltech logo that is stored at location 1
	LCD_Setcursor(2,2);
	LCD_Print("     Polltech     ");
	LCD_Setcursor(3,2);
	LCD_Print("    Instruments   ");
	LCD_PowerOn();
	delay_sec(0);
	LCD_Setcursor(2,2);
	LCD_Print("StackMonitoringKit");
	LCD_Setcursor(3,2);
	LCD_Print(" Model PEM-SMK 20 ");
	
	delay_sec(0);
}

void DoAutoZero(void){
		
	LCD_FullDisp(	" Ready for Auto-Zero",
					"(Keep Pitob Ass. Out",
					"& Sampling Pump Off)",
					"  Press Enter Key   ");		
	
	//w8 for user to press ENTER key
	WaitFor(EnterKey);
	//Enter key pressed, go to AUTOZERO of Pitot_DP, Orifice_DP and Orifice_GAS_DP.

	//User is expected to remove any input to system, hence read values are offsets
	LCD_FullDisp(	"  Doing Auto-Zero   ",
					"(Keep Pitob Ass. Out",
					"& Sampling Pump Off)",
					"                    ");	
	delay_sec(1);
	
	OrifPart_DP.Zero= ExtAnalogRead(Ch_Orifice_DP);
	LCD_Setcursor(4,2);
	LCD_Print("------");	//Graphic loading Effect ;)
	delay_sec(1);
	Pitot_DP.Zero=	ExtAnalogRead(Ch_Pitot_DP);
	LCD_Print("------");
	delay_sec(1);
	OrifGas_DP.Zero= ExtAnalogRead(Ch_Orifice_Gas_DP);
	LCD_Print("------");
	LCD_FullDisp(	"                    ",	
					"   Auto-Zero done   ",
					"  Press Enter Key   ",				
					"                    ");
	WaitFor(EnterKey);
}





void Initialize_Hardware(){
	Config_MatrixKeypad();  
	Config_LCD();
	configure_EEPROM();
	ConfigureSpi(SERCOMX0, 2000000);
	pinMode(F350Ready, INPUT);	//make input with pull up
	pinMode(BuzzerPin, WEAKOUT);
	pinMode(PA09, OUTPUT);
	PinSet(PA09);
	//pinMux(PB23, GCLKmux);	
}

int main (void)
{
	system_init();

	
	Initialize_Hardware();
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	#ifndef BypassLCD
	CalibrationModeChk();
	Disp_IntroLcdMsgs();	//Display company name, model name 	
//	DoAutoZero();
	LCD_Clrscr();
	#endif
	LCD_Setcursor(3,20);
	LCD_DataWrite(ARROW);
	
	LCD_Setcursor(1,1);
	ExtADCHi=ExtADCLo+ExtADCMid+ExtADCHi;
	while(1){
	Spi_Get3BytesfromExtADC(Ch_Orifice_DP);
	}
	//	volatile float asd= Compute_PT100Temp(9900);
					
	//LCD_FullDisp(	LCD_BLANK_LINE_MACRO,
	//" Erase stored data? ",
	//"    Yes   /   No    ",
	//LCD_BLANK_LINE_MACRO);
	//LCD_Frame2();		
//	volatile uint32_t as= CombineDigitstoNumber1(arr, 3);
volatile uint8_t selected=0;
	
//	selected =LCD_MenuHandle(MenuSize(CalibrationMenu),CalibrationMenu);
//	LCD_FullDisp(MSG[0],MSG[1],MSG[2],MSG[3]);
//	LCD_FullDisp(CalibrationMenu[0],CalibrationMenu[1],CalibrationMenu[2],CalibrationMenu[3]);
//	LCD_MenuDisplay(CalibrationMenu, 4);
	
	LCD_Setcursor(1,2);
	LCD_CursorOn();
	LCD_DispAscii(selected);
//	LCD_DispVariable(12345,2,10,0,1);

	Ambient_PT100.STDvalue=10021;
	volatile uint32_t ans=GetNumDataFromUser(32768,2,3,1,10);
	LCD_DispVariable(ans, 2, 3, 3, 10);
	volatile uint8_t a,b,c,d;

	a=EE_MISCAddr(c);
	b=EE_CALAddr(EE_OrifPart_Z);
	c=EE_CALAddr(EE_AuxTemp_STDUP);
	d=EE_CALAddr(EE_Abs_ADCLOW);
	a=a+b+c+d;
	
while (1)
{	
	PinSet(BuzzerPin);
	delay_sec(2);
	PinClr(BuzzerPin);
	delay_sec(2);	
}
	// Insert application code here, after the board has been initialized.
}