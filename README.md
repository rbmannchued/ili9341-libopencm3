# ILI9341 Driver for libopencm3

A minimal SPI display driver for the ILI9341 TFT controller, written for STM32 microcontrollers using [libopencm3](https://github.com/libopencm3/libopencm3).

> **Note:** Touch screen support is not implemented yet. This driver covers display output only.

---

## Default Pin Mapping

The driver ships configured for SPI1 on GPIOA (STM32F411):

| Signal | Pin  |
|--------|------|
| SCK    | PA5  |
| MOSI   | PA7  |
| CS     | PA4  |
| D/C    | PA3  |
| RESET  | PA2  |
| LED BL | PB0  |

---

## Configuration

Before compiling, open `ili9341_conf.h` and adjust the defines to match your hardware:

```c
#define TFT_SPI   SPI1
#define TFT_PORT  GPIOA
#define TFT_SCK   GPIO5
#define TFT_MOSI  GPIO7
#define TFT_CS    GPIO4
#define TFT_DC    GPIO3
#define TFT_RST   GPIO2

#define LED_PORT  GPIOB
#define LED_PIN   GPIO0

#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
```

Change the SPI peripheral, GPIO port, and pin assignments to match your schematic. If your board has a different resolution, update `ILI9341_WIDTH` and `ILI9341_HEIGHT` too.

---

## Integrating into Your Project

1. Copy the following files into your project:
   - `ili9341.c`
   - `ili9341.h`
   - `ili9341_conf.h`
   - `fonts.c`
   - `fonts.h`

2. Add `ili9341.c` and `fonts.c` to your `CFILES` in the Makefile.

3. Make sure `libopencm3` is available and `OPENCM3_DIR` points to it.

4. Set up SPI and GPIO in your own initialization code **before** calling `ili9341_init()`. The driver assumes SPI is already configured and running.

5. Provide a millisecond delay function so the driver can time the reset sequence correctly:

```c
ili9341_set_delay_function(my_delay_ms); // call before ili9341_init()
ili9341_init();
```

If no delay function is provided, the driver falls back to a blocking NOP loop calibrated for ~16 MHz — this may not work reliably at other clock speeds.

---

## API

```c
// Initialization
void ili9341_init(void);
void ili9341_set_rotation(uint8_t rotation); // 0–3
void ili9341_set_delay_function(void (*fn)(uint32_t));

// Dimensions (update after set_rotation)
uint16_t ili9341_get_width(void);
uint16_t ili9341_get_height(void);

// Drawing
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ili9341_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_fill_screen(uint16_t color);
void ili9341_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);

// Text
void ili9341_draw_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t fg, uint16_t bg);
void ili9341_write_string(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t fg, uint16_t bg);
```

Colors use the RGB565 format. Predefined colors are available (`ILI9341_RED`, `ILI9341_WHITE`, etc.) and a macro to build custom ones:

```c
uint16_t color = ILI9341_COLOR(255, 128, 0); // orange
```

Three bitmap fonts are included: `Font_7x10`, `Font_11x18`, `Font_16x26`.

---

## Credits

- Initialization sequence based on [martnak/STM32-ILI9341](https://github.com/martnak/STM32-ILI9341)
- Font bitmaps from [afiskon/stm32-ili9341](https://github.com/afiskon/stm32-ili9341)
