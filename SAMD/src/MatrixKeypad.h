#ifndef _Matrix_Keypad_h
#define _Matrix_Keypad_h



//Bit variables
volatile bool	ParaKey;
volatile bool	RunKey;
volatile bool	StartKey;
volatile bool	StopKey;
volatile bool	UpdateKey;
volatile bool	EnterKey;

//Alternate functions of above keys
#define EscKey		ParaKey
#define PrevKey		RunKey
#define IncrKey		StartKey
#define DecrKey		StopKey
#define NextKey		UpdateKey
#define StoreKey	EnterKey


//----------------------------------------------------------------------------
#define debounce_ms 30		//milliseconds to wait between two scan to confirm key press and debounce keys.
#define NoKeyDetected !KeyDetected

//Below is a working code
/************************************************************************/
/* Multi line macro. 
This Function will loop until said key is pressed
Usage: WaitFor(EnterKey); 
macro expansion made is

ClearKeys();
while(EnterKey==0)
 {
	Keyscan();
 }*/

/************************************************************************/
//usage WaitFor(EnterKey);
#define	WaitFor(ikey) \
ClearKeys();\
while(ikey==0){\
	Keyscan();\
}

/************************************************************************/
/* Multi Line MACRO.
This Function will loop untill said key is released
Usage:
Release(EnterKey);*/
/************************************************************************/
#define Release(ikey)\
do \
{Keyscan();\
} while (ikey==1);\
	
void ClearKeys(void);
bool KeyDetected(void);
void Config_MatrixKeypad(void);
void Keyscan(void);
void KeyscanWORelese(void);
void ReleaseKey(void);
void LoopUntilEnterPressed(void);
void WaitforAnyKey(void);
void AnykeyPulse(void);

#endif