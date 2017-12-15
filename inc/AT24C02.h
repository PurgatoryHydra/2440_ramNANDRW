#ifndef __AT24C02_H
#define __AT24C02_H

#include "Damascus_IIC.h"

extern void AT24C02_Init(void);
extern void AT24C02_WriteByte(uint8_t address, uint8_t data);
extern void AT24C02_WritePage(uint8_t pageNumber, uint8_t *data);
extern uint8_t AT24C02_ReadByte(uint8_t address);

#endif
