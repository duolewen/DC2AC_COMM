

//****************************************************************************************
//      File Name   : Communication.c
//     Function     : Communication with the DSP
//****************************************************************************************

#include "GloBalDef.h"

#define cTurnOn			0x01
#define cTurnOff 			0x00

#define 	cErrMesgLen			130//
#define	    cErrMesgStartAddr	 120//80//72
#define 	cErrRecordLen	     252//254//64
#define	cErrorMsgLength		     26

#define	cHourRecord		16
#define	cDayRecord		7
#define	cMonthRecord	12
#define	cYearRecord		20

#define	mErrorMsgBolck		0x08

#define	mErrorRecord		0x20

#define 	cModbusVersion 	100  //for the first version 20150531

#define 		cTrue      0x01
#define 		cFalse     0x00

#pragma pack(1)
struct EEPROMDefaultTable1
{
		INT16U	wTxUxMxSxEE;      		// 2
		INT16U	wTxUxMxSxHighEE;		// 4
		INT8U	bUserSerial[10];			// 14
		INT8U	EepromVersion;  			// 15
		INT8U	bLanguage;				//  16
		INT8U	bCOMAddr;				//  17
		INT8U	bWireOrWireLess;		//  18
		INT8U	bZigBeOrBlueToothOrWifi;	 //19
		INT8U	bDataLength;			// 20
		
		INT16U	wStartPVVoltageEE;		// 22
		INT16U	wStartDelayTimeEE;		// 24

		INT16U	wGridVoltHigh1EE;		// 26
		INT16U	wGridVoltHigh2EE;     		// 28 
		INT16U	wGridVoltLow1EE;      		// 30
		INT16U	wGridVoltLow2EE;      		// 32

		INT16U	wFreqHighEE;      		// 34
		INT16U	wFreqLowEE;      		// 36
		INT16U	wDCICutTimeEE;		// 38
	struct{							
		INT16U	unused0:7;	
		INT16U	bFreqDerateEnable:1;
		INT16U  bNLineDisconnect:1;
		INT16U	bPVSimilarSet:1;
		INT16U	bRS232to485Enable:1;
		INT16U	bAutoStartEE:1;
		INT16U   bCountrySetFlag:1;
		INT16U   bSPIEnableEE:1;
		INT16U   bLVRideEnableEE:1;
		INT16U	bOverLoadPercent:1;
		}fEepromSetting;						//40
		INT16U	wVHighCutTime1EE;		// 42
		INT16U	wVLowCutTime1EE;		// 44
		INT16U	wFHighCutTimeEE;		// 46
		INT16U	wFLowCutTimeEE;		// 48

		INT16U	wVHighCutTime2EE;		// 50
		INT16U	wVLowCutTime2EE;		// 52
		INT16U	wCosPhiOrderEE;  		//54

		INT16U	wStartVHighEE;			//56
		INT16U	wStartVLowEE;			//58
		INT16U	wStartFHighEE;			//60
		INT16U	wStartFLowEE;			//62
		INT16U	wPFLineLoad1;				//64
		INT16U	wPFLinePoint1;				//66
		INT16U	wPFLineLoad2;				//68
		INT16U	wPFLinePoint2;				//70
		INT16U	wPFLineLoad3;				//72
		INT16U	wPFLinePoint3;				//74
		INT16U	wPFLineLoad4;				//76
		INT16U	wPFLinePoint4;				//78
		INT16U	wPFModelEE;				//80
		INT16U	wPassword1;			//82
		INT16U	wPassword2;			//84
		INT16U	wPassword3;			//86
		INT16U	wHVoltReactive1;			//88
		INT16U	wHVoltReactive2;			//90
		INT16U	wLVoltReactive1;			//92
		INT16U	wLVoltReactive2;			//94
		INT16U 	w10MinAVLimit1 ;			//96
		INT16U 	wUserSetVoltH2;			//98
		INT8U	bLVRTKFactor;			// 99
		INT8U	bPFCmdMemoryEE; 		// 100		
		INT8U	bActiveRateOrderEE; 		// 101
		INT8U	bReactiveRateOrderEE; 	// 102
		INT8U	bSetPVModel;		//103
		INT8U	bFLrateEE;				// 104
		INT16U 	wPFLineVLockin;		//106
		INT16U 	wPFLineVLockout;		//108
		INT16U	wQvPowerLockin;		//110
		INT16U	wQvPowerLockout;	//112
		INT16U 	wReconnectTime;		//114
		INT16U	w60HzFreqLowEE;		// 116
		INT16U	w60HzFreqHighEE;		// 118

};
#pragma pack()

#pragma pack(1)
struct ADJValueStruct  //18 words , eeprom II NULL 40 bytes
{
	INT16U wPVV_1_AdjEE; //
	INT16U wPVV_2_AdjEE; //
	INT16U wPV1_Curr_AdjEE;// 
	INT16U wPV2_Curr_AdjEE;//
	INT16U wOPV_R_AdjEE;//
	INT16U wOPW_AdjEE; //    
	INT16U wRCurr_AdjEE;//
	INT16U wSCurr_AdjEE;//
	INT16U wTCurr_AdjEE;//
	INT16S wRDCI_AdjEE; //10 
	INT16S wSDCI_AdjEE;
	INT16U wPF_AdjEE;
	INT16U w30PF_AdjEE1; //30% +0.9
	INT16U w30PF_AdjEE2;	//30% -0.9
	INT16U w60PF_AdjEE1;
	INT16U w60PF_AdjEE2;
	INT16U w80PF_AdjEE1;
	INT16U w80PF_AdjEE2;
	INT16U wOPV_S_AdjEE;
	INT16U wOPV_T_AdjEE;	// 20 
	INT16U wRCurr_AdjEE2;//
	INT16U wSCurr_AdjEE2;//
	INT16U wTCurr_AdjEE2;//
	INT16U wOPW_AdjEE2; // 
	INT16U wBT1Curr_AdjEE2;
	INT16U wBT2Curr_AdjEE2;
	INT16U wLVRTFaultVolt1;//
	INT16U wLVRTFaultVolt2;
	INT16U wLVRTFaultVolt3;
	INT16U wLVRTFaultVolt4;	//30
	INT16U wLVRTFaultVoltHigh;
	INT16U wLVRTFaultTimer1;
	INT16U wLVRTFaultTimer2;
	INT16U wLVRTFaultTimer3;
	INT16U wLVRTFaultTimer4;
	INT16U wLVRTFaultTimerHigh;
	INT16U wFreqDerateStart;
	INT16U wShutDownCntEE;	//38
};
#pragma pack()
struct ADJValueStruct ADJValue;

struct RTCDateTime
{
	INT8U bSeconds;
	INT8U bMinutes;
	INT8U bHours;
	INT8U bDays;			// 1~31
	INT8U bMonths;			// 1~12
	INT8U bYears;			// 0~99, 0 for 2000
	INT8U bWeekdays;
	INT8S bTimeZone;		// UTC -12~+13
};


#pragma pack(1)
struct EEPROMDefaultTable3
{

