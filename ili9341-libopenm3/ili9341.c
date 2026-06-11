#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "ili9341.h"

/* --- ILI9341 command codes --- */
#define CMD_SWRESET   0x01
#define CMD_SLPOUT    0x11
#define CMD_GAMMASET  0x26
#define CMD_DISPON    0x29
#define CMD_CASET     0x2A
#define CMD_PASET     0x2B
#define CMD_RAMWR     0x2C
#define CMD_MADCTL    0x36
#define CMD_COLMOD    0x3A
#define CMD_FRMCTR1   0xB1
#define CMD_DFUNCTR   0xB6
#define CMD_PWCTR1    0xC0
#define CMD_PWCTR2    0xC1
#define CMD_VMCTR1    0xC5
#define CMD_VMCTR2    0xC7
#define CMD_PWCTRA    0xCB
#define CMD_PWCTRB    0xCF
#define CMD_PWRONSEQ  0xED
#define CMD_TIMCTRA   0xE8
#define CMD_TIMCTRB   0xEA
#define CMD_EN3GAMMA  0xF2
#define CMD_GMCTRP1   0xE0
#define CMD_GMCTRN1   0xE1
#define CMD_PUMPRATIO 0xF7

/* MADCTL bits */
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_BGR 0x08

static uint16_t _width  = ILI9341_WIDTH;
static uint16_t _height = ILI9341_HEIGHT;

static void default_delay_ms(uint32_t ms)
{
    for (volatile uint32_t i = 0; i < ms * 16000; i++)
        __asm__("nop");
}

static void (*delay_fn)(uint32_t) = default_delay_ms;

void ili9341_set_delay_function(void (*fn)(uint32_t))
{
    if (fn)
        delay_fn = fn;
}


static inline void cs_high(void) {
    while (SPI_SR(TFT_SPI) & SPI_SR_BSY);
    gpio_set(TFT_PORT, TFT_CS);
}

static inline void spi_tx(uint8_t byte)
{
    spi_xfer(TFT_SPI, byte);
}

