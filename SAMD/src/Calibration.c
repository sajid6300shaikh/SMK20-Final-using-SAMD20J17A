#include "GlobalVarsnPrototypes.h"
#include "Delay.h"
#include "S_PORT.h"
#include "S_GCLOCK.h"
#include "CharLCD.h"
#include "MatrixKeypad.h"
#include "ExtADC.h"
#include "LCDAdvanced.h"
#include "UserInterface.h"
#include "EEPmap.h"
#include "Calibration.h"
#include "Computations.h"

#define Passw	000

const char CalibrationMenu[][20]=
{
	{"1.Absolute Pressure"},	//0
	{"2.Parti. Orif.DP   "},	//1
	{"3.Gaseous Orif.DP  "},	//2
	{"4.Pitot DP         "},	//3
	{"5.Stack ThermoCup  "},	//4
	{"6.Ambient Temp.    "},	//5
	{"7.Meter Temp.      "},	//6
	{"8.Auxillary Temp.  "},	//7
	{"9.Particulate Flow "},	//8
	{"10.Gaseous Flow    "},	//9
};



void Cal_Disp2Point(volatile nAZSensors *Sensor){
	//Lower Count
	LCD_DispVariable(Sensor->ADCLower,0,5,3,7);
	//STD LOWER
	LCD_DispVariable(Sensor->STDLower,0,3,3,18);
	
	//Upper Count
	LCD_DispVariable(Sensor->ADCUpper, 0,5,4,7);
	
	//Std Upper
	LCD_DispVariable(Sensor->STDUpper,0,3,4,18);
};


void CalEdit_AbsPres(void){		//Absolute Pres Edit Cal
	int32_t EnteredData=0;
	Cal_Disp2Point(&Absolute_Pres);
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.ADCLower,0,5,3,7);
	if (EnteredData==-1)	//ESC key is pressed
	goto RETWoSaving;
	else
	{	//ENTER KEY IS PRESSED, save value in EEP
		Absolute_Pres.ADCLower=EnteredData;	//store ADC lower Data in respective sensors variable		
	}	
	
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.STDLower, 0, 3, 3,18);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		Absolute_Pres.STDLower=EnteredData;	//Store Std Lower Data
	}
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.ADCUpper, 0, 5, 4, 7);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		Absolute_Pres.ADCUpper=EnteredData;	//store ADCUpper counts
	}
	
	EnteredData=GetNumDataFromUserWithESC(Absolute_Pres.STDUpper, 0, 3, 4, 18);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		Absolute_Pres.STDUpper=EnteredData;	//store StdUpper counts
	}
	
	RETWoSaving:
	return;
}

void Calibrate_AbsPres(){
	
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"  Calibration of:   ",
						" Absolute Pressure  ",
						"Scan / Edit /Execute",
						"                    ");
		SelOpt =LCD_HorizontalScroll(1, 3,1,3,8,3,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan Mode: Abs.Pres.",
							"Calibration(in mmHg)",
							"Lower:       Std:   ",
							"Upper:       Std:   ");
			//display the calibration values.
			Cal_Disp2Point(&Absolute_Pres);
			AnykeyPulse();
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit Mode: Abs.Pres.",
							"Calibration(in mmHg)",
							"Lower:       Std:   ",
							"Upper:       Std:   ");
			CalEdit_AbsPres();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"ONLINE NOT IMPLEMENT");
			AnykeyPulse();
		}
	} while (SelOpt);
}

void Cal_Disp1Point(volatile AZSensors *Sensor, uint32_t NofdigitforSTD, uint8_t DecimalPosforStd){
	//This routine doesn't have Lower point 
		//Upper Count
		LCD_DispVariable(Sensor->ADCUpper, 0, 5, 3, 14);
		
		//Std Upper
		LCD_DispVariable(Sensor->STDUpper, DecimalPosforStd, NofdigitforSTD, 4,6);
	
	};


