#include "s3c2440.h"
#include "Damascus_GPIO.h"
#include "Damascus_UART.h"
#include "Damascus_IIC.h"
#include "nand.h"
#include "dm9000.h"
#include "AT24C02.h"

uint32_t delay(void);

#define TRANSFER_SIZE 51200

uint32_t bufferToTransfer[TRANSFER_SIZE] = {0};

void Main()
{
	uint32_t i = 'a';
	uint8_t dataToSend[8] = {0x45, 0x5f, 0x72, 0x10, 0x34, 6, 7, 8};
	Damascus_GPIO_Init(GPB, 10, GPIO_MODE_OUTPUT);
	Damascus_GPIO_Set(GPB, 10, false);
	Damascus_UART_Init_Polling(UART0, 115200);
	Damascus_UART_SendString(UART0, "start\r\n");
	AT24C02_Init();
	AT24C02_WritePage(0x30, dataToSend);
	for(i = 0; i < 8 ; i++)
		Damascus_UART_SendString(UART0, "read data: %x\r\n", AT24C02_ReadByte(0x30 + i));
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
	Damascus_UART_SendString(UART0, "raise: Signal # %d caught\n", signum);
	return 0;
}
