#ifndef __FLASH_H
#define __FLASH_H

#include "GlobalDef.h"

/*****升级标识*****/
#define E_UPDATE_FROM_NULL    0XFFFFFFFF //无升级事件
#define E_UPDATE_FROM_UDISK   0X11111111 //从U盘升级
#define E_UPDATE_FROM_EXFLASH 0X22222222 //从外部flash
#define E_UPDATE_FROM_SLEEP   0X33333333 //
#define E_UPDATE_FROM_DSGMOS  0X33333333 //因为放电Mos 关闭 重启
/******************/

#pragma pack(1)
typedef struct
{
    uint32_t uwTotalSize;
    uint16_t CheckSum;
}T_FILE_CONFIGDATA;
#pragma pack()//升级文件头信息

#pragma pack(1)
typedef struct
{
    uint32_t  UpdateFromFlag;   //升级来源标识
    uint32_t  UpdateType;       //升级类型
    uint32_t  ExFlashAddr;      //外部Flash地址(如果从外部flash升级)
    uint16_t  BootMode;         //启动方式
    uint16_t  UpdateFailReason; //升级失败原因
}T_INSFLASH_IAPDATA;/*****升级相关*****/
#pragma pack()

#define GD_FLASH_BASE          ((uint32_t)0x08000000)
#define GD_FLASH_SIZE          256 //页数 最多256页
#define GD_SECTOR_SIZE         ((uint16_t)0x800)//一页为2k

//******************用户定义区域***********************//
#define GDFLASH_APP_ADDR         0x08006000      // App程序运行地址  24K位置
#define GDFLASH_UPDATE_ADDR      0x08040000      // 升级文件地址     256的位置

#define GDFLASH_IAPUSERDATA_ADDR 0x0807A800                  //2K 升级数据信息  490
#define PAGE_FILE_RATEDATA_ADDR  0x0807B000                  //2K 额定参数      492
#define PAGE_FILE_CALIB_ADDR     0x0807B800                  //2K 校准参数      494
#define PAGE_FILE_PROTECT_ADDR   0x0807C000                  //2k 保护参数      496
#define PAGE_FILE_CONFIG_ADDR    0x0807C800                  //2k 配置参数      498
#define PAGE_FILE_KEY            0x0807D000                  //2K 密钥          500
//****************用户定义区域END**********************       //

uint8_t GDFLASH_Init(void);
void GDFLASH_WriteBuff(INT32U WriteAddr, uint8_t *pBuffer, INT32U NumToWrite);
void GDFLASH_ReadBuff(INT32U ReadAddr, INT8U *pBuffer, INT32U NumToRead);

void GDFlash_RwDisAble(void);

#endif
