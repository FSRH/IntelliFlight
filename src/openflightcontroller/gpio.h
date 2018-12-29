#ifndef INTELLIFLIGHT_GPIO_H
#define INTELLIFLIGHT_GPIO_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void gpioSetup(void); //
static void gpioSet(void);      //Gpio Enum z.B SD-Card 1
static void gpioClear(void);    //-"-
static bool gpioGet(void);      //-"-

static void gpioSetup(void) {
    /** Input Setup */
    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_INPUT, GPIO_OTYPE_PP, GPIO0 | GPIO1 | GPIO4);

    /** Output Setup */
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO2 | GPIO3 | GPIO4);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO2 | GPIO3 | GPIO4);

    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO2);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO2);

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO13);
    gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO13);

    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO12);
    gpio_set_output_options(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO12);

    rcc_periph_clock_enable(RCC_GPIOE);
    gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO14);
    gpio_set_output_options(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO14);
}

//static void gpioSet(void) {
//
//}
//
//static void gpioClear(void) {
//
//}
//
//static bool gpioGet(void) {
//
//}

#endif //INTELLIFLIGHT_GPIO_H
