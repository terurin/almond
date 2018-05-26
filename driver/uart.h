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

char uart_putc(char);
const char* uart_puts(const char*);
char uart_getc();
char* uart_gets(char*,size_t);
void uart_flush();







#endif