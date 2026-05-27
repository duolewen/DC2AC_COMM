#include "Endian.h"
//大端序转换
void sBigSetWord(uint8_t *buf,uint32_t udwData) 
{
    uint8_t h1 = udwData;
    uint8_t h2 = udwData >> 8;
    uint8_t h3 = udwData >> 16;
    uint8_t h4 = udwData >> 24;
    *(buf  + 0) = h4;
    *(buf  + 1) = h3;
    *(buf  + 2) = h2;
    *(buf  + 3) = h1;	
}
void sBigSetHalfWord(uint8_t *buf,uint16_t uwData)
{
    uint8_t h1 = uwData;
    uint8_t h2 = uwData >> 8;
    *(buf + 0) = h2;
    *(buf + 1) = h1;
}
void sBigSetByte(uint8_t *buf,uint8_t ubdata) 
{
    *(buf + 0) = ubdata;
}

uint32_t swBigGetWord(uint8_t *buf)
{
    uint32_t h1 = buf[0]; 
    uint32_t h2 = buf[1];
    uint32_t h3 = buf[2];
    uint32_t h4 = buf[3];
    
    uint32_t udwData = h1 << 24 | h2 << 16 | h3 << 8 | h4;
    return udwData;
}
uint16_t shwBigGetHalfWord(uint8_t *buf)
{ 
    uint16_t h1 = buf[0];
    uint16_t h2 = buf[1];
	
    uint16_t uwData = h1 << 8 | h2;
    return uwData;
}
uint8_t sbBigGetByte(uint8_t *buf)
{
    return *(buf+0);
}

//小端序转换
void sLittleSetWord(uint8_t *buf,uint32_t udwData) 
{
    uint8_t h1 = udwData;
    uint8_t h2 = udwData >> 8;
    uint8_t h3 = udwData >> 16;
    uint8_t h4 = udwData >> 24;
    *(buf  + 0) = h1;
    *(buf  + 1) = h2;
    *(buf  + 2) = h3;
    *(buf  + 3) = h4;	
}
void sLittleSetHalfWord(uint8_t *buf,uint16_t uwData)
{
    uint8_t h1 = uwData;
    uint8_t h2 = uwData >> 8;
    *(buf + 0) = h1;
    *(buf + 1) = h2;
}
void sLittleSetByte(uint8_t *buf,uint8_t ubdata) 
{
    *(buf + 0) = ubdata;
}

uint32_t swLittleGetWord(uint8_t *buf)
{
    uint32_t h1 = buf[3]; 
    uint32_t h2 = buf[2];
    uint32_t h3 = buf[1];
    uint32_t h4 = buf[0];
    
    uint32_t udwData = h1 << 24 | h2 << 16 | h3 << 8 | h4;
    return udwData;
}
uint16_t shwLittleGetHalfWord(uint8_t *buf)
{ 
    uint16_t h1 = buf[1];
    uint16_t h2 = buf[0];
	
    uint16_t uwData = h1 << 8 | h2;
    return uwData;
}
uint8_t sbLittleGetByte(uint8_t *buf)
{
    return *(buf+0);
}



