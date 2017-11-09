#include "nand.h"
#include "Damascus_UART.h"
#include "Damascus_GPIO.h"

extern uint32_t delay(void);

void NAND_Init()
{
	Damascus_GPIO_Init(GPIOA, 17, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPIOA, 18, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPIOA, 19, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPIOA, 20, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPIOA, 21, GPIO_MODE_SPECIAL0);
	Damascus_GPIO_Init(GPIOA, 22, GPIO_MODE_SPECIAL0);
	
	__IO NFCONF |= (1 << 12) | (4 << 8) | (1 << 4);
	__IO NFCONT &=~ (1 << 12);
	__IO NFCONT |= (1 << 0);
	__IO NFSTAT &=~ 0xFFFFFFFF;
}

void NAND_Reset()
{
	NAND_CS_L();
	NAND_ENABLE_RB();
	__IO NFCMMD = NAND_CMD_RESET;
	NAND_WAIT_BUSY();
	NAND_CS_H();
}

void NAND_ReadID(uint32_t *data)
{
	NAND_CS_L();
	NAND_ENABLE_RB();
	__IO NFCMMD = NAND_CMD_READID;
	__IO NFADDR = 0x00;
	delay();
	delay();
	delay();
	delay();
	delay();
	*data++ = __IO NFDATA;
	*data++ = __IO NFDATA;
	*data++ = __IO NFDATA;
	*data++ = __IO NFDATA;
	*data++ = __IO NFDATA;
	NAND_CS_H();
}

void NAND_EraseBlock(uint16_t block)
{
	uint32_t blockIndex = block << 6;
	uint8_t status = 0;
	NAND_Reset();
	NAND_CS_L();
	NAND_ENABLE_RB();
	__IO NFCMMD = NAND_CMD_ERASE1;
	__IO NFADDR = blockIndex & 0xFF;
	__IO NFADDR = (blockIndex >> 8) & 0xFF;
	__IO NFADDR = (blockIndex >> 16) & 0xFF;
	__IO NFCMMD = NAND_CMD_ERASE2;
	delay();
	NAND_WAIT_BUSY();
	__IO NFCMMD = NAND_CMD_STATUS;
	status = __IO NFDATA;
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
	__IO NFCMMD = NAND_CMD_READ1;
	__IO NFADDR = 0x00;
	__IO NFADDR = 0x00;
	__IO NFADDR = page & 0xFF;
	__IO NFADDR = (page >> 8) & 0xFF;
	__IO NFADDR = (page >> 16) & 0x01;
	__IO NFCMMD = NAND_CMD_READ2;
	NAND_WAIT_BUSY();
	for(i = 0; i  < PAGE_SIZE; i++)
	{
		*buffer++ = __IO NFDATA;
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
	__IO NFCMMD = NAND_CMD_WRITE1;
	__IO NFADDR = 0x0;
	__IO NFADDR = 0x0;
	__IO NFADDR = page & 0xFF;
	__IO NFADDR = (page >> 8)  & 0xFF;
	__IO NFADDR = (page >> 16) & 0x01;
	for(i = 0;i < PAGE_SIZE; i++)
	{
		__IO NFDATA = *buffer++;
	}
	__IO NFCMMD = NAND_CMD_WRITE2;
	NAND_WAIT_BUSY();
	__IO NFCMMD = NAND_CMD_STATUS;
	status = __IO NFDATA;
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

