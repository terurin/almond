// 作成者　terurin
// 用途　spiの通信(16bit)

#ifndef DRIVER_SPI_HEADER_GUARD
#define DRIVER_SPI_HEADER_GUARD
#include <stdbool.h>
#include <stdint.h>

void spi_init();

uint16_t spi_putw(uint16_t);
bool spi_full();
uint16_t spi_getw();
bool spi_empty(); 


#endif