void CalEdit_OrifPartDP(){
	int32_t EnteredData=0;
	Cal_Disp1Point(&OrifPart_DP, 4, 1);

	EnteredData=GetNumDataFromUserWithESC(OrifPart_DP.ADCUpper, 0, 5, 3, 14);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		OrifPart_DP.ADCUpper=EnteredData;	//store ADCUpper counts
	}
	
	EnteredData=GetNumDataFromUserWithESC(OrifPart_DP.STDUpper, 1, 4, 4, 6);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		OrifPart_DP.STDUpper=EnteredData;	//store StdUpper counts
	}	
	RETWoSaving:
	return;
	
}
void Calibrate_OrifPartDP(){
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"   Calibration of   ",
						"Particulate Orif. DP",
						"Scan / Edit /Execute",
						"                    ");
		SelOpt =LCD_HorizontalScroll(1, 3,1,3,8,3,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan Mode:Part.Orif.",
							"DP Calibration:     ",
							"Span-Counts:        ",
							"Std:       mmWC     ");
			//Display the calibration values.
			Cal_Disp1Point(&OrifPart_DP, 4,1);
			AnykeyPulse();
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit Mode:Part.Orif.",
							"DP Calibration:     ",
							"Span-Counts:        ",
							"Std:       mmWC     ");
			CalEdit_OrifPartDP();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"ONLINE NOT IMPLEMENT");
			AnykeyPulse();
		}
	} while (SelOpt);
	
}


void CalEdit_OrifGasDP(){
	int32_t EnteredData=0;
	Cal_Disp1Point(&OrifGas_DP,4,1);

	EnteredData=GetNumDataFromUserWithESC(OrifGas_DP.ADCUpper, 0, 5, 3, 14);
	if (ParaKey==1)
	goto RETWoSaving;
	else
	{
		OrifGas_DP.ADCUpper=EnteredData;	//store ADCUpper counts
	}
	
	EnteredData=GetNumDataFromUserWithESC(OrifGas_DP.STDUpper, 1, 4, 4, 6);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		OrifGas_DP.STDUpper=EnteredData;	//store StdUpper counts
	}
	RETWoSaving:
	return;
	
}
void Calibrate_OrifGasDP(){
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"   Calibration of   ",
						" Gaseous Orifice DP ",
						"Scan / Edit /Execute",
						"                    ");
		SelOpt =LCD_HorizontalScroll(1, 3,1,3,8,3,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan Mode: Gaseous  ",
							"Orif. DP Calibration",
							"Span-Counts:        ",
							"Std:       mmWC     ");
			//Display the calibration values.
			Cal_Disp1Point(&OrifGas_DP,4,1);
			AnykeyPulse();
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit Mode:Part.Orif.",
							"DP Calibration:     ",
							"Span-Counts:        ",
							"Std:       mmWC     ");
			CalEdit_OrifGasDP();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"ONLINE NOT IMPLEMENT");
			AnykeyPulse();
		}
	} while (SelOpt);
	
}

void CalEdit_PitotDP(){
	int32_t EnteredData=0;
	Cal_Disp1Point(&Pitot_DP,5,1);

	EnteredData=GetNumDataFromUserWithESC(Pitot_DP.ADCUpper, 0, 5, 3, 14);
	if (ParaKey==1)
	goto RETWoSaving;
	else
	{
		Pitot_DP.ADCUpper=EnteredData;	//store ADCUpper counts
	}
	
	EnteredData=GetNumDataFromUserWithESC(Pitot_DP.STDUpper, 1, 5, 4, 6);
	if (EnteredData==-1)
	goto RETWoSaving;
	else
	{
		Pitot_DP.STDUpper=EnteredData;	//store StdUpper counts
	}
	RETWoSaving:
	return;
}


void Calibrate_PitotDP(){
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"   Calibration of   ",
						"      Pitot DP      ",
						"Scan / Edit /Execute",
						"                    ");
		SelOpt =LCD_HorizontalScroll(1, 3,1,3,8,3,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan Mode: Pitot DP ",
							"Calibration:        ",
							"Span-Counts:        ",
							"Std:        mmWC    ");
				//Display the calibration values.
			Cal_Disp1Point(&Pitot_DP, 5,1);
			AnykeyPulse();
		}
			
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit Mode: Pitot DP ",
							"Calibration:        ",
							"Span-Counts:        ",
							"Std:       mmWC     ");
			CalEdit_PitotDP();
		}
			
		if (SelOpt==3)
		{
			LCD_OneLine(2,"ONLINE NOT IMPLEMENT");
			AnykeyPulse();
		}
	} while (SelOpt);
}

