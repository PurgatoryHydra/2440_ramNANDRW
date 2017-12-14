#include "nand.h"
#include "Damascus_UART.h"
#include "Damascus_GPIO.h"

extern uint32_t delay(void);

void NAND_Init()
{
	Damascus_GPIO_Init(GPA, 17, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPA, 18, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPA, 19, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPA, 20, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPA, 21, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPA, 22, GPIO_MODE_SPECIAL0);
	
	NAND->CONF |= (1 << 12) | (4 << 8) | (1 << 4);
	NAND->CONT &=~ (1 << 12);
	NAND->CONT |= (1 << 0);
	NAND->STAT &=~ 0xFFFFFFFF;
}

void NAND_Reset()
{
	NAND_CS_L();
	NAND_ENABLE_RB();
	NAND->CMMD = NAND_CMD_RESET;
	NAND_WAIT_BUSY();
	NAND_CS_H();
}

void NAND_ReadID(uint32_t *data)
{
	NAND_CS_L();
	NAND_ENABLE_RB();
	NAND->CMMD = NAND_CMD_READID;
	NAND->ADDR = 0x00;
	delay();
	delay();
	delay();
	delay();
	delay();
	*data++ = NAND->DATA;
	*data++ = NAND->DATA;
	*data++ = NAND->DATA;
	*data++ = NAND->DATA;
	*data++ = NAND->DATA;
	NAND_CS_H();
}

void NAND_EraseBlock(uint16_t block)
{
	uint32_t blockIndex = block << 6;
	uint8_t status = 0;
	NAND_Reset();
	NAND_CS_L();
	NAND_ENABLE_RB();
	NAND->CMMD = NAND_CMD_ERASE1;
	NAND->ADDR = blockIndex & 0xFF;
	NAND->ADDR = (blockIndex >> 8) & 0xFF;
	NAND->ADDR = (blockIndex >> 16) & 0xFF;
	NAND->CMMD = NAND_CMD_ERASE2;
	delay();
	NAND_WAIT_BUSY();
	NAND->CMMD = NAND_CMD_STATUS;
	status = NAND->DATA;
	if(status & 0x01)
	{
		Damascus_UART_SendString(UART0, "erase in error.\r\n");
	}
	else
	{
		Damascus_UART_SendString(UART0, "erase succeeded with code: %02x.\r\n", status);
	}
	NAND_CS_H();
}

void NAND_ReadPage(uint16_t block, uint16_t pageIndex, uint32_t *buffer)
{
	uint32_t page = (block << 6) + pageIndex;
	uint16_t i;
	NAND_Reset();
	NAND_CS_L();
	NAND_ENABLE_RB();
	NAND->CMMD = NAND_CMD_READ1;
	NAND->ADDR = 0x00;
	NAND->ADDR = 0x00;
	NAND->ADDR = page & 0xFF;
	NAND->ADDR = (page >> 8) & 0xFF;
	NAND->ADDR = (page >> 16) & 0x01;
	NAND->CMMD = NAND_CMD_READ2;
	NAND_WAIT_BUSY();
	for(i = 0; i  < PAGE_SIZE; i++)
	{
		*buffer++ = NAND->DATA;
	}
	NAND_CS_H();
}

void NAND_WritePage(uint16_t block, uint16_t pageIndex, uint32_t *buffer)
{
	uint32_t page = (block << 6) + pageIndex;
	uint16_t i;
	uint8_t status;
	NAND_Reset();
	NAND_CS_L();
	NAND_ENABLE_RB();
	NAND->CMMD = NAND_CMD_WRITE1;
	NAND->ADDR = 0x0;
	NAND->ADDR = 0x0;
	NAND->ADDR = page & 0xFF;
	NAND->ADDR = (page >> 8)  & 0xFF;
	NAND->ADDR = (page >> 16) & 0x01;
	for(i = 0;i < PAGE_SIZE; i++)
	{
		NAND->DATA = *buffer++;
	}
	NAND->CMMD = NAND_CMD_WRITE2;
	NAND_WAIT_BUSY();
	NAND->CMMD = NAND_CMD_STATUS;
	status = NAND->DATA;
	if(status & 0x01)
	{
		Damascus_UART_SendString(UART0, "erase in error.\r\n");
	}
	else
	{
		Damascus_UART_SendString(UART0, "write succeeded with code: %02x.\r\n", status);
	}
	NAND_CS_H();
}

void NAND_MovingFromRAMToNANDStart(uint32_t *src, int32_t size)
{
	uint16_t counterPage = 0;
	NAND_EraseBlock(0);
	for(; size > 0; size -= 512)
	{
		NAND_WritePage(0, counterPage, src);
		counterPage++;
		src += 512;
		Damascus_UART_SendString(UART0, "write NAND count %d size: %d \n.\r\n", counterPage, size * 4);
	}
	Damascus_UART_SendString(UART0, "moved %d bytes from ram to nand 0.\r\n", counterPage*512*4);
}

