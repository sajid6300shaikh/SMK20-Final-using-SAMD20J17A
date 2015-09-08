/*******************************************************************************************************************************/
/* This file contains Routine to Get Numeric Data from user and also display variables with proper Decimal Point on LCD*/
/*******************************************************************************************************************************/

#include "compiler.h"
#include "CharLCD.h"
#include "MatrixKeypad.h"
#include "UserInterface.h"


/*********************************************************************************************************************/
/* Parameter:
Number= any 8,16,32 bit data variable or constant which is to be displayed on LCD                                                                      
ROW,COl= Most significant Digit of the Data will be shown at this Position on LCD and rest digits will follow
DecimalPos= Decimal point position Eg: for 32.768 DecimalPos will be 3, for 654.2 decimalPos will be 1
NoofDigitsToDisplay= As Name suggests. 

Usage: LCD_DispVariable(12345, 2, 4, 1, 10);
23.45 will be displayed on LCD at 1st Row and 10th Column.
MSd will be neglected since only 4 digit are asked to be displayed */
/*********************************************************************************************************************/
void LCD_DispVariable(uint32_t Number, char DecimalPos, uint8_t NoofDigitsToDisplay, char Row, char Col){
	int8_t Digit[NoofDigitsToDisplay];	//temporary array to store BCD
	SplitNumbertoDigits(Number,  Digit, NoofDigitsToDisplay);
	
	LCD_Setcursor(Row, Col);	
	if (NoofDigitsToDisplay>=7)
	LCD_DispAscii(Digit[6]);				//LSB
	if (DecimalPos==6)
	LCD_DataWrite('.');
	
	
	if (NoofDigitsToDisplay>=6)
	LCD_DispAscii(Digit[5]);
	if (DecimalPos==5)
	LCD_DataWrite('.');
	
	if (NoofDigitsToDisplay>=5)
	LCD_DispAscii(Digit[4]);
	if (DecimalPos==4)
	LCD_DataWrite('.');
	
	if (NoofDigitsToDisplay>=4)
	LCD_DispAscii(Digit[3]);
	if (DecimalPos==3)
	LCD_DataWrite('.');
	
	if (NoofDigitsToDisplay>=3)
	LCD_DispAscii(Digit[2]);
	if (DecimalPos==2)
	LCD_DataWrite('.');
	
	if (NoofDigitsToDisplay>=2)
	LCD_DispAscii(Digit[1]);
	if (DecimalPos==1)
	LCD_DataWrite('.');
	
	if (NoofDigitsToDisplay>=1)
	LCD_DispAscii(Digit[0]);

}



/************************************************************************/
/* When a Digit is incremented or decremented by User, it should not go above 9 or below 0
This function takes care of proper bounds off.*/
/************************************************************************/
int8_t CheckDigitBound(int8_t Num){
	if (Num<0)
	Num=9;			//roll to 10
	if (Num>9)
	Num=0;			//roll to 0
	
	return Num;
}

/************************************************************************/
/* Splits a given no into individual digits.
the Splitted BCD digits are stored in DigitArray that is passed to the function
Usage:
int8_t BCDarray;	//Initialize and array to Store the BCD digits
SplitNumbertoDigits(Variable, BCDarray, 5);*/
/************************************************************************/
void SplitNumbertoDigits(uint32_t Num, int8_t DigitArray[], int8_t NoofDigits){
	
	int8_t i=0, val=0;
	uint32_t Temp=Num;
	
	while (i<NoofDigits)
	{
		val=Temp%10;
		DigitArray[i]= val;
		Temp=Temp/10;
		i++;
	}
	
}


/************************************************************************/
/* This Fucntion combines BCD digits to form a no.
Usage:
Array Index=  0 1 2 3 4 
Say Digit[5]={2,4,5,6,8}; is the array of 5 element containing 5 BCD digit
uin32_t ans = CombineDigitstoNumber(Digit, 5);
ans will contain no 86542*/
/************************************************************************/
uint32_t CombineDigitstoNumber(int8_t Digit[], int8_t NoofDigits){
	
	uint32_t BCD1=0,BCD2=0,BCD3=0,BCD4=0,BCD5=0,BCD6=0, BCD7=0;
	
		
	if (NoofDigits>0)
		BCD1 = Digit[0];
	
	if (NoofDigits>1)
		BCD2 = Digit[1]*10;
		
	if (NoofDigits>2)
		BCD3 = Digit[2]*100;
		
	if (NoofDigits>3)
		BCD4 = Digit[3]*1000;
		
	if (NoofDigits>4)
		BCD5 = Digit[4]*10000;
	
	if (NoofDigits>5)
		BCD6 = Digit[5]*100000;
		
	if (NoofDigits>6)
		BCD7 = Digit[6]*1000000;
		

	return (BCD7+BCD6+BCD5+BCD4+BCD3+BCD2+BCD1);
}


uint32_t CombineDigitstoNumber1(int8_t Digit[], int8_t NoofDigits){
	uint32_t num=0;
	while (NoofDigits>0)
	{
		num=num*10 + Digit[NoofDigits-1];
		NoofDigits--;
	}
	return num;
}


/************************************************************************/
/* Gets Numeric data Input from user with the help of Incr, Decr, next and prev Key
Usage:
uint32_t ans=GetNumDataFromUser(32768,4,5,2,10);      
Current value of variable will be displayed on LCD with given decimal point 
and user can edit the value and press enter to store new value of variable
5 digit is mostly used and this function can handle maximum of 7 Digits */
/************************************************************************/
uint32_t GetNumDataFromUser(uint32_t CurrentNum, uint8_t DecimalPos, uint8_t NoofDigit,  char Row, char Col){
	int8_t Digit[NoofDigit];
	//First Display Current Num on LCD
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
			return CurrentNum;
			
			LCD_Setcursor(Row,NewCol);
			LCD_DispAscii(Digit[i]);
			LCD_Setcursor(Row,NewCol);
		}
	}
}


/***********************************************************************************/
/* Gets Numeric data Input from user with the help of Incr, Decr, Next and Prev Key
When ESC key is pressed it returns -1 instead of current no.
Made this function to serve the purpose of CALIBRATION PASSWORD CHK.	*/
/***********************************************************************************/
int32_t GetNumDataFromUserWithESC(int32_t CurrentNum, uint8_t DecimalPos, uint8_t NoofDigit,  char Row, char Col){
	int8_t Digit[NoofDigit];
	//First Display Current Num on LCD
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
			LCD_Setcursor(Row,NewCol);
		}
	}
}