		INT16U  	wPowerWeekDay[cDayRecord];  //14
		INT16U  	wPowerMonth[cMonthRecord];     // 38
		INT16U  	wPowerEachHour0K1WH[cHourRecord]; ///70
		INT32U   dwPoweYear0K1WH[cYearRecord]; // 150
		INT8U  	Time[6];		//156
		INT16U 	wEtoday0k1wEE;		// 158
		INT32U 	dwInvertedP0K1WEE;       // 162 
		INT32U 	dwInvTime0p5SecEE;       // 166 
		INT32U  	dwPV_1_TodayEE;		// 170
		INT32U  	dwPV_2_TodayEE;	//174
		INT32U  	dwPV_1_TotalEE;		//178
		INT32U  	dwPV_2_TotalEE;		//182
		INT32U  	dwPVQ_TodayEE;		//186
		INT32U  	dwPVQ_TotalEE;		//190
		INT32U	dwReserved[7];		//218
		INT16U	dwReserved1[15];		//248	
};

#pragma pack(1)
struct EEPROMDefaultTable3 EEData32;


struct RTCDateTime sysTime;
struct EEPROMDefaultTable1 EEData1;
INT16U wOPPhase_R_Volt;
INT16U wOPPhase_S_Volt;
INT16U wOPPhase_T_Volt;
INT16U wOPPhase_R_Curr;
INT16U wOPPhase_S_Curr;
INT16U wOPPhase_T_Curr;
INT16U wFaultValueOPV;
INT16U wFaultValueOPF;
INT16U wOPFrecyReal;
INT8U	bEepromNeedWrite=0;
INT16U wTrackerPhaseNum;



INT8U bInterTxBuffer[40];
INT8U bInterRxBuffer[40];
//page 3  Error
INT8U   bErrorRecord[cErrRecordLen];

//page 2  Error && adjust
INT8U   bErrorMsg[cErrMesgLen];
INT8U bInvFaultBit;

INT8U bInterComType;

INT8U ubSelectFrequency5060HzFag;
INT8U bTurnOffCommand=0;
INT16U wWarningValue;
INT8U	bFanCheckCmd;

INT8U bDistanceOnOffOrder=cTurnOn;

INT8U bDSPFlashFlagSendOver=0;
INT16U wScopeInternalCnt;
INT16U uwInterruputScopeCnt;
 INT8U bDSPFlashFlag = 0;

INT8U bConstantPower;
INT8U bInvWarningBit;

INT8U bInverterWarningCode;

void sClrInverterPara(void);
void sFirmwareVersionCheck(void);
void sInterComReceiveExData(INT8U  bRxI);
void sInterComSendExData(INT8U  bTxI);  
extern void sInitSCI2(void);

INT8U sGetFaultBit(INT32U dwFaltCode)
{
	INT32U dwFaultT;
	INT8U bcnt;
	if(dwFaltCode==0)
		return(0);
	bcnt = 1;
	dwFaultT = 1;
	while (bcnt<=32)
	{
		if (dwFaltCode& dwFaultT)
		{
			return(bcnt);
		}
		else
		{
			bcnt++;
			dwFaultT = (dwFaultT<<1);
		}
	}
	return(0);
}

void sClrWarningCode(INT8U bWarningClr)
{
	bInverterWarningCode&=(~bWarningClr);
}

void sSetWarningCode(INT8U bWarningSet)
{
	bInverterWarningCode|=bWarningSet;
}

void sShiftErrorMsg()
{
	INT8U a;

	for(a=0;a<(cErrMesgLen-cErrorMsgLength);a++)
	{
		bErrorMsg[cErrMesgLen-a-1]=bErrorMsg[cErrMesgLen-a-cErrorMsgLength-1];
	}
	bErrorMsg[0]=bInvFaultBit+9;//for error code add 9 20140525
	bErrorMsg[1]=20;
	bErrorMsg[2]=sysTime.bYears;
	bErrorMsg[3]=sysTime.bMonths;
	bErrorMsg[4]=sysTime.bDays;
	bErrorMsg[5]=sysTime.bHours;
	bErrorMsg[6]=sysTime.bMinutes;
	bErrorMsg[7]=sysTime.bSeconds;
	bErrorMsg[8]=(INT8U)(wOPPhase_R_Volt>>8);
	bErrorMsg[9]=(INT8U)(wOPPhase_R_Volt&0x00ff);
	bErrorMsg[10]=(INT8U)(wOPPhase_S_Volt>>8);
	bErrorMsg[11]=(INT8U)(wOPPhase_S_Volt&0x00ff);
	bErrorMsg[12]=(INT8U)(wOPPhase_T_Volt>>8);
	bErrorMsg[13]=(INT8U)(wOPPhase_T_Volt&0x00ff);
	bErrorMsg[14] = (INT8U)(EEData1.wGridVoltHigh1EE>>8);
	bErrorMsg[15] = (INT8U)(EEData1.wGridVoltHigh1EE&0x00ff);
	bErrorMsg[16] = (INT8U)(EEData1.wGridVoltLow1EE>>8);
	bErrorMsg[17] = (INT8U)(EEData1.wGridVoltLow1EE&0x00ff);
	if(19==bErrorMsg[0])//ac v fault
	{
		bErrorMsg[18]=(INT8U)(wFaultValueOPV>>8);
		bErrorMsg[19]=(INT8U)(wFaultValueOPV&0x00ff);
	}
	else   //ac f fault 
	{
		bErrorMsg[18]=(INT8U)(wFaultValueOPF>>8);
		bErrorMsg[19]=(INT8U)(wFaultValueOPF&0x00ff);
	}
	bErrorMsg[20]=(INT8U)(wOPFrecyReal>>8);
	bErrorMsg[21]=(INT8U)(wOPFrecyReal&0x00ff);
	bErrorMsg[22]=(INT8U)(EEData1.wFreqHighEE>>8);
	bErrorMsg[23]=(INT8U)(EEData1.wFreqHighEE&0x00ff);
	bErrorMsg[24]=(INT8U)(EEData1.wFreqLowEE>>8);
	bErrorMsg[25]=(INT8U)(EEData1.wFreqLowEE&0x00ff);
	bEepromNeedWrite|=mErrorMsgBolck;
}

void sShiftErrorRecord()
{
	INT8U a;
	
	for(a = 0; a < 245; a++)
	{
		bErrorRecord[249 - a]=bErrorRecord[244 - a];
	}
	bErrorRecord[0] = bInvFaultBit + 9;//for error code add 9 20140525
	bErrorRecord[1] = sysTime.bMonths;
	bErrorRecord[2] = sysTime.bDays;
	bErrorRecord[3] = sysTime.bHours;
	bErrorRecord[4] = sysTime.bMinutes;
	bEepromNeedWrite|=mErrorRecord;
}

