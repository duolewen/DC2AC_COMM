#include "Untility.h"

/*
函数功能:   对字节的比特位进行倒序
函数参数:   需要倒序的单字节
函数返回值：倒序后的单字节
*/
uint8_t sbExchangeByte(uint8_t ubByte)
{
        ubByte = (ubByte & 0xaa) >> 1 | (ubByte & 0x55) << 1;
        ubByte = (ubByte & 0xcc) >> 2 | (ubByte & 0x33) << 2;
        ubByte = (ubByte & 0xf0) >> 4 | (ubByte & 0x0f) << 4;

        return ubByte;
}
