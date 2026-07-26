#include "Exter_CommTaskData.h"
#include "Dsp_M3_ComTaskData.h"

//para of vac
#define	cVacLowestVolt			1100
#define	cVacDefaultLowVolt		1800
#define	cVacHighestVolt			3100
#define	cVacDefaultHigh			2530
#define	cVacNominal				2300

//para of fac
#define	cVacLowestFreq			4500
#define	cVacDefaultLowFreq		4750
#define	cVacHighestFreq			5500
#define	cVacDefaultHighFreq		5150
#define	cVacNominalFreq			5000

INT8U sChkLowVolt(INT16U Value)
{
	if(Value>=cVacLowestVolt && Value<cVacNominal)	
	{
		return cTrue;
	}
	else
	{
		return cFalse;
	}
		
}

INT8U sChkHighVolt(INT16U Value)
{
	if(Value<=cVacHighestVolt && Value>cVacNominal)	
	{
		return cTrue;
	}	
	else
	{
		return cFalse;
	}	
}

INT8U sChkLowFreq(INT16U Value)
{
	if(Value>=cVacLowestFreq && Value<cVacNominalFreq)	
	{
		return cTrue;
	}	
	else
	{
		return cFalse;
	}
}

INT8U sChkHighFreq(INT16U Value)
{
	if(Value<=cVacHighestFreq && Value>cVacNominalFreq)	
	{
		return cTrue;
	}
	else
	{
		return cFalse;
	}		
}

INT8U sCalDays(INT8U bMonthValue,INT8U bYearValue)
{
	INT8U bDaysValue;
	
	if(bMonthValue==4 || bMonthValue==6 || bMonthValue==9 || bMonthValue==11) 
	{
		bDaysValue = 30;
	}
	else if(bMonthValue==2)
	{
		if((((INT16U)bYearValue+2000)%400==0)||(((INT16U)bYearValue+2000)%4==0 && ((INT16U)bYearValue+2000)%100!=0))	
			bDaysValue=29;
		else
			bDaysValue=28;
	}
	else
	{
		bDaysValue=31;					
	}
	return(bDaysValue);
}


INT8U sCHK60HzSpecValue(INT8U spec,INT8U VorF,INT16U Value)
{	
	if(VorF==0)	//V
	{	
		if(Value>=1600 && Value<=2800)	
			return cTrue;
		else
			return cFalse;
	}
	else // F
	{

		if(Value>=5600 && Value<=6400)
			return cTrue;
		else
			return cFalse;			
	}
}

INT8U sCHKModelTypeHigh(INT16U bHighModel)
{
	if((((bHighModel&0xF0)>>4)<cFactoryMax)&&((bHighModel&0x0F)<=0x01))
		return cTrue;
	else
		return cFalse;
}


void sCheckmodel(INT16U wData)
{
	EEData1.wTxUxMxSxEE =(wData &0xFFF0);
	//EEData1.wTxUxMxSxEE |=(INT16U)(cModelBit<<4);	
	EEData1.wTxUxMxSxEE |=(INT16U)(cSpecBit);	
}


INT8U sDataCheck(INT16U wData)
{
	if((wData<AdjValueMin)||(wData>AdjValueMax))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}