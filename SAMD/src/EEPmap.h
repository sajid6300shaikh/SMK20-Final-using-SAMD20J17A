
#ifndef __EEP_Mem_Map__
#define __EEP_Mem_Map__

//#define EEPadd(x) offsetof(EEPROMLayout, x)

struct __attribute__ ((packed)) EEMISC {
	 uint8_t    c;
	 int16_t    i;
	 uint32_t   l;
	 uint32_t	d;
 };
//  [8/11/2015]
 struct __attribute__ ((packed)) EECAL {
	 
	 //Auto Zero Sensors Parameters
	 int16_t	EE_OrifPart_Z;
	 uint16_t	EE_OrifPart_ADCUP;
	 uint16_t	EE_OrifPart_STDUP;
	 
	 int16_t	EE_OrifGas_Z;
	 uint16_t	EE_OrifGas_ADCUP;
	 uint16_t	EE_OrifGas_STDUP;
	 
	 int16_t	EE_Pitot_Z;
	 uint16_t	EE_Pitot_ADCUP;
	 uint16_t	EE_Pitot_STDUP;
	 
	  int16_t	EE_StackTC_Z;
	  uint16_t	EE_StackTC_ADCUP;
	  uint16_t	EE_StackTC_STDUP;
	 
	 //Non AutoZero Sensors Parameters
	 int32_t	EE_Abs_ADCLOW;
	 uint16_t	EE_Abs_STDLOW;
	 uint16_t	EE_Abs_ADCUP;
	 uint16_t	EE_Abs_STDUP;	  
	 
	 int32_t	EE_OrifPT100_ADCLOW;
	 uint16_t	EE_OrifPT100_STDLOW;
	 uint16_t	EE_OrifPT100_ADCUP;
	 uint16_t	EE_OrifPT100_STDUP;
	 
	 int32_t	EE_AmbPT100_ADCLOW;
	 uint16_t	EE_AmbPT100_STDLOW;
	 uint16_t	EE_AmbPT100_ADCUP;
	 uint16_t	EE_AmbPT100_STDUP;
	 //AUX Temp Measurement can be through PT100 or Thermocouple
	 //so this is the min 
	 int32_t	EE_AuxTemp_ADCLOW;
	 uint16_t	EE_AuxTemp_STDLOW;
	 uint16_t	EE_AuxTemp_ADCUP;
	 uint16_t	EE_AuxTemp_STDUP;
	 
	 	
 };

typedef struct __attribute__ ((packed)) EE_MAP {
	struct EEMISC	EEmis;
	struct EECAL	EEcal;
} EE_MAP;


#define EE_CALAddr(mbr)		(offsetof(EE_MAP, EEcal)+offsetof(struct EECAL,mbr))
#define EE_MISCAddr(mbr)	(offsetof(EE_MAP, EEmis)+offsetof(struct EEMISC,mbr))


#endif