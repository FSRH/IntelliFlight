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

/****************************************************************/
/*! @name		spi-header structure            */
/****************************************************************/
struct spi_interface {
    rcc_periph_clken clken;
    uint32_t spi;
};

/****************************************************************/
/*! @name		pin-definition structure
 *  @note       with alternative functions      */
/****************************************************************/
struct gpio_pin_altf {
    rcc_periph_clken clken;
    uint8_t alt_func_num;
    uint32_t gpioport;
    uint16_t gpios;
};

/****************************************************************/
/*! @name		pin-definition structure
 *  @note       without alternative functions   */
/****************************************************************/
struct gpio_pin {
    rcc_periph_clken clken;
    uint32_t gpioport;
    uint16_t gpios;
};

/*! @name external spi for the bmp280 and ak8963n sensor */
//const spi_interface spi_bmp280_ak8963n{.clken = RCC_SPI2, .spi = SPI2};
//const gpio_pin_altf spi_miso_bmp280_ak8963n = {.clken = RCC_GPIOB, .alt_func_num = GPIO_AF5, .gpioport = GPIOB, .gpios = GPIO14};
//const gpio_pin_altf spi_mosi_bmp280_ak8963n = {.clken = RCC_GPIOB, .alt_func_num = GPIO_AF5, .gpioport = GPIOB, .gpios = GPIO15};
//const gpio_pin_altf spi_sck_bmp280_ak8963n = {.clken = RCC_GPIOD, .alt_func_num = GPIO_AF5, .gpioport = GPIOD, .gpios = GPIO3};
//const gpio_pin spi_css_bmp280 = {.clken = RCC_GPIOD, .gpioport = GPIOD, .gpios = GPIO12};

/*! @name internal spi for the bmp280 and ak8963n sensor */
const spi_interface spi_bmp280_ak8963n{.clken = RCC_SPI4, .spi = SPI4};
const gpio_pin_altf spi_mosi_bmp280_ak8963n{.clken = RCC_GPIOE, .alt_func_num = GPIO_AF5, .gpioport = GPIOE, .gpios = GPIO6};
const gpio_pin_altf spi_miso_bmp280_ak8963n{.clken = RCC_GPIOE, .alt_func_num = GPIO_AF5, .gpioport = GPIOE, .gpios = GPIO5};
const gpio_pin_altf spi_sck_bmp280_ak8963n{.clken = RCC_GPIOE, .alt_func_num = GPIO_AF5, .gpioport = GPIOE, .gpios = GPIO2};
const gpio_pin spi_css_bmp280{.clken = RCC_GPIOA, .gpioport = GPIOA, .gpios = GPIO2};
const gpio_pin spi_css_ak896n{.clken = RCC_GPIOA, .gpioport = GPIOA, .gpios = GPIO3};

/*! @name internal spi for the icm20689-v2.2 sensor */
const spi_interface spi_icm20689{.clken = RCC_SPI1, .spi = SPI1};
const gpio_pin_altf spi_mosi_icm20689{.clken = RCC_GPIOA, .alt_func_num = GPIO_AF5, .gpioport = GPIOA, .gpios = GPIO7};
const gpio_pin_altf spi_miso_icm20689{.clken = RCC_GPIOA, .alt_func_num = GPIO_AF5, .gpioport = GPIOA, .gpios = GPIO6};
const gpio_pin_altf spi_sck_icm20689{.clken = RCC_GPIOA, .alt_func_num = GPIO_AF5, .gpioport = GPIOA, .gpios = GPIO5};

/*! @name sd-card detection switch A
 * @note input*/
const gpio_pin sdcard_A = {.clken = RCC_GPIOD, .gpioport = GPIOD, .gpios = GPIO0};

/*! @name sd-card detection switch B
 * @note input*/
const gpio_pin sdcard_B = {.clken = RCC_GPIOD, .gpioport = GPIOD, .gpios = GPIO1};

/*! @name s1 universal switch
 * @note input*/
const gpio_pin s1_swich = {.clken = RCC_GPIOD, .gpioport = GPIOD, .gpios = GPIO4};

/*! @name icm20689 initialisation
 * @note output*/
const gpio_pin icm20689_init = {.clken = RCC_GPIOA, .gpioport = GPIOA, .gpios = GPIO4};

/*! @name power pin
 * @note output*/
const gpio_pin power_pin = {.clken = RCC_GPIOB, .gpioport = GPIOB, .gpios = GPIO2};

/*! @name led 2
 * @note output
 * @note only on/off*/
const gpio_pin led2 = {.clken = RCC_GPIOC, .gpioport = GPIOC, .gpios = GPIO13};

/*! @name led 3
 * @note output
 * @note pwm controlled / dimmable*/
const gpio_pin_altf led3 = {.clken = RCC_GPIOB, .alt_func_num = GPIO_AF1, .gpioport = GPIOB, .gpios = GPIO2};
#endif //BOARD_DEFINES_H
