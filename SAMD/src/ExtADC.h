#ifndef __ExtADC_H__
#define __ExtADC_H__



/************************************************************************/
/* These are various ADC channels */
/************************************************************************/
typedef volatile enum {// start numbering with 1
	Ch_Orifice_Gas_DP=1,	//1
	Ch_Pitot_DP,			//2
	Ch_Absolute_Pres,		//3
	Ch_Orifice_DP,			//4
	Ch_Orifice_PT100,		//5
	Ch_Ambient_PT100,		//6
	Ch_Aux_PT100,			//7
	Ch_Aux_TC,				//8
	Ch_Stack_TC				//9
	
}EnumExtAdcChannel;

	
void Spi_Get3BytesfromExtADC(EnumExtAdcChannel Channel);
int32_t ExtAnalogRead(EnumExtAdcChannel Channel);

#endif