/*
 * conio API for minimal 80C88 breadboard system w/ ST16C550
 *
 * This file contains code used for this system only.
 * 
 * 10 March 2026 cookie99999
 */

#include <linuxmt/config.h>
#include <arch/io.h>
#include <arch/irq.h>
#include "console.h"
#include "conio.h"

/* 16550 registers */
#define UART_RBR 0x00 /* receive buffer register */
#define UART_THR 0x00 /* transmitter holding register */
#define UART_FCR 0x02 /* FIFO control register */
#define UART_LCR 0x03 /* line control register */
#define UART_LSR 0x05 /* line status register */
#define UART_DLL 0x00 /* divisor latch low */
#define UART_DLH 0x01 /* divisor latch high */

/* initialize*/
void conio_init(void)
{
    outb(0x80, UART_LCR); /* enable divisor latch access */
    outb(0x00, UART_DLH);
    outb(0x0c, UART_DLL); /* 1.8432MHz / 12 = 9600 baud */
    outb(0x00, UART_LCR); /* disable divisor latch */
    outb(0x03, UART_LCR); /* 8n1 no break */
    outb(0x07, UART_FCR); /* clear and enable fifos */
}

/*
 * Poll for console input available.
 * Return nonzero character received else 0 if none ready.
 * Called approximately every ~8/100 seconds.
 */
int conio_poll(void)
{
    if (inb(UART_LSR) & 1) /* Receive data ready */
	return (inb(UART_RBR));
    return 0;
}

void conio_putc(byte_t c)
{
    while((inb(UART_LSR) & 0x60) == 0); /* tx buf empty */
    outb(c, UART_THR); /* tx buf */
}
