#ifndef __NAND_H
#define __NAND_H

#include "s3c2440.h"

/* K9F Command */
#define 	NAND_CMD_READ1 	0x00
#define 	NAND_CMD_READ2 	0x30
#define 	NAND_CMD_READID	0x90
#define 	NAND_CMD_WRITE1	0x80
#define 	NAND_CMD_WRITE2	0x10
#define 	NAND_CMD_ERASE1	0x60
#define 	NAND_CMD_ERASE2	0xD0
#define 	NAND_CMD_STATUS	0x70
#define 	NAND_CMD_RESET 	0xFF

#define 	NAND_CS_L() 		{NAND->CONT &=~ (0x1 << 1);}
#define 	NAND_CS_H() 		{NAND->CONT |=  (0x1 << 1);}

#define 	NAND_ENABLE_RB() 	{NAND->STAT |= (1 << 2);}
#define 	NAND_WAIT_BUSY() 	{while(!(NAND->STAT &( 0x1 << 0)));}

#define PAGE_SIZE 512  //512*32 bits

extern void NAND_Init();
extern void NAND_Reset();
extern void NAND_ReadID(uint32_t *data);
extern void NAND_EraseBlock(uint16_t block);
extern void NAND_ReadPage(uint16_t block, uint16_t pageIndex, uint32_t *buffer);
extern void NAND_WritePage(uint16_t block, uint16_t pageIndex, uint32_t *buffer);
extern void NAND_MovingFromRAMToNANDStart(uint32_t *src, int32_t size);

#endif