/**
*@ Function  : Send Datas To Control Board
**/
void sUpdateInterCom_M3(INT8U bBufSt,INT8U bType)
{
	INT8U i;
	
	bInterTxBuffer[bBufSt] = bInterComType;
	
	if(bInterComType < 2)
	{
		bInterTxBuffer[3] = (INT8U)(EEData1.wTxUxMxSxEE>>8);
		bInterTxBuffer[4] = (INT8U)(EEData1.wTxUxMxSxEE&0x00FF);
		bInterTxBuffer[5] = (INT8U)(EEData1.wStartDelayTimeEE>>8);
		bInterTxBuffer[6] = (INT8U)(EEData1.wStartDelayTimeEE&0x00FF);
		bInterTxBuffer[7] = (INT8U)(EEData1.wStartPVVoltageEE>>8);
		bInterTxBuffer[8] = (INT8U)(EEData1.wStartPVVoltageEE&0x00FF);
		bInterTxBuffer[9] = (INT8U)(EEData1.wGridVoltHigh1EE>>8);
		bInterTxBuffer[10] = (INT8U)(EEData1.wGridVoltHigh1EE&0x00FF);
		bInterTxBuffer[11] = (INT8U)(EEData1.wGridVoltLow1EE>>8);
		bInterTxBuffer[12] = (INT8U)(EEData1.wGridVoltLow1EE&0x00FF);
		bInterTxBuffer[13] = (INT8U)(EEData1.wFreqHighEE>>8);
		bInterTxBuffer[14] = (INT8U)(EEData1.wFreqHighEE&0x00FF);
		bInterTxBuffer[15] = (INT8U)(EEData1.wFreqLowEE>>8);
		bInterTxBuffer[16] = (INT8U)(EEData1.wFreqLowEE&0x00FF);
	}	
	else if(bInterComType < 4)
	{
		bInterTxBuffer[3] = (INT8U)(ADJValue.wPVV_1_AdjEE>>8);
		bInterTxBuffer[4] = (INT8U)(ADJValue.wPVV_1_AdjEE&0x00FF);
		bInterTxBuffer[5] = (INT8U)(ADJValue.wPVV_2_AdjEE>>8);
		bInterTxBuffer[6] = (INT8U)(ADJValue.wPVV_2_AdjEE&0x00FF);
		bInterTxBuffer[7] = (INT8U)(ADJValue.wPV1_Curr_AdjEE>>8);
		bInterTxBuffer[8] = (INT8U)(ADJValue.wPV1_Curr_AdjEE&0x00FF);
		bInterTxBuffer[9] = (INT8U)(ADJValue.wPV2_Curr_AdjEE>>8);
		bInterTxBuffer[10] = (INT8U)(ADJValue.wPV2_Curr_AdjEE&0x00FF);
		bInterTxBuffer[11] = (INT8U)(ADJValue.wOPV_R_AdjEE>>8);
		bInterTxBuffer[12] = (INT8U)(ADJValue.wOPV_R_AdjEE&0x00FF);
		bInterTxBuffer[13] = (INT8U)(ADJValue.wPF_AdjEE >> 8);
		bInterTxBuffer[14] = (INT8U)(ADJValue.wPF_AdjEE & 0x00FF);
		bInterTxBuffer[15] = (INT8U)(ADJValue.wOPW_AdjEE>>8);      
		bInterTxBuffer[16] = (INT8U)(ADJValue.wOPW_AdjEE&0x00FF);   
	}
	else if(bInterComType < 6)
	{
		bInterTxBuffer[3] = (INT8U)(ADJValue.wRCurr_AdjEE>>8);
		bInterTxBuffer[4] = (INT8U)(ADJValue.wRCurr_AdjEE&0x00FF);
		bInterTxBuffer[5] = (INT8U)(ADJValue.wSCurr_AdjEE>>8);
		bInterTxBuffer[6] = (INT8U)(ADJValue.wSCurr_AdjEE&0x00FF);
		bInterTxBuffer[7] = (INT8U)(ADJValue.wTCurr_AdjEE>>8);
		bInterTxBuffer[8] = (INT8U)(ADJValue.wTCurr_AdjEE&0x00FF);  	
		bInterTxBuffer[9] = (INT8U)(EEData1.wVHighCutTime2EE>>8);
		bInterTxBuffer[10] = (INT8U)(EEData1.wVHighCutTime2EE&0x00FF);
		bInterTxBuffer[11] = (INT8U)(EEData1.wVLowCutTime2EE>>8);
		bInterTxBuffer[12] = (INT8U)(EEData1.wVLowCutTime2EE&0x00FF);
		bInterTxBuffer[13] = (INT8U)(EEData1.wTxUxMxSxHighEE >> 8);
		bInterTxBuffer[14] = (INT8U)(EEData1.wTxUxMxSxHighEE & 0x00FF);
		bInterTxBuffer[15] = (INT8U)(ubSelectFrequency5060HzFag);
		bInterTxBuffer[16] = bTurnOffCommand;
	}	
	else if(bInterComType < 8)
	{
		bInterTxBuffer[3] = 0xFF;
		bInterTxBuffer[4] = EEData1.bCOMAddr;
		bInterTxBuffer[5] = EEData1.bLanguage;
		bInterTxBuffer[6] = bInvFaultBit;
		for(i = 0;i < 10; i++)
		{
			bInterTxBuffer[7 + i] = EEData1.bUserSerial[i];
		}
	}
	else if(bInterComType < 10)
	{
		bInterTxBuffer[3] = (INT8U)(EEData32.wEtoday0k1wEE>>8);
		bInterTxBuffer[4] = (INT8U)(EEData32.wEtoday0k1wEE&0x00FF);
		bInterTxBuffer[5] = (INT8U)(EEData32.dwInvertedP0K1WEE>>24);
		bInterTxBuffer[6] = (INT8U)((EEData32.dwInvertedP0K1WEE&0x00FF0000)>>16);
		bInterTxBuffer[7] = (INT8U)((EEData32.dwInvertedP0K1WEE&0x0000FF00)>>8);
		bInterTxBuffer[8] = (INT8U)(EEData32.dwInvertedP0K1WEE&0x000000FF);
		bInterTxBuffer[9] = (INT8U)0;
		bInterTxBuffer[10] = (INT8U)0;
		bInterTxBuffer[11] = (INT8U)0;
		bInterTxBuffer[12] = (INT8U)0;
		bInterTxBuffer[13] =0;
		bInterTxBuffer[14] =0;
		bInterTxBuffer[15] =0;
		bInterTxBuffer[16] =0; 
	}
	else if(bInterComType < 12)
	{
		bInterTxBuffer[3] = (INT8U)(EEData1.wGridVoltLow2EE>>8);
		bInterTxBuffer[4] = (INT8U)(EEData1.wGridVoltLow2EE&0x00FF);
		bInterTxBuffer[5] = (INT8U)(EEData1.wGridVoltHigh2EE>>8);
		bInterTxBuffer[6] = (INT8U)(EEData1.wGridVoltHigh2EE&0x00FF);
		bInterTxBuffer[7] = (INT8U)(EEData1.wDCICutTimeEE>>8);
		bInterTxBuffer[8] = (INT8U)(EEData1.wDCICutTimeEE&0x00FF);
		bInterTxBuffer[9] = (INT8U)(EEData1.wVHighCutTime1EE>>8);
		bInterTxBuffer[10] = (INT8U)(EEData1.wVHighCutTime1EE&0x00FF);
		bInterTxBuffer[11] = (INT8U)(EEData1.wVLowCutTime1EE>>8);
		bInterTxBuffer[12] = (INT8U)(EEData1.wVLowCutTime1EE&0x00FF);
		bInterTxBuffer[13] = (INT8U)(EEData1.wFHighCutTimeEE>>8);
		bInterTxBuffer[14] = (INT8U)(EEData1.wFHighCutTimeEE&0x00FF);
		bInterTxBuffer[15] = (INT8U)(EEData1.wFLowCutTimeEE>>8);
		bInterTxBuffer[16] = (INT8U)(EEData1.wFLowCutTimeEE&0x00FF);			
	}
	else if(bInterComType < 14)
	{
		bInterTxBuffer[3] = EEData1.bPFCmdMemoryEE;   
		bInterTxBuffer[4] = (INT8U)(EEData1.wCosPhiOrderEE>>8);
		bInterTxBuffer[5] = (INT8U)(EEData1.wCosPhiOrderEE&0x00FF);
		bInterTxBuffer[6] = EEData1.bActiveRateOrderEE;
		bInterTxBuffer[7] = EEData1.bReactiveRateOrderEE;
		bInterTxBuffer[8] = bDistanceOnOffOrder;
		bInterTxBuffer[9] = EEData1.fEepromSetting.bAutoStartEE;
		bInterTxBuffer[10] = (INT8U)(EEData1.wPFModelEE & 0x00FF);
		bInterTxBuffer[11] = sysTime.bYears;
		bInterTxBuffer[12] = sysTime.bMonths;
		bInterTxBuffer[13] = sysTime.bDays;
		bInterTxBuffer[14] = sysTime.bHours;
		bInterTxBuffer[15] = sysTime.bMinutes;
		bInterTxBuffer[16] = sysTime.bSeconds;
		
	}
	else if(bInterComType < 16)
	{
		bInterTxBuffer[3] = (INT8U)(EEData1.wStartVHighEE >> 8);
		bInterTxBuffer[4] = (INT8U)(EEData1.wStartVHighEE & 0x00FF);
		bInterTxBuffer[5] = (INT8U)(EEData1.wStartVLowEE >> 8);
		bInterTxBuffer[6] = (INT8U)(EEData1.wStartVLowEE & 0x00FF);
		bInterTxBuffer[7] = (INT8U)(EEData1.wStartFHighEE >> 8);
		bInterTxBuffer[8] = (INT8U)(EEData1.wStartFHighEE & 0x00FF);
		bInterTxBuffer[9] = (INT8U)(EEData1.wStartFLowEE >> 8);
		bInterTxBuffer[10] = (INT8U)(EEData1.wStartFLowEE & 0x00FF);
		bInterTxBuffer[11] = 0;
		bInterTxBuffer[12] =0; 
		bInterTxBuffer[13] = (INT8U)(wTrackerPhaseNum >> 8);
		bInterTxBuffer[14] = (INT8U)wTrackerPhaseNum;
		bInterTxBuffer[15] = sysTime.bWeekdays;
		bInterTxBuffer[16] = (INT8U)cModbusVersion;
	}
	else if(bInterComType < 18)
	{
		bInterTxBuffer[3] =EEData1.fEepromSetting.bLVRideEnableEE;
		bInterTxBuffer[4] = (INT8U)EEData1.fEepromSetting.bOverLoadPercent;//zh 20151109 ;
		bInterTxBuffer[5] = 0; 
		bInterTxBuffer[6] = 0;

		bInterTxBuffer[7] = 0;
		bInterTxBuffer[8] = 0;
		bInterTxBuffer[9] = 0;
		bInterTxBuffer[10] = 0;
		bInterTxBuffer[11] = 0;
		bInterTxBuffer[12] = 0;
		bInterTxBuffer[13] = 0;
		bInterTxBuffer[14] = 0;
		bInterTxBuffer[15] = 0;
		bInterTxBuffer[16] = 0;
	}
	else if(bInterComType < 20)
	{
		bInterTxBuffer[3] = (INT8U)(ADJValue.w30PF_AdjEE1 >> 8);
		bInterTxBuffer[4] = (INT8U)(ADJValue.w30PF_AdjEE1 & 0x00FF);
		bInterTxBuffer[5] = (INT8U)(ADJValue.w30PF_AdjEE2 >> 8);
		bInterTxBuffer[6] = (INT8U)(ADJValue.w30PF_AdjEE2 & 0x00FF);
		bInterTxBuffer[7] = (INT8U)(ADJValue.w60PF_AdjEE1 >> 8);
		bInterTxBuffer[8] = (INT8U)(ADJValue.w60PF_AdjEE1 & 0x00FF);
		bInterTxBuffer[9] = (INT8U)(ADJValue.w60PF_AdjEE2 >> 8);
		bInterTxBuffer[10] = (INT8U)(ADJValue.w60PF_AdjEE2 & 0x00FF);
		bInterTxBuffer[11] = (INT8U)(ADJValue.w80PF_AdjEE1 >> 8);
		bInterTxBuffer[12] = (INT8U)(ADJValue.w80PF_AdjEE1 & 0x00FF);
		bInterTxBuffer[13] = (INT8U)(ADJValue.w80PF_AdjEE2 >> 8);
		bInterTxBuffer[14] = (INT8U)(ADJValue.w80PF_AdjEE2 & 0x00FF);
		bInterTxBuffer[15] = (INT8U)(EEData1.fEepromSetting.bSPIEnableEE & 0x00FF);
		bInterTxBuffer[16] = (INT8U)(EEData1.bFLrateEE);
	}
	else if(bInterComType < 22)
	{
		bInterTxBuffer[3] = (INT8U)(EEData1.wPFLineLoad1 >> 8);
		bInterTxBuffer[4] = (INT8U)(EEData1.wPFLineLoad1 & 0x00FF);
		bInterTxBuffer[5] = (INT8U)(EEData1.wPFLineLoad2 >> 8);
		bInterTxBuffer[6] = (INT8U)(EEData1.wPFLineLoad2 & 0x00FF);
		bInterTxBuffer[7] = (INT8U)(EEData1.wPFLinePoint2 >> 8);
		bInterTxBuffer[8] = (INT8U)(EEData1.wPFLinePoint2 & 0x00FF);
		bInterTxBuffer[9] = (INT8U)(EEData1.wPFLineLoad3  >> 8);
		bInterTxBuffer[10] = (INT8U)(EEData1.wPFLineLoad3  & 0x00FF);
		bInterTxBuffer[11] = (INT8U)(EEData1.wPFLinePoint3  >> 8);
		bInterTxBuffer[12] = (INT8U)(EEData1.wPFLinePoint3 & 0x00FF);
		bInterTxBuffer[13] = (INT8U)(EEData1.wPFLineLoad4  >> 8);
		bInterTxBuffer[14] = (INT8U)(EEData1.wPFLineLoad4  & 0x00FF);
		bInterTxBuffer[15] = (INT8U)(EEData1.wPFLinePoint4>> 8);
		bInterTxBuffer[16] = (INT8U)(EEData1.wPFLinePoint4 & 0x00FF);
	}
	else if(bInterComType < 24)
	{
		bInterTxBuffer[3] = (INT8U)(EEData1.wPFLinePoint1 >> 8);
		bInterTxBuffer[4] = (INT8U)(EEData1.wPFLinePoint1 & 0x00FF);
		bInterTxBuffer[5] = (INT8U)(EEData1.wPFModelEE >> 8);	
		bInterTxBuffer[6] = (INT8U)(EEData1.wPFModelEE & 0x00FF);
		bInterTxBuffer[7] = (INT8U)(EEData1.wHVoltReactive1 >> 8);
		bInterTxBuffer[8] = (INT8U)(EEData1.wHVoltReactive1 & 0x00FF);
		bInterTxBuffer[9] = (INT8U)(EEData1.wHVoltReactive2 >> 8);
		bInterTxBuffer[10] = (INT8U)(EEData1.wHVoltReactive2 & 0x00FF);
		bInterTxBuffer[11] = (INT8U)(EEData1.wLVoltReactive1 >> 8);
		bInterTxBuffer[12] = (INT8U)(EEData1.wLVoltReactive1 & 0x00FF);
		bInterTxBuffer[13] = (INT8U)(EEData1.wLVoltReactive2 >> 8);
		bInterTxBuffer[14] = (INT8U)(EEData1.wLVoltReactive2 & 0x00FF);
		bInterTxBuffer[15] = 0;
		bInterTxBuffer[16] = 0;
	}
	else if(bInterComType < 26)
	{
		bInterTxBuffer[3] = (INT8U)(bDSPFlashFlag);
		bInterTxBuffer[4] = (INT8U)EEData1.fEepromSetting.bCountrySetFlag;
		bInterTxBuffer[5] = (INT8U)(EEData32.dwPV_1_TodayEE>> 24);
		bInterTxBuffer[6] = (INT8U)(EEData32.dwPV_1_TodayEE>> 16);
		bInterTxBuffer[7] = (INT8U)(EEData32.dwPV_1_TodayEE>>8);
		bInterTxBuffer[8] = (INT8U)(EEData32.dwPV_1_TodayEE);  	
		bInterTxBuffer[9] = (INT8U)(EEData32.dwPV_1_TotalEE>> 24);
		bInterTxBuffer[10] = (INT8U)(EEData32.dwPV_1_TotalEE>> 16);
		bInterTxBuffer[11] = (INT8U)(EEData32.dwPV_1_TotalEE>> 8);
		bInterTxBuffer[12] = (INT8U)(EEData32.dwPV_1_TotalEE);
		bInterTxBuffer[13] = (INT8U)(EEData32.dwPV_2_TodayEE>> 24);
		bInterTxBuffer[14] = (INT8U)(EEData32.dwPV_2_TodayEE>> 16);
		bInterTxBuffer[15] = (INT8U)(EEData32.dwPV_2_TodayEE>> 8);
		bInterTxBuffer[16] = (INT8U)(EEData32.dwPV_2_TodayEE);
		if(bDSPFlashFlag==1)
		{
			bDSPFlashFlagSendOver=1;
		}
		else
		{
			bDSPFlashFlagSendOver=0;
		}
	}
	else if(bInterComType < 28)
	{
		bInterTxBuffer[3] = (INT8U)(EEData32.dwPV_2_TotalEE>> 24);
		bInterTxBuffer[4] = (INT8U)(EEData32.dwPV_2_TotalEE>> 16);
		bInterTxBuffer[5] = (INT8U)(EEData32.dwPV_2_TotalEE>> 8);
		bInterTxBuffer[6] = (INT8U)(EEData32.dwPV_2_TotalEE);
		bInterTxBuffer[7] = (INT8U)(EEData32.dwPVQ_TodayEE>> 24);
		bInterTxBuffer[8] = (INT8U)(EEData32.dwPVQ_TodayEE>> 16);  	
		bInterTxBuffer[9] = (INT8U)(EEData32.dwPVQ_TodayEE>> 8);
		bInterTxBuffer[10] = (INT8U)(EEData32.dwPVQ_TodayEE);
		bInterTxBuffer[11] = (INT8U)(EEData32.dwPVQ_TotalEE>> 24);
		bInterTxBuffer[12] = (INT8U)(EEData32.dwPVQ_TotalEE>> 16);  	
		bInterTxBuffer[13] = (INT8U)(EEData32.dwPVQ_TotalEE>> 8);
		bInterTxBuffer[14] = (INT8U)(EEData32.dwPVQ_TotalEE);
		bInterTxBuffer[15] = (INT8U)(EEData1.w10MinAVLimit1 >> 8);		
		bInterTxBuffer[16] = (INT8U)(EEData1.w10MinAVLimit1 & 0x00FF);
	}
	else if(bInterComType < 30)	
	{
		bInterTxBuffer[3] = (INT8U)(ADJValue.wOPV_S_AdjEE>>8);
		bInterTxBuffer[4] = (INT8U)(ADJValue.wOPV_S_AdjEE&0x00FF);
		bInterTxBuffer[5] = (INT8U)(ADJValue.wOPV_T_AdjEE>>8);
		bInterTxBuffer[6] = (INT8U)(ADJValue.wOPV_T_AdjEE&0x00FF);
		bInterTxBuffer[7] = (INT8U)(EEData1.wPFLineVLockin >> 8);
		bInterTxBuffer[8] = (INT8U)(EEData1.wPFLineVLockin & 0x00FF);
		bInterTxBuffer[9] = (INT8U)(EEData1.wPFLineVLockout >> 8);
		bInterTxBuffer[10] = (INT8U)(EEData1.wPFLineVLockout & 0x00FF);
		bInterTxBuffer[11] = (INT8U)(EEData1.wQvPowerLockin >> 8);
		bInterTxBuffer[12] = (INT8U)(EEData1.wQvPowerLockin & 0x00FF);
		bInterTxBuffer[13] = (INT8U)(EEData1.wQvPowerLockout >> 8);
		bInterTxBuffer[14] = (INT8U)(EEData1.wQvPowerLockout & 0x00FF);
		bInterTxBuffer[15] = (INT8U)(EEData1.wReconnectTime >> 8);
		bInterTxBuffer[16] = (INT8U)(EEData1.wReconnectTime & 0x00FF);
	}
	else if(bInterComType < 32)
	{
		bInterTxBuffer[3] = (INT8U)(EEData1.fEepromSetting.bFreqDerateEnable>>8);
		bInterTxBuffer[4] = (INT8U)(EEData1.fEepromSetting.bFreqDerateEnable&0x00FF);
		bInterTxBuffer[5] = (INT8U)(EEData1.bLVRTKFactor>>8);
		bInterTxBuffer[6] = (INT8U)(EEData1.bLVRTKFactor&0x00FF);
		bInterTxBuffer[7] = (INT8U)(ADJValue.wLVRTFaultVolt1>>8);
		bInterTxBuffer[8] = (INT8U)(ADJValue.wLVRTFaultVolt1&0x00FF);
		bInterTxBuffer[9] = (INT8U)(ADJValue.wLVRTFaultVolt2>>8);
		bInterTxBuffer[10] = (INT8U)(ADJValue.wLVRTFaultVolt2&0x00FF);
		bInterTxBuffer[11] = (INT8U)(ADJValue.wLVRTFaultVolt3>>8);
		bInterTxBuffer[12] = (INT8U)(ADJValue.wLVRTFaultVolt3&0x00FF);
		bInterTxBuffer[13] = (INT8U)(ADJValue.wLVRTFaultVolt4>>8);
		bInterTxBuffer[14] = (INT8U)(ADJValue.wLVRTFaultVolt4&0x00FF);
		bInterTxBuffer[15] = (INT8U)(ADJValue.wLVRTFaultVoltHigh>>8);
		bInterTxBuffer[16] = (INT8U)(ADJValue.wLVRTFaultVoltHigh&0x00FF);		
	}
	else  if(bInterComType < 34)
	{
		bInterTxBuffer[3] = (INT8U)(ADJValue.wLVRTFaultTimer1>>8);
		bInterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultTimer1&0x00FF);
		bInterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultTimer2>>8);
		bInterTxBuffer[6] = (INT8U)(ADJValue.wLVRTFaultTimer2&0x00FF);
		bInterTxBuffer[7] = (INT8U)(ADJValue.wLVRTFaultTimer3>>8);
		bInterTxBuffer[8] = (INT8U)(ADJValue.wLVRTFaultTimer3&0x00FF);
		bInterTxBuffer[9] = (INT8U)(ADJValue.wLVRTFaultTimer4>>8);
		bInterTxBuffer[10] = (INT8U)(ADJValue.wLVRTFaultTimer4&0x00FF);
		bInterTxBuffer[11] = (INT8U)(ADJValue.wLVRTFaultTimerHigh>>8);
		bInterTxBuffer[12] = (INT8U)(ADJValue.wLVRTFaultTimerHigh&0x00FF);	
		bInterTxBuffer[13] = (INT8U)(ADJValue.wRCurr_AdjEE2>>8);
		bInterTxBuffer[14] = (INT8U)(ADJValue.wRCurr_AdjEE2&0x00FF);
		bInterTxBuffer[15] = (INT8U)(ADJValue.wSCurr_AdjEE2>>8);
		bInterTxBuffer[16] = (INT8U)(ADJValue.wSCurr_AdjEE2&0x00FF);	
	}
	else  if(bInterComType < 36)
	{
		bInterTxBuffer[3] = (INT8U)(ADJValue.wTCurr_AdjEE2>>8);
		bInterTxBuffer[4] = (INT8U)(ADJValue.wTCurr_AdjEE2&0x00FF);
		bInterTxBuffer[5] = (INT8U)(ADJValue.wOPW_AdjEE2>>8);
		bInterTxBuffer[6] = (INT8U)(ADJValue.wOPW_AdjEE2&0x00FF);
		bInterTxBuffer[7] = (INT8U)(ADJValue.wFreqDerateStart>>8);
		bInterTxBuffer[8] = (INT8U)(ADJValue.wFreqDerateStart&0x00FF);
		bInterTxBuffer[9] = (INT8U)(ADJValue.wBT1Curr_AdjEE2>>8);
		bInterTxBuffer[10] = (INT8U)(ADJValue.wBT1Curr_AdjEE2&0x00FF);
		bInterTxBuffer[11] = (INT8U)(ADJValue.wBT2Curr_AdjEE2>>8);
		bInterTxBuffer[12] = (INT8U)(ADJValue.wBT2Curr_AdjEE2&0x00FF);
		bInterTxBuffer[13] = (INT8U)(bConstantPower & 0x00FF);
		bInterTxBuffer[14] = bInvWarningBit;
		bInterTxBuffer[15] = (INT8U)(wWarningValue>>8);
		bInterTxBuffer[16] = (INT8U)(wWarningValue&0x00FF);
	}
	else if(bInterComType < 38)
	{
		bInterTxBuffer[3] = (INT8U)EEData1.fEepromSetting.bNLineDisconnect;
		bInterTxBuffer[4] = (INT8U)EEData1.fEepromSetting.bPVSimilarSet;	
		bInterTxBuffer[5] = bFanCheckCmd;	
		bInterTxBuffer[6] = (INT8U)(EEData1.w60HzFreqLowEE>>8);
		bInterTxBuffer[7] = (INT8U)(EEData1.w60HzFreqLowEE&0x00FF);
		bInterTxBuffer[8] = (INT8U)(EEData1.w60HzFreqHighEE>>8);
		bInterTxBuffer[9] = (INT8U)(EEData1.w60HzFreqHighEE&0x00FF);		
		bInterTxBuffer[10] = (INT8U)EEData1.bSetPVModel;
	}
}
/**
@*      Function  : Receive Datas From Control Board	
**/
INT32U dwSpecLoad=200000;
INT16U wStartDelayTime;
INT16U wNTC_Tempr;
INT16U wINV_Tempr;
INT8U bInvStart;
INT16U wBus_Volt;
INT16U wPV_1_Volt;
INT16U wPV_2_Volt;
INT16U wPV_1_Curr;
INT16U wPV_2_Curr;
INT32U dwOP_RST_WattSec;
INT32U dwPV_1_Watt;
INT32U dwPV_2_Watt;
INT32U dwInputWatt;
INT8U bBoostOnFlag;
INT32U dwOP_R_WattSec;
INT32U dwOP_S_WattSec;
INT32U dwOP_T_WattSec;
INT16U wFaultValuePVV;
INT16U wFaultValuePVISO;
INT16U wFaultValueGFCI;
INT16U wFaultValueDCI;
INT16U wFaultValueNTCT;
INT8U bPVInputFlag; //20150513 for check PV input model  by PV voltage
INT16U wLEDDisplayFlag;//20150519 for LED display
INT16U wPFNumOrder;
INT32U dwOP_ReActPowerSec;
INT16U wBus_P_Volt;
INT16U wBus_N_Volt;
INT8U wReConnectConFlag;
INT8U bDeratingModeFlag;
INT16U wShutDownFlag;
INT32U dwOPFullLoad;
INT8U bCheckHardware;
INT16U wLoadPer100;
INT16U uwScope[45];
INT8U ubDSPControlFirmware[10] = {'V','D',':','0','1','.','0','0','0','0'} ;
void sDownInterCom_M3(INT8U bBufSt)
{
	INT8U bComType;
	INT8U i;

	bComType = bInterRxBuffer[bBufSt++];
	if(bComType == 0)
	{
		dwSpecLoad = ((((INT32U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]))<<16)&0xFFFF0000)
			+ (INT32U)((INT16U)bInterRxBuffer[bBufSt+2]*256 + (INT16U)bInterRxBuffer[bBufSt+3]);
		bBufSt += 4; //5 6 7 8
		wStartDelayTime= (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//9  10
		wNTC_Tempr = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//11 12
		wINV_Tempr = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//13 14
		bInvStart=bInterRxBuffer[bBufSt];
		bBufSt+=1; //15
		wBus_Volt =(INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt+=2; //16 17
		bBufSt+=1;//18
	}
	else if(bComType == 1)
	{
		wPV_1_Volt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//5 6
		wPV_2_Volt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//7 8
		wPV_1_Curr = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//9 10 
		wPV_2_Curr = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;	//11 12
		dwOP_RST_WattSec = ((((INT32U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]))*65536)&0xFFFF0000)
			+ (INT32U)((INT16U)bInterRxBuffer[bBufSt+2]*256 + (INT16U)bInterRxBuffer[bBufSt+3]);
		bBufSt += 4;//13 14 15 16
		bBoostOnFlag =  bInterRxBuffer[bBufSt];
		bBufSt += 1;//17
		bBufSt += 1;//18

		dwPV_1_Watt = (INT32U)wPV_1_Volt * wPV_1_Curr/10;
		dwPV_2_Watt = (INT32U)wPV_2_Volt * wPV_2_Curr/10;
		dwInputWatt = dwPV_1_Watt + dwPV_2_Watt;
		
	}
	else if(bComType == 2)
	{
		wOPPhase_R_Curr = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//5 6
		wOPPhase_S_Curr = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//7 8
		wOPPhase_T_Curr = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//9 10
		wOPPhase_R_Volt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;// 11 12
		wOPPhase_S_Volt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//13 14
		wOPPhase_T_Volt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//15 16
		dwOP_R_WattSec = (INT32U)wOPPhase_R_Volt*wOPPhase_R_Curr/10;
		dwOP_S_WattSec = (INT32U)wOPPhase_S_Volt*wOPPhase_S_Curr/10;
		dwOP_T_WattSec = (INT32U)wOPPhase_T_Volt*wOPPhase_T_Curr/10;

		wOPFrecyReal = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//17 18
	}
	else if(bComType == 3)
	{
		wFaultValueOPV = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//5 6
		wFaultValueOPF = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;// 7 8
		wFaultValuePVV = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//9 10
		wFaultValueDCI = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//11 12
		wFaultValueGFCI = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//13 14
		wFaultValuePVISO = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;//15 16
		wFaultValueNTCT = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt += 2;	//17 18
	}
	else if(bComType == 4)	
	{
		bPVInputFlag=bInterRxBuffer[bBufSt];	//5
		bBufSt+=1;//5
		wLEDDisplayFlag = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt+=2;//6 7
		bBufSt++;//8
		bBufSt++;//9
		bBufSt++;//10
		bBufSt+=2;//11 12
		bBufSt+=2;//13 14
		bBufSt+=2;//15 16
		bBufSt+=2;//17 18
	}
	else if(bComType==5)
	{
		wPFNumOrder=(INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);    // 0--20000
		bBufSt+=2; // 5 6
		dwOP_ReActPowerSec = ((((INT32U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]))<<16)&0xFFFF0000)
			+ (INT32U)((INT16U)bInterRxBuffer[bBufSt+2]*256 + (INT16U)bInterRxBuffer[bBufSt+3]);
		bBufSt+=4; //7 8 9 10
		wBus_P_Volt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt+=2; //11 12£»
		wBus_N_Volt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt+=2; //13 14
		bBufSt++; //15
		bDeratingModeFlag=bInterRxBuffer[bBufSt++];// 16
		wReConnectConFlag=bInterRxBuffer[bBufSt++];// 17
		bBufSt++;//18
	}
	else if(bComType==6)
	{
		wShutDownFlag = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);    // 0--20000
		bBufSt+=2; //5 6
		bBufSt++;//7
		bBufSt++;//8
		dwOPFullLoad = ((((INT32U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]))<<16)&0xFFFF0000)
			+ (INT32U)((INT16U)bInterRxBuffer[bBufSt+2]*256 + (INT16U)bInterRxBuffer[bBufSt+3]);
		bBufSt+=4; //9 10 11 12
		bCheckHardware = bInterRxBuffer[bBufSt++];	//13
		wLoadPer100 = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);    // 0--20000
		bBufSt+=2; //14 15
		bBufSt++;//16
	}
    else if(bComType==7)
	{
		wScopeInternalCnt = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt +=2;
		for(i=0;i<6;i++)
		{
			uwScope[wScopeInternalCnt*6+i] = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
			bBufSt +=2;
		}
	}
	else if(bComType==8)
	{
		uwScope[42] = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt +=2;
		uwScope[43] = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt +=2;
		uwScope[44] = (INT16U)((INT16U)bInterRxBuffer[bBufSt]*256 + (INT16U)bInterRxBuffer[bBufSt+1]);
		bBufSt +=2;
		ubDSPControlFirmware[1] = bInterRxBuffer[bBufSt++];
		ubDSPControlFirmware[3] = bInterRxBuffer[bBufSt++];
		ubDSPControlFirmware[4] = bInterRxBuffer[bBufSt++];
		ubDSPControlFirmware[6] = bInterRxBuffer[bBufSt++];
		ubDSPControlFirmware[7] = bInterRxBuffer[bBufSt++];
		ubDSPControlFirmware[8] = bInterRxBuffer[bBufSt++];
		ubDSPControlFirmware[9] = bInterRxBuffer[bBufSt++];
		sFirmwareVersionCheck();
	}
}

