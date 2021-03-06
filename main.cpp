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
    //TODO: Use dev id
    gpio_clear(spi_css_bmp280.gpioport, spi_css_bmp280.gpios);   //CSS (enable chip)

    spi_send8(spi_bmp280_ak8963n.spi, reg_addr); // request data
    spi_read8(spi_bmp280_ak8963n.spi);

    // write data to receive buffer
    for (int idx = 0; idx < static_cast<int>(len); idx++) {
        spi_send8(spi_bmp280_ak8963n.spi, 0x00);
        uint8_t answer = spi_read8(spi_bmp280_ak8963n.spi);
        data[idx] = static_cast<uint8_t>(BIT_GET_SUFFIX(answer, 8));
    }

    gpio_set(spi_css_bmp280.gpioport, spi_css_bmp280.gpios);   //CSS (disable chip)

    return BMP280_OK;
};

bmp280_com_fptr_t bmp280_com_write = [](uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) -> int8_t {
    //TODO: Use dev id
    gpio_clear(spi_css_bmp280.gpioport, spi_css_bmp280.gpios);   //CSS (enable chip)

    // write data beginning with register `reg_addr`
    for (int idx = 0; idx < static_cast<int>(len); idx++) {
        spi_send8(spi_bmp280_ak8963n.spi, reg_addr);
        spi_read8(spi_bmp280_ak8963n.spi);
        spi_send8(spi_bmp280_ak8963n.spi, data[idx]);
        spi_read8(spi_bmp280_ak8963n.spi);
    }

    gpio_set(spi_css_bmp280.gpioport, spi_css_bmp280.gpios);   //CSS (disable chip)

    return BMP280_OK;
};


bmp280_delay_fptr_t bmp280_delay = [](uint32_t period) -> void {
    msleep(period);
};

int main(void) {
    clockSetup();
    gpioSetup();
//    timerSetup();
    spi_setup();
    uartSetup();

    bmp280_dev bmp{};

/* Sensor interface over SPI with native chip select line */
    bmp.dev_id = 0;
    bmp.intf = BMP280_SPI_INTF;
    bmp.read = bmp280_com_read;
    bmp.write = bmp280_com_write;
    bmp.delay_ms = bmp280_delay;

    bmp280_config conf{};
    conf.spi3w_en = BMP280_SPI3_WIRE_DISABLE;
    conf.os_pres = BMP280_OS_16X;
    conf.os_temp = BMP280_OS_2X;
    conf.filter = BMP280_FILTER_COEFF_16;
    conf.odr = BMP280_ODR_0_5_MS;

    bmp280_uncomp_data data{};
    data.uncomp_temp = 0;
    data.uncomp_press = 0;

    bmp280_init(&bmp);
    bmp280_set_config(&conf, &bmp);
    bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);

    double comp_temp = 0;
    double comp_press = 0;

    while (true) {
        //TODO: BMP Einstellungen überprüfen
        bmp280_get_uncomp_data(&data, &bmp);
        comp_temp = bmp280_comp_temp_double(data.uncomp_temp, &bmp);
        comp_press = bmp280_comp_pres_double(data.uncomp_press, &bmp);

        gpio_toggle(GPIOC, GPIO13);
        for (int i = 0; i < 5000000; i++) {}
        if (counterIsr >= 5) {
            gpio_set(GPIOE, GPIO14);
        }
    }
}

#endif //INTELLIFLIGHT
