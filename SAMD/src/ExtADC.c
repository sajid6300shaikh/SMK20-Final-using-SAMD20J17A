#include "compiler.h"
#include "HardwareDefs.h"
#include "GlobalVarsnPrototypes.h"
#include "Delay.h"
#include "S_PORT.h"
#include "S_GCLOCK.h"
#include "CharLCD.h"
#include "MatrixKeypad.h"
#include "ExtADC.h"
#include "LCDAdvanced.h"
#include "Computations.h"
#include "UserInterface.h"
#include "EEPmap.h"
#include "Calibration.h"
#include "S_SPI.h"



/************************************************************************/
/* Gets ADC Data from F350 and stores it in global vars in this way,
bool ExtADCSign: stores sign bit (MSB of received ADC data in this register)
uin8_t ExtADCHi, ExtADCMid, ExtADCLo: these three 8 bit register will contain 3 bytes of ADC data
int32_t ExtADC24bitData: will contain combined 24 bit signed data



/************************************************************************/
void Spi_Get3BytesfromExtADC(EnumExtAdcChannel Channel)
{	
	uint8_t synccheck=0;
			
	do //synchronisation loop
	{	/*will loop until received data=AAh*/
		while(!digitalRead(F350Ready)); //w8 till Ext ADC is ready
		//transfer ADC channel CMD to F350 when it's ready
		PinClr(PA09);														//First data is just command for channel
		delay_us(5);
		SPIDataReg=6;
		delay_us(5);
		PinSet(PA09);
		synccheck=SPIDataReg;
		
	} while (synccheck!=0XAA);
	
	while(!digitalRead(F350Ready));
	PinClr(PA09);
	delay_us(5);
	SPIDataReg= 1;
	delay_us(5);
	PinSet(PA09);
	ExtADCHi=SPIDataReg;
	
	while(!digitalRead(F350Ready));
	PinClr(PA09);
	delay_us(5);
	SPIDataReg= 1;
	delay_us(5);
	PinSet(PA09);
	ExtADCMid=SPIDataReg;
	
	while(!digitalRead(F350Ready));
	PinClr(PA09);
	delay_us(5);
	SPIDataReg= 1;
	delay_us(5);
	PinSet(PA09);
	ExtADCLo=SPIDataReg;
	
		#ifdef DiSP_ADC_3Var_Data_for_debug
			LCD_DispVariable(synccheck, 0, 3, 1, 5);
			LCD_DispVariable(ExtADCHi, 0, 3, 2, 5);
			LCD_DispVariable(ExtADCMid, 0, 3, 3, 5);
			LCD_DispVariable(ExtADCLo, 0, 3, 4, 5);
		#endif
	
	volatile bool ExtADCSign=(0b10000000)&(ExtADCHi);
	ExtADCHi=ExtADCHi&(0b01111111);
	LCD_Setcursor(2,9);
	if (ExtADCSign==true)
	{	//ADC data is negative (ADC gives negative data in 2s complement form)
		//LCD_DataWrite('-');
		ExtADC24bitData=(ExtADCHi<<8)|(ExtADCMid);
		ExtADC24bitData=32768-ExtADC24bitData;						
	}
	
	else{
		//LCD_DataWrite('+');
	ExtADC24bitData=(ExtADCHi<<8)|(ExtADCMid);
	}
	LCD_DispVariable(ExtADC24bitData, 0, 6, 2, 10);
	// unsigned int result=(SPIHI<<9)|(SPIMID<<1)|(SPILO>>7);

}


int32_t ExtAnalogRead_SignInt(EnumExtAdcChannel ReqChannel){
	//here channel is variable of type EnumAdcChannel
	switch(ReqChannel){
		case Ch_Orifice_DP:
		
		return(-32769);
		break;
		
		case Ch_Orifice_Gas_DP:
		return(2);	
		break;
		
		case Ch_Pitot_DP:
		return(3);
		break;
		
		case Ch_Stack_TC:
		return(4);
		break;
		
		case Ch_Absolute_Pres:
		return(5);
		break;
		
		case Ch_Orifice_PT100:
		return(6);
		break;
		
		case Ch_Ambient_PT100:
		return(7);
		break;
		
		case Ch_Aux_PT100:
		return(8);
		break;
		
		default:
		return(0);	
	}
	
}