#include "compiler.h"
#include "GlobalVarsnPrototypes.h"
#include "Computations.h"
#include <math.h>





uint32_t Zero_Correction(int32_t AdcCount, int32_t	ZeroCount){
	return(AdcCount-ZeroCount);
}

/************************************************************************/
/* After Zero and Span Values are read from EEPROM Slope value of all sensors will be calculated once*/
/************************************************************************/
void ComputeEquation_ofall_Sensors(void){
	//AZ sensors requiring Slope from 1 point equation
	OrifPart_DP.Slope	= ComputeSlopefrom1point(&OrifPart_DP);
	OrifGas_DP.Slope	= ComputeSlopefrom1point(&OrifGas_DP);
	Pitot_DP.Slope		= ComputeSlopefrom1point(&Pitot_DP);
	Stack_TC.Slope		= ComputeSlopefrom1point(&Stack_TC);
	
	//nAZ sensors requiring slope form 2 point equation
	Absolute_Pres.Slope	= ComputeSlopefrom2Point(&Absolute_Pres);	
	Orifice_PT100.Slope	= ComputeSlopefrom2Point(&Orifice_PT100);
	Ambient_PT100.Slope	= ComputeSlopefrom2Point(&Ambient_PT100);
	Aux_Temp.Slope		= ComputeSlopefrom2Point(&Aux_Temp);
	
	//nAZ Sensors requiring Intercept form Slope-point equation
	Absolute_Pres.Intercept	= ComputeIntercept(&Absolute_Pres);
	Orifice_PT100.Intercept	= ComputeIntercept(&Orifice_PT100);
	Ambient_PT100.Intercept	= ComputeIntercept(&Ambient_PT100);
	Aux_Temp.Intercept		= ComputeIntercept(&Aux_Temp);
	
}

/********************************************************************************************************************/
/* Calculates Sensor.Slope value for AZSensors, since they have 1 point calibration, the eqn is given by
Slope=STDupper/ADCUpper;
usage:
	Orifice_DP.Slope	=ComputeSlopefrom1point(&Orifice_DP);*/
/********************************************************************************************************************/
float ComputeSlopefrom1point(volatile AZSensors *Sensor){
	return((float)Sensor->STDUpper/Sensor->ADCUpper);
}

/*********************************************************************************************************/
/* Calculates nAZ Sensor.slope value from two points since they have 2 point calibration, their eqn is,
Slope= (STDUpper-STDLower) / (ADCUpper-ADCLower);*/
/*********************************************************************************************************/
float ComputeSlopefrom2Point(volatile nAZSensors *Sensor){
	return((float) (Sensor->STDUpper - Sensor->STDLower)/(Sensor->ADCUpper-Sensor->ADCLower));
}

/************************************************************************/
/* Calculates Intercept for nAZSensors
Intercept=STDUpper - Slope*ADCUpper
							****WARNING***
This Function requires Slope value, so ComputeSlopefrom2Point should be called before this function
i.e. Slope value for nAZ Sensors should be calculated first.*/
/************************************************************************/
float ComputeIntercept(volatile nAZSensors *Sensor){
	return((float) Sensor->STDUpper - (Sensor->Slope * Sensor->ADCUpper));
}



uint16_t Compute_StackTCTemp(uint16_t mV)
{
	 float T;
	 float x=mV*10; //convert to microvolt
	if (x>=20640){
		const float 	a0=-1.318058e2;
		const float 	a1=4.830222e-2;
		const float 	a2=-1.646031e-6;
		const float 	a3=5.464731e-11;
		const float 	a4=-9.650715e-16;
		const float 	a5=8.802193e-21;
		const float 	a6=-3.110810e-26;
		
		T= (a0)+(a1*x) + (a2 *x*x) +(a3 *x*x*x)+(a4 *x*x*x*x)+(a5 *x*x*x*x*x)+(a6 *x*x*x*x*x*x)+0.5;		 		 
	}
	
	else{
		const float  	a0=0.0;			   //these are polynomial coefficients
		const float 	a1=2.508355e-2;
		const float 	a2=7.860106e-8;
		const float 	a3=-2.503131e-10;
		const float 	a4=8.315270e-14;
		const float 	a5=-1.228034e-17;
		const float 	a6=9.804036e-22;
		const float 	a7=-4.413030e-26;
		const float 	a8=1.057734e-30;
		const float 	a9=-1.052755e-35;
		
		//Inverse polynomial equation as documented																															//Add 0.5 to round off
		T= (a0)+(a1*x) + (a2 *x*x) +(a3 *x*x*x)+(a4 *x*x*x*x)+(a5 *x*x*x*x*x)+(a6 *x*x*x*x*x*x)+(a7 *x*x*x*x*x*x*x)+(a8 *x*x*x*x*x*x*x*x)+ (a9 *x*x*x*x*x*x*x*x*x)+0.5;
		
	}		
		return(T);
}

uint16_t Compute_PT100Temp(uint16_t Rt){	//Rt value will be from 10000 (100.00 ohm) to 13850 (138.50 Ohm)
	
	volatile float T;
	const float a=5.775E-5;
	const float b=0.39083;
	float c= (float)Rt*0.01-100.00;
	float sq;
	sq=(float)sqrt(b*b-4*a*c);
	T=(float)(b-sq)/(2*a) +0.05;		//add 0.005 to round off
	Pt100TempSign=0;
	if (T<0){
		Pt100TempSign=1;
		T=-T;
	}

	T=T*10;
	return T;
	//  LCD_DispVariable(T,2,5,1,9);
	
}


//
//unsigned int KTemp500(float x)
//{
//
//float T;
//const float  	a0=0.0;			   //these are polynomial coefficients
//const float 	a1=2.508355e-2;
//const float 	a2=7.860106e-8;
//const float 	a3=-2.503131e-10;
//const float 	a4=8.315270e-14;
//const float 	a5=-1.228034e-17;
//const float 	a6=9.804036e-22;
//const float 	a7=-4.413030e-26;
//const float 	a8=1.057734e-30;
//const float 	a9=-1.052755e-35;
//
////Inverse polynomial equation as documented
//T= (a0)+(a1*x) + (a2 *(x*x)) +(a3 *(x*x*x))+(a4 *(x*x*x*x))+(a5 *(x*x*x*x*x))+(a6 *(x*x*x*x*x*x))+(a7 *(x*x*x*x*x*x*x))+(a8 *(x*x*x*x*x*x*x*x))+ (a9 *(x*x*x*x*x*x*x*x*x));
//T=T*100+0.5;
//return T;
//}


//unsigned int KTemp1200(float x)
//{
////const float a0,a1,a2,a3,a4,a5,a6,a7,a8;
//
//float T;
//const float 	a0=-1.318058e2;
//const float 	a1=4.830222e-2;
//const float 	a2=-1.646031e-6;
//const float 	a3=5.464731e-11;
//const float 	a4=-9.650715e-16;
//const float 	a5=8.802193e-21;
//const float 	a6=-3.110810e-26;
//
//T= (a0)+(a1*x) + (a2 *(x*x)) +(a3 *(x*x*x))+(a4 *(x*x*x*x))+(a5 *(x*x*x*x*x))+(a6 *(x*x*x*x*x*x));
//T=T*100+0.5;
//return T;
//
//}