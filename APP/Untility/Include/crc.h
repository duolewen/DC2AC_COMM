

#ifndef __CRC_H__
#define __CRC_H__	

#include "GlobalDef.h"

unsigned char CRC8(unsigned char *ptr, unsigned char counter);
INT16U sCRC16(INT8U *pucFrame, INT16U usDataLen);

#endif
