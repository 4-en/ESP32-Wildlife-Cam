#ifndef PINS_H
#define PINS_H

// on board LED
#define LED_BUILTIN 2

// Camera pin definition
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// SDMMC card pins
#define SD_MMC_CMD 15
#define SD_MMC_CLK 14
#define SD_MMC_D0 2

/* different I2C pins, since default pins are used by camera */
#define SCL_PIN 13
#define SDA_PIN 33
#define UNUSED_PIN 32 // not used, but defined to remembe that which pin is not used

#endif