int32_t GetTCSTDfromUserWithTempDisplay(){
	char DecimalPos=2;
	char NoofDigit=4;
	char Row=4;
	char Col=6;
	int8_t Digit[NoofDigit];

	uint32_t CurrentNum=Stack_TC.STDUpper;
	LCD_DispVariable(CurrentNum, DecimalPos, NoofDigit, Row, Col);	//display mV
	//First Display Current Temp according to STD mV  on LCD
	
	//Separate Digits of num and store it in digit array
	SplitNumbertoDigits(CurrentNum, Digit, NoofDigit);
	LCD_CursorOn();
	int8_t i=NoofDigit-1;
	uint8_t NewCol=Col;
	LCD_Setcursor(Row,NewCol);
	ReleaseKey();
	while(1){
		if (KeyDetected())
		{
			Keyscan();
			if (IncrKey)
			{
				Digit[i]++;
				Digit[i]=CheckDigitBound(Digit[i]);
			}
			
			if (DecrKey)
			{
				Digit[i]--;
				Digit[i]=CheckDigitBound(Digit[i]);
			}
			
			if (NextKey || PrevKey)		//shift cursor to right or left with press of Next or Prev key respectively
			{
				if (NextKey)
				i--;
				if (PrevKey)
				i++;
				
				if (i>(NoofDigit-1))
				i=0;
				if (i<0)
				i=NoofDigit-1;
				NewCol=Col+NoofDigit-1-i;
				if (NewCol>=(Col+NoofDigit-DecimalPos))
				NewCol++;
			}
			
			if (EnterKey)
			return CombineDigitstoNumber(Digit,NoofDigit);
			
			if (EscKey)
			return(-1);
			
			LCD_Setcursor(Row,NewCol);
			LCD_DispAscii(Digit[i]);			
			uint16_t tempmV=CombineDigitstoNumber(Digit,NoofDigit);
			LCD_DispVariable(Compute_StackTCTemp(tempmV), 0, 4, 4, 15);
			LCD_Setcursor(Row,NewCol);
		}
	}
}
void CalEdit_StackTC(){
		int32_t EnteredData=0;
		//--------------------------
		Cal_Disp1Point(&Stack_TC, 4,2);		//disp adc count and mV
		LCD_DispVariable(Compute_StackTCTemp(Stack_TC.STDUpper), 0, 4, 4, 15);	//disp temp
		LCD_Setcursor(4,19);
		LCD_DataWrite(DegreeSymbol);
		//--------------------------
		
		EnteredData=GetNumDataFromUserWithESC(Stack_TC.ADCUpper, 0, 5, 3, 14);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Stack_TC.ADCUpper=EnteredData;	//store ADCUpper counts
		}		
		EnteredData=GetTCSTDfromUserWithTempDisplay();	//routine to edit mv and show temp
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Stack_TC.STDUpper=EnteredData;	//store StdUpper counts
		}
		RETWoSaving:
		return;
	
}
void Calibrate_StackTC(){
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"   Calibration of   ",
						" Stack Thermocouple ",
						"Scan / Edit /Execute",
						"                    ");
		SelOpt =LCD_HorizontalScroll(1, 3,1,3,8,3,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan Mode: Stack T/C",
							"Calibration:        ",
							"Span-Counts:        ",
							"Std:       mV      C");
			//Display the calibration values.
			Cal_Disp1Point(&Stack_TC, 4,2);			
			LCD_DispVariable(Compute_StackTCTemp(Stack_TC.STDUpper), 0, 4, 4, 15);
			LCD_Setcursor(4,19);
			LCD_DataWrite(DegreeSymbol);
			AnykeyPulse();
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit Mode: Stack T/C",
							"Calibration:        ",
							"Span-Counts:        ",
							"Std:       mV      C");
			CalEdit_StackTC();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"ONLINE NOT IMPLEMENT");
			AnykeyPulse();
		}
	} while (SelOpt);	//loop until para key is pressed and selopt is given value 0
}

