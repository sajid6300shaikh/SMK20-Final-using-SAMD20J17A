#include "compiler.h"

#ifndef _Globals_Included
#define _Globals_Included




//#define DiSP_ADC_3Var_Data_for_debug	//displays adc hi, mid, lo registers
//#define BypassLCD			//BYPASSES lcdS Chkbusy, so debuggong can be done without LCD

/********************************************************************************************************************************************/
/* Total 8 sensors
	Each will have its 32bit ADC value as received from External ADC.(actual ADC result Resolution is signed 17 bits)
	Apart from that these sensors will also have their Zero val(called Lower) and SPAN value(called Upper)
	
	Based on Calibration, sensors can be divided into 2 categories
	1. Sensors with Zeroing capability
	2. Sensors without Zeroing capability
	
1. Sensors with Zeroing Capability (AZSensors): These sensors can be zeroed anytime, hence they have 1 point calibration.(because at zero count of sensor std Min is 0)
	At Power On, the zero value of sensor is read into Zero.Sensor_name and afterwards all sensors readings are subtracted from this zero value.
	Hence while calibration only SPAN value of these sensors is stored and equation to convert their ADC count to physical paramete is Y=MX;
	where, X is currently read(and subtracted from zero) ADCCount, M is SLOPE, Y is corresponding calculated physical parameter (like for orifice_DP it can be 120.8 mmWC)
	and here Slope, M = SensorName.STDUpper/SensorName.ADCUpper;

2. Sensors without Zeroing capability (nAZSensors): These sensors have 2 point calibration
	These sensors can't be zeroed at every power on.
	While Calibration these sensors are subjected to Min STD value(STDLower) as well as Max STD value(STDUpper) and both points are stored in EEPROM along with corresponding ADC counts(ADClower and ADCUpper)
	Equation to convert these sensors ADC value to physical parameter is Y=M*X+C. (Straight line equation in Slope-Intercept form)
	where, X is currently read ADCCount, M is SLOPE, C is intercept, Y is corresponding calculated physical parameter(like for orifice_DP it can be 120.8 mmWC)
	M=(Y2-Y1)/(X2-X1)=(STDUpper-STDLower/ADCUpper-ADCLower).
	Difference, as compared to AZsensors calculation, here is 'C', y-intercept(it is there due to non zero STDLower)
	C=Y2-M*X2= STDUpper - M*ADCUpper.
	*/
/***********************************************************************************************************************************************/


typedef struct{
	int32_t AdcCount;	//signed 32 bit ADC count
	//signed 16 bit Zero count, though ADC will give 32 bit signed no, however zero value will be around 10-20 so we can truncate it to int16
	//it is called zero because these counts will corresponds to 000 of STD
	int16_t Zero;		
	//ADCUpper count will always be positive, these counts corresponds to upper(SPAN) Value of std
	uint16_t ADCUpper;		//16 bit span count
	//STDUpper is actual SPAN value in physical units
	uint16_t STDUpper;
	//Slope=STDSpan/ADCSpan; will be mulitiplied by ADCCount to get physical parameter
	float Slope;	
	//This member will have actual physical parameter value currently measured
	//STD value for these sensors is in mmWC
	uint16_t STDvalue;	
	}AZSensors;	//auto zero sensors

volatile AZSensors OrifPart_DP, OrifGas_DP, Pitot_DP, Stack_TC;

typedef struct{
	//current read ADC value
	int32_t AdcCount;
	//Minimum values will be stored during calibration when sensors are subjected to certain minimum physical input
	//ADCMin count will be positive (and can be more than 15 bit magnitude) for PT100 and absolute pressure but can be negative for TC
	int32_t ADCLower;
	//STDMin is actual Minimum value in physical units
	uint16_t STDLower;
	//ADCSpan count will always be positive
	uint16_t ADCUpper;		//16 bit span count
	//STDSpan is actual SPAN value in physical units
	uint16_t STDUpper;	
	//Slope=STDSpan/ADCSpan; will be mulitiplied by ADCCount to get physical parameter
	float Slope;
	//this member will have actual physical parameter value currently measured
	float Intercept;
	//STD value for TC is in mV, for PT100 it is in Ohms, and for Absolute pres it is in mmHg.
	//TC and PT100 std value will be further converted to the temp value using their respective polynomials
	uint16_t STDvalue; 
}nAZSensors; //non Auto zero sensors

volatile nAZSensors Absolute_Pres, Orifice_PT100, Ambient_PT100, Aux_Temp;

volatile bool	  ExtADCSign;
volatile uint16_t  ExtADC_16bitMag;

volatile uint8_t  ExtADCHi, ExtADCMid, ExtADCLo;
volatile int32_t  ExtADC24bitData;
volatile bool	  Pt100TempSign;	//global var used when PT100Temp data is calculated


void Initialize_Hardware(void);
void Set_LcdContrast(uint8_t ContrastValue);
void Disp_IntroLcdMsgs(void);
void Config_Contrast_PWM(void);
void DoAutoZero(void);
void Config_LCD(void);
uint8_t GetData(uint32_t a);

#endif