/**
@*File Name    : Send  Task
@*Function     : Communication with the Control Board
@*Execute every: 100ms
**/

#define cINV_ShutDown   	0x01
#define 	LenCom2DSP 	19

INT8U bM3toDSPSendFinished=1;
INT8U bDSPFlashStatus=0;
INT32U dwInverterFaultCode;
INT8U bInterComTxEnd;
INT8U bInterTxIndex;

INT16U sCalStringSum(INT8U *bpString,INT8U bStartN,INT8U bEndN)
{
	INT8U i;
	INT32U wStringSum=0;
	
	for(i=bStartN;i<=bEndN;i++)
	{
		wStringSum += (INT8U)((INT8U)(*(bpString+i))^i);
	}
	if(wStringSum==0||wStringSum>0xFFFF)
	{
		wStringSum = 0xFFFF;
	}
	return((INT16U)wStringSum);
}

INT8U sSCIWrite(INT8U sciid,INT8U *pstart,INT16U wLength)
{
	return(0);
}

void sDSP_M3ComTxTask(void *arg)
{
	static INT16U wComTxTemp0;
	
	while(1)
	{   

		if(bM3toDSPSendFinished==1)
		{
			bM3toDSPSendFinished=0;

			if(bDSPFlashFlagSendOver==1)
			{
				bDSPFlashStatus=1;
				continue;
			}
			
			if(bDSPFlashStatus==0)
			{
				bInterTxBuffer[0] = 'C';
				bInterTxBuffer[1] = (INT8U)(dwInverterFaultCode&0x000000FF); 				
				sUpdateInterCom_M3(2,bInterComType);
				if((++bInterComType) >=38) 
				{
					bInterComType = 1;
					if((++bInterComTxEnd) >= 2)
					{
						if(bTurnOffCommand&cINV_ShutDown)
						{
							bDSPFlashFlag=1;
						}
						bInterComTxEnd=0;
						bTurnOffCommand=0;
					}
				}
				wComTxTemp0 = sCalStringSum(bInterTxBuffer,0,(LenCom2DSP-3));
				bInterTxBuffer[LenCom2DSP-2] = (INT8U)(wComTxTemp0>>8); //25
				bInterTxBuffer[LenCom2DSP-1] = (INT8U)(wComTxTemp0&0x00FF);//26
				bInterTxIndex=0;
				sSCIWrite(2,bInterTxBuffer,LenCom2DSP); 
			}		
		}
		
		//OSTaskStart(sDSP_M3ComTxTask_PRIO, 0, 100);
		//OSMaskEventPend(0xFFFE);
	
	}
}

