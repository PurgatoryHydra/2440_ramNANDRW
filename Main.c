#include "s3c2440.h"
#include "Damascus_GPIO.h"
#include "Damascus_UART.h"

uint32_t delay(void);

void Main()
{
	uint8_t i = 'a';
	Damascus_GPIO_Init(GPIOB, 10, GPIO_MODE_OUTPUT);
	Damascus_GPIO_Set(GPIOB, 10, false);
	Damascus_UART_Init_Polling(UART0, 115200);
	while(1)
	{
		Damascus_UART_SendString(UART0, "%d", i);
		i++;
		if(i > 'z')
			i = 'a';
		delay();
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
