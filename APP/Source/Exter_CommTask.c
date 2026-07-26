#include "Exter_CommTask.h"
#include "DSP_M3_COMTaskData.h"
#include "Exter_CommTaskData.h"


#define 	cExSCILMax 		150

#define cRS232FlashDSP 3
#define cRS485FlashDSP 4

#define 		ExC0		('?')
#define 		ExC1		('#')
#define 		ExC2		('p')
#define  		cModBus_3    	0x03
#define  		cModBus_4    	0x04
#define 		cModBus_6    	0x06
#define  		cModBus_16  	16
#define 		cModBusOrder 	0x0F
#define  		cModBusData  	0xFF
#define  		cMaster      		0xF0

INT8U bRS485CommuStatus;
INT8U bExterRX485Index = 0;

INT16U wCounts485;
INT8U bRx485DataLenth;

INT8U bExterRX485Buffer[cExSCILMax+1];
INT8U bExterTX485Buff[cExSCILMax+1];
INT8U bExComNoRespon=0;
INT32U dwCOMcnt= 0;
INT8U bRS485DSPFlashFlag=0;
INT8U bFlashDisable=0;
INT8U bTx485DataLenth;
INT8U bExterTxBuffer[cExSCILMax+1];
INT8U bdisplayFlag; //for communication display LED state

INT8U UserShowTable[17]="      Clou      \0";

extern INT8U bDSPFlashStatus;
extern INT8U bInverterStatus;
extern INT8U bDSPFlashFlag ;
extern struct RTCDateTime sysTime;
extern INT8U bKWmodel;
extern INT16U wTrackerPhaseNum;
extern INT8U ubDSPControlFirmware[10];
extern const INT8U cbSTM3COMFirmware[10];
 INT8U bFactory;
extern INT16U wPFNumOrder;
extern INT16U wLEDDisplayFlag;//20150519 for LED display
extern INT8U  bFanCheckCmd;
extern INT8U bCheckHardware;
extern INT16U wStartDelayTime;
extern INT8U bSpecType;
INT8U aucCRCHi[256] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};
INT8U aucCRCLo[256] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};


INT16U usMBCRC16( INT8U * pucFrame, INT16U usLen ) 
{
    INT8U           ucCRCHi = 0xFF;
    INT8U           ucCRCLo = 0xFF;
    INT16U             iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( INT8U )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( INT16U )( ucCRCHi << 8 | ucCRCLo );
}


