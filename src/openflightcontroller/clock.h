#ifndef INTELLIFLIGHT_RCC_H
#define INTELLIFLIGHT_RCC_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

static volatile uint32_t system_millis;

void msleep(uint32_t delay);
uint32_t mtime(void);
static void clockSetup(void);

static void clockSetup(void) {
    /* Setup the hse as main clock and the internal clock frequency to 168 MHz */
    rcc_clock_setup_hse(&rcc_3v3[RCC_CLOCK_3V3_168MHZ], 25);

    /* clock rate / 168000 to get 1mS interrupt rate */
    systick_set_reload(168000);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();

    /* this done last */
    systick_interrupt_enable();

    /* Enable all gpio ports */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
}

/* sleep for delay milliseconds */
void msleep(uint32_t delay)
{
    uint32_t wake = system_millis + delay;
    while (wake > system_millis);
}

/* Getter function for the current time */
uint32_t mtime(void)
{
    return system_millis;
}

#endif //INTELLIFLIGHT_RCC_H
