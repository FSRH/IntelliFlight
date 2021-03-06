#ifndef INTELLIFLIGHT_TMP_H
#define INTELLIFLIGHT_TMP_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

struct spi {
    rcc_periph_clken clken;
    uint32_t spi;
};

struct pin {
    rcc_periph_clken clken;
    uint8_t alt_func_num;
    uint32_t gpioport;
    uint16_t gpios;
};

void tmp() {
    spi header;
    header.clken = RCC_SPI2;
    header.spi = SPI2;

    pin mosi;
    mosi.clken = RCC_GPIOB;
    mosi.alt_func_num = GPIO_AF5;
    mosi.gpioport = GPIOB;
    mosi.gpios = GPIO15;

    pin miso;
    miso.clken = RCC_GPIOB;
    miso.alt_func_num = GPIO_AF5;
    miso.gpioport = GPIOB;
    miso.gpios = GPIO14;

    pin sck;
    sck.clken = RCC_GPIOD;
    sck.alt_func_num = GPIO_AF5;
    sck.gpioport = GPIOD;
    sck.gpios = GPIO3;

    pin css;
    css.clken = RCC_GPIOD;
    css.gpioport = GPIOD;
    css.gpios = GPIO13;

    rcc_periph_clock_enable(header.clken);
    rcc_periph_clock_enable(mosi.clken);
    rcc_periph_clock_enable(miso.clken);
    rcc_periph_clock_enable(sck.clken);
    rcc_periph_clock_enable(css.clken);

    /* 1.Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins. */
    gpio_mode_setup(miso.gpioport, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, miso.gpios);
    gpio_mode_setup(mosi.gpioport, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, mosi.gpios);
    gpio_mode_setup(sck.gpioport, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, sck.gpios);
    gpio_set_af(miso.gpioport, miso.alt_func_num, miso.gpios);
    gpio_set_af(mosi.gpioport, mosi.alt_func_num, mosi.gpios);
    gpio_set_af(sck.gpioport, sck.alt_func_num, sck.gpios);
    gpio_set_output_options(mosi.gpioport, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, mosi.gpios);
    gpio_set_output_options(sck.gpioport, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, sck.gpios);

    /* Chip select spi header */
    gpio_set(css.gpioport, css.gpios);
    gpio_mode_setup(css.gpioport, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, css.gpios);

    uint16_t spi_cr1 = 0x00;
    /* 2. Write to the SPI_CR1 register: */

    /* a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 4). */
    spi_cr1 |= SPI_CR1_BAUDRATE_FPCLK_DIV_8;

    /* b) Configure the CPOL and CPHA bits combination to define one of the four
     * relationships between the data transfer and the serial clock (CPHA must be
     * cleared in NSSP mode). (Note: 2). */
    spi_cr1 |= SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE | SPI_CR1_CPHA_CLK_TRANSITION_1;

    /* c) Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
     * BIDIOE (RXONLY and BIDIMODE can't be set at the same time). */
    //TODO: Add the zero position of RXONLY, BIDIMODE and BIDIOE to libopencm3
    spi_cr1 |= 0x00;

    /* d) configure the lsbfirst bit to define the frame format (note: 2). */
    spi_cr1 |= SPI_CR1_MSBFIRST;

    /* e) Configure the CRCL and CRCEN bits if CRC is needed (while SCK clock signal is
     * at idle state). */
    //TODO: Add the zero position of CRCEN to libopencm3
    spi_cr1 |= SPI_CR1_CRCL_8BIT;

    /* f) Configure SSM and SSI (Notes: 2 & 3). */
    //TODO: Add the zero position of SSM and SSI to libopencm3
    spi_cr1 |= 0x00;

    /* g) Configure the MSTR bit (in multimaster NSS configuration, avoid conflict state on
     * NSS if master is configured to prevent MODF error). */
    //TODO: Add the zero position of SPI_CR1_SLAV to libopencm3
    spi_cr1 |= SPI_CR1_MSTR;

    uint16_t spi_cr2 = 0x00;
    /* 3. Write to SPI_CR2 register: */

    /* a) Configure the DS[3:0] bits to select the data length for the transfer. */
    spi_cr2 |= SPI_CR2_DS_8BIT;

    /* b) Configure SSOE (Notes: 1 & 2 & 3). */
    //TODO: Add the zero position of SPI_CR2_SSOE to libopencm3
    spi_cr2 |= 0x00;

    /* c) Set the FRF bit if the TI protocol is required (keep NSSP bit cleared in TI mode). */
    spi_cr2 |= SPI_CR2_FRF_MOTOROLA;

    /* d) Set the NSSP bit if the NSS pulse mode between two data units is required (keep
     * CHPA and TI bits cleared in NSSP mode). */
    //TODO: Add the zero position of SPI_CR2_NSSP to libopencm3
    spi_cr2 |= 0x00;

    /* e) Configure the FRXTH bit. The RXFIFO threshold must be aligned to the read
     * access size for the SPIx_DR register. */
    spi_cr2 |= 0x00;

    /* f) Initialize LDMA_TX and LDMA_RX bits if DMA is used in packed mode. */
    spi_cr2 |= 0x00;

    /* 4. Write to SPI_CRCPR register: Configure the CRC polynomial if needed. */
    uint16_t spi_crcpr = 0x00;

    /* 5. Write proper DMA registers: Configure DMA streams dedicated for SPI Tx and Rx in
     * DMA registers if the DMA streams are used. */
    // Not needed in this examle


    SPI_CR2(header.spi) = spi_cr1;
    SPI_CR1(header.spi) = spi_cr2;
    SPI_CRCPR(header.spi) = spi_crcpr;
}

