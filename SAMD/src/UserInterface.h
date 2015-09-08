

void SplitNumbertoDigits(uint32_t Num, int8_t DigitArray[], int8_t NoofDigits);
uint32_t CombineDigitstoNumber(int8_t Digit[], int8_t NoofDigits);
void LCD_DispVariable(uint32_t Number, char DecimalPos, uint8_t NoofDigitsToDisplay, char Row, char Col);
int8_t CheckDigitBound(int8_t Num);
uint32_t GetNumDataFromUser(uint32_t CurrentNum, uint8_t DecimalPos, uint8_t NoofDigit,  char Row, char Col);
int32_t GetNumDataFromUserWithESC(int32_t CurrentNum, uint8_t DecimalPos, uint8_t NoofDigit,  char Row, char Col);

uint32_t CombineDigitstoNumber1(int8_t Digit[], int8_t NoofDigits);

void LCD_DispVariable1(uint32_t Number, char DecimalPos, uint8_t NoofDigitsToDisplay, char Row, char Col);

/************************************************************************/
/* OLD Test Functions                                                */
/************************************************************************/
/*
void SplitNumbertoDigits(uint32_t Num, int8_t DigitArray[], int8_t NoofDigits)
{
	eparates no as
	DigitArray[0]=
	//Converts 8,16,32-bit binary no. to BCD to be displayed on screen.	
	if (NoofDigits>0)
	DigitArray[0] = Num      - (Num/10)*10;					//Least Significant Digit
	
	if (NoofDigits>1)
	DigitArray[1] = Num/10   - (Num/100)*10;
	
	if (NoofDigits>2)
	DigitArray[2] = Num/100  - (Num/1000)*10;
	
	if (NoofDigits>3)
	DigitArray[3] = Num/1000 - (Num/10000)*10;

	if (NoofDigits>4)
	DigitArray[4] = Num/10000- (Num/100000)*10;					
	
	if (NoofDigits>5)
	DigitArray[5] = Num/100000 -(Num/1000000)*10;
	
	if (NoofDigits>6)
	DigitArray[6]=	Num/1000000-(Num/10000000)*10;			//most significant digit
	
}



uint32_t Get4DigitData(uint32_t CurrentNum, uint8_t DecimalPos, char Row, char Col){
	int8_t Digit[4];			//
	//First Display Current Num on LCD
	LCD_DispVariable(CurrentNum, Row,Col,DecimalPos,4);	//
	//Separate Digits of num and store it in digit array
	//Digit[3] = CurrentNum/1000 - (CurrentNum/10000)*10;
	//Digit[2] = CurrentNum/100  - (CurrentNum/1000)*10;
	//Digit[1] = CurrentNum/10   - (CurrentNum/100)*10;
	//Digit[0] = CurrentNum      - (CurrentNum/10)*10;
	//------------------------------------
	SplitNumbertoDigits(CurrentNum,4,Digit);
	LCD_CursorOn();
	int8_t i=3;		//
	uint8_t NewCol=Col;	//will be changed on press of next/update key
	LCD_Setcursor(Row,NewCol);
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
			
			if (NextKey || PrevKey)
			{
				if (NextKey)
				i--;
				if (PrevKey)
				i++;
				
				if (i>3)		//
				i=0;
				if (i<0)
				i=3;	//
				NewCol++;
				//		NewCol=Col+3-i;		//
				if (i==0)			//accounting for a decimal point
				NewCol++;
				if (NewCol==3-DecimalPos+Col+1)	//
				NewCol++;
			}
			
			
			if (EnterKey)
			return CombineDigits(Digit,4);
			
			LCD_Setcursor(Row,NewCol);
			LCD_DispAscii(Digit[i]);
			LCD_Setcursor(Row,NewCol);
		}
	}
	
}


uint32_t Get5DigitData(uint32_t CurrentNum, uint8_t DecimalPos, char Row, char Col){
	int8_t Digit[5];
	//First Display Current Num on LCD
	LCD_DispVariable(CurrentNum, Row,Col,DecimalPos,5);
	//Separate Digits of num and store it in digit array
	Separate5DigitNo(Digit, CurrentNum);
	SplitNumbertoDigits(CurrentNum,5,Digit);
	LCD_CursorOn();
	int8_t i=4;
	uint8_t shift=0;
	uint8_t NewCol=Col;
	LCD_Setcursor(Row,NewCol);
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
			
			if (NextKey)
			{
				i--;
				
				if (i>4)
				i=0;
				if (i<0)
				i=4;
				//NewCol=Col+4-i;
				//if (i==0)
				//NewCol++;
				NewCol=Col+4-i;
				if (NewCol>=(Col+5-DecimalPos))
				NewCol++;
			}
			
			if (PrevKey)
			{
				i++;
				if (i>4)
				i=0;
				if (i<0)
				i=4;
				NewCol=Col+4-i;
				if (NewCol>=(Col+5-DecimalPos))
				NewCol++;
			}
			
			if (EnterKey)
			return CombineDigits(Digit,5);
			
			LCD_Setcursor(Row,NewCol);
			LCD_DispAscii(Digit[i]);
			LCD_Setcursor(Row,NewCol);
		}
	}
}
void Separate5DigitNo(int8_t DigitArray[], uint32_t Num)
{

	separates no as
	Eg: for 32768. BCD5=3, BCD4=2...
	//Converts 16-bit binary no. to BCD to be displayed on screen.
	DigitArray[4] = Num/10000;					//most significant digit
	DigitArray[3] = Num/1000 - (Num/10000)*10;
	DigitArray[2] = Num/100  - (Num/1000)*10;
	DigitArray[1] = Num/10   - (Num/100)*10;
	DigitArray[0] = Num      - (Num/10)*10;
}
*/