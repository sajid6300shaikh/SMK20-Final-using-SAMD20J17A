#include "compiler.h"
#include "S_PORT.h"
#include "Delay.h"
#include "MatrixKeypad.h"
#include "HardwareDefs.h"



/**********************************************************************************************************/
 /*	For key scan,
2 COL configured as Input with Pull up resistor, and 3 rows as Totem-pole OUTPUT
	1. We will first roughly chk for any key press by making all Rows LOW and Checking Both COL for LOW
	2. If any key detected we will wait for debounce period      
	3. And then clear each ROW and check each col to determine which key is pressed   */
/**********************************************************************************************************/
void Config_MatrixKeypad(){	//call this once in your program
	pinMode(COL1, INPUP);		//Make Columns Input with pull up.
	pinMode(COL2, INPUP);		//INPUP stands for input with pull up.
	//Make ROWs as Output
	pinMode(ROW1, OUTPUT);		
	pinMode(ROW2, OUTPUT);		
	pinMode(ROW3, OUTPUT);
}

void ClearKeys(){
	ParaKey	=0;
	RunKey	=0;
	StartKey=0;
	StopKey	=0;
	UpdateKey=0;
	EnterKey =0;
}

bool KeyDetected(){
	PinClr(ROW1);
	PinClr(ROW2);
	PinClr(ROW3);
	if ((digitalRead(COL1)==0) || (digitalRead(COL2)==0))
	{
		return(true);
	}
	else
		return(false);
}


void Keyscan(){			//call this whenever key status required

	if (KeyDetected())	//if both columns are high, then no key is pressed
	{	//program will enter when any key is pressed
		delay_ms(debounce_ms);	//debounce delay
		if (KeyDetected())	//check again
		{						 
			//ROW1
			PinClr(ROW1);		//activate ROW 1 and check both columns
			PinSet(ROW2);
			PinSet(ROW3);
			 StartKey	= !(digitalRead(COL1));
			 RunKey		= !(digitalRead(COL2));
			
			//ROW2
			PinSet(ROW1);
			PinClr(ROW2);		//activate ROW 2 and check both columns
			PinSet(ROW3);
			 EnterKey	= !(digitalRead(COL1));
			 StopKey	= !(digitalRead(COL2));
			
			//ROW3
			PinSet(ROW1);		//activate ROW 3 and check both columns
			PinSet(ROW2);
			PinClr(ROW3);
			 ParaKey= !(digitalRead(COL1));
			 UpdateKey = !(digitalRead(COL2));
			while (KeyDetected())
			{
				delay_ms(1);	//w8 until all keys are released
			}
		}
		else
			ClearKeys();	
	}
	else
		ClearKeys();
}



void KeyscanWORelese(){			//call this whenever key status required

if (KeyDetected())	//if both columns are high, then no key is pressed
{	//program will enter when any key is pressed
	delay_ms(debounce_ms);	//debounce delay
	if (KeyDetected())	//check again
	{
		//ROW1
		PinClr(ROW1);		//activate ROW 1 and check both columns
		PinSet(ROW2);
		PinSet(ROW3);
		StartKey	= !(digitalRead(COL1));
		RunKey		= !(digitalRead(COL2));
		
		//ROW2
		PinSet(ROW1);
		PinClr(ROW2);		//activate ROW 2 and check both columns
		PinSet(ROW3);
		EnterKey	= !(digitalRead(COL1));
		StopKey	= !(digitalRead(COL2));
		
		//ROW3
		PinSet(ROW1);		//activate ROW 3 and check both columns
		PinSet(ROW2);
		PinClr(ROW3);
		ParaKey= !(digitalRead(COL1));
		UpdateKey = !(digitalRead(COL2));
	}
	else
	ClearKeys();
}
else
ClearKeys();
}



void ReleaseKey(void){
	delay_ms(debounce_ms);
	while(KeyDetected());
	delay_ms(debounce_ms);
}

void WaitforAnyKey(void){
	delay_ms(debounce_ms);
	while(!KeyDetected());
	delay_ms(debounce_ms);
}
	
/************************************************************************/
/* This function waits for for user to press key and then waits for the key to release*/

/************************************************************************/
void AnykeyPulse(void){
	WaitforAnyKey();
	ReleaseKey();
}

void LoopUntilEnterPressed(void){
	ClearKeys();
	while (!EnterKey)
	{	
		Keyscan();
	}	
}



