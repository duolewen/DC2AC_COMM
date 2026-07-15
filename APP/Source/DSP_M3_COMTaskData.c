#include "DSP_M3_COMTaskData.h"

#define cS_SpecNo		16

/********************************************************************************
  Model Macro										                                           
*********************************************************************************/
#define c16KModel		4
#define	c18KModel		5
#define	c20KModel		6


struct EEPROMDefaultTable1 EEData1;
struct ADJValueStruct      ADJValue;
struct EEPROMDefaultTable3 EEData32;

//page 3  Error
INT8U   bErrorRecord[cErrRecordLen];

//page 2  Error && adjust
INT8U   bErrorMsg[cErrMesgLen];
INT8U   bInvFaultBit;

INT8U bCOMMode;
INT8U bZigbeORBlueOWifi;

INT8U bUserType;
INT8U bSpecType;
INT8U bKWmodel;
INT8U bProtectSet;

INT8U ubSelectFrequency5060HzFag;

INT8U bDistanceOnOffOrder=cTurnOn;
INT8U bTurnOffCommand=0;


const INT16U c60HzSpecLimitF[1][2] = 
{
	//Flow,Fhigh	
	{5930,6050},	
};

const INT16U cSpecLimitV[cS_SpecNo][17] = 
{	
	//VL    VH, 	FL,	FH,	DCI VLT,VHT,FLT,FHT 	VL2   VH2  VL2T  VH2T 	VonH VonL fonH fonL
	{1845,2525,4753,5147, 3,	4,	4,   3,	3,	1845, 2525, 	5,  	5,	2700,1600,5400,4600},	// 0 VDE0126
	{1850,2630,4753,5015, 3,	4,	4,   3,	3,	1845, 2525, 	5,  	5,	2700,1600,5400,4600},	// 1 VDE0126
	{1955,2530,4750,5050,10, 	75,  5,  	5,  	5,	1150,2530,	5,	5,	2700,1600,5400,4600},	// 2 Belgium
	{2000,2500,4910,5090, 5,	3,	3,   3,	3,  	2000, 2500,	3, 	3,	2700,1600,5400,4600},	// 3 TUV000
	{2070,2630,4600,5400,60,	3,	3,	3,	3,  	1840, 2750,	3, 	3,	2700,1600,5400,4600},// 4 AS4777
	{1840,2750,4750,5200,10,	10, 	5,	5,	5,   	1610, 2640,	5,	5,	2530,1955,5010,4990},	// 5 CEI
	{1960,2520,4803,5047,50,	72,	72,	144,	24,	1960, 2630,	72,	8,	2700,1600,5400,4600}, // 6 SP1699 == RD1663
	{1845,2640,4753,5097,	3,  	20, 	20,	20, 	20,	1845, 2640, 	20,	20,	2700,1600,5400,4600},	// 7 Xila
	{1845,2640,4753,5147,	3,  	4,   	4,  	3,  	3,   	1870,2240,	3,	3,	2530,1840,5005,4750},	// 8 N4105		
	{2010,2510,4753,5147,	3, 	120,	50,	997,	4497,1845,2630,	23,	23,	2700,1600,5400,4600}, // 9 G59 == should G83
	{2100,2500,4710,5045,	3,	100,100,	100,100,	2100,2500,	100,100,	2700,1600,5400,4600}, // 10 Ireland
	{1870,2420,4803,5047,	5,  	98,	98,	8,	8,	1100, 2970,	3,	1,	2700,1600,5400,4600}, // 11 CGC
	{2070,2520,4753,5197,60,	500,2000,10,	10,	2070,2520,	500,2000,2700,1600,5400,4600},	// 12 EN50438 
	{2010,2510,4753,5147,	3, 	120,	50,	997,	4497,1845,2630,	23,	23,	2700,1600,5400,4600}, // 13 G59 
	{1955,2530,4750,5050,10, 	75,  5,  	5,  	5,	1150,2530,	5,	5,	2700,1600,5400,4600},	// 14 Reserved
	{1840,2760,4750,5150,3,      200,  50,  200,  200,   1035,2900,    150,   2,2700,2180,5050,4750},
};


