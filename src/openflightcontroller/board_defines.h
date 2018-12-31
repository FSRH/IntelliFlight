#ifndef BOARD_DEFINES_H
#define BOARD_DEFINES_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>

#include <stdint.h>
#include <stddef.h>

#include "openflightcontroller/clock.h"
#include "openflightcontroller/gpio.h"
#include "openflightcontroller/timer.h"
#include "openflightcontroller/usart.h"

/*! @name spi-header structure */
struct spi_interface {
    rcc_periph_clken clken;
    uint32_t spi;
};

/*! @name pin-definition structure */
struct gpio_pin {
    rcc_periph_clken clken;
    uint8_t alt_func_num;
    uint32_t gpioport;
    uint16_t gpios;
};

/*! @name external spi for th bmp280 and ak8963n sensor */
const spi_interface spi_bmp280_ak8963n{.clken = RCC_SPI2, .spi = SPI2};

const gpio_pin spi_miso_bmp280_ak8963n = {.clken = RCC_GPIOB, .alt_func_num = GPIO_AF5, .gpioport = GPIOB, .gpios = GPIO14};
const gpio_pin spi_mosi_bmp280_ak8963n = {.clken = RCC_GPIOB, .alt_func_num = GPIO_AF5, .gpioport = GPIOB, .gpios = GPIO15};
const gpio_pin spi_sck_bmp280_ak8963n = {.clken = RCC_GPIOD, .alt_func_num = GPIO_AF5, .gpioport = GPIOD, .gpios = GPIO3};
const gpio_pin spi_css_bmp280 = {.clken = RCC_GPIOD, .alt_func_num = NULL, .gpioport = GPIOD, .gpios = GPIO12};

/*! @name internal spi for th bmp280 and ak8963n sensor */
//const spi_interface spi_bmp280_ak8963n {.clken = RCC_SPI4, .spi = SPI4};
//
//const gpio_pin spi_mosi_bmp280_ak8963n {.clken = RCC_GPIOE, .alt_func_num = GPIO_AF5, .gpioport = GPIOE, .gpios = GPIO6};
//const gpio_pin spi_miso_bmp280_ak8963n {.clken = RCC_GPIOE, .alt_func_num = GPIO_AF5, .gpioport = GPIOE, .gpios = GPIO5};
//const gpio_pin spi_sck_bmp280_ak8963n {.clken = RCC_GPIOE, .alt_func_num = GPIO_AF5, .gpioport = GPIOE, .gpios = GPIO2};
//const gpio_pin spi_css_bmp280 {.clken = RCC_GPIOA, .alt_func_num = NULL, .gpioport = GPIOA, .gpios = GPIO2};
//const gpio_pin spi_css_ak896n {.clken = RCC_GPIOA, .alt_func_num = NULL, .gpioport = GPIOA, .gpios = GPIO3};

#endif //BOARD_DEFINES_H