//************************************************
//    MODBUS  3
//    read  holding register
//************************************************
INT8U bHoldingReserveFlag;
INT8U bModbusERROR = 0;
INT8U bModbusSend[100];
INT8U LCDManuInf[4][17]=
{
	"Model:          \0", //0 ;model
	"Ver  :V00R00C00 \0", // 1:Version
	"SN   :          \0",// 2: number
	"     CP20KTL    \0",//  3:CP20ktl
};
#define 	cFirmwareVersionIndex	1
INT32U dwRatedPower;
INT8U bTestProcess=0;
void sReadHoldingRegister(INT8UP bRXDataBuff)
{
	INT16U wStartAddress,wNumberOfReg,wCRC1Temp;
	INT16U i,j;
	INT16U wTransmitData;
	INT8U  bFault=0;
	INT8U  bBlockNum;

     wStartAddress = *(bRXDataBuff+2);
     wStartAddress = (wStartAddress<<8)|*(bRXDataBuff+3);
     wNumberOfReg  = *(bRXDataBuff+4);
     wNumberOfReg  = (wNumberOfReg<<8)|*(bRXDataBuff+5);
	 
	if(wStartAddress < 2)
	 {
	 	bHoldingReserveFlag=1;
	 }
	 else
	 {
	 	bHoldingReserveFlag=0;
		wStartAddress -= 2;
	 }//for resever 2 reg 20150610
	   
	bBlockNum=wStartAddress/45;
	
	if(((wStartAddress+wNumberOfReg)>(bBlockNum+1)*45)||(wStartAddress>=225)||(wNumberOfReg>45)\
	   	|| (wNumberOfReg==0))
	{
	      bFault=1;
	}

	if((bFault==1)||(bHoldingReserveFlag == 1))///for0 \ 1 register  reserve 20150610
	{
		bExterTxBuffer[1]=(cModBus_3|0x80);
		bExterTxBuffer[2]=1;
		wCRC1Temp = usMBCRC16(bExterTxBuffer,3);
		bExterTxBuffer[3]= (INT8U)(wCRC1Temp);
		bExterTxBuffer[4]= (INT8U)(wCRC1Temp>>8);
		bModbusERROR=1;
		return;
	}
	
	wStartAddress = wStartAddress*2;
	wNumberOfReg  = wNumberOfReg*2;
	
	bExterTxBuffer[2]=wNumberOfReg;
	
	if((wStartAddress>>1)<45)
	{
		wTransmitData=EEData1.fEepromSetting.bAutoStartEE;		//02 On/Off  
		wTransmitData=(wTransmitData<<8)|bDistanceOnOffOrder;
			bModbusSend[0]=(INT8U)(wTransmitData>>8);
			bModbusSend[1]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.fEepromSetting.bSPIEnableEE; 	// 03 SPI Enable
			bModbusSend[2]=(INT8U)(wTransmitData>>8);
			bModbusSend[3]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.bPFCmdMemoryEE; 					//04 PF memary 
			bModbusSend[4]=(INT8U)(wTransmitData>>8);
			bModbusSend[5]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.bActiveRateOrderEE; 				//05 Active Rate 
			bModbusSend[6]=(INT8U)(wTransmitData>>8);
			bModbusSend[7]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.bReactiveRateOrderEE;				// 06 ReActive Rate
			bModbusSend[8]=(INT8U)(wTransmitData>>8);
			bModbusSend[9]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wCosPhiOrderEE; 					//07 wCosPhiOrder 
			bModbusSend[10]=(INT8U)(wTransmitData>>8);
			bModbusSend[11]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwRatedPower>>16); 		// 08 Rated power H
			bModbusSend[12]=(INT8U)(wTransmitData>>8);
			bModbusSend[13]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwRatedPower); 			//09 Rated power L
			bModbusSend[14]=(INT8U)(wTransmitData>>8);
			bModbusSend[15]=(INT8U)(wTransmitData);
		wTransmitData=6000; 								//10 Normal work voltage
			bModbusSend[16]=(INT8U)(wTransmitData>>8);
			bModbusSend[17]=(INT8U)(wTransmitData);
		wTransmitData=0; 								//  11 Firmware version (high)
			bModbusSend[18]=LCDManuInf[cFirmwareVersionIndex][6];
			bModbusSend[19]=LCDManuInf[cFirmwareVersionIndex][7];
		wTransmitData=0;								//  12 Firmware version (middle)
			bModbusSend[20]=LCDManuInf[cFirmwareVersionIndex][8];
			bModbusSend[21]=LCDManuInf[cFirmwareVersionIndex][9];
		wTransmitData=0;								//  13 Firmware version (low)
			bModbusSend[22]=LCDManuInf[cFirmwareVersionIndex][10];
			bModbusSend[23]=LCDManuInf[cFirmwareVersionIndex][11];
		wTransmitData=0;								//  14 Control Firmware version (high)
			bModbusSend[24]= LCDManuInf[cFirmwareVersionIndex][12]; ; 
			bModbusSend[25]= LCDManuInf[cFirmwareVersionIndex][13];
		wTransmitData=0;								//  15 Control Firmware version (middle)
			bModbusSend[26]= LCDManuInf[cFirmwareVersionIndex][14]; 
			bModbusSend[27]= 0x00;
		wTransmitData=0;								//  16 Control Firmware version (low)
			bModbusSend[28]= 0x00;
			bModbusSend[29]= 0x00; 
			
		wTransmitData=(INT16U)(EEData1.bLanguage);			//17 LCD language 
			bModbusSend[30]=(INT8U)(wTransmitData>>8);
			bModbusSend[31]=(INT8U)(wTransmitData);
		wTransmitData=0;										//18 LCD Contrast 
			bModbusSend[32]=(INT8U)(wTransmitData>>8);
			bModbusSend[33]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wStartPVVoltageEE;				//19 Input start voltage 
			bModbusSend[34]=(INT8U)(wTransmitData>>8);
			bModbusSend[35]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wStartDelayTimeEE; 				//20 Start Delly time  
			bModbusSend[36]=(INT8U)(wTransmitData>>8);
			bModbusSend[37]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wGridVoltLow1EE;					//21 Grid voltage low limit protect 
			bModbusSend[38]=(INT8U)(wTransmitData>>8);
			bModbusSend[39]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wGridVoltHigh1EE;					//22 Grid voltage high limit 1 
			bModbusSend[40]=(INT8U)(wTransmitData>>8);
			bModbusSend[41]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wFreqLowEE;						//23 Grid frequency low limit 
			bModbusSend[42]=(INT8U)(wTransmitData>>8);
			bModbusSend[43]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wFreqHighEE;						//24 Grid high frequency limit 
			bModbusSend[44]=(INT8U)(wTransmitData>>8);
			bModbusSend[45]=(INT8U)(wTransmitData);
		wTransmitData=0; 										//25 Serial number 5 
		for(i = 0;i < 10;i++)
		{
			bModbusSend[46 + i]=EEData1.bUserSerial[i];	//26 27 28 29 Serial number
		}
		
		wTransmitData=0; 								//30 reserve 
			bModbusSend[56]=(INT8U)(wTransmitData>>8);
			bModbusSend[57]=(INT8U)(wTransmitData);
		wTransmitData=0; 								//31 reserve
			bModbusSend[58]=(INT8U)(wTransmitData>>8);
			bModbusSend[59]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.bCOMAddr; 		//32 Com Address 
			bModbusSend[60]=(INT8U)(wTransmitData>>8);
			bModbusSend[61]=(INT8U)(wTransmitData);
		wTransmitData=0; 								//33 FlashStart  
			bModbusSend[62]=(INT8U)(wTransmitData>>8);
			bModbusSend[63]=(INT8U)(wTransmitData);
		wTransmitData=0; 								//34 Reset User Info  
			bModbusSend[64]=(INT8U)(wTransmitData>>8);
			bModbusSend[65]=(INT8U)(wTransmitData);
		wTransmitData=0; 								//  35 Reset to factory 
			bModbusSend[66]=(INT8U)(wTransmitData>>8);
			bModbusSend[67]=(INT8U)(wTransmitData);
		wTransmitData=bTestProcess; 					//  36 AutoTestStart  
			bModbusSend[68]=0;
			bModbusSend[69]=bTestProcess;
		wTransmitData=EEData1.wGridVoltLow2EE; 			//  37 Vac low 2  
			bModbusSend[70]=(INT8U)(wTransmitData>>8);
			bModbusSend[71]=(INT8U)(wTransmitData);	
		wTransmitData=EEData1.wGridVoltHigh2EE; 		//  38 Vac high 2   
			bModbusSend[72]=(INT8U)(wTransmitData>>8);
			bModbusSend[73]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wFreqLowEE;				//  39 Fac low  
			bModbusSend[74]=(INT8U)(wTransmitData>>8);
			bModbusSend[75]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wFreqHighEE;				//  40 Fac high 
			bModbusSend[76]=(INT8U)(wTransmitData>>8);
			bModbusSend[77]=(INT8U)(wTransmitData);	
		wTransmitData = EEData1.wStartVLowEE;			//  41 Start V Low  
			bModbusSend[78] = (INT8U)(wTransmitData >> 8);
			bModbusSend[79] = (INT8U)(wTransmitData);	
		wTransmitData = EEData1.wStartVHighEE;				// 42 Start V High  
			bModbusSend[80] = (INT8U)(wTransmitData >> 8);
			bModbusSend[81] = (INT8U)(wTransmitData);	
		wTransmitData = EEData1.wStartFLowEE;				// 43 Start F Low
			bModbusSend[82] = (INT8U)(wTransmitData >> 8);
			bModbusSend[83] = (INT8U)(wTransmitData);	
		wTransmitData = EEData1.wStartFHighEE;				//  44 Start F High
			bModbusSend[84] = (INT8U)(wTransmitData >> 8);
			bModbusSend[85] = (INT8U)(wTransmitData);		
		wTransmitData = (INT16U)bKWmodel;					//  45 DTC  
			bModbusSend[86] = 8;
			bModbusSend[87]=(INT8U)(wTransmitData);	
		wTransmitData = wTrackerPhaseNum;					//  46 TP  
			bModbusSend[88]=(INT8U)(wTransmitData >> 8);
			bModbusSend[89]=(INT8U)(wTransmitData);
		for(i=wStartAddress,j=3;i<(wStartAddress+wNumberOfReg);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress>>1)<90)			// Paragraph 2 : 47~91 
	{
		wTransmitData=(INT16U)sysTime.bYears+2000;		// 47 System time-year,offset 2000
			bModbusSend[0]=(INT8U)(wTransmitData>>8);
			bModbusSend[1]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)sysTime.bMonths;			// 48 System time- Month 
			bModbusSend[2]=(INT8U)(wTransmitData>>8);
			bModbusSend[3]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)sysTime.bDays;			// 49 System time- Day 
			bModbusSend[4]=(INT8U)(wTransmitData>>8);
			bModbusSend[5]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)sysTime.bHours;			// 50 System time- Hour 
			bModbusSend[6]=(INT8U)(wTransmitData>>8);
			bModbusSend[7]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)sysTime.bMinutes;			// 51 System time- Min 
			bModbusSend[8]=(INT8U)(wTransmitData>>8);
			bModbusSend[9]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)sysTime.bSeconds;			// 52 System time- Sec 
			bModbusSend[10]=(INT8U)(wTransmitData>>8);
			bModbusSend[11]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wVLowCutTime1EE*20;		// 53 Vac low1 time 
			bModbusSend[12]=(INT8U)(wTransmitData>>8);
			bModbusSend[13]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wVHighCutTime1EE*20;	// 54 Vac high1 time 
			bModbusSend[14]=(INT8U)(wTransmitData>>8);
			bModbusSend[15]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wVLowCutTime2EE*20;		// 55 Vac low2 time 
			bModbusSend[16]=(INT8U)(wTransmitData>>8);
			bModbusSend[17]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wVHighCutTime2EE*20;	// 56 Vac high2 time 
			bModbusSend[18]=(INT8U)(wTransmitData>>8);
			bModbusSend[19]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wFLowCutTimeEE*20;		// 57 Fac low1 time 
			bModbusSend[20]=(INT8U)(wTransmitData>>8);
			bModbusSend[21]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wFHighCutTimeEE*20;		// 58 Fac high1 time 
			bModbusSend[22]=(INT8U)(wTransmitData>>8);
			bModbusSend[23]=(INT8U)(wTransmitData);
		wTransmitData = 0;								// 59 Fac low2 time 
			bModbusSend[24]=(INT8U)(wTransmitData>>8);
			bModbusSend[25]=(INT8U)(wTransmitData);
		wTransmitData = 0;								// 60 Fac high2 time 
			bModbusSend[26]=(INT8U)(wTransmitData>>8);
			bModbusSend[27]=(INT8U)(wTransmitData);

			for(i=28,j=0;i<44;i++,j++)				// 61-68 Manufacturer information 
			{
				bModbusSend[i]=UserShowTable[j];
			}
		wTransmitData = 0;							// 69 reserve
			bModbusSend[44] =0; 
			bModbusSend[45] =0; 
		wTransmitData = 0;						// 70 reserve
			bModbusSend[46] =0; 
			bModbusSend[47] =0;
		wTransmitData = 0;						// 71 reserve
			bModbusSend[48] =0;
			bModbusSend[49] =0;
		wTransmitData = 0;						// 72 reserve
			bModbusSend[50] =0;
			bModbusSend[51] =0;
		wTransmitData = 0;						// 73 reserve
			bModbusSend[52] = 0;
			bModbusSend[53] = 0;
		wTransmitData = (INT16U)sysTime.bWeekdays;	// 74 Weekly
			bModbusSend[54] = (INT8U)(wTransmitData >> 8);
			bModbusSend[55] = (INT8U)(wTransmitData);
		wTransmitData = (INT16U)cModbusVersion;				// 75 Modbus Version
			bModbusSend[56] = (INT8U)(wTransmitData >> 8);
			bModbusSend[57] = (INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.fEepromSetting.bCountrySetFlag; //76 reserve
			bModbusSend[58] = (INT8U)(wTransmitData >> 8);
			bModbusSend[59] = (INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.fEepromSetting.bRS232to485Enable; //77 RS232 to 485 Enable
			bModbusSend[60] = (INT8U)(wTransmitData >> 8);
			bModbusSend[61] = (INT8U)(wTransmitData);
		wTransmitData = (INT16U)0;							//78 reserve 
			bModbusSend[62] = (INT8U)(wTransmitData >> 8);
			bModbusSend[63] = (INT8U)(wTransmitData);
		wTransmitData = (INT16U)0;							//79 reserve
			bModbusSend[64] = (INT8U)(wTransmitData >> 8);
			bModbusSend[65] = (INT8U)(wTransmitData);
		wTransmitData = (INT16U)0;							//80 reserve
			bModbusSend[66] = (INT8U)(wTransmitData >> 8);
			bModbusSend[67] = (INT8U)(wTransmitData);
		wTransmitData = (INT16U)0;							//81
			bModbusSend[68] = (INT8U)(wTransmitData >> 8);
			bModbusSend[69] = (INT8U)(wTransmitData);
		wTransmitData=(INT16U)bFactory;						//82
            bModbusSend[70] = (INT8U)(wTransmitData >> 8);
		    bModbusSend[71] = (INT8U)(wTransmitData);
		wTransmitData = ADJValue.wFreqDerateStart;			//83
			bModbusSend[72] = (INT8U)(wTransmitData >> 8);
			bModbusSend[73] = (INT8U)(wTransmitData);
		wTransmitData = 0;									//84
			bModbusSend[74] = (INT8U)(wTransmitData >> 8);
			bModbusSend[75] = (INT8U)(wTransmitData);
		wTransmitData = EEData1.w60HzFreqLowEE;				//85
			bModbusSend[76] = (INT8U)(wTransmitData >> 8);
			bModbusSend[77] = (INT8U)(wTransmitData);
		wTransmitData = EEData1.w60HzFreqHighEE;			//86
			bModbusSend[78] = (INT8U)(wTransmitData >> 8);
			bModbusSend[79] = (INT8U)(wTransmitData);
		for(i = 80;i < 90;i++)
			bModbusSend[i] = 0;
		
		for((i=wStartAddress-90),j=3;i<(wStartAddress+wNumberOfReg-90);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 135) 	// Paragraph 2 : 92~136 
	{	
		wTransmitData = (INT16U)EEData1.wPFLineLoad1;					//92 PF limit line point 1 load percent
			bModbusSend[0]=(INT8U)(wTransmitData>>8);
			bModbusSend[1]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFLinePoint1;					//93 PF limit line point 1 power factor
			bModbusSend[2]=(INT8U)(wTransmitData>>8);
			bModbusSend[3]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFLineLoad2;					//94 PF limit line point 2 load percent
			bModbusSend[4]=(INT8U)(wTransmitData>>8);
			bModbusSend[5]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFLinePoint2;					//95 PF limit line point 2 power factor
			bModbusSend[6]=(INT8U)(wTransmitData>>8);
			bModbusSend[7]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFLineLoad3;					//96 PF limit line point 3 load percent
			bModbusSend[8]=(INT8U)(wTransmitData>>8);
			bModbusSend[9]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFLinePoint3;					//97 PF limit line point 3 power factor
			bModbusSend[10]=(INT8U)(wTransmitData>>8);
			bModbusSend[11]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFLineLoad4;					//98 PF limit line point 4 load percent
			bModbusSend[12]=(INT8U)(wTransmitData>>8);
			bModbusSend[13]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFLinePoint4;					//99 PF limit line point 4 power factor
			bModbusSend[14]=(INT8U)(wTransmitData>>8);
			bModbusSend[15]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)0;										//100 
			bModbusSend[16]=(INT8U)(wTransmitData>>8);
			bModbusSend[17]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.wPFModelEE;						// 101 PF Model
			bModbusSend[18]=(INT8U)(wTransmitData>>8);
			bModbusSend[19]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)EEData1.bFLrateEE;						// 102	FLrate
			bModbusSend[20]=(INT8U)(wTransmitData>>8);
			bModbusSend[21]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.w80PF_AdjEE1;//103 
			bModbusSend[22]=(INT8U)(wTransmitData>>8);
			bModbusSend[23]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.w60PF_AdjEE1;//104
			bModbusSend[24]=(INT8U)(wTransmitData>>8);
			bModbusSend[25]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.w30PF_AdjEE1;//105
			bModbusSend[26]=(INT8U)(wTransmitData>>8);
			bModbusSend[27]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.w80PF_AdjEE2;//106
			bModbusSend[28]=(INT8U)(wTransmitData>>8);
			bModbusSend[29]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.w60PF_AdjEE2;//107
			bModbusSend[30]=(INT8U)(wTransmitData>>8);
			bModbusSend[31]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.w30PF_AdjEE2;//108
			bModbusSend[32]=(INT8U)(wTransmitData>>8);
			bModbusSend[33]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.fEepromSetting.bLVRideEnableEE;//109
			bModbusSend[34]=(INT8U)(wTransmitData>>8);
			bModbusSend[35]=(INT8U)(wTransmitData);			
		wTransmitData = EEData1.wHVoltReactive1;//110
			bModbusSend[36]=(INT8U)(wTransmitData>>8);
			bModbusSend[37]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wHVoltReactive2;//111
			bModbusSend[38]=(INT8U)(wTransmitData>>8);
			bModbusSend[39]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wLVoltReactive1;//112
			bModbusSend[40]=(INT8U)(wTransmitData>>8);
			bModbusSend[41]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wLVoltReactive2;//113
			bModbusSend[42]=(INT8U)(wTransmitData>>8);
			bModbusSend[43]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.w10MinAVLimit1;			// 114	//Vset20120817
			bModbusSend[44]=(INT8U)(wTransmitData>>8);
			bModbusSend[45]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wQvPowerLockin;			// 115	Q(v) lock in active power of CEI021
			bModbusSend[46]=(INT8U)(wTransmitData>>8);
			bModbusSend[47]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wPFLineVLockin;			// 116	Lock in gird volt of CEI021
			bModbusSend[48]=(INT8U)(wTransmitData>>8);
			bModbusSend[49]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wPFLineVLockout;			// 117	Lock out gird volt of CEI021
			bModbusSend[50]=(INT8U)(wTransmitData>>8);
			bModbusSend[51]=(INT8U)(wTransmitData);
		wTransmitData = 1;									// 118	Above 6KwSystem for CEI021
			bModbusSend[52]=(INT8U)(wTransmitData>>8);
			bModbusSend[53]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.fEepromSetting.bFreqDerateEnable;			// 119	Frequency Derating Enable 
			bModbusSend[54]=(INT8U)(wTransmitData>>8);
			bModbusSend[55]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wQvPowerLockout;			// 120	Q(v) lock Out active power of CEI021
			bModbusSend[56]=(INT8U)(wTransmitData>>8);
			bModbusSend[57]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.wReconnectTime;			// 121	Restart Delay Time after fault back;
			bModbusSend[58]=(INT8U)(wTransmitData>>8);
			bModbusSend[59]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.bLVRTKFactor; 		//122 
			bModbusSend[60]=(INT8U)(wTransmitData>>8);
			bModbusSend[61]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultVolt1;			// 123	LVFRT low fault value 1
			bModbusSend[62]=(INT8U)(wTransmitData>>8);
			bModbusSend[63]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultTimer1;			//124 LVFRT low fault time 1
			bModbusSend[64]=(INT8U)(wTransmitData>>8);
			bModbusSend[65]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultVolt2;			//125 	LVFRT low fault value 2
			bModbusSend[66]=(INT8U)(wTransmitData>>8);
			bModbusSend[67]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultTimer2;			//126 LVFRT low fault time 2
			bModbusSend[68]=(INT8U)(wTransmitData>>8);
			bModbusSend[69]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultVolt3;			//127  	LVFRT low fault value 3
			bModbusSend[70]=(INT8U)(wTransmitData>>8);
			bModbusSend[71]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultTimer3;			//128 LVFRT low fault time3
			bModbusSend[72]=(INT8U)(wTransmitData>>8);
			bModbusSend[73]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultVolt4;			//129 LVFRT low fault value 4
			bModbusSend[74]=(INT8U)(wTransmitData>>8);
			bModbusSend[75]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultTimer4;			//130 LVFRT low fault time4
			bModbusSend[76]=(INT8U)(wTransmitData>>8);
			bModbusSend[77]=(INT8U)(wTransmitData);
		wTransmitData = ADJValue.wLVRTFaultVoltHigh; 		//131 LVFRT low fault value hihg
			bModbusSend[78]=(INT8U)(wTransmitData>>8);
			bModbusSend[79]=(INT8U)(wTransmitData);
			wTransmitData = ADJValue.wLVRTFaultTimerHigh; 	//132 LVFRT low fault time high
			bModbusSend[80]=(INT8U)(wTransmitData>>8);
			bModbusSend[81]=(INT8U)(wTransmitData);
		wTransmitData = wPFNumOrder; 		//133 Powerfactor  0-20000
			bModbusSend[82]=(INT8U)(wTransmitData>>8);
			bModbusSend[83]=(INT8U)(wTransmitData);
		wTransmitData = wLEDDisplayFlag;    //134 for test LED display;
			bModbusSend[84] = (INT8U)(wTransmitData>>8);
			bModbusSend[85] = (INT8U)(wTransmitData);
		for(i = 86; i < 90; i++)
			bModbusSend[i] = 0;		
		for((i=wStartAddress-180),j=3;i<(wStartAddress+wNumberOfReg-180);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
		
	}
	else if((wStartAddress>>1)<180)
	{
	      for(i=0;i<30;i++)
		{
			bModbusSend[i] = 0;
		}	
		wTransmitData=bFanCheckCmd;							//152
			bModbusSend[30]=(INT8U)(wTransmitData>>8);
			bModbusSend[31]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.fEepromSetting.bNLineDisconnect;	//153	//NLineSet20130422
			bModbusSend[32]=(INT8U)(wTransmitData>>8);
			bModbusSend[33]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.fEepromSetting.bPVSimilarSet;	    /*154 PVSimilarSet*/
			bModbusSend[34]=(INT8U)(wTransmitData>>8);
			bModbusSend[35]=(INT8U)(wTransmitData);
		wTransmitData=bCheckHardware;	                        /*155 GFCI*/
			bModbusSend[36]=(INT8U)(wTransmitData>>8);
			bModbusSend[37]=(INT8U)(wTransmitData);
		wTransmitData=0;//                /*156 GFCI*/
			bModbusSend[38]=(INT8U)(wTransmitData>>8);
			bModbusSend[39]=(INT8U)(wTransmitData);
		wTransmitData =0; 	//157
			bModbusSend[40]=(INT8U)(wTransmitData>>8);
			bModbusSend[41]=(INT8U)(wTransmitData);
		wTransmitData=0;//                        //158 
			bModbusSend[42]=(INT8U)(wTransmitData>>8);
			bModbusSend[43]=(INT8U)(wTransmitData);
		wTransmitData=0;//                         //159 
			bModbusSend[44]=(INT8U)(wTransmitData>>8);
			bModbusSend[45]=(INT8U)(wTransmitData);
		wTransmitData=0;//                          //160 
			bModbusSend[46]=(INT8U)(wTransmitData>>8);
			bModbusSend[47]=(INT8U)(wTransmitData);
		wTransmitData=0;//  	                        //161 GFCI/
			bModbusSend[48]=(INT8U)(wTransmitData>>8);
			bModbusSend[49]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*162 */
			bModbusSend[50]=(INT8U)(wTransmitData>>8);
			bModbusSend[51]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*163 */
			bModbusSend[52]=(INT8U)(wTransmitData>>8);
			bModbusSend[53]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*164 */
			bModbusSend[54]=(INT8U)(wTransmitData>>8);
			bModbusSend[55]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*165*/
			bModbusSend[56]=(INT8U)(wTransmitData>>8);
			bModbusSend[57]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*166*/
			bModbusSend[58]=(INT8U)(wTransmitData>>8);
			bModbusSend[59]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*167*/
			bModbusSend[60]=(INT8U)(wTransmitData>>8);
			bModbusSend[61]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*168*/
			bModbusSend[62]=(INT8U)(wTransmitData>>8);
			bModbusSend[63]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*169*/
			bModbusSend[64]=(INT8U)(wTransmitData>>8);
			bModbusSend[65]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*170*/
			bModbusSend[66]=(INT8U)(wTransmitData>>8);
			bModbusSend[67]=(INT8U)(wTransmitData);
		wTransmitData=0;			 /*171*/
			bModbusSend[68]=(INT8U)(wTransmitData>>8);
			bModbusSend[69]=(INT8U)(wTransmitData);
	
	
		wTransmitData=0;					// 172
			bModbusSend[70]=(INT8U)(cbSTM3COMFirmware[0]);
			bModbusSend[71]=(INT8U)(cbSTM3COMFirmware[1]);
		wTransmitData=0;					// 173
			bModbusSend[72]=(INT8U)cbSTM3COMFirmware[2];
			bModbusSend[73]=(INT8U)cbSTM3COMFirmware[3];
		wTransmitData=0;					// 174
			bModbusSend[74]=(INT8U)cbSTM3COMFirmware[4];
			bModbusSend[75]=(INT8U)cbSTM3COMFirmware[5];
		wTransmitData=0;					// 175
			bModbusSend[76]=(INT8U)cbSTM3COMFirmware[6];
			bModbusSend[77]=(INT8U)cbSTM3COMFirmware[7];
				wTransmitData=0;			// 176
			bModbusSend[78]=(INT8U)cbSTM3COMFirmware[8];
			bModbusSend[79]=(INT8U)cbSTM3COMFirmware[9];
		wTransmitData=0;					// 177
			bModbusSend[80]=(INT8U)ubDSPControlFirmware[0];
			bModbusSend[81]=(INT8U)ubDSPControlFirmware[1];
		wTransmitData=0;					// 178
			bModbusSend[82]=(INT8U)ubDSPControlFirmware[2];
			bModbusSend[83]=(INT8U)ubDSPControlFirmware[3];
		wTransmitData=0;					// 179
			bModbusSend[84]=(INT8U)ubDSPControlFirmware[4];
			bModbusSend[85]=(INT8U)ubDSPControlFirmware[5];
		wTransmitData=0;					// 180
			bModbusSend[86]=(INT8U)ubDSPControlFirmware[6];
			bModbusSend[87]=(INT8U)ubDSPControlFirmware[7];
		wTransmitData=0;					// 181
			bModbusSend[88]=(INT8U)ubDSPControlFirmware[8];
			bModbusSend[89]=(INT8U)ubDSPControlFirmware[9];
			
		for((i=wStartAddress-270),j=3;i<(wStartAddress+wNumberOfReg-270);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else  if((wStartAddress>>1)<225)
	{
		for(i=0;i<90;i++)
		{
			bModbusSend[i]=0;
		}

		wTransmitData=wStartDelayTime;					// 182
			bModbusSend[0]=(INT8U)(wTransmitData>>8);
			bModbusSend[1]=(INT8U)(wTransmitData);
		wTransmitData=0;					// 183
			bModbusSend[2]=(INT8U)(0);
			bModbusSend[3]=(INT8U)(bdisplayFlag);
		wTransmitData=0;					//184
			bModbusSend[4] = (INT8U)(0);
			bModbusSend[5] = (INT8U)(ubSelectFrequency5060HzFag);
			
		wTransmitData=0;					//185
			bModbusSend[6] = (INT8U)(0);
			bModbusSend[7] = (INT8U)(0);
		wTransmitData=0;					//186
			bModbusSend[8] = (INT8U)(0);
			bModbusSend[9] = (INT8U)(0);
		wTransmitData=0;					//187
			bModbusSend[10] = (INT8U)(0);
			bModbusSend[11] = (INT8U)(0);
		wTransmitData=0;					//188
			bModbusSend[12] = (INT8U)(0);
			bModbusSend[13] = (INT8U)(0);
		wTransmitData=0;					//189
			bModbusSend[14] = (INT8U)(0);
			bModbusSend[15] = (INT8U)(0);
		wTransmitData=0;					//190
			bModbusSend[16] = (INT8U)(0);
			bModbusSend[17] = (INT8U)(0);
		wTransmitData=0;					//191
			bModbusSend[18] = (INT8U)(0);
			bModbusSend[19] = (INT8U)(0);
		wTransmitData=0;					//192
			bModbusSend[20] = (INT8U)(0);
			bModbusSend[21] = (INT8U)(0);
		wTransmitData=0;					//193
			bModbusSend[22] = (INT8U)(0);
			bModbusSend[23] = (INT8U)(0);		
		wTransmitData=EEData1.wTxUxMxSxHighEE; //194	  model high	
			bModbusSend[24]=(INT8U)(wTransmitData>>8);
			bModbusSend[25]=(INT8U)(wTransmitData);
		wTransmitData=EEData1.wTxUxMxSxEE; 	    //195   model low
			bModbusSend[26]=(INT8U)(wTransmitData>>8);
			bModbusSend[27]=(INT8U)(wTransmitData);
		wTransmitData = EEData1.fEepromSetting.bOverLoadPercent; //196  over laod Enable zh 20151109;
		 	bModbusSend[28]=(INT8U)(wTransmitData>>8);
			bModbusSend[29]=(INT8U)(wTransmitData);  
			
		for((i=wStartAddress-360),j=3;i<(wStartAddress+wNumberOfReg-360);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
		
	}
	wCRC1Temp = usMBCRC16(bExterTxBuffer,(wNumberOfReg+3));
	bExterTxBuffer[j]= (INT8U)(wCRC1Temp);
	bExterTxBuffer[j+1]= (INT8U)(wCRC1Temp>>8);
}
INT8U  bInputReserveFlag;///for reserve regsiter20150610
extern INT32U dwInputWatt;
extern INT16U wPV_1_Volt;
extern INT16U wPV_2_Volt;
extern INT16U wPV_1_Curr;
extern INT16U wPV_2_Curr;
extern INT32U dwPV_1_Watt;
extern INT32U dwPV_2_Watt;
extern INT32U dwOP_RST_WattSec;
extern INT16U wOPFrecyReal;
extern INT16U wOPPhase_R_Volt;
extern INT16U wOPPhase_S_Volt;
extern INT16U wOPPhase_T_Volt;
extern INT16U wOPPhase_R_Curr;
extern INT16U wOPPhase_S_Curr;
extern INT16U wOPPhase_T_Curr;
extern INT32U dwOP_R_WattSec;
extern INT32U dwOP_S_WattSec;
extern INT32U dwOP_T_WattSec;
extern INT16U wNTC_Tempr;
extern INT16U wINV_Tempr;
extern INT16U wFaultValuePVV;
extern INT16U wFaultValuePVISO;
extern INT16U wFaultValueGFCI;
extern INT16U wFaultValueDCI;
extern INT16U wFaultValueNTCT;
extern INT16U wFaultValueOPV;
extern INT16U wFaultValueOPF;
extern INT16U wBus_P_Volt;
extern INT16U wBus_N_Volt;
extern INT8U bConstantPower;
extern INT32U dwOP_ReActPowerSec;
extern INT8U bInvWarningBit;
extern INT16U wWarningValue;
extern INT16U wLoadPer100;
extern INT32U dwOPFullLoad;
extern INT16U uwScope[45];
/**
@*     MODBUS 4
@*     Read input register
**/

extern INT8U	bEepromNeedWrite;

void sReadInputRegister(INT8UP bRXDataBuff)
{
	
	INT16U wStartAddress,wNumberOfReg,wCRC1Temp;
	INT16U i,j;
	INT16U wTransmitData;
	INT8U  bFault=0;
	INT8U  bBlockNum;
      wStartAddress = *(bRXDataBuff+2);
       wStartAddress = (wStartAddress<<8)|*(bRXDataBuff+3);
       wNumberOfReg  = *(bRXDataBuff+4);
       wNumberOfReg  = (wNumberOfReg<<8)|*(bRXDataBuff+5);

	 
	 if(wStartAddress < 2)
	 {	
	 	bInputReserveFlag=1;
	 }
	 else
	 {
	 	bInputReserveFlag=0;
		wStartAddress -= 2;
	 }//for resever 2 reg 20150610
	   bBlockNum=wStartAddress/45;
 	if(((wStartAddress+wNumberOfReg)>(bBlockNum+1)*45)||(wStartAddress>=630)||(wNumberOfReg>45)\
	   	|| (wNumberOfReg==0))
	{
		bFault=1;
	}

	if((bFault==1)||(bInputReserveFlag==1))
	{
		bExterTxBuffer[1]=(cModBus_4|0x80);
		bExterTxBuffer[2]=1;
		wCRC1Temp = usMBCRC16(bExterTxBuffer,3);
		bExterTxBuffer[3]= (INT8U)(wCRC1Temp);
		bExterTxBuffer[4]= (INT8U)(wCRC1Temp>>8);
		bModbusERROR=1;
		return;
	}
	wStartAddress = wStartAddress*2;
	wNumberOfReg  = wNumberOfReg*2;
	
	bExterTxBuffer[2]=wNumberOfReg;
	
	if((wStartAddress>>1)<45)
	{
		wTransmitData=(INT16U)(bInverterStatus);			/* 02 Inverter Status */
			bModbusSend[0]=(INT8U)(wTransmitData>>8);
			bModbusSend[1]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwInputWatt>>16);			/* 03 Input power (high) */
			bModbusSend[2]=(INT8U)(wTransmitData>>8);
			bModbusSend[3]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwInputWatt);				/* 04 Input power (low) */
			bModbusSend[4]=(INT8U)(wTransmitData>>8);
			bModbusSend[5]=(INT8U)(wTransmitData);
		wTransmitData=wPV_1_Volt;							/* 05 PV1 voltage */
			bModbusSend[6]=(INT8U)(wTransmitData>>8);
			bModbusSend[7]=(INT8U)(wTransmitData);
		wTransmitData=wPV_1_Curr;							/* 06 PV1Curr */
			bModbusSend[8]=(INT8U)(wTransmitData>>8);
			bModbusSend[9]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwPV_1_Watt>>16);			/* 07 PV1 input watt (high)*/
			bModbusSend[10]=(INT8U)(wTransmitData>>8);
			bModbusSend[11]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwPV_1_Watt);				/* 08 PV1 input watt (low)*/
			bModbusSend[12]=(INT8U)(wTransmitData>>8);
			bModbusSend[13]=(INT8U)(wTransmitData);
		wTransmitData=wPV_2_Volt;							/* 09 PV2 voltage */
			bModbusSend[14]=(INT8U)(wTransmitData>>8);
			bModbusSend[15]=(INT8U)(wTransmitData);
		wTransmitData=wPV_2_Curr;							/* 10 PV2Curr  */
			bModbusSend[16]=(INT8U)(wTransmitData>>8);
			bModbusSend[17]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwPV_2_Watt>>16);			/* 11 PV2 input watt (high) */
			bModbusSend[18]=(INT8U)(wTransmitData>>8);
			bModbusSend[19]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwPV_2_Watt);				/* 12 PV2 input watt (low) */
			bModbusSend[20]=(INT8U)(wTransmitData>>8);
			bModbusSend[21]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_RST_WattSec>>16);		/* 13 Output power (high) */
			bModbusSend[22]=(INT8U)(wTransmitData>>8);
			bModbusSend[23]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_RST_WattSec);			/* 14 Output power (low) */
			bModbusSend[24]=(INT8U)(wTransmitData>>8);
			bModbusSend[25]=(INT8U)(wTransmitData);
		wTransmitData=wOPFrecyReal;							/* 15 Grid frequency */
			bModbusSend[26]=(INT8U)(wTransmitData>>8);
			bModbusSend[27]=(INT8U)(wTransmitData);
		wTransmitData=wOPPhase_R_Volt;						/* 16 Vac1 */
			bModbusSend[28]=(INT8U)(wTransmitData>>8);
			bModbusSend[29]=(INT8U)(wTransmitData);
		wTransmitData=wOPPhase_R_Curr;						/* 17 Iac1 */
			bModbusSend[30]=(INT8U)(wTransmitData>>8);
			bModbusSend[31]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_R_WattSec>>16);			/* 18 Pac1 H */
			bModbusSend[32]=(INT8U)(wTransmitData>>8);
			bModbusSend[33]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_R_WattSec);				/* 19 Pac1 L */
			bModbusSend[34]=(INT8U)(wTransmitData>>8);
			bModbusSend[35]=(INT8U)(wTransmitData);
		wTransmitData=wOPPhase_S_Volt;						/* 20 Vac2 */
			bModbusSend[36]=(INT8U)(wTransmitData>>8);
			bModbusSend[37]=(INT8U)(wTransmitData);
		wTransmitData=wOPPhase_S_Curr;						/* 21 Iac2 */
			bModbusSend[38]=(INT8U)(wTransmitData>>8);
			bModbusSend[39]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_S_WattSec>>16);			/* 22 Pac2 H  */
			bModbusSend[40]=(INT8U)(wTransmitData>>8);
			bModbusSend[41]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_S_WattSec);				/* 23 Pac2 L */
			bModbusSend[42]=(INT8U)(wTransmitData>>8);
			bModbusSend[43]=(INT8U)(wTransmitData);
		wTransmitData=wOPPhase_T_Volt;						/* 24 Vac3 */
			bModbusSend[44]=(INT8U)(wTransmitData>>8);
			bModbusSend[45]=(INT8U)(wTransmitData);
		wTransmitData=wOPPhase_T_Curr;						/* 25 Iac3 */
			bModbusSend[46]=(INT8U)(wTransmitData>>8);
			bModbusSend[47]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_T_WattSec>>16);			/* 26 Pac3 H */
			bModbusSend[48]=(INT8U)(wTransmitData>>8);
			bModbusSend[49]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(dwOP_T_WattSec);				/* 27 Pac3 L */
			bModbusSend[50]=(INT8U)(wTransmitData>>8);
			bModbusSend[51]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(EEData32.wEtoday0k1wEE>>16);			/* 28 Energy today H */
			bModbusSend[52]=(INT8U)(wTransmitData>>8);
			bModbusSend[53]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(EEData32.wEtoday0k1wEE);				/* 29 Energy today L */
			bModbusSend[54]=(INT8U)(wTransmitData>>8);
			bModbusSend[55]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(EEData32.dwInvertedP0K1WEE>>16);		/* 30 Energy total H */
			bModbusSend[56]=(INT8U)(wTransmitData>>8);
			bModbusSend[57]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(EEData32.dwInvertedP0K1WEE);			/* 31 Energy total L */
			bModbusSend[58]=(INT8U)(wTransmitData>>8);
			bModbusSend[59]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(EEData32.dwInvTime0p5SecEE >>16);					/* 32 Time total H */
			bModbusSend[60]=(INT8U)(wTransmitData>>8);
			bModbusSend[61]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(EEData32.dwInvTime0p5SecEE );						/* 33 Time total L */
			bModbusSend[62]=(INT8U)(wTransmitData>>8);
			bModbusSend[63]=(INT8U)(wTransmitData);
		wTransmitData=wNTC_Tempr; 							/* 34 Temperature */
			bModbusSend[64]=(INT8U)(wTransmitData>>8);
			bModbusSend[65]=(INT8U)(wTransmitData);
		wTransmitData=wFaultValuePVISO;						/* 35 ISO fault Value */
			bModbusSend[66]=(INT8U)(wTransmitData>>8);
			bModbusSend[67]=(INT8U)(wTransmitData);
		wTransmitData=wFaultValueGFCI; 						/* 36 GFCI fault Value */
			bModbusSend[68]=(INT8U)(wTransmitData>>8);
			bModbusSend[69]=(INT8U)(wTransmitData);
		wTransmitData=wFaultValueDCI;						/* 37 DCI fault Value */
			bModbusSend[70]=(INT8U)(wTransmitData>>8);
			bModbusSend[71]=(INT8U)(wTransmitData);
		wTransmitData=wFaultValuePVV; 						/* 38 PV voltage fault value */
			bModbusSend[72]=(INT8U)(wTransmitData>>8);
			bModbusSend[73]=(INT8U)(wTransmitData);
		wTransmitData=wFaultValueOPV; 						/* 39 AC voltage fault value */
			bModbusSend[74]=(INT8U)(wTransmitData>>8);
			bModbusSend[75]=(INT8U)(wTransmitData);
		wTransmitData=wFaultValueOPF;						/* 40 AC frequency fault value */
			bModbusSend[76]=(INT8U)(wTransmitData>>8);
			bModbusSend[77]=(INT8U)(wTransmitData);
		wTransmitData=wFaultValueNTCT; 						/*41 Temperature fault value */
			bModbusSend[78]=(INT8U)(wTransmitData>>8);
			bModbusSend[79]=(INT8U)(wTransmitData);
		wTransmitData=(INT16U)(bInvFaultBit); 				/* 42 Inverter fault bit */
			bModbusSend[80]=(INT8U)(wTransmitData>>8);
			bModbusSend[81]=(INT8U)(wTransmitData);
		wTransmitData=wINV_Tempr;							/* 43 IPM Temperature */
			bModbusSend[82]=(INT8U)(wTransmitData>>8);
			bModbusSend[83]=(INT8U)(wTransmitData);
		wTransmitData=wBus_P_Volt; 							/* 44 P Bus Voltage */
			bModbusSend[84]=(INT8U)(wTransmitData>>8);
			bModbusSend[85]=(INT8U)(wTransmitData);
		wTransmitData=wBus_N_Volt;							/* 45 N Bus Voltage */
			bModbusSend[86]=(INT8U)(wTransmitData>>8);
			bModbusSend[87]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)bConstantPower;						// 46 CheckStep
			bModbusSend[88]=(INT8U)(wTransmitData>>8);
			bModbusSend[89]=(INT8U)(wTransmitData);
			
		for(i=wStartAddress,j=3;i<(wStartAddress+wNumberOfReg);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress>>1)<90)					/* Paragraph 2 : 47~91 */
	{
		wTransmitData = wPFNumOrder;				// 47 IPF
			bModbusSend[0]=(INT8U)(wTransmitData>>8);
			bModbusSend[1]=(INT8U)(wTransmitData);
		wTransmitData = 0;                            //48  
			bModbusSend[2]=(INT8U)(wTransmitData>>8);
			bModbusSend[3]=(INT8U)(wTransmitData);
		wTransmitData = 0;//49
			bModbusSend[4]=(INT8U)(wTransmitData>>8);
			bModbusSend[5]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_1_TodayEE>>16);	// 50 Epv1_today H
			bModbusSend[6]=(INT8U)(wTransmitData>>8);
			bModbusSend[7]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_1_TodayEE);		// 51 Epv1_today L
			bModbusSend[8]=(INT8U)(wTransmitData>>8);
			bModbusSend[9]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_1_TotalEE>>16);	// 52 Epv1_total H
			bModbusSend[10]=(INT8U)(wTransmitData>>8);
			bModbusSend[11]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_1_TotalEE);		// 53 Epv1_total L
			bModbusSend[12]=(INT8U)(wTransmitData>>8);
			bModbusSend[13]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_2_TodayEE>>16);    //54 Epv2_today H
			bModbusSend[14]=(INT8U)(wTransmitData>>8);
			bModbusSend[15]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_2_TodayEE);          // 55 Epv2_today L
			bModbusSend[16]=(INT8U)(wTransmitData>>8);
			bModbusSend[17]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_2_TotalEE>>16);	//56 Epv2_total H
			bModbusSend[18]=(INT8U)(wTransmitData>>8);
			bModbusSend[19]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPV_2_TotalEE);		//57 Epv2_total L
			bModbusSend[20]=(INT8U)(wTransmitData>>8);
			bModbusSend[21]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)((EEData32.dwPV_2_TotalEE+EEData32.dwPV_1_TotalEE)>>16);	//58 Epv_total H
			bModbusSend[22]=(INT8U)(wTransmitData>>8);
			bModbusSend[23]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)((EEData32.dwPV_2_TotalEE+EEData32.dwPV_1_TotalEE));	//59 Epv_total L
			bModbusSend[24]=(INT8U)(wTransmitData>>8);
			bModbusSend[25]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(dwOP_ReActPowerSec>>16);    //60 Rac H
			bModbusSend[26]=(INT8U)(wTransmitData>>8);
			bModbusSend[27]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(dwOP_ReActPowerSec);          // 61 Rac  L
			bModbusSend[28]=(INT8U)(wTransmitData>>8);
			bModbusSend[29]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPVQ_TodayEE>>16);	//62 E_rac_today H
			bModbusSend[30]=(INT8U)(wTransmitData>>8);
			bModbusSend[31]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPVQ_TodayEE);		//63 E_rac_today L
			bModbusSend[32]=(INT8U)(wTransmitData>>8);
			bModbusSend[33]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPVQ_TotalEE>>16);	//64 E_rac_total H
			bModbusSend[34]=(INT8U)(wTransmitData>>8);
			bModbusSend[35]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(EEData32.dwPVQ_TotalEE);		//65 E_rac_ total L
			bModbusSend[36]=(INT8U)(wTransmitData>>8);
			bModbusSend[37]=(INT8U)(wTransmitData);
		if(bInvWarningBit==0)
			wTransmitData=0;
		else
			wTransmitData = (INT16U)(bInvWarningBit+9);				//66  Warning code //for add 9 20150525
			bModbusSend[38]=(INT8U)(wTransmitData>>8);
			bModbusSend[39]=(INT8U)(wTransmitData);

		wTransmitData = wWarningValue; 							//67 
			bModbusSend[40]=(INT8U)(wTransmitData>>8);
			bModbusSend[41]=(INT8U)(wTransmitData);
		wTransmitData = wLoadPer100;							//68 Load percent
			bModbusSend[42]=(INT8U)(wTransmitData>>8);
			bModbusSend[43]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)(dwOPFullLoad>>16);						//69 op load limit
			bModbusSend[44]=(INT8U)(wTransmitData>>8);
			bModbusSend[45]=(INT8U)(wTransmitData);
		wTransmitData = (INT16U)dwOPFullLoad;					//70 dwOPFullLoad low
			bModbusSend[46]=(INT8U)(wTransmitData>>8);
			bModbusSend[47]=(INT8U)(wTransmitData);
		
		for(i = 48; i < 90; i++)
			bModbusSend[i] = 0;	
		for((i=wStartAddress-90),j=3;i<(wStartAddress+wNumberOfReg-90);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress>>1)<135)					/* Paragraph 3 : 92~136 */
	{
		for(i=0;i<5;i++)
		{
			wTransmitData=(INT16U)bErrorMsg[0+cErrorMsgLength*i];			/* 92 Grid Fault record 1 - code */
			bModbusSend[0+i*10]=(INT8U)(wTransmitData>>8);
			bModbusSend[1+i*10]=(INT8U)(wTransmitData);
			bModbusSend[2+i*10]=bErrorMsg[2+cErrorMsgLength*i];
			bModbusSend[3+i*10]=bErrorMsg[3+cErrorMsgLength*i];
			bModbusSend[4+i*10]=bErrorMsg[4+cErrorMsgLength*i];
			bModbusSend[5+i*10]=bErrorMsg[5+cErrorMsgLength*i];	
			bModbusSend[6+i*10]=bErrorMsg[6+cErrorMsgLength*i];
			bModbusSend[7+i*10]=bErrorMsg[7+cErrorMsgLength*i];
			bModbusSend[8+i*10]=bErrorMsg[18+cErrorMsgLength*i];
			bModbusSend[9+i*10]=bErrorMsg[19+cErrorMsgLength*i];
		}
		for(i=50;i<90;i++)							/* 117~136 = 0 */
			bModbusSend[i]=0;
		
		for((i=wStartAddress-180),j=3;i<(wStartAddress+wNumberOfReg-180);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress>>1)<180)					/* Paragraph 4 : 137~181 */
	{
		for(i=0;i<90;i++)					
			bModbusSend[i]=0;
	
		for((i=wStartAddress-270),j=3;i<(wStartAddress+wNumberOfReg-270);i++,j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 225)		// Paragraph 4 : 182~226  /Inverter Error record 1~9
	{
		for(i = 0; i <= 40; i += 5)
		{
			bModbusSend[i * 2] = 0;
			bModbusSend[i * 2 + 1] = bErrorRecord[i + 0];					  //	Inverter Error record - code
			bModbusSend[i * 2 + 2] = 0;									  //Inverter Error record - year
			bModbusSend[i * 2 + 3] = bErrorRecord[i + 1];					  //Inverter Error record - month
			bModbusSend[i * 2 + 4] = bErrorRecord[i + 2];					  //Inverter Error record - day
			bModbusSend[i * 2 + 5] = bErrorRecord[i + 3];					  //Inverter Error record - hour
			bModbusSend[i * 2 + 6] = bErrorRecord[i + 4];					  //Inverter Error record - min
			bModbusSend[i * 2 + 7] = bModbusSend[i * 2 + 8] = bModbusSend[i * 2 + 9] = 0; //Inverter Error record - sec & value
		}
		for((i = wStartAddress - 360), j = 3; i < (wStartAddress + wNumberOfReg - 360); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 270)									// Inverter Error record 10~18
	{
		for(i = 0; i <= 40; i += 5)
		{
			bModbusSend[i * 2] = 0;
			bModbusSend[i * 2 + 1] = bErrorRecord[i + 45];					  //	Inverter Error record - code
			bModbusSend[i * 2 + 2] = 0;									  //Inverter Error record - year
			bModbusSend[i * 2 + 3] = bErrorRecord[i + 46];					  //Inverter Error record - month
			bModbusSend[i * 2 + 4] = bErrorRecord[i + 47];					  //Inverter Error record - day
			bModbusSend[i * 2 + 5] = bErrorRecord[i + 48];					  //Inverter Error record - hour
			bModbusSend[i * 2 + 6] = bErrorRecord[i + 49];					  //Inverter Error record - min
			bModbusSend[i * 2 + 7] = bModbusSend[i * 2 + 8] = bModbusSend[i * 2 + 9] = 0; //Inverter Error record - sec & value
		}
		for((i = wStartAddress - 450), j = 3; i < (wStartAddress + wNumberOfReg - 450); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 315)								// Inverter Error record 19~27
	{
		for(i = 0; i <= 40; i += 5)
		{
			bModbusSend[i * 2] = 0;
			bModbusSend[i * 2 + 1] = bErrorRecord[i + 90];					  //	Inverter Error record - code
			bModbusSend[i * 2 + 2] = 0;									  //Inverter Error record - year
			bModbusSend[i * 2 + 3] = bErrorRecord[i + 91];					  //Inverter Error record - month
			bModbusSend[i * 2 + 4] = bErrorRecord[i + 92];					  //Inverter Error record - day
			bModbusSend[i * 2 + 5] = bErrorRecord[i + 93];					  //Inverter Error record - hour
			bModbusSend[i * 2 + 6] = bErrorRecord[i + 94];					  //Inverter Error record - min
			bModbusSend[i * 2 + 7] = bModbusSend[i * 2 + 8] = bModbusSend[i * 2 + 9] = 0; //Inverter Error record - sec & value
		}
		for((i = wStartAddress - 540), j = 3; i < (wStartAddress + wNumberOfReg - 540); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 360)							// Inverter Error record 28~36
	{
		for(i = 0; i <= 40; i += 5)
		{
			bModbusSend[i * 2] = 0;
			bModbusSend[i * 2 + 1] = bErrorRecord[i + 135];					  //	Inverter Error record - code
			bModbusSend[i * 2 + 2] = 0;									  //Inverter Error record - year
			bModbusSend[i * 2 + 3] = bErrorRecord[i + 136];					  //Inverter Error record - month
			bModbusSend[i * 2 + 4] = bErrorRecord[i + 137];					  //Inverter Error record - day
			bModbusSend[i * 2 + 5] = bErrorRecord[i + 138];					  //Inverter Error record - hour
			bModbusSend[i * 2 + 6] = bErrorRecord[i + 139];					  //Inverter Error record - min
			bModbusSend[i * 2 + 7] = bModbusSend[i * 2 + 8] = bModbusSend[i * 2 + 9] = 0; //Inverter Error record - sec & value
		}
		for((i = wStartAddress - 630), j = 3; i < (wStartAddress + wNumberOfReg - 630); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 405)						// Inverter Error record 36~45
	{
		for(i = 0; i <= 40; i += 5)
		{
			bModbusSend[i * 2] = 0;
			bModbusSend[i * 2 + 1] = bErrorRecord[i + 180];					  //	Inverter Error record - code
			bModbusSend[i * 2 + 2] = 0;									  //Inverter Error record - year
			bModbusSend[i * 2 + 3] = bErrorRecord[i + 181];					  //Inverter Error record - month
			bModbusSend[i * 2 + 4] = bErrorRecord[i + 182];					  //Inverter Error record - day
			bModbusSend[i * 2 + 5] = bErrorRecord[i + 183];					  //Inverter Error record - hour
			bModbusSend[i * 2 + 6] = bErrorRecord[i + 184];					  //Inverter Error record - min
			bModbusSend[i * 2 + 7] = bModbusSend[i * 2 + 8] = bModbusSend[i * 2 + 9] = 0; //Inverter Error record - sec & value
		}
		for((i = wStartAddress - 720), j = 3; i < (wStartAddress + wNumberOfReg - 720); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 450)					// Inverter Error record 45~50
	{
		for(i = 0; i <= 20; i += 5)
		{
			bModbusSend[i * 2] = 0;
			bModbusSend[i * 2 + 1] = bErrorRecord[i + 225];					  //	Inverter Error record - code
			bModbusSend[i * 2 + 2] = 0;									  //Inverter Error record - year
			bModbusSend[i * 2 + 3] = bErrorRecord[i + 226];					  //Inverter Error record - month
			bModbusSend[i * 2 + 4] = bErrorRecord[i + 227];					  //Inverter Error record - day
			bModbusSend[i * 2 + 5] = bErrorRecord[i + 228];					  //Inverter Error record - hour
			bModbusSend[i * 2 + 6] = bErrorRecord[i + 229];					  //Inverter Error record - min
			bModbusSend[i * 2 + 7] = bModbusSend[i * 2 + 8] = bModbusSend[i * 2 + 9] = 0; //Inverter Error record - sec & value
		}
		for(i = 50;i < 90; i++)
		{
			bModbusSend[i] = 0;
		}
		for((i = wStartAddress - 810), j = 3; i < (wStartAddress + wNumberOfReg - 810); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress >> 1) < 495)					// Electcicy
	{
		for(i = 0; i <= 15; i ++)		//Power Hour		//0--15
		{
			bModbusSend[i * 4] = 0;
			bModbusSend[i * 4 + 1] = 0;					  
			bModbusSend[i * 4 + 2] = (EEData32.wPowerEachHour0K1WH[i]>>8);	
			bModbusSend[i * 4 + 3] = (EEData32.wPowerEachHour0K1WH[i]&0xFF);	
		}

		for(i = 64; i <90; i ++)
		{
			bModbusSend[i] = 0;

		}

		for((i = wStartAddress - 900), j = 3; i < (wStartAddress + wNumberOfReg - 900); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	//ZXF 2015.09.16
	else if((wStartAddress >> 1) < 540)					// Electcicy
	{
		for(i=0;i<6;i++)		
		{
			bModbusSend[i] = 0;
		}
		//Power day 500-513
		for(i = 0; i <7; i ++)		
		{
			bModbusSend[6+i * 4] = 0;
			bModbusSend[6+i * 4 + 1] = 0;					  
			bModbusSend[6+i * 4 + 2] = (EEData32.wPowerWeekDay[i]>>8);	
			bModbusSend[6+i * 4 + 3] = (EEData32.wPowerWeekDay[i]&0xFF);	
		}

		//Power month 514-537
		for(i = 0; i <12; i ++)		
		{
			bModbusSend[34+i * 4] = 0;
			bModbusSend[34+i * 4 + 1] = 0;					  
			bModbusSend[34+i * 4 + 2] = (EEData32.wPowerMonth[i]>>8);	
			bModbusSend[34+i * 4 + 3] = (EEData32.wPowerMonth[i]&0xFF);	
		}
		// power year 538-541
		for(i = 0; i <2; i ++)		
		{
			bModbusSend[82+i * 4] = ((EEData32.dwPoweYear0K1WH[i]&0xFF000000)>>24);
			bModbusSend[82+i * 4 + 1] = ((EEData32.dwPoweYear0K1WH[i]&0x00FF0000)>>16);				  
			bModbusSend[82+i * 4 + 2] = ((EEData32.dwPoweYear0K1WH[i]&0x0000FF00)>>8);	
			bModbusSend[82+i * 4 + 3] = (EEData32.dwPoweYear0K1WH[i]&0xFF);	
		}
		

		for((i = wStartAddress - 990), j = 3; i < (wStartAddress + wNumberOfReg - 990); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	//ZXF 2015.09.16
	else if((wStartAddress >> 1) < 585)	
	{	
		for(i=2;i<20;i++)
		{
			bModbusSend[(i-2) * 4] = ((EEData32.dwPoweYear0K1WH[i]&0xFF000000)>>24);
			bModbusSend[(i-2) * 4 + 1] = ((EEData32.dwPoweYear0K1WH[i]&0x00FF0000)>>16);				  
			bModbusSend[(i-2) * 4 + 2] = ((EEData32.dwPoweYear0K1WH[i]&0x0000FF00)>>8);	
			bModbusSend[(i-2) * 4 + 3] = (EEData32.dwPoweYear0K1WH[i]&0xFF);	
		}
		for(i = 72; i <90; i ++)
		{
			bModbusSend[i] = 0;
		}
		for((i = wStartAddress - 1080), j = 3; i < (wStartAddress + wNumberOfReg - 1080); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	else if((wStartAddress>>1)<630)
	{
		for(i=0;i<45;i++)
		{
			bModbusSend[i*2] = (INT8U)(uwScope[i]>>8);
			bModbusSend[i*2+1] = (INT8U)(uwScope[i]);
		}
		for((i = wStartAddress - 1170), j = 3; i < (wStartAddress + wNumberOfReg - 1170); i++, j++)
		{
			bExterTxBuffer[j] = bModbusSend[i];
		}
	}
	wCRC1Temp = usMBCRC16(bExterTxBuffer,(wNumberOfReg+3));
	bExterTxBuffer[j]= (INT8U)(wCRC1Temp);
	bExterTxBuffer[j+1]= (INT8U)(wCRC1Temp>>8);
}

INT8U	bEepromNeedRead=0;
INT8U b3062_FashFlag;
INT8U 	bSpecPasswordType=0; 
extern INT8U bInterComTxEnd;

#define	mSettingBlock		0x01
#define	cPVTurnOnDefaultVolt		3500
#define	cPVTurnOnLow			2000
#define	cPVTurnOnHigh			8000
INT8U bSetDateTimeFlag;
INT8U bSetDateTimeCnt;
INT8U bDaysThisMonth;
INT8U bCountryCnt;
INT8U bCountryFlag;
INT8U   bTurnOffSPSCnt=0;
INT8U 	bPasswordUnLock=0;
void sPresetSingleRegister(INT8UP bRXDataBuff)

{
	INT32U dwRealValue;    // adj use
	INT16U wRegisterAddr;
	INT16U wSetValue;
	INT16U wCRCTemp;
	INT16U wAdjustTemp;
	INT8U  bSendErrMsg=0;
	INT8U  i;
	static INT8U	wSetFlag;
	static INT16U	wSetValueTemp;

	if(*bRXDataBuff== 0)
	{
	//	if(bMaster!=cMaster)
		{	
			bExComNoRespon=1;
		}
	}
	else if(*bRXDataBuff!=EEData1.bCOMAddr)
	{
		bExComNoRespon=1;
		return;
	}

	if(bEepromNeedRead!=0)	//need to read eeprom, do not setting and write eeprom
	{
		//bSendErrMsg=1;
		bExterTxBuffer[1]= (INT8U)(0x80|cModBus_6);
		bExterTxBuffer[2]= 1;
		wCRCTemp = usMBCRC16(bExterTxBuffer,3);
		bExterTxBuffer[3]= (INT8U)(wCRCTemp);
		bExterTxBuffer[4]= (INT8U)(wCRCTemp>>8);
		bModbusERROR=1;
		return;
	}
	wRegisterAddr = *(bRXDataBuff+2);
	wRegisterAddr = (wRegisterAddr<<8)|*(bRXDataBuff+3);
	wSetValue = *(bRXDataBuff+4);
	wSetValue = (wSetValue<<8)|*(bRXDataBuff+5);

	bExterTxBuffer[0] = *(bRXDataBuff+0);
	bExterTxBuffer[1] = *(bRXDataBuff+1);
	
	bExterTxBuffer[2] = *(bRXDataBuff+2);
	bExterTxBuffer[3] = *(bRXDataBuff+3);
	if(wRegisterAddr>=2)
	{
		wRegisterAddr -=2;
	}
	else if((wRegisterAddr ==0) || (wRegisterAddr==1))
	{
		//bSendErrMsg=1;
		bExterTxBuffer[1]= (INT8U)(0x80|cModBus_6);
		bExterTxBuffer[2]= 1;
		wCRCTemp = usMBCRC16(bExterTxBuffer,3);
		bExterTxBuffer[3]= (INT8U)(wCRCTemp);
		bExterTxBuffer[4]= (INT8U)(wCRCTemp>>8);
		bModbusERROR=1;
		return;	
	}////for resever 2 reg 20150610  
	
	switch(wRegisterAddr)
	{
		case 0:
			if((wSetValue&0x00FF)==0) // turn off
			{
				bDistanceOnOffOrder = cTurnOff;
			}
			else if((wSetValue&0x00FF)==0x0001)
			{
				bDistanceOnOffOrder = cTurnOn;
			}
			else
				bSendErrMsg=1;
			
		/*	if((wSetValue&0xFF00)==0)             		
			{
				EEData1.fEepromSetting.bAutoStartEE = 0;
				bEepromNeedWrite |= mSettingBlock;
			}
			else if((wSetValue&0xFF00)==0x0100)
			{
				EEData1.fEepromSetting.bAutoStartEE = 1;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;*/
			bExterTxBuffer[4] = EEData1.fEepromSetting.bAutoStartEE;
			bExterTxBuffer[5] = bDistanceOnOffOrder;
			break;
			
		case 1:
			if(wSetValue == 0)
			{
				EEData1.fEepromSetting.bSPIEnableEE = 0;	//disable
				bEepromNeedWrite |= mSettingBlock;
			}
			else if(wSetValue == 1)
			{
				EEData1.fEepromSetting.bSPIEnableEE = 1;	//enable
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.fEepromSetting.bSPIEnableEE;
			break;
			
		case 2:											/* PF CMD memory state */
			if(wSetValue==0x0001)
			{
				EEData1.bPFCmdMemoryEE=1;
				bEepromNeedWrite |= mSettingBlock;
			}
			else if(wSetValue==0)
			{
				EEData1.bPFCmdMemoryEE=0;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.bPFCmdMemoryEE;
			break;
			
		case 3:											/* Active P Rate */
			if((wSetValue<=100)||(wSetValue==0xFF))
			{
				
				
				EEData1.bActiveRateOrderEE = wSetValue;
				if(EEData1.bPFCmdMemoryEE==1)
				{					
					bEepromNeedWrite |= mSettingBlock;
				}
					
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.bActiveRateOrderEE;
			break;
			
		case 4:											/* Reactive P Rate */
			if(wSetValue<=65)	
			{
				EEData1.bReactiveRateOrderEE = wSetValue;
				if(EEData1.bPFCmdMemoryEE==1)
					bEepromNeedWrite |= mSettingBlock;
			}
			else if(wSetValue==0xFF)
			{
				EEData1.bReactiveRateOrderEE = wSetValue;
				if(EEData1.bPFCmdMemoryEE==1)
					bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.bReactiveRateOrderEE;
			break;
			
		case 5:											// Power factor 
			if((wSetValue<=20000 && wSetValue>=17500)|| \
				(wSetValue<=2500))			
			{
				EEData1.wCosPhiOrderEE = wSetValue;
				if(EEData1.bPFCmdMemoryEE==1)
					bEepromNeedWrite |= mSettingBlock;
			}
			else if(wSetValue==0xFFFF)
			{
				EEData1.wCosPhiOrderEE = wSetValue;
				if(EEData1.bPFCmdMemoryEE==1)
					bEepromNeedWrite |= mSettingBlock;
			}
			else 
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wCosPhiOrderEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wCosPhiOrderEE&0x00FF);
			break;
			
		case 6:
			if(wSetValue<=31)
			{
				bInvFaultBit = (INT8U)wSetValue;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = bInvFaultBit;
			break;
			
		case 15:										/* set LCD language */
			if(wSetValue<cLangNum)
			{	
				EEData1.bLanguage=(INT8U)wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.bLanguage;
			break;	
			
		case 17:										/* set Input start voltage */
			if(wSetValue>=cPVTurnOnLow && wSetValue<=cPVTurnOnHigh)
			{
				EEData1.wStartPVVoltageEE = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wStartPVVoltageEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wStartPVVoltageEE&0x00FF);
			break;
			
		case 18:										/* set Start time */
			if(wSetValue<=900)
			{
				EEData1.wStartDelayTimeEE=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wStartDelayTimeEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wStartDelayTimeEE&0x00FF);
			break;
			
		case 19:										/* Grid voltage low limit 1 */
			if(sChkLowVolt(wSetValue)==cTrue)
			{
			    EEData1.wGridVoltLow1EE=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wGridVoltLow1EE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wGridVoltLow1EE&0x00FF);
			break;
			
		case 20:										/* Grid voltage high limit 1 */
			if(sChkHighVolt(wSetValue)==cTrue)
			{
			    EEData1.wGridVoltHigh1EE=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wGridVoltHigh1EE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wGridVoltHigh1EE&0x00FF);
			break;
			
		case 21:
			if(sChkLowFreq(wSetValue)==cTrue)
			{
			    EEData1.wFreqLowEE=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wFreqLowEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wFreqLowEE&0x00FF);
			break;
			
		case 22:
			if(sChkHighFreq(wSetValue)==cTrue)
			{
			    EEData1.wFreqHighEE=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wFreqHighEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wFreqHighEE&0x00FF);
			break;
			
		case 23:									/* Serial number 1 */
			EEData1.bUserSerial[0]=(INT8U )((wSetValue&0xFF00)>>8);
			EEData1.bUserSerial[1]=(INT8U )(wSetValue&0x00FF);
			bExterTxBuffer[4] = EEData1.bUserSerial[0];
			bExterTxBuffer[5] = EEData1.bUserSerial[1];
			break;
			
		case 24:									/* Serial number 2 */
			EEData1.bUserSerial[2]=(INT8U )((wSetValue&0xFF00)>>8);
			EEData1.bUserSerial[3]=(INT8U )(wSetValue&0x00FF);
			bExterTxBuffer[4] = EEData1.bUserSerial[2];
			bExterTxBuffer[5] = EEData1.bUserSerial[3];
			break;
			
		case 25:									/* Serial number 3 */
			EEData1.bUserSerial[4]=(INT8U )((wSetValue&0xFF00)>>8);
			EEData1.bUserSerial[5]=(INT8U )(wSetValue&0x00FF);
			bExterTxBuffer[4] = EEData1.bUserSerial[4];
			bExterTxBuffer[5] = EEData1.bUserSerial[5];
			break;
			
		case 26:									/* Serial number 4*/
			EEData1.bUserSerial[6]=(INT8U )((wSetValue&0xFF00)>>8);
			EEData1.bUserSerial[7]=(INT8U )(wSetValue&0x00FF);
			bExterTxBuffer[4] = EEData1.bUserSerial[6];
			bExterTxBuffer[5] = EEData1.bUserSerial[7];
			break;
			
		case 27:									/* Serial number 5 */
			EEData1.bUserSerial[8]=(INT8U )((wSetValue&0xFF00)>>8);
			EEData1.bUserSerial[9]=(INT8U )(wSetValue&0x00FF);
			bExterTxBuffer[4] = EEData1.bUserSerial[8];
			bExterTxBuffer[5] = EEData1.bUserSerial[9];
			bEepromNeedWrite |= mSettingBlock;
			break;
	
		case 30:									/* Communicate  address */
			if((wSetValue!=0)&&(wSetValue!=255))
			{	
				EEData1.bCOMAddr=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.bCOMAddr;
			break;
		case 31:/* FlashStart */
			if(bFlashDisable==1)
			{
				bSendErrMsg=1;
			}
			else if((wSetValue==1)||(wSetValue==0x0101))
			{
				b3062_FashFlag=cFlash_STM32_Start;
			}
			else if(wSetValue==0x0100)
			{
				if(bInverterStatus!=cInvterterStatus)	 
				{
					bDSPFlashFlag=1;
					bInverterStatus = cFlashStatus;

				}
				else
				{ 
					bTurnOffCommand=cINV_ShutDown;	//else turn off inverter status first
					bDistanceOnOffOrder=cTurnOff;
					bInterComTxEnd = 0;
				}
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = wSetValue/256;
			bExterTxBuffer[5] = wSetValue%256;
			break;
		case 32:									/* Reset User Info */
			if(wSetValue==1)
			{
				EEData32.wEtoday0k1wEE=0;
				EEData32.dwInvertedP0K1WEE=0;
				EEData32.dwInvTime0p5SecEE=0;  
				
				EEData32.dwPV_1_TodayEE=0;
				EEData32.dwPV_1_TotalEE=0;
				EEData32.dwPV_2_TodayEE=0;
				EEData32.dwPV_2_TotalEE=0;
				EEData32.dwPVQ_TodayEE=0;
				EEData32.dwPVQ_TotalEE=0;
		
				for(i=0;i<cDayRecord;i++)
				{
					EEData32.wPowerWeekDay[i]=0;
				}
				for(i=0;i<cMonthRecord;i++)
				{
					EEData32.wPowerMonth[i]=0;
				}
				for(i=0;i<cHourRecord;i++)
				{
					EEData32.wPowerEachHour0K1WH[i]=0;
				}
				for(i=0;i<cYearRecord;i++)
				{
					EEData32.dwPoweYear0K1WH[i]=0;
				}
				for(i=0;i < cErrMesgLen;i++)   // clr Error message
					bErrorMsg[i]=0;
				for(i=0;i < cErrRecordLen;i++)
					bErrorRecord[i]=0;// clr VF Fault message
				
				bEepromNeedWrite|=(mEnegyBlock1|mErrorMsgBolck|mErrorRecord);						
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = 0;
			break;
			
		case 33:/* Reset to factory */
			if((wSetValue<=0x03)&&(wSetValue>0))
			{
				if((wSetValue&0x01)==0x01)
				{
					ResetSetValue();
					bEepromNeedWrite|=(mEnegyBlock1|mErrorMsgBolck|mErrorRecord|mSettingBlock);	 // write 1,2,4,5 zone
					//bTurnOffSPSCnt=1;
				}
				if((wSetValue&0x02)==0x02)
				{
				
					ResetADJvalue();
					bEepromNeedWrite|=mAdjustBlock;
				}
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = 0;
			break;
			
		case 35:									/* Grid voltage low limit protect 2 */
			if(sChkLowVolt(wSetValue)==cTrue)
			{
			    EEData1.wGridVoltLow2EE=wSetValue; // level 2
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wGridVoltLow2EE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wGridVoltLow2EE&0x00FF);
			break;
			
		case 36:									/* Grid voltage high limit protect 2 */
			if(sChkHighVolt(wSetValue)==cTrue)
			{
			    EEData1.wGridVoltHigh2EE=wSetValue; // level 2
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wGridVoltHigh2EE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wGridVoltHigh2EE&0x00FF);
			break;
			
		case 37:									/* Grid frequency low limit protect 2 */
			break;
		case 38:									/* Grid frequency high limit protect 2 */
			break;
			
		case 39:
			if(sChkLowVolt(wSetValue) == cTrue)
			{
				EEData1.wStartVLowEE = wSetValue;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wStartVLowEE >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wStartVLowEE & 0x00FF);
			bEepromNeedWrite |= mSettingBlock;
			break;
			
		case 40:
			if(sChkHighVolt(wSetValue) == cTrue)
			{
				EEData1.wStartVHighEE = wSetValue;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wStartVHighEE >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wStartVHighEE & 0x00FF);
			bEepromNeedWrite |= mSettingBlock;
			break;
			
		case 41:
			if(sChkLowFreq(wSetValue) == cTrue)
			{
				EEData1.wStartFLowEE = wSetValue;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wStartFLowEE >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wStartFLowEE & 0x00FF);
			bEepromNeedWrite |= mSettingBlock;
			break;
			
		case 42:
			if(sChkHighFreq(wSetValue) == cTrue)
			{
				EEData1.wStartFHighEE = wSetValue;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wStartFHighEE >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wStartFHighEE & 0x00FF);
			bEepromNeedWrite |= mSettingBlock;
			break;
			
		case 45:									/* System time-year */
			if(wSetValue>12 && wSetValue<=99)	//larger than 2013
			{
				sysTime.bYears = (INT8U)wSetValue;
				bSetDateTimeFlag = 1;
				bSetDateTimeCnt = 251;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = sysTime.bYears;
			break;
			
		case 46:									/* System time- Month */
			if(wSetValue>0 && wSetValue<=12)
			{
				sysTime.bMonths = (INT8U)wSetValue;
				bSetDateTimeFlag = 1;
				bSetDateTimeCnt = 251;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = sysTime.bMonths;
			break;
			
		case 47:									/* System time- Day */

			bDaysThisMonth = sCalDays(sysTime.bMonths,sysTime.bYears);
			if(wSetValue > 0 && wSetValue <= bDaysThisMonth)
			{
				sysTime.bDays = (INT8U)wSetValue;
				bSetDateTimeFlag = 1;
				bSetDateTimeCnt = 251;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = sysTime.bDays;
			break;
			
		case 48:								/* System time- Hour */
			if(wSetValue<=23)
			{
				sysTime.bHours = (INT8U)wSetValue;
				bSetDateTimeFlag = 1;
				bSetDateTimeCnt = 251;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = sysTime.bHours;
			break;
			
		case 49:								/* System time- Min */
			if(wSetValue<=59)
			{
				sysTime.bMinutes = (INT8U)wSetValue;
				bSetDateTimeFlag = 1;
				bSetDateTimeCnt = 251;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = sysTime.bMinutes;			
			break;
			
		case 50:								/* System time- Second */
			if(wSetValue<=59)
			{
				sysTime.bSeconds=(INT8U)wSetValue;
				bSetDateTimeFlag = 1;
				bSetDateTimeCnt = 251;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = sysTime.bSeconds;
			break;
			
		case 51:								/* Grid voltage low limit protect time 1 */
			if((wSetValue>=20 && wSetValue<=50000))
			{	
				EEData1.wVLowCutTime1EE=wSetValue/20;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)((EEData1.wVLowCutTime1EE*20)>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wVLowCutTime1EE*20&0x00FF);
			break;
			
		case 52:								/* Grid voltage high limit protect time 1 */
			if((wSetValue>=20 && wSetValue<=50000))
			{
				EEData1.wVHighCutTime1EE=wSetValue/20;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)((EEData1.wVHighCutTime1EE*20)>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wVHighCutTime1EE*20&0x00FF);
			break;
			
		case 53:								/* Grid voltage low limit protect time  2 */
			if((wSetValue>=20 && wSetValue<=50000))
			{	
				EEData1.wVLowCutTime2EE=wSetValue/20;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)((EEData1.wVLowCutTime2EE*20)>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wVLowCutTime2EE*20&0x00FF);
			break;

		case 54:								/* Grid voltage high limit protect time  2 */
			if((wSetValue>=20 && wSetValue<=50000))
			{	
				EEData1.wVHighCutTime2EE=wSetValue/20;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wVHighCutTime2EE*20>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wVHighCutTime2EE*20&0x00FF);
			break;
			
		case 55:								/* Grid frequency low limit protect time  1 */
			if((wSetValue>=20 && wSetValue<=50000))
			{	
				EEData1.wFLowCutTimeEE=wSetValue/20;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wFLowCutTimeEE*20>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wFLowCutTimeEE*20&0x00FF);
			break;
			
		case 56:								/* Grid frequency high limit protect time  1 */
            //ZXF 2015.09.06  FHT
			if((bSpecType==cS_G59)||(bSpecType==cS_G83))
			{
			    EEData1.wFHighCutTimeEE=((INT32U)wSetValue+65536)/20;
				bEepromNeedWrite |= mSettingBlock;
			}
			else if((wSetValue>=20 && wSetValue<=50000))
			{	
				EEData1.wFHighCutTimeEE=wSetValue/20;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.wFHighCutTimeEE*20>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wFHighCutTimeEE*20&0x00FF);
			break;
			
		case 57:								/* Grid frequency low limit protect time  2 */
			break;
		case 58:								/* Grid frequency high limit protect time  2 */
			break;
			
		case 72:
			if(wSetValue <= 6)
			{
				sysTime.bWeekdays = (INT8U)wSetValue;
				bSetDateTimeFlag = 1;
				bSetDateTimeCnt = 251;
			}
			else
				bSendErrMsg = 1;
			
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = sysTime.bWeekdays;
			break;
			
		case 74:							//LCD Set Country	//SetCountry0723
			if((wSetValue == 0) && (bInverterStatus != cInvterterStatus))
			{
				EEData1.fEepromSetting.bCountrySetFlag = 0;
				bCountryFlag = 0;
				bCountryCnt = 0;
				bEepromNeedWrite |= mSettingBlock;
			}
			else if((wSetValue == 1) && (bInverterStatus != cInvterterStatus))
			{
				EEData1.fEepromSetting.bCountrySetFlag = 1;
				sChangeSpecSet();
				bEepromNeedWrite |= (mSettingBlock|mAdjustBlock);
				bTurnOffSPSCnt = 1;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = (INT8U)EEData1.fEepromSetting.bCountrySetFlag;
			break;
			
		case 75:
			if(wSetValue <=1)
			{
				EEData1.fEepromSetting.bRS232to485Enable = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else 
			{
				bSendErrMsg = 1;
			}
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = (INT8U)EEData1.fEepromSetting.bRS232to485Enable;
			break;
			
		case 81:							/* PF limit line point 1 load percent */
			if((wSetValue > 5000) && (wSetValue <= 5500))
			{
				ADJValue.wFreqDerateStart= wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}
			else
			{
				bSendErrMsg = 1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wFreqDerateStart >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wFreqDerateStart& 0x00FF);
			break;
			
		case 83:
			if(sCHK60HzSpecValue(bSpecType,1,wSetValue)==cTrue)
			{
				EEData1.w60HzFreqLowEE = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.w60HzFreqLowEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.w60HzFreqLowEE&0x00FF);
			break;
			
		case 84:
			if(sCHK60HzSpecValue(bSpecType,1,wSetValue)==cTrue)
			{
				EEData1.w60HzFreqHighEE = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData1.w60HzFreqHighEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.w60HzFreqHighEE&0x00FF);
			break;
			
		case 90:							/* PF limit line point 1 load percent */
			if((wSetValue > 0) && (wSetValue <= 100))
			{
				EEData1.wPFLineLoad1 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{
				bSendErrMsg = 1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLineLoad1 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLineLoad1& 0x00FF);
			break;
			
		case 91:							/* PF limit line point 1 power factor*/
			if((wSetValue <=cUnderExcitedPF075) || (wSetValue >= cOverExcitedPF075))
			{
				EEData1.wPFLinePoint1 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{
				bSendErrMsg = 1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLinePoint1 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLinePoint1& 0x00FF);
			break;
			
		case 92:							/* PF limit line point 2 load percent */
			if((wSetValue > 0) && (wSetValue <= 100))
			{
				EEData1.wPFLineLoad2 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{	
				bSendErrMsg = 1;
			}

			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLineLoad2 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLineLoad2& 0x00FF);
			break;
			
		case 93:							/* PF limit line point 2 power factor*/
			if((wSetValue <=cUnderExcitedPF075) || (wSetValue >= cOverExcitedPF075))
			{
				EEData1.wPFLinePoint2 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{	
				bSendErrMsg = 1;
			}

			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLinePoint2 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLinePoint2& 0x00FF);
			break;
			
		case 94:							/* PF limit line point 3 load percent */
			if((wSetValue <=100) || (wSetValue == 255))
			{
				EEData1.wPFLineLoad3 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{	
				bSendErrMsg = 1;
			}

			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLineLoad3 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLineLoad3& 0x00FF);
			break;
			
		case 95:							/* PF limit line point 3 power factor*/
			if((wSetValue <=cUnderExcitedPF075) || (wSetValue >= cOverExcitedPF075))
			{
				EEData1.wPFLinePoint3 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{	
				bSendErrMsg = 1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLinePoint3 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLinePoint3& 0x00FF);
			break;
			
		case 96:						/* PF limit line point 4 load percent */
			if((wSetValue <=100) || (wSetValue == 255))
			{
				EEData1.wPFLineLoad4 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{	
				bSendErrMsg = 1;
			}

			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLineLoad4 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLineLoad4& 0x00FF);
			break;
			
		case 97:					/* PF limit line point 4 power factor*/
			if((wSetValue <=cUnderExcitedPF075) || (wSetValue >= cOverExcitedPF075))
			{
				EEData1.wPFLinePoint4 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{	
				bSendErrMsg = 1;
			}

			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLinePoint4 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLinePoint4& 0x00FF);
			break;
			
		case 98:
			if(wSetValue == 1)
			{
				bEepromNeedWrite |= mSettingBlock;
			}
			else if(wSetValue == 0)
			{
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = (INT8U)(wSetValue >> 8);
			bExterTxBuffer[5] = (INT8U)(wSetValue& 0x00FF);
			break;
			
		case 99:						/* Set PF function Model*/
			if(wSetValue <= 7)
			{
				EEData1.wPFModelEE = wSetValue;
				if(EEData1.bPFCmdMemoryEE==1)
				{
					bEepromNeedWrite |= mSettingBlock;
				}
			}
			else
			{
				bSendErrMsg = 1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wPFModelEE >> 8);;
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFModelEE & 0x00FF);
			break;
			
		case 100:
			if(wSetValue < 100)
			{
				EEData1.bFLrateEE = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.bFLrateEE;
			break;
			
		case 101:													// PFAdj  80% +0.9
			if(wSetValue <= cPFAdjMax && wSetValue >= cPFAdjMin)
			{
				ADJValue.w80PF_AdjEE1 = wSetValue;
				bEepromNeedWrite |= mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.w80PF_AdjEE1 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.w80PF_AdjEE1 & 0x00FF);
			break;
			
		case 102:													// PFAdj  60% +0.9
			if(wSetValue <= cPFAdjMax && wSetValue >= cPFAdjMin)
			{
				ADJValue.w60PF_AdjEE1 = wSetValue;
				bEepromNeedWrite |= mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.w60PF_AdjEE1 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.w60PF_AdjEE1 & 0x00FF);
			break;
			
		case 103:													// PFAdj  30% +0.9
			if(wSetValue <= cPFAdjMax && wSetValue >= cPFAdjMin)
			{
				ADJValue.w30PF_AdjEE1 = wSetValue;
				bEepromNeedWrite |= mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.w30PF_AdjEE1 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.w30PF_AdjEE1 & 0x00FF);
			break;
			
		case 104:													// PFAdj  80% -0.9
			if(wSetValue <= cPFAdjMax && wSetValue >= cPFAdjMin)
			{
				ADJValue.w80PF_AdjEE2= wSetValue;
				bEepromNeedWrite |= mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.w80PF_AdjEE2 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.w80PF_AdjEE2 & 0x00FF);
			break;
			
		case 105:													// PFAdj  60% -0.9
			if(wSetValue <= cPFAdjMax && wSetValue >= cPFAdjMin)
			{
				ADJValue.w60PF_AdjEE2= wSetValue;
				bEepromNeedWrite |= mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.w60PF_AdjEE2 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.w60PF_AdjEE2 & 0x00FF);
			break;
			
		case 106:													// PFAdj  30% -0.9
			if(wSetValue <= cPFAdjMax && wSetValue >= cPFAdjMin)
			{
				ADJValue.w30PF_AdjEE2= wSetValue;
				bEepromNeedWrite |= mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.w30PF_AdjEE2 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.w30PF_AdjEE2 & 0x00FF);
			break;
			
		case 107:													// 107 Low Voltage ride through
			if(wSetValue <= 1)
			{
				EEData1.fEepromSetting.bLVRideEnableEE = (INT8U)wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
				bSendErrMsg = 1;
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = EEData1.fEepromSetting.bLVRideEnableEE;
			break;
			
		case 108:													// CEI V1s
			if(wSetValue <= 2530 && wSetValue >= 2300)
			{
				EEData1.wHVoltReactive1= wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wHVoltReactive1 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wHVoltReactive1& 0x00FF);
			break;

		case 109:													// CEI V2s
			if(wSetValue <= 2530 && wSetValue >= 2300)
			{
				EEData1.wHVoltReactive2= wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wHVoltReactive2 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wHVoltReactive2& 0x00FF);
			break;

		case 110:													// CEI V1l
			if(wSetValue < 2300 && wSetValue >= 2070)
			{
				EEData1.wLVoltReactive1= wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wLVoltReactive1 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wLVoltReactive1& 0x00FF);
			break;
			
		case 111:													// CEI V2l
			if(wSetValue < 2300 && wSetValue >= 2070)
			{
				EEData1.wLVoltReactive2= wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wLVoltReactive2 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wLVoltReactive2& 0x00FF);
			break;
			
		case 112:													// CEI V2l
			if((wSetValue >= cV10minFaultVoltLow) && (wSetValue <= cV10minFaultVoltHigh))
			{
				EEData1.w10MinAVLimit1 = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.w10MinAVLimit1 >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.w10MinAVLimit1 & 0x00FF);
			break;
			
		case 113:
			if(wSetValue<=100)
			{
                             EEData1.wQvPowerLockin=wSetValue;
                             bEepromNeedWrite |=mSettingBlock;
			}
			else
			{
               bSendErrMsg=1;
			}
			bExterTxBuffer[4]=(INT8U)(EEData1.wQvPowerLockin>>8);
			bExterTxBuffer[5]=(INT8U)(EEData1.wQvPowerLockin & 0x00FF);
            break;
			
		case 114:
			if((wSetValue >=2300) && (wSetValue <= 2600))
			{
				EEData1.wPFLineVLockin = wSetValue;
				bEepromNeedWrite|=mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLineVLockin >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLineVLockin & 0x00FF);
			break;
			
		case 115:												//	Lock in gird volt of CEI021
			if((wSetValue >=2000) && (wSetValue <= 2600))
			{
				EEData1.wPFLineVLockout = wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wPFLineVLockout >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wPFLineVLockout & 0x00FF);
			break;
			
		case 117:												//	Frequency Derating Enable
			if(wSetValue <=1) 
			{
				EEData1.fEepromSetting.bFreqDerateEnable = wSetValue;
				bEepromNeedWrite|=mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.fEepromSetting.bFreqDerateEnable >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.fEepromSetting.bFreqDerateEnable & 0x00FF);
			break;
			
		case 118:												//	Q(v) lock Out active power of CEI021
			if(wSetValue <= 100)
			{
				EEData1.wQvPowerLockout = wSetValue;
				bEepromNeedWrite|=mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wQvPowerLockout >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wQvPowerLockout & 0x00FF);
			break;
			
		case 119:								//Restart Delay Time after fault back;
			if((wSetValue <= 2000))
			{
				EEData1.wReconnectTime = wSetValue;
				bEepromNeedWrite|=mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wReconnectTime >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wReconnectTime & 0x00FF);
			break;
			
		case 120:
			if(wSetValue <= 100)
			{
				EEData1.bLVRTKFactor = wSetValue;
				bEepromNeedWrite|=mSettingBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.bLVRTKFactor >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.bLVRTKFactor & 0x00FF);
			break;
			
		case 121:
			if(wSetValue <= 2300)
			{
				ADJValue.wLVRTFaultVolt1 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultVolt1 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultVolt1 & 0x00FF);
			break;
			
		case 122:
			if(wSetValue <= 5000)
			{
				ADJValue.wLVRTFaultTimer1 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultTimer1 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultTimer1 & 0x00FF);
			break;
			
		case 123:
			if(wSetValue <= 2300 )
			{
				ADJValue.wLVRTFaultVolt2 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultVolt2 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultVolt2 & 0x00FF);
			break;
			
		case 124:
			if(wSetValue <= 5000)
			{
				ADJValue.wLVRTFaultTimer2 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultTimer2 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultTimer2 & 0x00FF);
			break;
			
		case 125:
			if(wSetValue <= 2300)
			{
				ADJValue.wLVRTFaultVolt3 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultVolt3 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultVolt3 & 0x00FF);
			break;
			
		case 126:
			if(wSetValue <= 5000)
			{
				ADJValue.wLVRTFaultTimer3 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultTimer3 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultTimer3 & 0x00FF);
			break;
			
		case 127:
			if(wSetValue <= 2300)
			{
				ADJValue.wLVRTFaultVolt4 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultVolt4 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultVolt4 & 0x00FF);
			break;
			
		case 128:
			if(wSetValue <= 5000)
			{
				ADJValue.wLVRTFaultTimer4 = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultTimer4 >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultTimer4 & 0x00FF);
			break;
			
		case 129:
			if(wSetValue >= 2300)
			{
				ADJValue.wLVRTFaultVoltHigh = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultVoltHigh >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultVoltHigh & 0x00FF);
			break;
			
		case 130:
			if(wSetValue <= 5000)
			{
				ADJValue.wLVRTFaultTimerHigh = wSetValue;
				bEepromNeedWrite|=mAdjustBlock;
			}			
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wLVRTFaultTimerHigh >> 8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wLVRTFaultTimerHigh & 0x00FF);
			break;
			
		case 135:
			if(wSetValue==0)
			{
				bSpecPasswordType=0;
			}
			else if(wSetValue==1)
			{
				if((bPasswordUnLock&cPasswordUnlock)==cPasswordUnlock)
				{
					bSpecPasswordType=1;
				}
				else
				{
					bSendErrMsg=1;
				}
			}
			else if(wSetValue==2)
			{
				bSpecPasswordType=0;
				bPasswordUnLock=0;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = bSpecPasswordType;
			break;

		case 136:  //reserve
			if(bSpecPasswordType==1)		//
			{
				EEData1.wPassword3=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
				if((*(bRXDataBuff+4)=='S')&&(*(bRXDataBuff+5)=='P'))	//pw0712
				{
					bPasswordUnLock|=cSuperPassword3Bit;
					if((bPasswordUnLock&cSuperPasswordUnlock)==cSuperPasswordUnlock)
					{
						bPasswordUnLock|=cPasswordUnlock;
						EEData1.wPassword1=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');	//pw0712
						EEData1.wPassword2=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
						EEData1.wPassword3=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
					}
					bSendErrMsg=0;
				}

				if(EEData1.wPassword3==wSetValue)
				{
					bPasswordUnLock|=cPassword3Bit;
					bSendErrMsg=0;
				}
			}
			bExterTxBuffer[4] = (INT8U)(wSetValue >> 8);
			bExterTxBuffer[5] = (INT8U)(wSetValue& 0x00FF);
			break;

		case 137: //reserve
			if(bSpecPasswordType==1)		//write password3
			{
				EEData1.wPassword2=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
				if((*(bRXDataBuff+4)=='E')&&(*(bRXDataBuff+5)=='C'))	//pw0712
				{
					bPasswordUnLock|=cSuperPassword2Bit;
					if((bPasswordUnLock&cSuperPasswordUnlock)==cSuperPasswordUnlock)
					{
						bPasswordUnLock|=cPasswordUnlock;
						EEData1.wPassword1=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');	//pw0712
						EEData1.wPassword2=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
						EEData1.wPassword3=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
					}
					bSendErrMsg=0;
				}

				if(EEData1.wPassword2==wSetValue)
				{
					bPasswordUnLock|=cPassword2Bit;
					bSendErrMsg=0;
				}
			}
			bExterTxBuffer[4] = (INT8U)(wSetValue >> 8);
			bExterTxBuffer[5] = (INT8U)(wSetValue& 0x00FF);
			break;

		case 138:  //reserve
			if(bSpecPasswordType==1)		//write password3
			{
				EEData1.wPassword1=wSetValue;
				bEepromNeedWrite |= mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
				if((*(bRXDataBuff+4)=='p')&&(*(bRXDataBuff+5)=='w'))	//pw0712
				{
					bPasswordUnLock|=cSuperPassword1Bit;
					if((bPasswordUnLock&cSuperPasswordUnlock)==cSuperPasswordUnlock)
					{
						bPasswordUnLock|=cPasswordUnlock;
						EEData1.wPassword1=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');	//pw0712
						EEData1.wPassword2=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
						EEData1.wPassword3=(INT16U)(((INT16U)'X'<<8)|(INT8U)'X');
					}
					bSendErrMsg=0;
				}

				if(EEData1.wPassword1==wSetValue)
				{
					bPasswordUnLock|=cPassword1Bit;
					bSendErrMsg=0;
				}
			}
			bExterTxBuffer[4] = (INT8U)(wSetValue >> 8);
			bExterTxBuffer[5] = (INT8U)(wSetValue& 0x00FF);
			break;
			
		case 150:
			if(wSetValue <=1) 
			{
				bFanCheckCmd = wSetValue;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(bFanCheckCmd >> 8);
			bExterTxBuffer[5] = (INT8U)(bFanCheckCmd & 0x00FF);
			break;
		
		case 151:	
			if(wSetValue <=1)
			{
				EEData1.fEepromSetting.bNLineDisconnect=wSetValue;
				bEepromNeedWrite|=mSettingBlock;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.fEepromSetting.bNLineDisconnect >> 8);
			bExterTxBuffer[5] = (INT8U)(EEData1.fEepromSetting.bNLineDisconnect & 0x00FF);
			break;

		case 152:
			if(wSetValue<=1)
			{
			  	 EEData1.fEepromSetting.bPVSimilarSet=wSetValue;
			  	 bEepromNeedWrite|=mSettingBlock;
			}
			else
			{
              			 bSendErrMsg=1;
			}
            		bExterTxBuffer[4] = (INT8U)(EEData1.fEepromSetting.bPVSimilarSet>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.fEepromSetting.bPVSimilarSet&0x00FF);
			break;

		case 192:									/* Inverter Moudle (high) */
			if(sCHKModelTypeHigh(wSetValue) )
			{
				EEData1.wTxUxMxSxHighEE = wSetValue;
			}
			else
				bSendErrMsg=1;

			bExterTxBuffer[4] = (INT8U)(EEData1.wTxUxMxSxHighEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wTxUxMxSxHighEE&0x00FF);
			break;
		case 193:
		    if((sCHKModelType(wSetValue) )&& (bInverterStatus != cInvterterStatus))
		     {	
				EEData1.wTxUxMxSxEE = wSetValue;

				sCheckmodel(EEData1.wTxUxMxSxEE);
				
				EEData1.fEepromSetting.bCountrySetFlag = 1;	//SetCountry0723
		
				sChangeSpecSet();
				bEepromNeedWrite |= (mSettingBlock|mAdjustBlock);
				bTurnOffSPSCnt=1;			
		    	}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.wTxUxMxSxEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.wTxUxMxSxEE&0x00FF);
			break;
		case 194:
			if((wSetValue<=1)&&(wSetValue>=0))
			{
				EEData1.fEepromSetting.bOverLoadPercent = wSetValue;
			}
			else
			{
				bSendErrMsg = 1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData1.fEepromSetting.bOverLoadPercent>>8);
			bExterTxBuffer[5] = (INT8U)(EEData1.fEepromSetting.bOverLoadPercent&0x00FF);
			break;

		
		//=======================================================================================//
		case 4097:		// Input Reg offset is 0x0FFF \03 Input power (high) 
			break;
		case 4098:								// 04 Input power (low) 
			break;
		case 4099:								// 05 PV1 voltage 
			if(wPV_1_Volt>1000)
			{						
				dwRealValue = ((INT32U)wPV_1_Volt<<10)/ADJValue.wPVV_1_AdjEE;	
				wAdjustTemp= (((INT32U)(wSetValue) << 10)/dwRealValue);
				if(sDataCheck(wAdjustTemp))
				{
					ADJValue.wPVV_1_AdjEE=wAdjustTemp;						
					bEepromNeedWrite |= mAdjustBlock;
				}
				else
				{
					bSendErrMsg=1;
				}

			}
			else
			{
				 bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wPVV_1_AdjEE>>8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wPVV_1_AdjEE&0x00FF);
			break;
		case 4100:								// 06 PV1 input current 
			if(bConstantPower==c80PerLoadCommand)
		    {
				if((wPV_1_Curr>10)&&(wLoadPer100>50)) 
				{				
					dwRealValue = ((INT32U)wPV_1_Curr<<10)/ADJValue.wPV1_Curr_AdjEE;
					wAdjustTemp = (((INT32U)(wSetValue)<<10)/dwRealValue);			
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wPV1_Curr_AdjEE=wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
		 			bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wPV1_Curr_AdjEE>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wPV1_Curr_AdjEE&0x00FF);
		    }
			else if(bConstantPower==c30PerLoadCommand)
			{
	           if((wPV_1_Curr>10) &&(wLoadPer100<50))
				{				
					dwRealValue = ((INT32U)wPV_1_Curr<<10)/ADJValue.wBT1Curr_AdjEE2;
					wAdjustTemp= (((INT32U)(wSetValue)<<10)/dwRealValue);
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wBT1Curr_AdjEE2=wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wBT1Curr_AdjEE2>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wBT1Curr_AdjEE2&0x00FF);
			}
			else
			{
				bSendErrMsg=1;
			}
			break;
		case 4101:								//07 PV1 input watt (high) 
			break;
		case 4102:								//08 PV1 input watt (low) 
			break;
		case 4103:								//09 PV2 voltage 
			if(wPV_2_Volt>100)
			{						
				dwRealValue = ((INT32U)wPV_2_Volt<<10)/ADJValue.wPVV_2_AdjEE;	
				 wAdjustTemp= (((INT32U)(wSetValue)<<10)/dwRealValue);				
				if(sDataCheck(wAdjustTemp))
				{
					ADJValue.wPVV_2_AdjEE=wAdjustTemp;						
					bEepromNeedWrite |= mAdjustBlock;
				}
				else
				{
					bSendErrMsg=1;
				}
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wPVV_2_AdjEE>>8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wPVV_2_AdjEE&0x00FF);
			break;
		case 4104:								// 10 PV2 input current 
			 if(bConstantPower==c80PerLoadCommand)
			    {
				   	 if((wPV_2_Curr>10) &&(wLoadPer100>50))
					{				
						dwRealValue = ((INT32U)wPV_2_Curr<<10)/ADJValue.wPV2_Curr_AdjEE;
						wAdjustTemp= (((INT32U)(wSetValue)<<10)/dwRealValue);
						if(sDataCheck(wAdjustTemp))
						{
							ADJValue.wPV2_Curr_AdjEE=wAdjustTemp;						
							bEepromNeedWrite |= mAdjustBlock;
						}
						else
						{
							bSendErrMsg=1;
						}
					}
					 else
					 {
						bSendErrMsg=1;
					 }
				
					bExterTxBuffer[4] = (INT8U)(ADJValue.wPV2_Curr_AdjEE>>8);
					bExterTxBuffer[5] = (INT8U)(ADJValue.wPV2_Curr_AdjEE&0x00FF);
			    }
			else if(bConstantPower==c30PerLoadCommand)
			{
				if((wPV_2_Curr>10) &&(wLoadPer100<50))
				{				
					dwRealValue = ((INT32U)wPV_2_Curr<<10)/ADJValue.wBT2Curr_AdjEE2;
					wAdjustTemp= (((INT32U)(wSetValue)<<10)/dwRealValue);
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wBT2Curr_AdjEE2=wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				
				bExterTxBuffer[4] = (INT8U)(ADJValue.wBT2Curr_AdjEE2>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wBT2Curr_AdjEE2&0x00FF);	
			}
			else
			{
				bSendErrMsg=1;
			}
			break;
		case 4105:								// 11 PV2 input watt (high) 
			break;
		case 4106:								// 12 PV2 input watt (low) 
			break;
		case 4107:								// 13 Output power (high) 
			if(wSetValue <= 3 && dwOP_RST_WattSec > 500)
			{
				wSetValueTemp = wSetValue;
				wSetFlag = 1;
			}
			else
			{
				bSendErrMsg = 1;
				wSetFlag = 0;
			}
			bExterTxBuffer[4] = 0;
			bExterTxBuffer[5] = (INT8U)(wSetValueTemp);
			break;
		case 4108:								// 14 Output power (low) 
			if(bConstantPower==c80PerLoadCommand)
			{
				if(wSetValue > 0  && dwOP_RST_WattSec >500 && wSetFlag == 1) 
				{				
					dwRealValue = ((INT32U)dwOP_RST_WattSec<<10)/ADJValue.wOPW_AdjEE;
					wAdjustTemp= ((((INT32U)wSetValueTemp << 16) + (INT32U)wSetValue) << 10) / dwRealValue;
					wSetValueTemp = 0;
					wSetFlag = 0;				
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wOPW_AdjEE =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wOPW_AdjEE>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wOPW_AdjEE&0x00FF);
			}
			else if(bConstantPower==c30PerLoadCommand)
			{
            			  if(wSetValue > 0  && dwOP_RST_WattSec >500 && wSetFlag == 1) 
				{				
					dwRealValue = ((INT32U)dwOP_RST_WattSec<<10)/ADJValue.wOPW_AdjEE2;
					wAdjustTemp= ((((INT32U)wSetValueTemp << 16) + (INT32U)wSetValue) << 10) / dwRealValue;
					wSetValueTemp = 0;
					wSetFlag = 0;					
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wOPW_AdjEE2 =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
			   	else
				{
					bSendErrMsg=1;
				}
			  	 bExterTxBuffer[4] = (INT8U)(ADJValue.wOPW_AdjEE2>>8);
			   	bExterTxBuffer[5] = (INT8U)(ADJValue.wOPW_AdjEE2&0x00FF);
			}
			else
			{
				bSendErrMsg=1;
			}
			break;
		case 4109:								// 15 Grid frequency 
			break;
		case 4110:								// 16 Vac1 
			if(wOPPhase_R_Volt>500) 
			{				
				dwRealValue = ((INT32U)wOPPhase_R_Volt<<10)/ADJValue.wOPV_R_AdjEE;
				wAdjustTemp= (((INT32U)(wSetValue) << 10)/dwRealValue);
				if(sDataCheck(wAdjustTemp))
				{
					ADJValue.wOPV_R_AdjEE =wAdjustTemp;						
					bEepromNeedWrite |= mAdjustBlock;
				}
				else
				{
					bSendErrMsg=1;
				}
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wOPV_R_AdjEE>>8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wOPV_R_AdjEE&0x00FF);
			break;
		case 4111:								// 17 Iac1 
	        if(bConstantPower==c80PerLoadCommand)
			{
				if((wOPPhase_R_Curr>10) &&(wLoadPer100>50))
				{				
					dwRealValue = ((INT32U)wOPPhase_R_Curr<<10)/ADJValue.wRCurr_AdjEE;
					wAdjustTemp= (((INT32U)(wSetValue)<<10)/dwRealValue);
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wRCurr_AdjEE =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wRCurr_AdjEE>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wRCurr_AdjEE&0x00FF);
			}
			else if(bConstantPower==c30PerLoadCommand)
			{
		        if((wOPPhase_R_Curr>10) &&(wLoadPer100<50))
				{
					dwRealValue = ((INT32U)wOPPhase_R_Curr<<10)/ADJValue.wRCurr_AdjEE2;
					wAdjustTemp =((INT32U)(wSetValue)<<10)/dwRealValue;
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wRCurr_AdjEE2 =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
					bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4]=(INT8U)(ADJValue.wRCurr_AdjEE2>>8);
				bExterTxBuffer[5]=(INT8U)(ADJValue.wRCurr_AdjEE2&0x00FF);
			}
			else
			{
				bSendErrMsg=1;
			}
			
			break;
		case 4112:								//18 Pac1 H 
			break;
		case 4113:								//19 Pac1 L 
			break;
		case 4114:								// 20 Vac2 
			if(wOPPhase_S_Volt>500) 		
			{				
				dwRealValue = ((INT32U)wOPPhase_S_Volt<<10)/ADJValue.wOPV_S_AdjEE;
				wAdjustTemp = (((INT32U)(wSetValue) << 10)/dwRealValue);				
				if(sDataCheck(wAdjustTemp))
				{
					ADJValue.wOPV_S_AdjEE =wAdjustTemp;						
					bEepromNeedWrite |= mAdjustBlock;
				}
				else
				{
					bSendErrMsg=1;
				}
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wOPV_S_AdjEE>>8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wOPV_S_AdjEE&0x00FF);
			break;
		case 4115:								// 19 Iac2 
			if(bConstantPower==c80PerLoadCommand)
			{
				if((wOPPhase_S_Curr>10) &&(wLoadPer100>50))
				{				
					dwRealValue = ((INT32U)wOPPhase_S_Curr<<10)/ADJValue.wSCurr_AdjEE;
					wAdjustTemp = (((INT32U)(wSetValue)<<10)/dwRealValue);
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wSCurr_AdjEE =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wSCurr_AdjEE>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wSCurr_AdjEE&0x00FF);
			}
			else if(bConstantPower==c30PerLoadCommand)
			{
				if((wOPPhase_S_Curr>10) &&(wLoadPer100<50))
				{				
					dwRealValue = ((INT32U)wOPPhase_S_Curr<<10)/ADJValue.wSCurr_AdjEE2;
					wAdjustTemp= (((INT32U)(wSetValue)<<10)/dwRealValue);
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wSCurr_AdjEE2 =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wSCurr_AdjEE2>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wSCurr_AdjEE2&0x00FF);
			}
			 else
			{
				bSendErrMsg=1;
			}
			break;
		case 4116:								// Pac2 H 
			break;
		case 4117:								// Pac2 L 
			break;
		case 4118:								// 22 Vac3 
			if(wOPPhase_T_Volt>50) 	
			{				
				dwRealValue = ((INT32U)wOPPhase_T_Volt<<10)/ADJValue.wOPV_T_AdjEE;
				wAdjustTemp = (((INT32U)(wSetValue) << 10)/dwRealValue);				
				if(sDataCheck(wAdjustTemp))
				{
					ADJValue.wOPV_T_AdjEE =wAdjustTemp;						
					bEepromNeedWrite |= mAdjustBlock;
				}
				else
				{
					bSendErrMsg=1;
				}
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(ADJValue.wOPV_T_AdjEE>>8);
			bExterTxBuffer[5] = (INT8U)(ADJValue.wOPV_T_AdjEE&0x00FF);
			break;
		case 4119:								// 23 Iac3 
		    if(bConstantPower==c80PerLoadCommand)
			{
				if((wOPPhase_T_Curr>10) &&(wLoadPer100>50))
				{				
					dwRealValue = ((INT32U)wOPPhase_T_Curr<<10)/ADJValue.wTCurr_AdjEE;
					wAdjustTemp = (((INT32U)(wSetValue)<<10)/dwRealValue);
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wTCurr_AdjEE =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wTCurr_AdjEE>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wTCurr_AdjEE&0x00FF);
			}
			else if(bConstantPower==c30PerLoadCommand)
			{
				if((wOPPhase_T_Curr>10)&&(wLoadPer100<50))
				{    
					dwRealValue=((INT32U)wOPPhase_T_Curr<<10)/ADJValue.wTCurr_AdjEE2;				
					wAdjustTemp=(((INT32U)(wSetValue)<<10)/dwRealValue);					
					if(sDataCheck(wAdjustTemp))
					{
						ADJValue.wTCurr_AdjEE2 =wAdjustTemp;						
						bEepromNeedWrite |= mAdjustBlock;
					}
					else
					{
						bSendErrMsg=1;
					}
				}
				else
				{
					bSendErrMsg=1;
				}
				bExterTxBuffer[4] = (INT8U)(ADJValue.wTCurr_AdjEE2>>8);
				bExterTxBuffer[5] = (INT8U)(ADJValue.wTCurr_AdjEE2&0x00FF);
			}
			else
			{
				bSendErrMsg=1;
			}
			break;
			
		case 4120:								// Pac3 H 
			break;
		case 4121:								// Pac3 L 
			break;
		case 4122:								// 26 Energy today H 
			break;
		case 4123:								// 27 Energy today L
			if(wSetValue==0)
			{
				EEData32.wEtoday0k1wEE=0;
				bEepromNeedWrite|=mEnegyBlock1;			
			}
			else
				bSendErrMsg=1;
			bExterTxBuffer[4] = (INT8U)(EEData32.wEtoday0k1wEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData32.wEtoday0k1wEE&0x00FF);
			break;
		case 4124:								// Energy total H 
			EEData32.dwInvertedP0K1WEE=((INT32U)wSetValue<<16);
			bExterTxBuffer[4] = (INT8U)(wSetValue>>8);
			bExterTxBuffer[5] = (INT8U)(wSetValue&0x00FF);
			break;
		case 4125:								// Energy total L 
            EEData32.dwInvertedP0K1WEE+=wSetValue;
            EEData32.dwPV_2_TotalEE = (EEData32.dwInvertedP0K1WEE>>1)*100/98;
            EEData32.dwPV_1_TotalEE = (EEData32.dwInvertedP0K1WEE>>1)*100/98;
            bEepromNeedWrite|=mEnegyBlock1;
			bExterTxBuffer[4] = (INT8U)(EEData32.dwInvertedP0K1WEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData32.dwInvertedP0K1WEE&0x00FF);
			break;
		case 4126:								// Time total H 
			break;
		case 4127:								// Time total L 
			if(wSetValue==0)
			{
				EEData32.dwInvTime0p5SecEE=0;  
				bEepromNeedWrite|=mEnegyBlock1;	
			}
			else
			{
				bSendErrMsg=1;
			}	
			bExterTxBuffer[4] = (INT8U)(EEData32.dwInvTime0p5SecEE >>8);
			bExterTxBuffer[5] = (INT8U)(EEData32.dwInvTime0p5SecEE &0x00FF);
			break;
		case 4128:								// 32 Temperature 
			break;
		case 4137:								// 41 IPM Temperature
			break;
		case 4138:								// 42 P Bus Voltage 
			break;
		case 4139:								// 43 N Bus Voltage
			break;
		case 4140:								// 44 CheckStep
			if (wSetValue <= 4)	
			{
				bConstantPower = (INT8U)wSetValue;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(bConstantPower>>8);
			bExterTxBuffer[5] = (INT8U)(bConstantPower&0x00FF);
			break;

		case 4141:								// 45 IPF
			break;
		case 4142:								// 46 DCI  S 
			break;
		case 4144:								// 48 Energy from PV1 today H 
			break;
		case 4145:								// 49 Energy from PV1  today L
			if(wSetValue==0)
			{
				EEData32.dwPV_1_TodayEE = 0;							
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData32.dwPV_1_TodayEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData32.dwPV_1_TodayEE);
			break;
		case 4146:								// 50 Energy from PV1 in tatal H 
			break;
		case 4147:								// 51 Energy from PV1 in tatal L 
			if(wSetValue==0)
			{
				EEData32.dwPV_1_TotalEE = 0;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData32.dwPV_1_TotalEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData32.dwPV_1_TotalEE);
			break;
		case 4148:								// 52 Energy from PV2 today H 
			break;
		case 4149:								// 53 Energy from PV2  today L
			if(wSetValue==0)
			{
				EEData32.dwPV_2_TodayEE = 0;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData32.dwPV_2_TodayEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData32.dwPV_2_TodayEE);
			break;
			
		case 4150:								// 54 Energy from PV2 in tatal H 
			break;
			
		case 4151:								// 55 Energy from PV2 in tatal L 
			if(wSetValue==0)
			{
				EEData32.dwPV_2_TotalEE = 0;
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(EEData32.dwPV_2_TotalEE>>8);
			bExterTxBuffer[5] = (INT8U)(EEData32.dwPV_2_TotalEE);
			break;
		case 4152:								// 56 Energy from PV in total H 
			break;
		case 4153:								// 57 Energy from PV in total L
			if(wSetValue==0)
			{	
			}
			else
			{
				bSendErrMsg=1;
			}
			bExterTxBuffer[4] = (INT8U)(0>>8);
			bExterTxBuffer[5] = (INT8U)(0);
			break;
			
		default:								
			bSendErrMsg=1;
			break;	
	}

	if(bSendErrMsg==0)
	{
		wCRCTemp = usMBCRC16(bExterTxBuffer,6);
		bExterTxBuffer[6]= (INT8U)(wCRCTemp);
		bExterTxBuffer[7]= (INT8U)(wCRCTemp>>8);
		bModbusERROR=0;
	}
	else
	{
		bExterTxBuffer[1]= (INT8U)(0x80|cModBus_6);
		bExterTxBuffer[2]= 1;
		wCRCTemp = usMBCRC16(bExterTxBuffer,3);
		bExterTxBuffer[3]= (INT8U)(wCRCTemp);
		bExterTxBuffer[4]= (INT8U)(wCRCTemp>>8);
		bModbusERROR=1;
	}
}

INT8U bTxDataLenth;
INT8U bRxSCI0DataLenth;
INT8U bExterTXSCI0Buff[cExSCILMax+1];

void sParsing_modbus(INT8UP bRXDataBuff,INT8U bRXDataLen, INT8U bSciNO)
{
 
    INT8U wComd=0;	
      INT8U i;
	
	if(*bRXDataBuff != 0)
	{                                 
		if(*bRXDataBuff ==EEData1.bCOMAddr)
		{	
			wComd=1;
			bFlashDisable=0;
		}
		else if(( bSciNO == 0)&&(EEData1.fEepromSetting.bRS232to485Enable==1))
		{
			bFlashDisable=1;//if comaddr=0,disable flash
		  	wCounts485++;
			//RS485_EN(1); 
			//sTaskStopXMs(OSPrioCur,30);
			 bTx485DataLenth = bRXDataLen;
			for(i = 0;i<bTx485DataLenth;i++)
				bExterTX485Buff[i] = *(bRXDataBuff+i); 
			//sSCIWrite(1,bExterTX485Buff,bTx485DataLenth);// 485sent out
			//sTaskStopXMs(OSPrioCur,bRxSCI0DataLenth+20);
			//RS485_EN(0);
			wComd=0;
			return;
		}
		else
		{
			bFlashDisable=1;//if comaddr=0,disable flash
			wComd=0;
			return;
		}
	}
	else if(*(bRXDataBuff+1)==cModBus_6)
	{
		bExComNoRespon=1;
		bFlashDisable=1;//if comaddr=0,disable flash
	}	
	bExterTxBuffer[0]=EEData1.bCOMAddr;
	
	bExterTxBuffer[1]=*(bRXDataBuff+1);
    
    if((wComd==1||*bRXDataBuff==0) && (*(bRXDataBuff+1)>=3&&*(bRXDataBuff+1)<=16))
   	{		
		switch(*(bRXDataBuff+1))
		{
			case cModBus_3:
					sReadHoldingRegister(bRXDataBuff);
					break;
			case cModBus_4:
					sReadInputRegister(bRXDataBuff);
					break;
			case cModBus_6:
					sPresetSingleRegister(bRXDataBuff);
					break;
			default:
					bExComNoRespon=1;
					break;
		}
   	}
    	else
	{
		bExComNoRespon = 1; //checksum Error
		bFlashDisable=1;	//comaddress wrong
    	}
    
	if (bExComNoRespon==1)
	{
		bExComNoRespon =0;
		return;
	}
	
	if(bExterTxBuffer[1]==16 ||bExterTxBuffer[1]==6)
	{
		bTxDataLenth = 8;
	}
	else 
	{
		bTxDataLenth = bExterTxBuffer[2]+5;
	}
	
	if(bModbusERROR == 1)
	{
		bTxDataLenth = 5;
		bModbusERROR = 0;
	}
	
	if(bSciNO == 0)
	{   
	   	if(bDSPFlashFlag==1)
		{
			bRS485DSPFlashFlag=0;
		}		
		bRxSCI0DataLenth = bTxDataLenth;
		 
		for(i = 0;i<bRxSCI0DataLenth;i++)
		{
			bExterTXSCI0Buff[i] = bExterTxBuffer[i]; 
		}
		wCounts485 = 0;
		//sSCIWrite(0,bExterTXSCI0Buff,bRxSCI0DataLenth);// SCI1 sent out
	}
	else if(bSciNO == 1)
	{
		if(bDSPFlashFlag==1)
		{
			bRS485DSPFlashFlag=1;
		}	
		bTx485DataLenth = bTxDataLenth;
		for(i = 0;i<bTx485DataLenth;i++)
		{
			bExterTX485Buff[i] = bExterTxBuffer[i]; 
		}
		//RS485_EN(1);
		//sTaskStopXMs(OSPrioCur,30);
		//sSCIWrite(1,bExterTX485Buff,bTx485DataLenth);// SCI1 sent out
		//sTaskStopXMs(OSPrioCur,bTx485DataLenth+20);
		//RS485_EN(0);
	}
}

void sExter485ComTask(void *arg)
{
	static INT8U  bRxD = 0;
	static INT8U ReadCount=0;
    static INT8U bTemp = 1;
   	static INT16U wCRC16=0; 
	static INT32U dwNO485Cnt = 257400;
	static INT8U	wTimeOutCnt;
	 
	while(1)
	{  
		ReadCount=0;

		if(dwNO485Cnt < 257400)
		{	
			dwNO485Cnt++;				
		}
		else 
		{
			bRS485CommuStatus=0;
		}

        //²åÈëÐÅºÅÁ¿
			switch(bExterRX485Index) 
			{
				case 0:	
					bExterRX485Buffer[bExterRX485Index]=bRxD;
					bExterRX485Index++;  
				break;
				
				case 1:		
					if((bRxD==cModBus_3)||(bRxD==cModBus_4)||(bRxD==cModBus_6))
					{	
						bExterRX485Buffer[bExterRX485Index]=bRxD;
						bExterRX485Index++;
					}
					else
					{
						bExterRX485Index=0;
					}
				break;
				
				case 2:
					{
						bExterRX485Buffer[bExterRX485Index]=bRxD;
						bExterRX485Index++;
					}	
				break;
				
				default:	
					 if((wCounts485 > 0)&&((bExterRX485Buffer[1]==cModBus_3)||(bExterRX485Buffer[1]==cModBus_4)
							||(bExterRX485Buffer[1]==cModBus_6)))
					{							
						if((bExterRX485Buffer[1]==16 )||(bExterRX485Buffer[1]==6))
						{
							bRx485DataLenth = 8;
						}
						else 
						{
							bRx485DataLenth = bExterRX485Buffer[2]+5;
						}
						if(bRx485DataLenth >= cExSCILMax)
						{
							bRx485DataLenth = 0;
							bExterRX485Index=0;
							continue;
						}
						bExterRX485Buffer[bExterRX485Index]=bRxD;
						bExterRX485Index++;	
						if(bExterRX485Index==bRx485DataLenth)
						{
							wCRC16=usMBCRC16(bExterRX485Buffer,(bRx485DataLenth-2));	
							if(((wCRC16&0XFF)==bExterRX485Buffer[bRx485DataLenth-2])
								&&((wCRC16>>8)==bExterRX485Buffer[bRx485DataLenth-1]))
							{
								wCounts485 = 0;
								//sTaskStopXMs(OSPrioCur,90);
								//sSCIWrite(0,bExterRX485Buffer,bRx485DataLenth);// SCI1 sent out
								bExterRX485Index=0; 
							}
							else 
							{
								bExterRX485Index = 0;	
							}
							
						}
					}
					else if((bExterRX485Buffer[1]==cModBus_3)||(bExterRX485Buffer[1]==cModBus_4)||
							(bExterRX485Buffer[1]==cModBus_6))
					{
						if(bExterRX485Buffer[1]==16)
						{
							bRx485DataLenth = 9+bExterRX485Buffer[6];//length
						}	
						else 
						{
							bRx485DataLenth = 8;
						}
						if(bRx485DataLenth >= cExSCILMax)
						{
							bRx485DataLenth = 0;
							bExterRX485Index=0;
							continue;
						}
						if(bExterRX485Index< bRx485DataLenth)
						{    
							bExterRX485Buffer[bExterRX485Index]=bRxD;
							bExterRX485Index++;	
						}
						else
						{
							bExterRX485Index = 0;
						}
							
						if(bExterRX485Index==bRx485DataLenth)
						{    
							wCRC16=usMBCRC16(bExterRX485Buffer,(bRx485DataLenth-2));	
							if(((wCRC16&0XFF)==bExterRX485Buffer[bRx485DataLenth-2])
								&&((wCRC16>>8)==bExterRX485Buffer[bRx485DataLenth-1]))
							{    
								sParsing_modbus(bExterRX485Buffer,bRx485DataLenth,1);
								bExterRX485Index = 0;								
							}
							else 
							{
								bExterRX485Index = 0;	
							}
						}
					}
					else
					{
						wCounts485 = 0;
						bExterRX485Index = 0;
						//sSCIInitial(1,cExSCILMax);
					}
				break;				      
			}
			
			wTimeOutCnt=0;	
			bRS485CommuStatus=1;
			dwNO485Cnt=0;
			dwCOMcnt= 0;
			ReadCount++;
			if(ReadCount == 12)
			{
			    //OSTaskStart(sExter485ComTask_PRIO,0,7);
				//OSTaskPend();
				ReadCount=0;
			}
		}

		if(wTimeOutCnt>=72)		      
		{                                          
			bExterRX485Index=0;	
			wTimeOutCnt=0;	
		}
		else
		{
			wTimeOutCnt++;	
		}


		if((1==bDSPFlashStatus)&&(1==bRS485DSPFlashFlag))
		{
			bInverterStatus = cFlashStatus;	 
			bDSPFlashFlag=cRS485FlashDSP;
			while(1)
			{
				//OSTaskStart(sExter485ComTask_PRIO,0,7);
				//OSTaskPend();
			}
		}
		//OSTaskStart(sExter485ComTask_PRIO,0,7);
		//OSTaskPend();
	//}
} 