/********************************************************************************************/
/* Displays Pt100 Temp  with -/+ sign on LCD at given row and Col, only used in calibration,
In normal mode sign won't be displayed coz temp is expected to be positive only.*/
/********************************************************************************************/
void DispPt100TempWithSign(uint16_t PT100Resistance, char Row, char Col){
	//returns corresponding temp and puts sign in 'Pt100TempSign' Global var
	uint16_t Temperature=Compute_PT100Temp(PT100Resistance);
	bool TempSign=Pt100TempSign;
	LCD_Setcursor(Row,Col-1);
	if (TempSign==1)
		LCD_DataWrite('-');	//display minus sign
	else
		LCD_DataWrite('+');
	LCD_DispVariable(Temperature, 1, 4, Row, Col);
}

void Cal_DispPT100Points(volatile nAZSensors *PT100Sens){
	LCD_DispVariable(PT100Sens->ADCLower, 0, 5, 2, 5);//lower adc pt
	LCD_DispVariable(PT100Sens->ADCUpper, 0, 5, 2, 15);//upper adc
	
	LCD_DispVariable(PT100Sens->STDLower, 2, 5, 3, 5);//lower std resistace
	DispPt100TempWithSign(PT100Sens->STDLower, 4, 5);
	
	LCD_DispVariable(PT100Sens->STDUpper, 2, 5, 3, 14);
	DispPt100TempWithSign(PT100Sens->STDUpper, 4, 14);
}


/****************************************************************************************************/
/* Reusable code for getting STD PT100 data from user while displaying corresponding Temp data
It is an offshoot of GetNumdatafromUserfunction	*/
/****************************************************************************************************/
int32_t GetPT100STDfromUserWithTempDisplay(uint16_t STDRes, char Row, char Col){
	char TempRow=Row+1;
	char TempCol=Col;
	char NoofDigit=5;
	char DecimalPos=2;
	int8_t Digit[NoofDigit];
	uint32_t CurrentNum=STDRes;
	LCD_DispVariable(CurrentNum, DecimalPos, NoofDigit, Row, Col);
	//Separate Digits of num and store it in digit array
	SplitNumbertoDigits(CurrentNum, Digit, NoofDigit);			
	LCD_CursorOn();
	int8_t i=NoofDigit-1;
	uint8_t NewCol=Col;
	LCD_Setcursor(Row,NewCol);
	ReleaseKey();
	while(1){
		if (KeyDetected())
		{
			Keyscan();
			if (IncrKey)
			{
				Digit[i]++;
				Digit[i]=CheckDigitBound(Digit[i]);
			}
			
			if (DecrKey)
			{
				Digit[i]--;
				Digit[i]=CheckDigitBound(Digit[i]);
			}
			
			if (NextKey || PrevKey)		//shift cursor to right or left with press of Next or Prev key respectively
			{
				if (NextKey)
				i--;
				if (PrevKey)
				i++;
				
				if (i>(NoofDigit-1))
				i=0;
				if (i<0)
				i=NoofDigit-1;
				NewCol=Col+NoofDigit-1-i;
				if (NewCol>=(Col+NoofDigit-DecimalPos))
				NewCol++;
			}
			
			if (EnterKey)
			return CombineDigitstoNumber(Digit,NoofDigit);
			
			if (EscKey)
			return(-1);
			
			LCD_Setcursor(Row,NewCol);
			LCD_DispAscii(Digit[i]);
			uint16_t TempRes= CombineDigitstoNumber(Digit,NoofDigit);
			DispPt100TempWithSign(TempRes, TempRow, TempCol);
			LCD_Setcursor(Row,NewCol);
		}
	}
}

