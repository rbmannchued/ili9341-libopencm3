#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "ili9341.h"
#include "fonts.h"

int main(void)
{
    rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    ili9341_init();

    ili9341_fill_screen(ILI9341_BLACK);

    /* ili9341_write_string(10, 10,  "ILI9341 + libopencm3", Font_7x10,  ILI9341_WHITE,  ILI9341_BLACK); */
    /* ili9341_write_string(10, 30,  "Hello, World!",        Font_11x18, ILI9341_YELLOW, ILI9341_BLACK); */
    /* ili9341_write_string(10, 60,  "ABCDEF 0123456789",    Font_16x26, ILI9341_CYAN,   ILI9341_BLACK); */

    /* Red rectangle */
    ili9341_fill_rectangle(10, 100, 100, 60, ILI9341_RED);

    /* Green rectangle */
    ili9341_fill_rectangle(120, 100, 100, 60, ILI9341_GREEN);

    /* Single pixel */
    ili9341_draw_pixel(120, 200, ILI9341_WHITE);

    while (1);
    return 0;
}
