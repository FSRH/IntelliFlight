#ifndef CONSOLE_H
#define CONSOLE_H

#include <libopencm3/stm32/rcc.h>
#include <setjmp.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f7/nvic.h>
#include <libopencm3/cm3/systick.h>

void console_putc(char c);
char console_getc(int wait);
void console_puts(char *s);
int console_gets(char *s, int len);

/* this is for fun, if you type ^C to this example it will reset */
//#define RESET_ON_CTRLC

#ifdef RESET_ON_CTRLC

/* Jump buffer for setjmp/longjmp */
jmp_buf	jump_buf;

static void do_the_nasty(void);
/*
 * do_the_nasty
 *
 * This is a hack to implement the equivalent of a signal interrupt
 * in a system without signals or a kernel or scheduler. Basically
 * when the console_getc() function reads a '^C' character, it munges
 * the return address of the interrupt to here, and then this function
 * does a longjump to the last place we did a setjmp.
 */
static void do_the_nasty(void)
{
    longjmp(jump_buf, 1);
    while (1);
}
#endif

/* This is a ring buffer to holding characters as they are typed
 * it maintains both the place to put the next character received
 * from the UART, and the place where the last character was
 * read by the program. See the README file for a discussion of
 * the failure semantics.
 */
#define RECV_BUF_SIZE	128		/* Arbitrary buffer size */
char recv_buf[RECV_BUF_SIZE];
volatile int recv_ndx_nxt;		/* Next place to store */
volatile int recv_ndx_cur;		/* Next place to read */

/* For interrupt handling we add a new function which is called
 * when recieve interrupts happen. The name (UART7_isr) is created
 * by the irq.json file in libopencm3 calling this interrupt for
 * UART7 'UART7', adding the suffix '_isr', and then weakly binding
 * it to the 'do nothing' interrupt function in vec.c.
 *
 * By defining it in this file the linker will override that weak
 * binding and instead bind it here, but you have to get the name
 * right or it won't work. And you'll wonder where your interrupts
 * are going.
 */
void uart7_isr(void)
{
    uint32_t	reg;
    int			i;

    do {
        reg = USART_ISR(CONSOLE_UART);
        if (reg & USART_FLAG_RXNE) {
            recv_buf[recv_ndx_nxt] = USART_RDR(CONSOLE_UART);
#ifdef RESET_ON_CTRLC
            /* Check for "reset" */
            if (recv_buf[recv_ndx_nxt] == '\003') {
                /* reset the system volatile definition of
                 * return address on the stack to insure it
                 * gets stored, changed to point to the
                 * trampoline function (do_the_nasty) which is
                 * required because we need to return of an
                 * interrupt to get the internal value of the
                 * LR register reset and put the processor back
                 * into "Thread" mode from "Handler" mode.
                 *
                 * See the PM0214 Programming Manual for Cortex
                 * M, pg 42, to see the format of the Cortex M4
                 * stack after an interrupt or exception has
                 * occurred.
                 */
                volatile uint32_t *ret = (&reg) + 7;

                *ret = (uint32_t) &do_the_nasty;
                return;
            }
#endif
            /* Check for "overrun" */
            i = (recv_ndx_nxt + 1) % RECV_BUF_SIZE;
            if (i != recv_ndx_cur) {
                recv_ndx_nxt = i;
            }
        }
    } while ((reg & USART_ISR_RXNE) != 0); /* can read back-to-back
						 interrupts */
}

/*
 * console_putc(char c)
 *
 * Send the character 'c' to the USART, wait for the USART
 * transmit buffer to be empty first.
 */
void console_putc(char c)
{
    uint32_t	reg;
    do {
        reg = USART_ISR(CONSOLE_UART);
    } while ((reg & USART_ISR_TXE) == 0);
    USART_TDR(CONSOLE_UART) = (uint16_t) c & 0xff;
}

/*
 * char = console_getc(int wait)
 *
 * Check the console for a character. If the wait flag is
 * non-zero. Continue checking until a character is received
 * otherwise return 0 if called and no character was available.
 *
 * The implementation is a bit different however, now it looks
 * in the ring buffer to see if a character has arrived.
 */
char console_getc(int wait)
{
    char		c = 0;

    while ((wait != 0) && (recv_ndx_cur == recv_ndx_nxt));
    if (recv_ndx_cur != recv_ndx_nxt) {
        c = recv_buf[recv_ndx_cur];
        recv_ndx_cur = (recv_ndx_cur + 1) % RECV_BUF_SIZE;
    }
    return c;
}

/*
 * void console_puts(char *s)
 *
 * Send a string to the console, one character at a time, return
 * after the last character, as indicated by a NUL character, is
 * reached.
 */
void console_puts(char *s)
{
    while (*s != '\000') {
        console_putc(*s);
        /* Add in a carraige return, after sending line feed */
        if (*s == '\n') {
            console_putc('\r');
        }
        s++;
    }
}

/*
 * int console_gets(char *s, int len)
 *
 * Wait for a string to be entered on the console, limited
 * support for editing characters (back space and delete)
 * end when a <CR> character is received.
 */
int console_gets(char *s, int len)
{
    char *t = s;
    char c;

    *t = '\000';
    /* read until a <CR> is received */
    while ((c = console_getc(1)) != '\r') {
        if ((c == '\010') || (c == '\127')) {
            if (t > s) {
                /* send ^H ^H to erase previous character */
                console_puts("\010 \010");
                t--;
            }
        } else {
            *t = c;
            console_putc(c);
            if ((t - s) < len) {
                t++;
            }
        }
        /* update end of string with NUL */
        *t = '\000';
    }
    return t - s;
}

void countdown(void);

/*
 * countdown
 *
 * Count down for 20 seconds to 0.
 *
 * This provides an example function which is constantly
 * printing for 20 seconds and not looking for typed characters.
 * however with the interrupt driven receieve queue you can type
 * ^C while it is counting down and it will be interrupted.
 */
void countdown(void)
{
    int i = 200;
    while (i-- > 0) {
        console_puts("Countdown: ");
        console_putc((i / 600) + '0');
        console_putc(':');
        console_putc(((i % 600) / 100) + '0');
        console_putc((((i % 600) / 10) % 10) + '0');
        console_putc('.');
        console_putc(((i % 600) % 10) + '0');
        console_putc('\r');
        msleep(100);
    }
}

#endif //CONSOLE_H
