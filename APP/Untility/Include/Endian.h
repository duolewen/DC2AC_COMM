#ifndef UBTILITY_H_
#define UBTILITY_H_

#include "stdint.h"

//大端序转换
void sBigSetWord(uint8_t *buf,uint32_t udwData);
void sBigSetHalfWord(uint8_t *buf,uint16_t uwData);
void sBigSetByte(uint8_t *buf,uint8_t ubdata);
uint32_t swBigGetWord(uint8_t *buf);
uint16_t shwBigGetHalfWord(uint8_t *buf);
uint8_t sbBigGetByte(uint8_t *buf);

//小端序转换
void sLittleSetWord(uint8_t *buf,uint32_t udwData);
void sLittleSetHalfWord(uint8_t *buf,uint16_t uwData);
void sLittleSetByte(uint8_t *buf,uint8_t ubdata);
uint32_t swLittleGetWord(uint8_t *buf);
uint16_t shwLittleGetHalfWord(uint8_t *buf);
uint8_t sbLittleGetByte(uint8_t *buf);

#endif
