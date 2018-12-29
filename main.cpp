#ifndef INTELLIFLIGHT
#define INTELLIFLIGHT

#include <bit_utils.h>
#include <openflightcontroller/board_defines.h>
#include <bmp280.h>
#include <openflightcontroller/spi.h>

int counterIsr = 0;

void uart4_isr(void) {
    counterIsr++;
    uint32_t reg32 = USART_RDR(UART4);
}

void uart5_isr(void) {
    counterIsr++;
    uint32_t reg32 = USART_RDR(UART5);
}

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
void uart7_isr(void) {
    counterIsr++;
    uint32_t reg32 = USART_RDR(UART7);
}

void uart8_isr(void) {
    counterIsr++;
    uint32_t reg32 = USART_RDR(UART8);
}

//TODO: outsource the handler to new file clock.c
void sys_tick_handler(void) {
    system_millis++;
}


bmp280_com_fptr_t bmp280_com_read = [](uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) -> int8_t {
    gpio_clear(BMP280_CSS_PORT, BMP280_CSS_GPIO);   //CSS (enable chip)

    spi_send(BMP280_MAG_SPI, reg_addr); // request data
//    uint16_t answer = spi_read8(BMP280_MAG_SPI);


    // write data to receive buffer
    for (int idx = 0; idx < (len / 8); idx++) {
        auto answer = spi_read8(BMP280_MAG_SPI);
        data[idx] = static_cast<uint8_t>(BIT_GET_SUFFIX(answer, 8));
    }

    gpio_set(BMP280_CSS_PORT, BMP280_CSS_GPIO);   //CSS (disable chip)

    return BMP280_OK;
};

bmp280_com_fptr_t bmp280_com_write = [](uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) -> int8_t {
    gpio_clear(BMP280_CSS_PORT, BMP280_CSS_GPIO);   //CSS (enable chip)

    // write data beginning with register `reg_addr`
    for (int idx = 0; idx < (len / 8); idx++) {
        spi_send(BMP280_MAG_SPI, static_cast<uint16_t> (reg_addr + idx));
        spi_read8(BMP280_MAG_SPI);
        spi_send(BMP280_MAG_SPI, data[idx]);
        spi_read8(BMP280_MAG_SPI);
    }

    gpio_set(BMP280_CSS_PORT, BMP280_CSS_GPIO);   //CSS (disable chip)

    return BMP280_OK;
};


bmp280_delay_fptr_t bmp280_delay = [](uint32_t period) -> void {
//    msleep(period);
};

int main(void) {
    clockSetup();
    gpioSetup();
//    timerSetup();
    spi_setup();
    uartSetup();

    int8_t rslt;
    bmp280_dev bmp;

/* Sensor interface over SPI with native chip select line */
    bmp.dev_id = 0;
    bmp.intf = BMP280_SPI_INTF;
    bmp.read = bmp280_com_read;
    bmp.write = bmp280_com_write;
    bmp.delay_ms = bmp280_delay;

    bmp280_config conf;
    conf.spi3w_en = BMP280_SPI3_WIRE_DISABLE;
    conf.os_pres = BMP280_OS_16X;
    conf.os_temp = BMP280_OS_2X;
    conf.filter = BMP280_FILTER_COEFF_16;
    conf.odr = BMP280_ODR_0_5_MS;

    rslt = bmp280_init(&bmp);
    rslt = bmp280_set_config(&conf, &bmp);

    while (true) {
//        gpio_clear(GPIOA, GPIO2);
//        spi_send(SPI4, 0xD0);
//        uint16_t answer = spi_read(SPI4);
//        uint16_t answer2 = spi_read(SPI4);
//        gpio_set(GPIOA, GPIO2);

//        int8_t answer = bmp280_get_regs(0xD0, 0x0, 0, &bmp);

//        gpio_clear(BMP280_CSS_PORT, BMP280_CSS_GPIO);   //CSS (enable chip)
//        spi_send(BMP280_MAG_SPI, 0xD0); // request data
//        for (int i = 0; i < 2; i++)
//            spi_read(BMP280_MAG_SPI);
//        gpio_set(BMP280_CSS_PORT, BMP280_CSS_GPIO);

        gpio_toggle(GPIOC, GPIO13);
        for (int i = 0; i < 5000000; i++) {}
        if (counterIsr >= 5) {
            gpio_set(GPIOE, GPIO14);
        }
    }
}

#endif //INTELLIFLIGHT