static void write_cmd(uint8_t cmd)
{
    gpio_clear(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    spi_send(SPI1, cmd);
    while (SPI_SR(SPI1) & SPI_SR_BSY);
    cs_high();
}

static void write_data(uint8_t data)
{
    gpio_set(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    spi_send(SPI1, data);
    while (SPI_SR(SPI1) & SPI_SR_BSY);
    cs_high();
}

static void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    write_cmd(CMD_CASET);
    gpio_set(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    spi_tx(x0 >> 8); spi_tx(x0 & 0xFF);
    spi_tx(x1 >> 8); spi_tx(x1 & 0xFF);
    cs_high();

    write_cmd(CMD_PASET);
    
    gpio_set(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    
    spi_tx(y0 >> 8); spi_tx(y0 & 0xFF);
    spi_tx(y1 >> 8); spi_tx(y1 & 0xFF);
    cs_high();

    write_cmd(CMD_RAMWR);
}


static void tft_reset(void)
{
    gpio_set(TFT_PORT, TFT_RST);
    delay_fn(5);
    gpio_clear(TFT_PORT, TFT_RST);
    delay_fn(20);
    gpio_set(TFT_PORT, TFT_RST);
    delay_fn(150);
}

/* --- Public API --- */

 void ili9341_init(void)
{
    /* spi_setup(); */

    /* Hardware reset */
    tft_reset();

    /* Initialization sequence copied from Martnak: https://github.com/martnak/STM32-ILI9341  */
    write_cmd(CMD_SWRESET);
    delay_fn(120);

    //POWER CONTROL A
    write_cmd(0xCB);
    write_data(0x39);
    write_data(0x2C);
    write_data(0x00);
    write_data(0x34);
    write_data(0x02);

    //POWER CONTROL B
    write_cmd(0xCF);
    write_data(0x00);
    write_data(0xC1);
    write_data(0x30);

//DRIVER TIMING CONTROL A
    write_cmd(0xE8);
    write_data(0x85);
    write_data(0x00);
    write_data(0x78);

//DRIVER TIMING CONTROL B
    write_cmd(0xEA);
    write_data(0x00);
    write_data(0x00);

//POWER ON SEQUENCE CONTROL
    write_cmd(0xED);
    write_data(0x64);
    write_data(0x03);
    write_data(0x12);
    write_data(0x81);

//PUMP RATIO CONTROL
    write_cmd(0xF7);
    write_data(0x20);
//POWER CONTROL,VRH[5:0]
    write_cmd(0xC0);
    write_data(0x23);

//POWER CONTROL,SAP[2:0];BT[3:0]
    write_cmd(0xC1);
    write_data(0x10);

//VCM CONTROL
    write_cmd(0xC5);
    write_data(0x3E);
    write_data(0x28);

//VCM CONTROL 2
    write_cmd(0xC7);
    write_data(0x86);

//MEMORY ACCESS CONTROL
    write_cmd(0x36);
    write_data(0x48);

//PIXEL FORMAT
    write_cmd(0x3A);
    write_data(0x55);

//FRAME RATIO CONTROL, STANDARD RGB COLOR
    write_cmd(0xB1);
    write_data(0x00);
    write_data(0x18);

//DISPLAY FUNCTION CONTROL
    write_cmd(0xB6);
    write_data(0x08);
    write_data(0x82);
    write_data(0x27);

//3GAMMA FUNCTION DISABLE
    write_cmd(0xF2);
    write_data(0x00);

//GAMMA CURVE SELECTED
    write_cmd(0x26);
    write_data(0x01);

//POSITIVE GAMMA CORRECTION
    write_cmd(0xE0);
    write_data(0x0F);
    write_data(0x31);
    write_data(0x2B);
    write_data(0x0C);
    write_data(0x0E);
    write_data(0x08);
    write_data(0x4E);
    write_data(0xF1);
    write_data(0x37);
    write_data(0x07);
    write_data(0x10);
    write_data(0x03);
    write_data(0x0E);
    write_data(0x09);
    write_data(0x00);

//NEGATIVE GAMMA CORRECTION
    write_cmd(0xE1);
    write_data(0x00);
    write_data(0x0E);
    write_data(0x14);
    write_data(0x03);
    write_data(0x11);
    write_data(0x07);
    write_data(0x31);
    write_data(0xC1);
    write_data(0x48);
    write_data(0x08);
    write_data(0x0F);
    write_data(0x0C);
    write_data(0x31);
    write_data(0x36);
    write_data(0x0F);

    //EXIT SLEEP
    write_cmd(0x11);
    delay_fn(120);

    write_cmd(CMD_DISPON);//gfim
    delay_fn(10);
    //turn on backlight
    gpio_set(LED_PORT, LED_PIN);
}


void ili9341_set_rotation(uint8_t rotation)
{
    uint8_t madctl;
    switch (rotation & 0x03) {
        case 0:
            madctl = MADCTL_BGR;
            _width = ILI9341_WIDTH; _height = ILI9341_HEIGHT;
            break;
        case 1:
            madctl = MADCTL_MV | MADCTL_BGR;
            _width = ILI9341_HEIGHT; _height = ILI9341_WIDTH;
            break;
        case 2:
            madctl = MADCTL_MY | MADCTL_BGR;
            _width = ILI9341_WIDTH; _height = ILI9341_HEIGHT;
            break;
        default:
            madctl = MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR;
            _width = ILI9341_HEIGHT; _height = ILI9341_WIDTH;
            break;
    }
    write_cmd(CMD_MADCTL);
    write_data(madctl);
}

uint16_t ili9341_get_width(void)  { return _width;  }
uint16_t ili9341_get_height(void) { return _height; }

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= _width || y >= _height) return;
    set_address_window(x, y, x, y);
    gpio_set(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    spi_tx(color >> 8);
    spi_tx(color & 0xFF);
    cs_high();
}

void ili9341_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if (x >= _width || y >= _height) return;
    if (x + w > _width)  w = _width  - x;
    if (y + h > _height) h = _height - y;

    set_address_window(x, y, x + w - 1, y + h - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    gpio_set(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    for (uint32_t n = (uint32_t)w * h; n > 0; n--) {
        spi_tx(hi);
        spi_tx(lo);
    }
    cs_high();
}

void ili9341_fill_screen(uint16_t color)
{
    ili9341_fill_rectangle(0, 0, _width, _height, color);
}

void ili9341_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)
{
    if (x + w > _width || y + h > _height) return;

    set_address_window(x, y, x + w - 1, y + h - 1);

    gpio_set(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    for (uint32_t n = (uint32_t)w * h; n > 0; n--, data++) {
        spi_tx(*data >> 8);
        spi_tx(*data & 0xFF);
    }
    cs_high();
}

void ili9341_draw_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t fg, uint16_t bg)
{
    if (x + font.width > _width || y + font.height > _height) return;
    if (ch < 0x20 || ch > 0x7E) ch = '?';

    const uint16_t *glyph = font.data + (ch - 0x20) * font.height;

    set_address_window(x, y, x + font.width - 1, y + font.height - 1);

    gpio_set(TFT_PORT, TFT_DC);
    gpio_clear(TFT_PORT, TFT_CS);
    for (uint8_t row = 0; row < font.height; row++) {
        uint16_t bits = glyph[row];
        for (uint8_t col = 0; col < font.width; col++) {
            uint16_t color = (bits & (0x8000 >> col)) ? fg : bg;
            spi_tx(color >> 8);
            spi_tx(color & 0xFF);
        }
    }
    cs_high();
}

void ili9341_write_string(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t fg, uint16_t bg)
{
    while (*str) {
        if (x + font.width > _width) {
            x = 0;
            y += font.height;
        }
        if (y + font.height > _height) break;
        ili9341_draw_char(x, y, *str, font, fg, bg);
        x += font.width;
        str++;
    }
}

