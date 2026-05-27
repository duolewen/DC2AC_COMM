#include "FLASH.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "string.h"

SemaphoreHandle_t xMutex_Flash    = 0;

INT32U GdFlashRead[GD_SECTOR_SIZE / 4] = {0};

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.

static INT32U GDFLASH_ReadWord(INT32U faddr)
{
    return *(__IO INT32U*)faddr; 
}

static INT8U GDFLASH_ReadByte(INT32U faddr)
{
    return *(__IO INT8U*)faddr; 
}


//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
static void GDFLASH_Write_NoCheck(INT32U WriteAddr, INT32U *pBuffer, INT32U NumToWrite)   
{
    INT32U i;
    for(i = 0; i < NumToWrite; i++)
    {
        fmc_word_program(WriteAddr, pBuffer[i]);
        WriteAddr += 4;//地址增加2.
    }
}

void GDFLASH_Read(INT32U ReadAddr, INT32U *pBuffer, INT32U NumToRead)  
{
    INT16U i;
    
    for(i=0; i < NumToRead; i++)
    {
        pBuffer[i] = GDFLASH_ReadWord(ReadAddr);  //读取4个字节.
        ReadAddr += 4;   //偏移4个字节.
    }

}

uint8_t GDFLASH_Init(void)
{
    xMutex_Flash = xSemaphoreCreateMutex();
    return (xMutex_Flash != 0);
}

void GDFLASH_WriteBuff(INT32U WriteAddr, uint8_t *pBuffer, INT32U NumToWrite) //读取指定buff 写入   地址必须为页地址 否则无法写入
{
    uint32_t Page     = 0;
    uint32_t PageAddr = 0;
    
    uint32_t ActualLen = 0;
    uint32_t TotalWriLen = 0;

    xSemaphoreTake(xMutex_Flash, portMAX_DELAY) ;
    
    if(WriteAddr < GD_FLASH_BASE  //地址小于基地址
       || WriteAddr >= GD_FLASH_BASE + (GD_SECTOR_SIZE * GD_FLASH_SIZE)
       || WriteAddr + NumToWrite >= GD_FLASH_BASE + (GD_SECTOR_SIZE * GD_FLASH_SIZE))// 或者长度超限
    {
        xSemaphoreGive(xMutex_Flash) ;
        return;
    }
    fmc_unlock();
    ob_unlock();

    while(NumToWrite)
    {
        Page     = (WriteAddr - GD_FLASH_BASE) / GD_SECTOR_SIZE ;//页编号
        PageAddr = GD_FLASH_BASE + Page * GD_SECTOR_SIZE; //页地址
    
        ActualLen = GD_SECTOR_SIZE - (WriteAddr - PageAddr);//真实写入长度
        ActualLen = NumToWrite < ActualLen?NumToWrite:ActualLen; //长度不够写了
        
        fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
        
        memset((uint8_t *)&GdFlashRead,0,sizeof(GdFlashRead));
        GDFLASH_Read(PageAddr, GdFlashRead, GD_SECTOR_SIZE / 4);//读出整个页的内容
    
        uint8_t ubCompareRet = 0;
        for(uint32_t i = 0; i <  GD_SECTOR_SIZE / 4 ; i++)//校验数据
        {
          if(GdFlashRead[i] != 0xFFFFFFFF)
          {
             ubCompareRet = 1;
             break;//需要擦除
          }
        }
        if(ubCompareRet)//需要擦除
            fmc_page_erase(PageAddr);//擦除这个扇区
    
        uint8_t *pCopyAddr = ( uint8_t *)GdFlashRead;
        memcpy(pCopyAddr + (WriteAddr - PageAddr ) ,pBuffer + TotalWriLen ,ActualLen);

        fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
        GDFLASH_Write_NoCheck(PageAddr , GdFlashRead, GD_SECTOR_SIZE / 4);//写入整个扇区
        
        TotalWriLen += ActualLen;
        
        WriteAddr += ActualLen;
        NumToWrite -= ActualLen;
    }
    ob_lock();
    fmc_lock();//上锁
    
    xSemaphoreGive(xMutex_Flash) ;
}

void GDFLASH_ReadBuff(INT32U ReadAddr, INT8U *pBuffer, INT32U NumToRead)  
{
    xSemaphoreTake(xMutex_Flash, portMAX_DELAY) ;
    for(uint32_t i = 0;i < NumToRead;i++)
    {
        pBuffer[i] = GDFLASH_ReadByte(ReadAddr+i);
    }
    xSemaphoreGive(xMutex_Flash) ;
}

void GDFlash_RwDisAble(void)
{
   xSemaphoreTake(xMutex_Flash, portMAX_DELAY) ;
}