INT8U sCHKModelType(INT16U chkV)
{
	if(
	(((chkV&0x0F00)>>8)<= 0x0F)  // user
	&& (((chkV&0x00F0)>>4) >= c16KModel &&((chkV&0x00F0)>>4) <= c20KModel) // model //because adding the 16Kw inverter 20151102
	&& ((chkV&0x000F)>0&&(chkV&0x000F)<=cS_SpecNo) // spec
	)
	{
		return cTrue;
	}
	else
	{
		return cFalse;
	}              
}

INT8U  ReadSpecType(void)
{      
	INT8U ubKeyTypeTem;
    
	ubKeyTypeTem = 0x01;//GPIO_ReadInputDataBit(cSelcet5060model_Port, cSelcet5060model_Pin);      //bit 0:5060hz select
	ubKeyTypeTem |= 0x01;//(GPIO_ReadInputDataBit(cSelctPVmodel1_Port, cSelctPVmodel1_Pin)<<1); //bit1:PV input model 1
	//ubKeyTypeTem |=	(GPIO_ReadInputDataBit(cSelctPVmodel2_Port, cSelctPVmodel2_Pin)<<2);//bit2: Pv input model 2
	//ubKeyTypeTem |= (GPIO_ReadInputDataBit(module4_Port, module4_Pin)<<3);		     //bit3 reserve
	ubKeyTypeTem |=0xF0;
    
	ubKeyTypeTem = ~ubKeyTypeTem;
	return(ubKeyTypeTem);
}


void sChangeSpecSet(void)
{
	if(sCHKModelType(EEData1.wTxUxMxSxEE))
	{
		bProtectSet = (INT8U)((EEData1.wTxUxMxSxEE&0xF000)>>12);
		bUserType = (INT8U)((EEData1.wTxUxMxSxEE&0x0F00)>>8);
		bKWmodel =	(INT8U)((EEData1.wTxUxMxSxEE&0x00F0)>>4);//cModelBit;
		bSpecType =cSpecBit;//(INT8U)(EEData1.wTxUxMxSxEE&0x000F);//for Spect in the programm 20150526
	}
	else
	{
		bProtectSet = 0x0F;
		bUserType = 0;
		bKWmodel =cModelBit; //for model in the programm 20150526
		bSpecType =cSpecBit;//for Spect in the programm 20150526
	}
	
	EEData1.wGridVoltLow1EE = cSpecLimitV[bSpecType][0];
	EEData1.wGridVoltHigh1EE = cSpecLimitV[bSpecType][1];
	EEData1.wFreqLowEE = cSpecLimitV[bSpecType][2];
	EEData1.wFreqHighEE = cSpecLimitV[bSpecType][3];
	
	EEData1.wDCICutTimeEE = cSpecLimitV[bSpecType][4];
	EEData1.wVLowCutTime1EE = cSpecLimitV[bSpecType][5];
	EEData1.wVHighCutTime1EE= cSpecLimitV[bSpecType][6];
	EEData1.wFLowCutTimeEE = cSpecLimitV[bSpecType][7];
	EEData1.wFHighCutTimeEE= cSpecLimitV[bSpecType][8];
	
	EEData1.wGridVoltLow2EE = cSpecLimitV[bSpecType][9];
	EEData1.wGridVoltHigh2EE = cSpecLimitV[bSpecType][10];
	EEData1.wVLowCutTime2EE = cSpecLimitV[bSpecType][11];
	EEData1.wVHighCutTime2EE = cSpecLimitV[bSpecType][12];
	
	EEData1.wStartVHighEE = cSpecLimitV[bSpecType][13];
	EEData1.wStartVLowEE = cSpecLimitV[bSpecType][14];
	EEData1.wStartFHighEE = cSpecLimitV[bSpecType][15];
	EEData1.wStartFLowEE = cSpecLimitV[bSpecType][16];

	EEData1.wHVoltReactive2=2530;
	EEData1.wHVoltReactive1=2484;	
	EEData1.wLVoltReactive1=2116;
	EEData1.wLVoltReactive2=2070;
	
	if(bProtectSet&cCustomer)
	{
		EEData1.wDCICutTimeEE = 150;
		if(EEData1.wFHighCutTimeEE<50)
		{
			EEData1.wFHighCutTimeEE = 50;
		}
		if(EEData1.wVHighCutTime1EE<50)
		{
			EEData1.wVHighCutTime1EE=50;
		}
		if(EEData1.wVLowCutTime1EE<50)
		{
			EEData1.wVLowCutTime1EE=50;
		}
		if(EEData1.wVLowCutTime2EE <50)
		{
			EEData1.wVLowCutTime2EE =50;
		}
		if(EEData1.wVHighCutTime1EE<50)
		{
			EEData1.wVHighCutTime1EE=50;
		}
		if( EEData1.wFLowCutTimeEE<50) 
		{
			EEData1.wFLowCutTimeEE=50;
		}
	}
	
	if((bSpecType==cS_CEI0_21)||(bSpecType==cS_BDEW))
	{
        EEData1.fEepromSetting.bSPIEnableEE=1;
		EEData1.fEepromSetting.bLVRideEnableEE = 1;
		EEData1.bFLrateEE = 24;
		ADJValue.wFreqDerateStart=5030;
		EEData1.fEepromSetting.bFreqDerateEnable=0;

		EEData1.wPFLineVLockin=2415;
		EEData1.wPFLineVLockout=2300;
		EEData1.wQvPowerLockin=20;
		EEData1.wQvPowerLockout=5;
			
		ADJValue.wLVRTFaultVolt1=250;
		ADJValue.wLVRTFaultTimer1=2000;
		ADJValue.wLVRTFaultVolt2=550;
		ADJValue.wLVRTFaultTimer2=2000;
		ADJValue.wLVRTFaultVolt3=1150;
		ADJValue.wLVRTFaultTimer3=2000;
		ADJValue.wLVRTFaultVolt4=1700;
		ADJValue.wLVRTFaultTimer4=3500;
		ADJValue.wLVRTFaultVoltHigh=2800;
		ADJValue.wLVRTFaultTimerHigh=3500;
		
	}
	else
	{
		EEData1.fEepromSetting.bSPIEnableEE=1;
		EEData1.fEepromSetting.bLVRideEnableEE = 0;
	}

	if(bSpecType == cS_AS4777)	
	{
		EEData1.wStartDelayTimeEE=65;
	}
	else
	{
		EEData1.wStartDelayTimeEE=30;
	}
	
	if((bSpecType==cS_N4105) || (bSpecType==cS_AS4777)||(bSpecType==cS_EN50438))
	{
		EEData1.wReconnectTime = 65;
	}
	else if((bSpecType == cS_G59) || (bSpecType == cS_Ireland) || (bSpecType == cS_CGC) )
	{
		EEData1.wReconnectTime = 200;
	
	}
	else if(bSpecType==cS_XILA)
	{
		EEData1.wReconnectTime = 182;	
	}
	else if(bSpecType==cS_CEI0_21)
	{
		EEData1.wReconnectTime = 300;
	}
	else
	{
		EEData1.wReconnectTime= 30;
	}	
	
	ubSelectFrequency5060HzFag =ReadSpecType();
	if((ubSelectFrequency5060HzFag & cSelect60Hz) == cSelect60Hz)
	{
		EEData1.w60HzFreqLowEE =  c60HzSpecLimitF[0][0];
		EEData1.w60HzFreqHighEE =  c60HzSpecLimitF[0][1];
	}
}

