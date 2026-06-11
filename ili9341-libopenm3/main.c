#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include "ili9341.h"
#include "fonts.h"

static void spi_gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_SPI1);

    /* SCK, MOSI — AF5 */
    gpio_mode_setup(TFT_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TFT_SCK | TFT_MOSI);
    gpio_set_af(TFT_PORT, GPIO_AF5, TFT_SCK | TFT_MOSI);
    gpio_set_output_options(TFT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                            TFT_SCK | TFT_MOSI);

    /* CS, DC, RST — saída */
    gpio_mode_setup(TFT_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    TFT_CS | TFT_DC | TFT_RST);
    gpio_set_output_options(TFT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                            TFT_CS | TFT_DC | TFT_RST);
    gpio_set(TFT_PORT, TFT_CS);

    /* LED backlight — saída */
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_set_output_options(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED_PIN);

    spi_init_master(SPI1,
                    SPI_CR1_BAUDRATE_FPCLK_DIV_4,
                    SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1,
                    SPI_CR1_DFF_8BIT,
                    SPI_CR1_MSBFIRST);
    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);
    spi_enable(SPI1);
}


int main(void)
{
    rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    spi_gpio_setup();
    ili9341_init();

    // Landscape Orientation
    ili9341_set_rotation(1);

    ili9341_fill_screen(ILI9341_BLACK);

    /* ili9341_write_string(10, 10,  "ILI9341 + libopencm3", Font_7x10,  ILI9341_WHITE,  ILI9341_BLACK); */
    ili9341_write_string(10, 30,  "Hello, World!",        Font_11x18, ILI9341_YELLOW, ILI9341_BLACK);


    /* Red rectangle */
    ili9341_fill_rectangle(50, 50, 50, 50, ILI9341_RED);

    /* Green rectangle */


    /* Row of single pixels */
    for(int x = 0; x < 240/2; x++){
	ili9341_draw_pixel(x, 50, ILI9341_WHITE);
	ili9341_draw_pixel(x, 50, ILI9341_WHITE);
    }
    ili9341_fill_rectangle(0, 101, 100, 50, ILI9341_GREEN);
    while (1);
    return 0;
}
