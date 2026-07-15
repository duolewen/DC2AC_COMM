#ifndef DSP_M3_COMTASKDATA_H_
#define DSP_M3_COMTASKDATA_H_

#include "GlobalDef.h"

#define	cHourRecord		16
#define	cDayRecord		7
#define	cMonthRecord	12
#define	cYearRecord		20


#define	cPVTurnOnDefaultVolt		3500

#define	cV10minFaultVoltDefault  	2530
#define	cV10minFaultVoltLow		    2300
#define	cV10minFaultVoltHigh		2760	

#define     RS232                   0x01
#define     Exter_Wireless   		0x02
#define     Inter_Wireless       	0x03

#define     Bluetooth       	    0x01
#define     Zigbee           	    0x02
#define     Wifi             	    0x04

#define 	cErrMesgLen			    130//
#define	    cErrMesgStartAddr	    120//80//72
#define 	cErrRecordLen	        252//254//64
#define	    cErrorMsgLength		    26

#define 		cTrue      0x01
#define 		cFalse     0x00

#define cSPS_ShutDown	0x80
#define cINV_ShutDown   0x01
#define cTurnOn			0x01
#define cTurnOff        0x00

/********************************************************************************/
#define cS_SpecNo		16

#define cS_VDE0126		1
#define cS_Belgium		2
#define cS_TUV000		3
#define cS_AS4777		4
#define cS_CEI0_21		5
#define cS_RD1663		6
#define cS_XILA			7
#define cS_N4105		8
#define cS_G83			9
#define cS_Ireland		10
#define cS_CGC			11
#define cS_EN50438		12
#define cS_G59			13
#define cS_Reserved		14		
#define cS_BDEW			15

/********************************************************************************
  Model Macro										                                           
*********************************************************************************/
#define c16KModel		4
#define	c18KModel		5
#define	c20KModel		6

/********************************************************************************/

//for define model and stander;
#define cSpecBit	cS_CGC	
#define cModelBit c20KModel   

/********************************************************************************/
#define cCustomer		0x08
#define cEnaDCI			0x04
#define cEnaGFCI			0x02
#define cEnaISO			0x01
/********************************************************************************/

/*key select*/
#define cSelect60Hz 0x01
#define cSelectInputModel1 0x02
#define cSelectInputModel2 0x04

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
#pragma pack()

extern INT8U   bErrorRecord[cErrRecordLen];
extern INT8U   bErrorMsg[cErrMesgLen];
extern INT8U   bInvFaultBit;

extern struct ADJValueStruct      ADJValue;
extern struct EEPROMDefaultTable1 EEData1;
extern struct EEPROMDefaultTable3 EEData32;

extern INT8U ubSelectFrequency5060HzFag;
extern INT8U bDistanceOnOffOrder;
extern INT8U bTurnOffCommand;
#endif