/** SPI1 */
/* Enable the GPIO ports whose pins we are using */
gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO5 | GPIO6 | GPIO7);
gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);

/* Set SCK and MOSI to otype_pp*/
gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO5 | GPIO7);

/* Chip select line */
gpio_set(GPIOA, GPIO4);
gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);

rcc_periph_clock_enable(RCC_SPI1);

cr_tmp = SPI_CR1_BAUDRATE_FPCLK_DIV_8 | SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_CPHA | SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE;

SPI_CR2(SPI1) |= SPI_CR2_SSOE;
SPI_CR1(SPI1) = cr_tmp;

/** SPI2 */
/* Enable the GPIO ports whose pins we are using */
gpio_mode_setup(SPI_HEADER_MISO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, SPI_HEADER_MISO_GPIO);
gpio_mode_setup(SPI_HEADER_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, SPI_HEADER_MOSI_GPIO);
gpio_mode_setup(SPI_HEADER_SCK_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, SPI_HEADER_SCK_GPIO);
gpio_set_af(SPI_HEADER_MISO_PORT, SPI_HEADER_MISO_AF, SPI_HEADER_MISO_GPIO);
gpio_set_af(SPI_HEADER_MOSI_PORT, SPI_HEADER_MOSI_AF, SPI_HEADER_MOSI_GPIO);
gpio_set_af(SPI_HEADER_SCK_PORT, SPI_HEADER_SCK_AF, SPI_HEADER_SCK_GPIO);

/* Set SCK and MOSI to otype_pp*/
gpio_set_output_options(SPI_HEADER_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SPI_HEADER_MOSI_GPIO);
gpio_set_output_options(SPI_HEADER_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SPI_HEADER_SCK_GPIO);

/* Chip select spi header */
gpio_set(SPI_HEADER_CSS_PORT, SPI_HEADER_CSS_GPIO);
gpio_mode_setup(SPI_HEADER_CSS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_HEADER_CSS_GPIO);


//    TODO: Spi Einstellungen überprüfen! Irgend etwas ist hier noch defekt!
spi_reset(SPI_HEADER_SPI);
rcc_periph_clock_enable(SPI_HEADER_RCC);

spi_set_baudrate_prescaler(SPI_HEADER_SPI, 8);
spi_set_master_mode(SPI_HEADER_SPI);
spi_set_clock_phase_1(SPI_HEADER_SPI);
spi_set_clock_polarity_1(SPI_HEADER_SPI);
spi_send_msb_first(SPI_HEADER_SPI);
spi_set_dff_8bit(SPI_HEADER_SPI);
spi_enable(SPI_HEADER_SPI);

/** BMP280_SPI */
/* Enable the GPIO ports whose pins we are using */
gpio_mode_setup(BMP280_MISO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, BMP280_MISO_GPIO);
gpio_mode_setup(BMP280_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, BMP280_MOSI_GPIO);
gpio_mode_setup(BMP280_SCK_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, BMP280_SCK_GPIO);
gpio_set_af(BMP280_MISO_PORT, BMP280_MISO_AF, BMP280_MISO_GPIO);
gpio_set_af(BMP280_MOSI_PORT, BMP280_MOSI_AF, BMP280_MOSI_GPIO);
gpio_set_af(BMP280_SCK_PORT, BMP280_SCK_AF, BMP280_SCK_GPIO);

/* Set SCK and MOSI to otype_pp*/
gpio_set_output_options(BMP280_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BMP280_MOSI_GPIO);
gpio_set_output_options(BMP280_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BMP280_SCK_GPIO);

/* Chip select p+t */
gpio_set(BMP280_CSS_PORT, BMP280_CSS_GPIO);
gpio_mode_setup(BMP280_CSS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BMP280_CSS_GPIO);

/* Chip select mag. */
gpio_set(MAG_CSS_PORT, MAG_CSS_GPIO);
gpio_mode_setup(MAG_CSS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MAG_CSS_GPIO);

spi_reset(BMP280_MAG_SPI);
rcc_periph_clock_enable(BMP280_MAG_RCC);

spi_set_baudrate_prescaler(BMP280_MAG_SPI, 8);
spi_set_master_mode(BMP280_MAG_SPI);
spi_set_clock_phase_1(BMP280_MAG_SPI);
spi_set_clock_polarity_1(BMP280_MAG_SPI);
spi_send_msb_first(BMP280_MAG_SPI);
spi_set_dff_8bit(BMP280_MAG_SPI);
spi_enable(BMP280_MAG_SPI);
#endif //INTELLIFLIGHT_TMP_H
