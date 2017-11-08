#ifndef __NAND_H
#define __NAND_H

#include "s3c2440.h"

/* S3C Nand Controller SFR */
#define 	NFCONF 		0x4E000000
#define 	NFCONT 		0x4E000004
#define 	NFCMMD 		0x4E000008
#define 	NFADDR 		0x4E00000C
#define 	NFDATA 		0x4E000010
#define 	NFMECCD0	0x4E000014
#define 	NFMECCD1	0x4E000018
#define 	NFECCD 		0x4E00001C
#define 	NFSTAT 		0x4E000020
#define 	NFESTAT0	0x4E000024
#define 	NFESTAT1	0x4E000028
#define 	NFMECC0		0x4E00002C
#define 	NFMECC1		0x4E000030
#define 	NFSECC 		0x4E000034
#define 	NFSBLK 		0x4E000038
#define 	NFEBLK 		0x4E00003C

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

#define 	NAND_CS_L() 		{__IO NFCONT &=~ (0x1 << 1);}
#define 	NAND_CS_H() 		{__IO NFCONT |=  (0x1 << 1);}

#define 	NAND_ENABLE_RB() 	{__IO NFSTAT |= (1 << 2);}
#define 	NAND_WAIT_BUSY() 	{while(!(__IO NFSTAT &( 0x1 << 0)));}

extern void NAND_Init();
extern void NAND_Reset();
extern void NAND_ReadID(uint8_t *data);
extern void NAND_EraseBlock(uint16_t block);
extern void NAND_ReadPage(uint16_t block, uint16_t pageIndex, uint8_t *buffer);
extern void NAND_WritePage(uint16_t block, uint16_t pageIndex, uint8_t *buffer);

#endif


