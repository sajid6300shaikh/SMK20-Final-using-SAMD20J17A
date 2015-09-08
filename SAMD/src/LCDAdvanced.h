
#define MenuSize(a)	sizeof(a)/LCDCOLS

void LCD_DispFourOptionsIncrementingFrom(uint8_t StartLine, const char MenuOptions[][LCDCOLS]);
void LCD_DispMenuArrow(uint8_t Row);
int8_t CheckMenuIndexBounds(int8_t MenuIndex, int8_t TotalOptions);
uint8_t LCD_MenuHandle(uint8_t TotalOptions, const char MenuOptions[][LCDCOLS]);
int8_t CheckVertPtrBounds(int8_t Pointer, int8_t NOL);
uint8_t LCD_VerticalScroll(int8_t NoofLinesToScroll);
void LCD_HorizCurPos(char CursorPos, char row1, char col1, char row2, char col2, char row3, char col3);
uint8_t LCD_HorizontalScroll(uint8_t DefaultPosition, char row1, char col1, char row2, char col2, char row3, char col3);