/**
@*File Name   : Receive  Task
@*Function     : Communication with the Control Board
**/

#define 	cSCI2RxEnded			0x08



INT8U sSCIRead(INT8U sciid,INT8U *pOutBuf)
{ 
    #if 0
	SciRxStatus *pscib;
	if(sciid == 0||sciid == 1)
	{
		pscib = &SCIRXB[sciid];
		if(pscib->bRxStatus == cSciRxBufEmpty)
			return (cSciRxBufEmpty);
		if(pscib->bReadCnt < pscib->bRxSize)
		{
			*pOutBuf = *(pscib->bpRxBuffer + pscib->bReadCnt);
			pscib->bReadCnt ++;
			if(pscib->bRxLength > 0)
				pscib->bRxLength --;
			if(pscib->bRxLength == 0)
			{
				pscib->bRxStatus = cSciRxBufEmpty;
				pscib->bRxCnt = 0;
				pscib->bReadCnt =0;
			}
		}
		else
		{
			pscib->bRxStatus = cSciRxBufEmpty;
			pscib->bRxCnt = 0;
			pscib->bReadCnt =0;
			pscib->bRxLength = 0;
		}
		return (cSciRxRdy);
	}
	else if(sciid == 2)
	{
		pscib = &SCIRXB[sciid];
		if(pscib->bRxStatus == cSciRxBufEmpty)
			return (cSciRxBufEmpty);
		if(pscib->bReadCnt < pscib->bRxSize)
		{
			*pOutBuf = *(pscib->bpRxBuffer + pscib->bReadCnt);
			pscib->bReadCnt ++;
			if(pscib->bRxLength < pscib->bRxSize)
				pscib->bRxLength ++;
			if(pscib->bRxLength == pscib->bRxCnt)
			{
				pscib->bRxStatus = cSciRxBufEmpty;
				pscib->bRxCnt = 0;
				pscib->bReadCnt =0;
				pscib->bRxLength = 0;
			}
		}
		else
		{
			pscib->bRxStatus = cSciRxBufEmpty;
			pscib->bRxCnt = 0;
			pscib->bReadCnt =0;
			pscib->bRxLength = 0;
		}
		return (cSciRxRdy);
	
	}
    #endif
   return 1;
}

