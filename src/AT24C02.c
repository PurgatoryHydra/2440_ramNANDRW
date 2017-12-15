#include "AT24C02.h"
#include "Damascus_IIC.h"
#include "Damascus_UART.h"

void AT24C02_Init(void)
{
	Damascus_IIC_Init();
}

void AT24C02_Delay()
{
	uint8_t i,j;
	for(i = 0 ; i < 100; i++)
		for(j = 0; j < 100; j++);
}

void AT24C02_WriteByte(uint8_t address, uint8_t data)
{
	Damascus_IIC_WriteStart(0xa0);
	Damascus_IIC_WriteByte(address);
	Damascus_IIC_WriteByte(data);
	Damascus_IIC_WriteStop();
	AT24C02_Delay();
}

void AT24C02_WritePage(uint8_t pageNumber, uint8_t *data)
{
	uint8_t i;
	Damascus_IIC_WriteStart(0xa0);
	Damascus_IIC_WriteByte(pageNumber);
	for(i = 0; i < 8; i++)
		Damascus_IIC_WriteByte(*(data + i));
	Damascus_IIC_WriteStop();
	AT24C02_Delay();
}

uint8_t AT24C02_ReadCurrentAddress()
{
	uint8_t data;
	Damascus_IIC_ReadStart(0xa1);
	data = Damascus_IIC_ReadByte(1);
	Damascus_IIC_ReadStop();

	return data;
}

uint8_t AT24C02_ReadByte(uint8_t address)
{
	uint8_t data;
	Damascus_IIC_WriteStart(0xa0);
	Damascus_IIC_WriteByte(address);
	Damascus_IIC_ReadStart(0xa0);
	/* Address match signal. */
	Damascus_IIC_ReadByte(0);
	data = Damascus_IIC_ReadByte(1);
	Damascus_IIC_ReadStop();

	return data;
}
