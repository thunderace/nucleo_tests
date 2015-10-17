/*
 * Copyright (c) 2015 Francesco Balducci
 *
 * This file is part of nucleo_tests.
 *
 *    nucleo_tests is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    nucleo_tests is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with nucleo_tests.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "syscalls.h"
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>


static
int stdio_usart_write(int fd, char *ptr, int len)
{
    int i;
    /* TODO: non-blocking */
    for(i = 0; i < len; i++)
    {
        usart_send_blocking(USART2, ptr[i]);
    }
    return len;
}

static
int stdio_usart_read(int fd, char *ptr, int len)
{
    /* TODO */
    return 0;
}

static
void stdio_usart_init(void)
{
    uint32_t baud = 57600;
    uint32_t clock = 8000000;
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2);
    
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);
	USART2_BRR = ((2 * clock) + baud) / (2 * baud);
    
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	usart_enable(USART2);
}


static
void fileno_out_init(int fd)
{
    struct fd *f;
    f = syscall_get_file_struct(fd);

    f->stat.st_mode = _IFCHR;
    f->write = stdio_usart_write;
    f->read = stdio_usart_read;
    f->isatty = 1;
    f->isopen = 1;
}

__attribute__((__constructor__))
void stdio_init(void)
{
    stdio_usart_init();
    fileno_out_init(STDOUT_FILENO);
    fileno_out_init(STDERR_FILENO);
}

static
void fileno_out_delete(int fd)
{
    struct fd *f;
    f = syscall_get_file_struct(fd);
    f->isopen = 0;
}

__attribute__((__destructor__))
void stdio_delete(void)
{
    fileno_out_delete(STDOUT_FILENO);
    fileno_out_delete(STDERR_FILENO);
}
