#ifndef W5500_HAL_H
#define W5500_HAL_H

#include "stdint.h"

void wizchip_select(void);
void wizchip_deselect(void);
void wizchip_write(uint8_t byte);
uint8_t wizchip_read(void);

#endif
