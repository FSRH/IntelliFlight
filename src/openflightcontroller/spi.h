#ifndef INTELLIFLIGHT_SPI_H
#define INTELLIFLIGHT_SPI_H

#include <openflightcontroller/board_defines.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

uint32_t cr_tmp;

static void spi_setup(void);

uint16_t read_reg(int reg);

static void write_reg(uint8_t reg, uint8_t value);

uint8_t read_xyz(int16_t vecs[3]);

struct spi_port {
    uint32_t gpioport;
    uint16_t gpios;
    uint32_t spi;
};

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

static void spi_setup(void) {
    rcc_periph_clock_enable(spi_bmp280_ak8963n.clken);
    rcc_periph_clock_enable(spi_mosi_bmp280_ak8963n.clken);
    rcc_periph_clock_enable(spi_miso_bmp280_ak8963n.clken);
    rcc_periph_clock_enable(spi_sck_bmp280_ak8963n.clken);
    rcc_periph_clock_enable(spi_css_bmp280.clken);

    /* 1.Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins. */
    gpio_mode_setup(spi_miso_bmp280_ak8963n.gpioport, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, spi_miso_bmp280_ak8963n.gpios);
    gpio_mode_setup(spi_mosi_bmp280_ak8963n.gpioport, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, spi_mosi_bmp280_ak8963n.gpios);
    gpio_mode_setup(spi_sck_bmp280_ak8963n.gpioport, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, spi_sck_bmp280_ak8963n.gpios);
    gpio_set_af(spi_miso_bmp280_ak8963n.gpioport, spi_miso_bmp280_ak8963n.alt_func_num, spi_miso_bmp280_ak8963n.gpios);
    gpio_set_af(spi_mosi_bmp280_ak8963n.gpioport, spi_mosi_bmp280_ak8963n.alt_func_num, spi_mosi_bmp280_ak8963n.gpios);
    gpio_set_af(spi_sck_bmp280_ak8963n.gpioport, spi_sck_bmp280_ak8963n.alt_func_num, spi_sck_bmp280_ak8963n.gpios);
    gpio_set_output_options(spi_mosi_bmp280_ak8963n.gpioport, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, spi_mosi_bmp280_ak8963n.gpios);
    gpio_set_output_options(spi_sck_bmp280_ak8963n.gpioport, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, spi_sck_bmp280_ak8963n.gpios);

    /* Chip select spi header */
    gpio_set(spi_css_bmp280.gpioport, spi_css_bmp280.gpios);
    gpio_mode_setup(spi_css_bmp280.gpioport, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, spi_css_bmp280.gpios);


    uint16_t spi_cr1 = 0x00;
    /* 2. Write to the SPI_CR1 register: */

    /* a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 4). */
    spi_cr1 |= SPI_CR1_BAUDRATE_FPCLK_DIV_8;

    /* b) Configure the CPOL and CPHA bits combination to define one of the four
     * relationships between the data transfer and the serial clock (CPHA must be
     * cleared in NSSP mode). (Note: 2). */
    spi_cr1 |= SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE | SPI_CR1_CPHA;

    /* c) Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
     * BIDIOE (RXONLY and BIDIMODE can't be set at the same time). */
    //TODO: Add the zero position of RXONLY, BIDIMODE and BIDIOE to libopencm3
    spi_cr1 |= 0x00;

    /* d) configure the lsbfirst bit to define the frame format (note: 2). */
    spi_cr1 |= SPI_CR1_MSBFIRST;

    /* e) Configure the CRCL and CRCEN bits if CRC is needed (while SCK clock signal is
     * at idle state). */
    //TODO: Add the zero position of CRCEN to libopencm3
    spi_cr1 |= SPI_CR1_CRCL_8BIT | SPI_CR1_CRCEN;

    /* f) Configure SSM and SSI (Notes: 2 & 3). */
    //TODO: Add the zero position of SSM and SSI to libopencm3
    spi_cr1 |= 0x00;

    /* g) Configure the MSTR bit (in multimaster NSS configuration, avoid conflict state on
     * NSS if master is configured to prevent MODF error). */
    //TODO: Add the zero position of SPI_CR1_SLAV to libopencm3
    spi_cr1 |= SPI_CR1_MSTR;

    /* spi enable like in the example */
    spi_cr1 |= SPI_CR1_SPE;

    uint16_t spi_cr2 = 0x00;
    /* 3. Write to SPI_CR2 register: */

    /* a) Configure the DS[3:0] bits to select the data length for the transfer. */
    spi_cr2 |= SPI_CR2_DS_8BIT;

    /* b) Configure SSOE (Notes: 1 & 2 & 3). */
    //TODO: Add the zero position of SPI_CR2_SSOE to libopencm3
    spi_cr2 |= SPI_CR2_SSOE;

    /* c) Set the FRF bit if the TI protocol is required (keep NSSP bit cleared in TI mode). */
    spi_cr2 |= SPI_CR2_FRF_MOTOROLA;

    /* d) Set the NSSP bit if the NSS pulse mode between two data units is required (keep
     * CHPA and TI bits cleared in NSSP mode). */
    //TODO: Add the zero position of SPI_CR2_NSSP to libopencm3
    spi_cr2 |= 0x00;

    /* e) Configure the FRXTH bit. The RXFIFO threshold must be aligned to the read
     * access size for the SPIx_DR register. */
    spi_cr2 |= SPI_CR2_FRXTH;

    /* f) Initialize LDMA_TX and LDMA_RX bits if DMA is used in packed mode. */
    spi_cr2 |= 0x00;

    /* 4. Write to SPI_CRCPR register: Configure the CRC polynomial if needed. */
    uint16_t spi_crcpr = 0x00;

    /* 5. Write proper DMA registers: Configure DMA streams dedicated for SPI Tx and Rx in
     * DMA registers if the DMA streams are used. */
    // Not needed in this examle

    SPI_CRCPR(spi_bmp280_ak8963n.spi) = spi_crcpr;
    SPI_CR2(spi_bmp280_ak8963n.spi) = spi_cr2;
    SPI_CR1(spi_bmp280_ak8963n.spi) = spi_cr1;
}

#endif //INTELLIFLIGHT_SPI_H