#define		cSciTxRdy				0
#define		cSciTxBusy				1

#define		cSciRxRdy				0
#define		cSciRxBufEmpty			1

#define 	LenDSP2Com 	21

#define	mEnegyBlock1		0x04
#define cInvterterStatus			1

#define cBComFault				0x00000001//error:10  communicatiion fault    			1 

INT8U bInterRxIndex;
INT8U bInverterStatus;
INT8U bRelayBNeed;
INT16U  wWriteEEPROMCnt;
INT8U bDSPDataParsing;


INT8U sFaultFlagCHK(INT32U dwFaultFlag)
{
	if(dwInverterFaultCode&dwFaultFlag)
	{
		return(cTrue);
	}
	else
	{
		return(cFalse);
	}
}

void sClrFaultCode(INT32U dwFaultClr)
{
    /*
        OS_CPU_SR  cpu_sr=0;
	OS_ENTER_CRITICAL();
	dwInverterFaultCode &= (~dwFaultClr);
	OS_EXIT_CRITICAL();
    */
}

void sSetFaultCode(INT32U dwFaultSet)
{
    /*
        OS_CPU_SR  cpu_sr=0;
	OS_ENTER_CRITICAL();
	dwInverterFaultCode |= dwFaultSet;
	OS_EXIT_CRITICAL();
    */
}

