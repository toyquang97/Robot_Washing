#include "Flash.h"

uint8_t lengthPage;

void delete_Buffer(char* data){
	uint8_t len = strlen(data);
	for(uint8_t i = 0;i < len; i++)
  {
		data[i] = 0;
  }
}

void Flash_Erase(uint32_t addr){
	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR |= FLASH_CR_PER;			//PAGE ERASE SET
	FLASH->AR = addr;
	FLASH->CR |= FLASH_CR_STRT;
	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->SR &= ~FLASH_SR_BSY;
	FLASH->CR &= ~FLASH_CR_PER; 
}

void Flash_Write_Int(uint32_t addr, int data){
	HAL_FLASH_Unlock();
	FLASH->CR |= FLASH_CR_PG;
	while((FLASH->SR&FLASH_SR_BSY));
	*(__IO uint16_t*)addr = data;
	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR &=~ FLASH_CR_PG;
	HAL_FLASH_Lock();
}

uint16_t Flash_Read_Int(uint32_t addr){
	uint16_t* val = (uint16_t*)addr;
	return *val;
}

void Flash_Write_Char(uint32_t addr, char *data){
	HAL_FLASH_Unlock();
	int i;
	FLASH->CR |= FLASH_CR_PG;
	int var = 0;
	lengthPage = strlen(data);
	for(i = 0; i < lengthPage;i+=1){
		while((FLASH->SR&FLASH_SR_BSY)){}
			var = (int)data[i];
		*(__IO uint16_t*)(addr + i*2) = var;
	}
	while((FLASH->SR&FLASH_SR_BSY)){};
	FLASH->CR &= ~FLASH_CR_PG;
	HAL_FLASH_Lock();
}

void Flash_ReadChar(char *dataOut, uint32_t addr1, uint32_t addr2){
	int check = 0;
	delete_Buffer(dataOut);
	if((unsigned char)Flash_Read_Int(addr2+(uint32_t)2)==255){
		check = (unsigned char)Flash_Read_Int(addr2)-48;
	}
	else{
		check = ((unsigned char)Flash_Read_Int(addr2)-48)*10 + (unsigned char)Flash_Read_Int(addr2+2)-48;
	}
	for(int i = 0;i < check;i++)
  {
		dataOut[i] = Flash_Read_Int(addr1 + (uint32_t)(i*2));
  }
}

void Flash_ProgramPage(char *dataIn, uint32_t addr1, uint32_t addr2){
	HAL_FLASH_Unlock();
	Flash_Erase(addr1);
	Flash_Write_Char(addr1,dataIn);
	HAL_Delay(100);
	char tempbuf[5] = {0};
	sprintf(tempbuf,"%d",lengthPage);
	HAL_FLASH_Unlock();
	Flash_Erase(addr2);
	Flash_Write_Char(addr2,tempbuf);
}
