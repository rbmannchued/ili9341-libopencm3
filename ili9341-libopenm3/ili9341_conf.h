#ifndef __ILI9341_CONF_H__
#define __ILI9341_CONF_H__

#define STM32F4

#define TFT_SPI   SPI1
#define TFT_PORT  GPIOA
#define TFT_SCK   GPIO5   /* PA5 = SPI1_SCK  */
#define TFT_MOSI  GPIO7   /* PA7 = SPI1_MOSI */
#define TFT_CS    GPIO4   /* PA4 = CS  (SW)  */
#define TFT_DC    GPIO3   /* PA3 = D/C       */
#define TFT_RST   GPIO2   /* PA2 = RESET     */

#define LED_PORT  GPIOB
#define LED_PIN   GPIO0

#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320



#endif
