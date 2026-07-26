#ifndef EXTER_COMMTASKDATA_H_
#define EXTER_COMMTASKDATA_H_

#include "GlobalDef.h"

#define INT8UP        INT8U *
#define INT8SP        INT8S *																		
#define INT16UP       INT16U *
#define INT32UP		  INT32U *

#define cFlash_STM32_Start	1
#define cFlash_STM32_Stop	0
#define cFlash_DSP_Start 1
#define cFlash_DSP_Stop 0

#define	cUnderExcitedPF075		2500
#define	cOverExcitedPF075		17500

#define 	cPFAdjMin 	3100	//4096*0.8
#define 	cPFAdjMax 	5100	//4096*1.2

#define cPasswordUnlock			0x07
#define cSuperPasswordUnlock		0x70

#define cSuperPassword3Bit			0x40
#define cSuperPassword2Bit			0x20
#define cSuperPassword1Bit			0x10

#define cPassword3Bit			0x04
#define cPassword2Bit			0x02
#define cPassword1Bit			0x01

#define	    cFactoryMax		2
#define 	cFirmwareVersionIndex	1

#define 	AdjValueMin 		810  //allow Value Adjust Range 0.9*1024 (717~ 1330)
#define 	AdjValueMax 	1228  //1024*1.1

#define	c80PerLoadCommand		1
#define	c60PerLoadCommand		2
#define	c30PerLoadCommand		3
#define	c100PerLoadCommand		4

INT8U sChkLowVolt(INT16U Value);
INT8U sChkHighVolt(INT16U Value);

INT8U sChkLowFreq(INT16U Value);
INT8U sChkHighFreq(INT16U Value);

INT8U sCalDays(INT8U bMonthValue,INT8U bYearValue);
INT8U sCHK60HzSpecValue(INT8U spec,INT8U VorF,INT16U Value);

INT8U sCHKModelTypeHigh(INT16U bHighModel);
void sCheckmodel(INT16U wData);
INT8U sDataCheck(INT16U wData);
#endif