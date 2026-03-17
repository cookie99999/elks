/*
 * Generic 8259 PIC on minimal breadboard system
 *
 * This file contains code used for a barebones 80C88 system.
 * 
 * 11 March 2026 cookie99999
 */

#include <linuxmt/config.h>
#include <linuxmt/errno.h>
#include <linuxmt/sched.h>
#include <linuxmt/types.h>

#include <arch/ports.h>
#include <arch/8018x.h>
#include <arch/io.h>
#include <arch/irq.h>

void initialize_irq(void)
{
  outb(0x13, PIC1_CMD); /* edge triggered, single PIC */
  outb(0x08, PIC1_DATA); /* vector offset (could probably do 8 like pc/xt, maybe change */
  outb(0x01, PIC1_DATA); /* no nesting+buffer, normal eoi, x86 mode */
  outb(0xff, PIC1_DATA); /* disable all IRQs */
}

void enable_irq(unsigned int irq)
{
    unsigned char mask;

    mask = ~(1 << (irq & 7));
    unsigned char cache_11 = inb(PIC1_DATA);
    cache_11 &= mask;
    outb(cache_11, PIC1_DATA);
}

int remap_irq(int irq)
{
    /* no remaps */
    return irq;
}

void disable_irq(unsigned int irq)
{
    flag_t flags;
    unsigned char mask = 1 << (irq & 7);

    save_flags(flags);
    clr_irq();
    unsigned char cache_11 = inb(PIC1_DATA);
    cache_11 |= mask;
    outb(cache_11, PIC1_DATA);
    restore_flags(flags);
}

// Get interrupt vector from IRQ
int irq_vector(int irq)
{
    return irq + 0x08;
}