INT8U sFaultCHK(void)
{
	if(dwInverterFaultCode==0)
	{
		return(cFalse);
	}
	else
	{
		return(cTrue);
	}
}
#define cFaultStatus				3
#define cFlashStatus				4
INT8U   bFaultMemory;
void sDSP_M3ComRxTask(void *arg)
{
	static  INT8U bInterComLCnt;
	static INT8U bEvent;
    static INT8U bTemp;
	static INT8U bRXD;
	static INT16U wComRxTemp0;
	static INT8U bComFaultClrCnt=0;
	
	while(1)
	{
	
		if(bDSPFlashStatus==0)
		{
			if(bEvent&cSCI2RxEnded)
			{
				while((bTemp = sSCIRead(2, &bRXD))==cSciRxRdy)
				{
					if(bInterRxIndex==0)
					{
						if(bRXD=='D')
						{
							bInterRxBuffer[0] = bRXD;
							bInterRxIndex++;
						}
					}
					else if(bInterRxIndex < LenDSP2Com)
					{
						bInterRxBuffer[bInterRxIndex] = bRXD;
						bInterRxIndex++;
					}
				}
				wComRxTemp0 = (INT16U)bInterRxBuffer[LenDSP2Com-2]*256 +(INT16U)bInterRxBuffer[LenDSP2Com-1];//27,28
				if(wComRxTemp0 == (sCalStringSum(bInterRxBuffer, 0, (LenDSP2Com-3))))
				{
					bInverterStatus = bInterRxBuffer[1];
					bInvFaultBit = bInterRxBuffer[2];
					bRelayBNeed = bInterRxBuffer[3];
					if((wWriteEEPROMCnt > 2400) && (bInverterStatus != cInvterterStatus))
					{
						bEepromNeedWrite|=mEnegyBlock1;							
						wWriteEEPROMCnt = 0;						
					}
					sDownInterCom_M3(4);
				}
				bInterComLCnt = 0;
				bInterRxIndex=0;
				bDSPDataParsing=0;

				if(sFaultFlagCHK(cBComFault))
				{
					bComFaultClrCnt++;
					if(bComFaultClrCnt>3)
					{
						bComFaultClrCnt=0;
						sClrFaultCode(cBComFault);
					}
				}
			}			
			else
			{
				bComFaultClrCnt=0;
				bInterComLCnt++;
				if(bInterComLCnt>20)   //500ms*20=10s
				{
					bInterComLCnt = 0;
					sSetFaultCode(cBComFault);
					bInterRxIndex=0;	
					sClrInverterPara();			
				}
				else if( bInterComLCnt > 10 )
				{
					//sInitSCI2();
				}
			}
			
			if(sFaultCHK())
			{
				bInvFaultBit = sGetFaultBit(dwInverterFaultCode);
				if(bInverterStatus!=cFaultStatus && bInverterStatus!=cFlashStatus)
				{
					bInverterStatus = cFaultStatus;
				}
			}
			
			if(bInvFaultBit!=0)
			{	
				if((bInvFaultBit+9)!=bErrorRecord[0])//for error code add 9 20140525
				{
					sShiftErrorRecord();
				}
				
				if(((bInvFaultBit==19)||(bInvFaultBit==20))&& (bFaultMemory ==cTrue))
				{
					sShiftErrorMsg();
					bFaultMemory=cFalse;
				}
			}
			
		}
		//bEvent=OSTaskPend();
	}

}

