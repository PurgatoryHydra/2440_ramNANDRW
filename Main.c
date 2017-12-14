#include "s3c2440.h"
#include "Damascus_GPIO.h"
#include "Damascus_UART.h"
#include "nand.h"
#include "dm9000.h"

uint32_t delay(void);

#define TRANSFER_SIZE 51200

uint32_t bufferToTransfer[TRANSFER_SIZE] = {0};

void Main()
{
	uint32_t i = 'a';
	Damascus_GPIO_Init(GPB, 10, GPIO_MODE_OUTPUT);
	Damascus_GPIO_Set(GPB, 10, false);
	Damascus_UART_Init_Polling(UART0, 115200);
	Damascus_UART_SendString(UART0, "UART0 initialized with baudrate 115200... \n\r");
	NAND_Init();
	Damascus_UART_SendString(UART0, "NAND Flash Controller initialized...\n\r");
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
