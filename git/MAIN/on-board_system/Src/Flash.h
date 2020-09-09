#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"

/********/
#define DATA_ADDRESS		((uint32_t)0x0801FC00)
#define LENGTH_START_ADDRESS	((uint32_t)0x0801F810)
/********/
void delete_Buffer(char* data);
void Flash_Erase(uint32_t addr);
void Flash_Write_Int(uint32_t addr, int data);
uint16_t Flash_Read_Int(uint32_t addr);
void Flash_Write_Char(uint32_t addr, char *data);
void Flash_ReadChar(char *dataOut, uint32_t addr1, uint32_t addr2);
void Flash_ProgramPage(char *dataIn, uint32_t addr1, uint32_t addr2);

#endif