void ResetADJvalue(void)
{
	ADJValue.wPVV_1_AdjEE = 1024;
	ADJValue.wPVV_2_AdjEE = 1024;
	ADJValue.wPV1_Curr_AdjEE = 1024;
	ADJValue.wPV2_Curr_AdjEE = 1024;
	ADJValue.wBT1Curr_AdjEE2 = 1024;
	ADJValue.wBT2Curr_AdjEE2 = 1024;
	ADJValue.wOPV_R_AdjEE = 1024;
	ADJValue.wOPV_S_AdjEE = 1024;
	ADJValue.wOPV_T_AdjEE = 1024;
	ADJValue.wOPW_AdjEE = 1024;
	ADJValue.wOPW_AdjEE2 = 1024;
	ADJValue.wRCurr_AdjEE = 1024;
	ADJValue.wRCurr_AdjEE2 = 1024;
	ADJValue.wSCurr_AdjEE = 1024;
	ADJValue.wSCurr_AdjEE2 = 1024;
	ADJValue.wTCurr_AdjEE = 1024;
	ADJValue.wTCurr_AdjEE2 = 1024;
	ADJValue.wRDCI_AdjEE = 0;
	ADJValue.wSDCI_AdjEE = 0;
	ADJValue.w30PF_AdjEE1=4096;
	ADJValue.w30PF_AdjEE2 = 4096;
	ADJValue.w60PF_AdjEE1 = 4096;
	ADJValue.w60PF_AdjEE2 = 4096;
	ADJValue.w80PF_AdjEE1 = 4096;
	ADJValue.w80PF_AdjEE2 = 4096;
	ADJValue.wPF_AdjEE = 4096;
}