void CalEdit_AmbTemp(){
		Cal_DispPT100Points(&Ambient_PT100);
		int32_t EnteredData=0;
		EnteredData=GetNumDataFromUserWithESC(Ambient_PT100.ADCLower, 0, 5, 2, 5);
		if (EnteredData==-1)	//ESC key is pressed
		goto RETWoSaving;
		else
		{	//ENTER KEY IS PRESSED, save value in EEP
			Ambient_PT100.ADCLower=EnteredData;	//store ADC lower Data in respective sensors variable
		}
		
		EnteredData=GetPT100STDfromUserWithTempDisplay(Ambient_PT100.STDLower, 3,5);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Ambient_PT100.STDLower=EnteredData;	//Store Std Lower Data
		}
		EnteredData=GetNumDataFromUserWithESC(Ambient_PT100.ADCUpper, 0, 5, 2, 15);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Ambient_PT100.ADCUpper=EnteredData;	//store ADCUpper counts
		}
		
		EnteredData=GetPT100STDfromUserWithTempDisplay(Ambient_PT100.STDUpper, 3, 14);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Ambient_PT100.STDUpper=EnteredData;	//store StdUpper counts
		}
		
		RETWoSaving:
		return;	
}

void Calibrate_AmbTemp(){
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"Calibration of Amb. ",
						"Temp. Parameter for ",
						" PT100 Resistances  ",
						"Scan / Edit /Execute");
		SelOpt =LCD_HorizontalScroll(1, 4,1,4,8,4,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan:Amb.Temp. Cali.",
							"L/C:      U/C:      ",
							"Std:                ", //"Std:$$$.$$?  $$$.$$?"
							"          C        C");//"   -$$$.$°C +$$$.$°C"	//also show sign -/+
			//Display the calibration values.
			Cal_DispPT100Points(&Ambient_PT100);			
			LCD_Setcursor(4,19);
			LCD_DataWrite(DegreeSymbol);
			LCD_Setcursor(4,10);
			LCD_DataWrite(DegreeSymbol);
			AnykeyPulse();
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit:Amb.Temp. Cali.",
							"L/C:      U/C:      ",
							"Std:                ", //"Std:$$$.$$?  $$$.$$?"
							"          C        C");//"   -$$$.$°C +$$$.$°C"	//also show sign -/+
			LCD_Setcursor(4,19);
			LCD_DataWrite(DegreeSymbol);
			LCD_Setcursor(4,10);
			LCD_DataWrite(DegreeSymbol);								
			CalEdit_AmbTemp();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"ONLINE NOT IMPLEMENT");
			AnykeyPulse();
		}
	} while (SelOpt);	//loop until para key is pressed and selopt is given value 0
	
}

void CalEdit_OrificeTemp(){
		Cal_DispPT100Points(&Orifice_PT100);
		int32_t EnteredData=0;
		EnteredData=GetNumDataFromUserWithESC(Orifice_PT100.ADCLower, 0, 5, 2, 5);
		if (EnteredData==-1)	//ESC key is pressed
		goto RETWoSaving;
		else
		{	//ENTER KEY IS PRESSED, save value in EEP
			Orifice_PT100.ADCLower=EnteredData;	//store ADC lower Data in respective sensors variable
		}
		
		EnteredData=GetPT100STDfromUserWithTempDisplay(Orifice_PT100.STDLower, 3,5);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Orifice_PT100.STDLower=EnteredData;	//Store Std Lower Data
		}
		EnteredData=GetNumDataFromUserWithESC(Orifice_PT100.ADCUpper, 0, 5, 2, 15);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Orifice_PT100.ADCUpper=EnteredData;	//store ADCUpper counts
		}
		
		EnteredData=GetPT100STDfromUserWithTempDisplay(Orifice_PT100.STDUpper, 3, 14);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Orifice_PT100.STDUpper=EnteredData;	//store StdUpper counts
		}
		RETWoSaving:
		return;	
}