void sClrInverterPara(void)
{
	wOPPhase_R_Curr = 0;
	wOPPhase_S_Curr = 0;
	wOPPhase_T_Curr = 0;
	dwOP_RST_WattSec = 0;
	dwOP_R_WattSec = 0;
	dwOP_S_WattSec = 0;
	dwOP_T_WattSec = 0;
	dwPV_1_Watt = 0;
	wPV_1_Curr=0;
	dwPV_2_Watt = 0;
	wPV_2_Curr=0;
	dwInputWatt = 0;
}
const INT8U cbSTM3COMFirmware[10]={'V','M',':','0','1','.','0','0','0','4'};
#define cFirmwareVersionWarning 0x10 //14
void sFirmwareVersionCheck(void)
{
	static INT8U bVersionFirmwareCheckOK=0;
	static INT8U bVersionCheckFailCnt;
	INT8U bFirmwareCheckOKFlag;

	bFirmwareCheckOKFlag=1;
	
	if(bVersionFirmwareCheckOK==0)
	{
		if(((cbSTM3COMFirmware[1]=='M')&&(ubDSPControlFirmware[1]!='D') )||\
			((cbSTM3COMFirmware[3]!=ubDSPControlFirmware[3]))||((cbSTM3COMFirmware[4]!=ubDSPControlFirmware[4])))
		{
			bFirmwareCheckOKFlag=0;
		}
		if(bFirmwareCheckOKFlag==1)
		{
			bVersionFirmwareCheckOK=1;
			sClrWarningCode(cFirmwareVersionWarning);
		}
		else
		{
			bVersionCheckFailCnt++;
			if(bVersionCheckFailCnt>5)
			{
				bVersionCheckFailCnt=0;
				sSetWarningCode(cFirmwareVersionWarning);
			}
		}
	}
}

