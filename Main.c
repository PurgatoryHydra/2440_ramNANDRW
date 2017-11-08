#include "s3c2440.h"
#include "Damascus_GPIO.h"
#include "Damascus_UART.h"
#include "nand.h"

uint32_t delay(void);

uint8_t bufferToWrite[512] = {0x12};
uint8_t bufferToRead[512] = {0x34};
uint8_t bufferID[5] = {0, 0, 0, 0, 0};

void Main()
{
	uint16_t i = 'a';
	Damascus_GPIO_Init(GPIOB, 10, GPIO_MODE_OUTPUT);
	Damascus_GPIO_Set(GPIOB, 10, false);
	Damascus_UART_Init_Polling(UART0, 115200);
	Damascus_UART_SendString(UART0, "UART0 initialized with baudrate 115200... \n\r");
	NAND_Init();
	Damascus_UART_SendString(UART0, "NAND Flash Controller initialized...\n\r");
	
	for(i = 512; i > 0; i--)
	{
		bufferToWrite[i] = i % 256;
	}
	NAND_EraseBlock(17);
	NAND_WritePage(17, 1, bufferToWrite);
	NAND_ReadPage(17, 1, bufferToRead);
	Damascus_UART_SendString(UART0, "Read data: \n\r");
	for(i = 0; i < 512; i++)
	{
		Damascus_UART_SendString(UART0, "%02x ", bufferToRead[i]);
		if(!((i + 1) % 32))
		{
			Damascus_UART_SendString(UART0, "\n\r");
		}
	}
	while(1)
	{
	}
}

uint32_t delay()
{
	uint32_t n = 10000, m = 10000, data = 0;
	for(;n > 0;n--)
	{
		for(;m > 0;m--)
		{
			data += 1;
		}
	}
	return data;
}

int raise(int signum)
{
	//printf("raise: Signal # %d caught\n", signum);
	return 0;
}