void Calibrate_OrifTemp(){
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"Calibration of Meter",
						"Temp. Parameter for ",
						" PT100 Resistances  ",
						"Scan / Edit /Execute");
		SelOpt =LCD_HorizontalScroll(1, 4,1,4,8,4,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan:MeterTemp.Cali.",
							"L/C:      U/C:      ",
							"Std:                ", //"Std:$$$.$$?  $$$.$$?"
							"          C        C");//"   -$$$.$°C +$$$.$°C"	//also show sign -/+
			//Display the calibration values.
			Cal_DispPT100Points(&Orifice_PT100);
			LCD_Setcursor(4,19);
			LCD_DataWrite(DegreeSymbol);
			LCD_Setcursor(4,10);
			LCD_DataWrite(DegreeSymbol);
			AnykeyPulse();
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit:MeterTemp.Cali.",
							"L/C:      U/C:      ",
							"Std:                ", //"Std:$$$.$$?  $$$.$$?"
							"          C        C");//"   -$$$.$°C +$$$.$°C"	//also show sign -/+
			LCD_Setcursor(4,19);
			LCD_DataWrite(DegreeSymbol);
			LCD_Setcursor(4,10);
			LCD_DataWrite(DegreeSymbol);
			CalEdit_OrificeTemp();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"* NOT IMPLEMENTED  *");
			AnykeyPulse();
		}
	} while (SelOpt);	//loop until para key is pressed and selopt is given value 0
	
}


void CalEdit_AuxPT100(){
		Cal_DispPT100Points(&Aux_Temp);
		int32_t EnteredData=0;
		EnteredData=GetNumDataFromUserWithESC(Aux_Temp.ADCLower, 0, 5, 2, 5);
		if (EnteredData==-1)	//ESC key is pressed
		goto RETWoSaving;
		else
		{	//ENTER KEY IS PRESSED, save value in EEP
			Aux_Temp.ADCLower=EnteredData;	//store ADC lower Data in respective sensors variable
		}
		
		EnteredData=GetPT100STDfromUserWithTempDisplay(Aux_Temp.STDLower, 3,5);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Aux_Temp.STDLower=EnteredData;	//Store Std Lower Data
		}
		EnteredData=GetNumDataFromUserWithESC(Aux_Temp.ADCUpper, 0, 5, 2, 15);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Aux_Temp.ADCUpper=EnteredData;	//store ADCUpper counts
		}
		
		EnteredData=GetPT100STDfromUserWithTempDisplay(Aux_Temp.STDUpper, 3, 14);
		if (EnteredData==-1)
		goto RETWoSaving;
		else
		{
			Aux_Temp.STDUpper=EnteredData;	//store StdUpper counts
		}
		RETWoSaving:
		return;	
	
}
void Calibrate_AuxTemp(){
	int8_t SelOpt=0;
	do
	{
		LCD_FullDisp(	"Calibration of Auxi.",
						"Temp. Parameter for ",
						" PT100 Resistances  ",
						"Scan / Edit /Execute");
		SelOpt =LCD_HorizontalScroll(1, 4,1,4,8,4,14);
		ReleaseKey();
		if (SelOpt==1)
		{	//Scan mode selected.
			LCD_FullDisp(	"Scan:Auxi.PT100 Cali",
							"L/C:      U/C:      ",
							"Std:                ", //"Std:$$$.$$?  $$$.$$?"
							"          C        C");//"   -$$$.$°C +$$$.$°C"	//also show sign -/+
			//Display the calibration values.
			Cal_DispPT100Points(&Aux_Temp);
			LCD_Setcursor(4,19);
			LCD_DataWrite(DegreeSymbol);
			LCD_Setcursor(4,10);
			LCD_DataWrite(DegreeSymbol);
			AnykeyPulse();		//wait for any key to be pressed and released
		}
		
		if (SelOpt==2)
		{
			LCD_FullDisp(	"Edit:Auxi.PT100 Cali",
							"L/C:      U/C:      ",
							"Std:                ", //"Std:$$$.$$?  $$$.$$?"
							"          C        C");//"   -$$$.$°C +$$$.$°C"	//also show sign -/+
			LCD_Setcursor(4,19);
			LCD_DataWrite(DegreeSymbol);
			LCD_Setcursor(4,10);
			LCD_DataWrite(DegreeSymbol);
			CalEdit_AuxPT100();
		}
		
		if (SelOpt==3)
		{
			LCD_OneLine(2,"* NOT IMPLEMENTED  *");
			AnykeyPulse();
		}
	} while (SelOpt);	//loop until para key is pressed and selopt is given value 0
	
}

