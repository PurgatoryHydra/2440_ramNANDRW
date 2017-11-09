#include "s3c2440.h"
#include "Damascus_GPIO.h"
#include "Damascus_UART.h"
#include "nand.h"

uint32_t delay(void);

#define TRANSFER_SIZE 3000

uint32_t bufferToTransfer[TRANSFER_SIZE] = {0};

void Main()
{
	uint32_t i = 'a';
	Damascus_GPIO_Init(GPIOB, 10, GPIO_MODE_OUTPUT);
	Damascus_GPIO_Set(GPIOB, 10, false);
	Damascus_UART_Init_Polling(UART0, 115200);
	Damascus_UART_SendString(UART0, "UART0 initialized with baudrate 115200... \n\r");
	NAND_Init();
	Damascus_UART_SendString(UART0, "NAND Flash Controller initialized...\n\r");
	
	Damascus_UART_SendString(UART0, "copying from SDRAM. \r\n");
	for(i = 0;i < TRANSFER_SIZE; i++)
	{
		bufferToTransfer[i] = __IO (0x32000000+i*4);
	}
	Damascus_UART_SendString(UART0, "copyed from SDRAM. \r\n");
	NAND_WritePage(0, 0, bufferToTransfer);
	NAND_MovingFromRAMToNANDStart(bufferToTransfer, TRANSFER_SIZE);	
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
