#include "CharLCD.h"
#include "LCDAdvanced.h"
#include "MatrixKeypad.h"

/************************************************************************/
/* Has all high level function of LCD to Handle Menu based UI			*/
/************************************************************************/

void LCD_DispFourOptionsIncrementingFrom(uint8_t StartLine, const char MenuOptions[][LCDCOLS]){
	for (char i=0; i<=3; i++)
	{
		LCD_Setcursor(i+1,1);
		LCD_Print(MenuOptions[StartLine+i]);
	}
	
}

void LCD_DispMenuArrow(uint8_t Row){
	//arrow symbol pointing menu option is shown at extreme right on LCD
	//Clear other Rows
	LCD_Setcursor(1,20);
	LCD_DataWrite(' ');
	LCD_Setcursor(2,20);
	LCD_DataWrite(' ');
	LCD_Setcursor(3,20);
	LCD_DataWrite(' ');
	LCD_Setcursor(4,20);
	LCD_DataWrite(' ');
	
	LCD_Setcursor(Row,20);
	LCD_DataWrite(ARROW);
}

int8_t CheckMenuIndexBounds(int8_t MenuIndex, int8_t TotalOptions){
	if (MenuIndex<1)
	MenuIndex=TotalOptions;
	else if (MenuIndex>TotalOptions)
	MenuIndex=1;
	return MenuIndex;
}

uint8_t LCD_MenuHandle(uint8_t TotalOptions, const char MenuOptions[][LCDCOLS])
{
	//TotalOptions is nothing but no. of items in menu
	//Keeps actual option no. from 1 to TotalOptions
	int8_t MenuIndex=1;		//keep it signed to facilitate bounds checking
	//keeps row no where arrow symbol is displayed (1-4, since 4 line LCD).
	int8_t ArrowLoc=1;		//kept it signed to facilitate bounds checking
	//In LCD_DispOptionsIncrementingFrom(optionNo, OptionStringArray) DispOptionno is used to keep track of displayed MSG group
	int8_t DispOptionNo=0;
	LCD_DispFourOptionsIncrementingFrom(0, MenuOptions);	//displays Strings from 0 to 3 of the given string 2D array
	//displays arrow symbol according to location on LCD at 20th Column and Row given by ArrowLoc
	LCD_DispMenuArrow(ArrowLoc);
	ReleaseKey();
	while(1){	//loop forever, this function will be exited by press EnterKey or ParaKey
		if(KeyDetected())
		{//comes here when key is pressed
			Keyscan();		//check which key is pressed
			if(IncrKey)		//increment key pressed
			{
				MenuIndex--;
				ArrowLoc--;
				MenuIndex=CheckMenuIndexBounds(MenuIndex, TotalOptions);	//Adjust MenuIndex if it is below zero or above Totaloptions
				if (ArrowLoc<1)
				{
					if (DispOptionNo==0)
					{
						DispOptionNo=TotalOptions-4;
						ArrowLoc=4;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);						
					}
					else
					{
						DispOptionNo--;
						ArrowLoc=1;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);						
					}
					
				}
			}

			else if(DecrKey)
			{
				MenuIndex++;
				ArrowLoc++;
				MenuIndex=CheckMenuIndexBounds(MenuIndex, TotalOptions);
				if (ArrowLoc>4)
				{
					if (DispOptionNo==(TotalOptions-4))
					{
						DispOptionNo=0;
						ArrowLoc=1;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);
						
					}
					else
					{
						DispOptionNo++;
						ArrowLoc=4;
						LCD_DispFourOptionsIncrementingFrom(DispOptionNo, MenuOptions);
						
					}
					
				}
			}
			
			else if(EnterKey)
			{
				return MenuIndex;
			}
			else if (ParaKey)
			{
				return 0;
			}
			LCD_DispMenuArrow(ArrowLoc);
		}
	}
}