void DoCalibration(){
	int8_t selected=0;
	LCD_CursorOff();
	do
	{
		selected =LCD_MenuHandle(MenuSize(CalibrationMenu),CalibrationMenu);
		
		switch(selected){
			case 1:
			Calibrate_AbsPres();
			break;
			case 2:
			Calibrate_OrifPartDP();
			break;
			case 3:
			Calibrate_OrifGasDP();
			break;
			case 4:
			Calibrate_PitotDP();
			break;
			case 5:
			Calibrate_StackTC();
			break;
			case 6:
			Calibrate_AmbTemp();
			break;
			case 7:
			Calibrate_OrifTemp();
			break;
			case 8:
			Calibrate_AuxTemp();
			break;
			case 9:
			//			Calibrate_PartFlow();
			break;
			case 10:
			//		Calibrate_GasFlow();
			break;
			case 0:
			break;
			default:
			LCD_OneLine(2,"Invalid Cal Option");
		}
	} while (selected!=0);
}


void CalibrationModeChk(){	//chk for key combo and password
	//check if key combo is pressed and proceed with calibration
	int32_t EnteredPass=0;
	if (KeyDetected())
	{ //chk for key combo is any key press is detected
		KeyscanWORelese();
		if ((ParaKey&&StartKey)&&!(RunKey||StopKey||UpdateKey||EnterKey)) //enter cal only when para and update key is pressed
		{	//calibration key combination  detected
			
			LCD_Clrscr();
			LCD_OneLine(1,"  Calibration Mode  ");
			//wait for key combo to be released
			Release(ParaKey||UpdateKey);
			do
			{
				LCD_FullDisp(	"  Calibration Mode  ",
				"   Enter Password   ",
				LCD_BLANK_LINE_MACRO,
				LCD_BLANK_LINE_MACRO);
				
				EnteredPass=GetNumDataFromUserWithESC(0,0,7,3,8);
				if (EnteredPass==-1)	//-1 is just  used to check when esc key is pressed
				{
					return;			//exit cal mode if para/ESC key is pressed
				}
				
				else if (EnteredPass!=Passw)
				{
					LCD_OneLine(4,"   Wrong Password!! ");
					delay_sec(1);
					LCD_OneLine(4,LCD_BLANK_LINE_MACRO);
				}
			}while(EnteredPass!=Passw);
			
			if (EnteredPass==Passw)
			DoCalibration();

		}	//if (keycombo chk) close brac
	}	//if (keydetect) close brac
}	//function close brac


//
//void Cal_Edit2Point(volatile nAZSensors *Sensor){
	//int32_t EnteredData=0;
	//Cal_Disp2Point(Sensor);
	//
	//EnteredData=GetNumDataFromUserWithESC(Sensor->ADCLower,0,5,3,7);
	//if (EnteredData==-1)	//ESC key is pressed
	//goto RETWoSaving;
	//else					//ENTER KEY IS PRESSED
	//Sensor->ADCLower=EnteredData;	//store ADC lower Data in respective sensors variable
	//
	//EnteredData=GetNumDataFromUserWithESC(Sensor->STDLower, 0, 3, 3,18);
	//if (EnteredData==-1)
	//goto RETWoSaving;
	//else
	//Sensor->STDLower=EnteredData;	//Store Std Lower Data
	//
	//EnteredData=GetNumDataFromUserWithESC(Sensor->ADCUpper, 0, 5, 4, 7);
	//if (EnteredData==-1)
	//goto RETWoSaving;
	//else
	//Sensor->ADCUpper=EnteredData;	//store ADCUpper counts
	//
	//EnteredData=GetNumDataFromUserWithESC(Sensor->STDUpper, 0, 3, 4, 18);
	//if (EnteredData==-1)
	//goto RETWoSaving;
	//else
	//Sensor->STDUpper=EnteredData;	//store StdUpper counts
	//
	//
	//RETWoSaving:
	//return;
//}