void ResetSetValue(void)
{
	INT8U i;
	
	EEData1.bLanguage=1;
	EEData1.bCOMAddr=1;			
	EEData1.wStartPVVoltageEE = cPVTurnOnDefaultVolt;
	EEData1.wStartDelayTimeEE = 60;
	EEData1.fEepromSetting.bAutoStartEE=1;	
	EEData1.fEepromSetting.bRS232to485Enable=1;
	EEData1.fEepromSetting.bCountrySetFlag = 1;

	EEData32.wEtoday0k1wEE=0;
	EEData32.dwInvertedP0K1WEE=0;
	EEData32.dwInvTime0p5SecEE=0; 
	EEData32.dwPV_1_TodayEE=0;
	EEData32.dwPV_1_TotalEE=0;
	EEData32.dwPV_2_TodayEE=0;
	EEData32.dwPV_2_TotalEE=0;
	EEData32.dwPVQ_TodayEE=0;
	EEData32.dwPVQ_TotalEE=0;

	bCOMMode = RS232;
	bZigbeORBlueOWifi = Zigbee;
	EEData1.bWireOrWireLess= bCOMMode;
	EEData1.bZigBeOrBlueToothOrWifi=bZigbeORBlueOWifi;

	EEData1.wPassword1=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
	EEData1.wPassword2=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
	EEData1.wPassword3=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');		

	EEData1.bPFCmdMemoryEE=0;
	EEData1.bActiveRateOrderEE=0xFF; 	
	EEData1.bReactiveRateOrderEE=0xFF; 	
	EEData1.wCosPhiOrderEE=0xFFFF; 
	EEData1.wPFModelEE=0;
	
	EEData1.bFLrateEE= 24;
	EEData1.wPFLineVLockin=2530;
	EEData1.wPFLineVLockout=2415;
	EEData1.wQvPowerLockin=20;
	EEData1.wQvPowerLockout=5;
	EEData1.fEepromSetting.bFreqDerateEnable=0;
	EEData1.fEepromSetting.bPVSimilarSet=0;
	EEData1.fEepromSetting.bNLineDisconnect=0;
	

	EEData1.bLVRTKFactor=20;
	ADJValue.wLVRTFaultVolt1=250;
	ADJValue.wLVRTFaultTimer1=2000;
	ADJValue.wLVRTFaultVolt2=550;
	ADJValue.wLVRTFaultTimer2=2000;
	ADJValue.wLVRTFaultVolt3=1150;
	ADJValue.wLVRTFaultTimer3=2000;
	ADJValue.wLVRTFaultVolt4=1700;
	ADJValue.wLVRTFaultTimer4=3500;
	ADJValue.wLVRTFaultVoltHigh=2800;
	ADJValue.wLVRTFaultTimerHigh=3500;
	

	ADJValue.wFreqDerateStart=5030;
	ADJValue.wShutDownCntEE=0;
	EEData1.w10MinAVLimit1=cV10minFaultVoltDefault;
	EEData1.fEepromSetting.bOverLoadPercent = 0;

	for(i=0;i<cDayRecord;i++)
	{
		EEData32.wPowerWeekDay[i]=0; 		//clr day energy
	}
	for(i=0;i<cMonthRecord;i++)
	{
		EEData32.wPowerMonth[i]=0;		 	//clr month energy
	}
	for(i=0;i<cHourRecord;i++)
	{
		EEData32.wPowerEachHour0K1WH[i]=0;	//clr hour energy
	}
	for(i=0;i<cYearRecord;i++)
	{
		EEData32.dwPoweYear0K1WH[i]=0;		 //clr year eenergy
	}
	for(i=0;i < cErrMesgLen;i++)  
	{
		bErrorMsg[i]=0;						// clr Error message	
	}			
	for(i=0;i < cErrRecordLen;i++) 
	{
		bErrorRecord[i]=0;					// clr VF Fault message
	}
	sChangeSpecSet();						//Initial all parameter need to turn on the inverter
    
    bDistanceOnOffOrder=cTurnOn;
}