int8_t CheckVertPtrBounds(int8_t Pointer, int8_t NOL){
	if (Pointer<4-NOL+1)
	Pointer=4;
	else if (Pointer>4)
	Pointer=4-NOL+1;
	return Pointer;
}

uint8_t LCD_VerticalScroll(int8_t NoofLinesToScroll){
	int8_t NOL=NoofLinesToScroll;
	//Initialize Index
	int8_t Pointer= 4-NOL+1;
	LCD_Setcursor(Pointer,20);
	LCD_DataWrite(ARROW);
	ReleaseKey();
	while(1){	//loop forever, this function will be exited by press EnterKey or ParaKey
		if(KeyDetected())
		{//comes here when key is pressed
			Keyscan();		//check which key is pressed
			if(IncrKey)		//increment key pressed
			{
				LCD_Setcursor(Pointer,20);
				LCD_DataWrite(' '); //clr previous space
				Pointer--;
				Pointer=CheckVertPtrBounds(Pointer, NOL);	//Adjust MenuIndex if it is below zero or above Totaloptions
				LCD_Setcursor(Pointer,20);
				LCD_DataWrite(ARROW);
			}

			else if(DecrKey)
			{
				LCD_Setcursor(Pointer,20);
				LCD_DataWrite(' '); //clr previous space
				Pointer++;
				Pointer=CheckVertPtrBounds(Pointer, NOL);	//Adjust MenuIndex if it is below zero or above Totaloptions
				LCD_Setcursor(Pointer,20);
				LCD_DataWrite(ARROW);
			}
			
			else if(EnterKey)
			{
				return (Pointer-(4-NOL));
			}
			else if (ParaKey)
			{
				return 0;
			}

		}
	}
}



void LCD_HorizCurPos(char CursorPos, char row1, char col1, char row2, char col2, char row3, char col3){
	
	switch(CursorPos){
		case 1:
		LCD_Setcursor(row1,col1);
		break;
		
		case 2:
		LCD_Setcursor(row2,col2);
		break;
		
		case 3:
		LCD_Setcursor(row3,col3);
		break;
		
		default: //any other value will show error on LCD
		LCD_OneLine(2, "Bug: LCD_HorizCur");
	}
}

/*******************************************************************/
/*	This function takes 2 or 3 options LCD positions and Scrolls 
	between them and returns selected value (1 or 2 or 3)
	row1, col1 corresponds to option1 and so on.
	If only 2 options are there then keep row3 and col3 zero*/
/*******************************************************************/
uint8_t LCD_HorizontalScroll(uint8_t DefaultPosition, char row1, char col1, char row2, char col2, char row3, char col3){
	LCD_CursorOn();	
	int8_t NoOfOpt=3;	//by default 3 option are to be scrolled
	//If row3 is 0 means only 2 options are to be scrolled
	if (row3==0)			
	NoOfOpt=2;	// modify no of options
	
	int8_t CurPoint=DefaultPosition;	
	//Put cursor on default option position
	LCD_HorizCurPos(DefaultPosition, row1, col1, row2, col2, row3, col3);
	ReleaseKey();
	while(1){	//loop forever, this function will be exited by press EnterKey or ParaKey
		if(KeyDetected())
		{//comes here when key is pressed
			Keyscan();		//check which key is pressed
			if(NextKey)		//increment key pressed
			{
				CurPoint++;
				if (CurPoint>NoOfOpt)
					CurPoint=1; //if overflows			
			}

			else if(PrevKey)
			{
				CurPoint--;
				if (CurPoint<1)
					CurPoint=NoOfOpt; //if underflows
			}
			
			else if(EnterKey)				//exit point
			{
				LCD_CursorOff();
				return (CurPoint);
			}
			else if (ParaKey)				//exit point
			{
				LCD_CursorOff();
				return 0;
			}
			
			LCD_HorizCurPos(CurPoint, row1, col1, row2, col2, row3, col3);

		}
	}

}


