/*
 *  作成 terurin
 *  用途 uartを介しての通信
 */

#pragma once
#ifndef DRIVER_UART_HEADER_GUARD
#define DRIVER_UART_HEADER_GUARD

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

void uart_init();

bool uart_empty();
char uart_putc(char);
char uart_getc();
/*
const uint8_t* uart_write(const uint8_t*,size_t);
const char* uart_puts(const char*);

*/
/*
uint8_t* uart_read(uint8_t*,size_t);
char* uart_gets(char*,size_t);

void uart_flush();
*/

#endif