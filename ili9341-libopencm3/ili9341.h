#ifndef __ILI9341_H__
#define __ILI9341_H__

#include <stdint.h>
#include "ili9341_conf.h"
#include "fonts.h"


/* --- Hardware pins (SPI1 on GPIOA) --- */
#ifndef LED_PORT
#define LED_PORT  GPIOB
#endif
#ifndef LED_PIN
#define LED_PIN   GPIO0
#endif

#ifndef TFT_SPI
#define TFT_SPI   SPI1
#endif
#ifndef TFT_PORT
#define TFT_PORT  GPIOA
#endif
#ifndef TFT_SCK
#define TFT_SCK   GPIO5   /* PA5 = SPI1_SCK  */
#endif
#ifndef TFT_MOSI
#define TFT_MOSI  GPIO7   /* PA7 = SPI1_MOSI */
#endif
#ifndef TFT_CS
#define TFT_CS    GPIO4   /* PA4 = CS  (SW)  */
#endif
#ifndef TFT_DC
#define TFT_DC    GPIO3   /* PA3 = D/C       */
#endif
#ifndef TFT_RST
#define TFT_RST   GPIO2   /* PA2 = RESET     */
#endif


/* --- Display geometry --- */
#ifndef ILI9341_WIDTH
#define ILI9341_WIDTH  240
#endif
#ifndef ILI9341_HEIGHT
#define ILI9341_HEIGHT 320
#endif

/* --- RGB565 color helpers --- */
#define ILI9341_COLOR(r, g, b) \
    ((uint16_t)(((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

#define ILI9341_BLACK   0x0000
#define ILI9341_WHITE   0xFFFF
#define ILI9341_RED     0xF800
#define ILI9341_GREEN   0x07E0
#define ILI9341_BLUE    0x001F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_ORANGE  0xFD20
#define ILI9341_GRAY    0x8410

/* --- API --- */
void ili9341_init(void);
void ili9341_set_rotation(uint8_t rotation); /* 0-3 */

void ili9341_set_delay_function(void (*fn)(uint32_t)); // if not set, driver will use blocking delay function and may not work

uint16_t ili9341_get_width(void);
uint16_t ili9341_get_height(void);

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ili9341_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_fill_screen(uint16_t color);
void ili9341_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);

void ili9341_draw_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t fg, uint16_t bg);
void ili9341_write_string(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t fg, uint16_t bg);

#endif /* __ILI9341_H__ */
