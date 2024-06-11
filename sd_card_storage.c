#include "sd_card_storage.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#include <stdio.h>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"

void sdtest() 
{
    time_init();

    puts("Hello, world!");

    sd_card_t *sdcard = sd_get_by_num(0);


    FRESULT result = f_mount(&sdcard->fatfs, sdcard->pcName, 1);

    if (FR_OK != result) 
    {
        panic("f_mount error: %s (%d)\n", FRESULT_str(result), result);
    }

    FIL fil;
    const char* const filename = "filename.txt";
    result = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != result && FR_EXIST != result)
    {
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(result), result);
    }
    if (f_printf(&fil, "Hello, world!\n") < 0) 
    {
        printf("f_printf failed\n");
    }
    result = f_close(&fil);
    if (FR_OK != result) 
    {
        printf("f_close error: %s (%d)\n", FRESULT_str(result), result);
    }

    f_unmount(sdcard->pcName);

    printf("Goodbye, world!");
}

// #define PIN_CS 5
// #define PIN_SCK 2
// #define PIN_MOSI 3
// #define PIN_MISO 4

// void sd_send_command(uint8_t cmd, uint32_t arg) {
//     uint8_t crc = 0x95; // Dummy CRC + Stop (CMD0)
//     if (cmd != 0) crc = 0xFF;

//     uint8_t buf[6];
//     buf[0] = 0x40 | cmd;
//     buf[1] = (arg >> 24) & 0xFF;
//     buf[2] = (arg >> 16) & 0xFF;
//     buf[3] = (arg >> 8) & 0xFF;
//     buf[4] = arg & 0xFF;
//     buf[5] = crc;

//     gpio_put(PIN_CS, 0); // Select the card
//     spi_write_blocking(spi0, buf, 6);
//     gpio_put(PIN_CS, 1); // Deselect the card
// }

// uint8_t sd_get_response() {
//     uint8_t response;
//     gpio_put(PIN_CS, 0); // Select the card
//     for (int i = 0; i < 8; i++) { // Read up to 8 bytes to get a response
//         spi_read_blocking(spi0, 0xFF, &response, 1);
//         if (response != 0xFF) break;
//     }
//     gpio_put(PIN_CS, 1); // Deselect the card
//     return response;
// }

// bool listFiles2() 
// {

//     // Initialize SPI
//     spi_init(spi0, 100 * 1000); // 100 kHz for initialization
//     gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
//     gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
//     gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
//     bi_decl(bi_4pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCK, 5, GPIO_FUNC_SPI));


//     gpio_init(PIN_CS);
//     gpio_set_dir(PIN_CS, GPIO_OUT);
//     gpio_put(PIN_CS, 1); // Deselect the card


//     while (1) {

//         // Send 80 clock cycles to prepare SD card
//         for (int i = 0; i < 10; i++) {
//             spi_write_blocking(spi0, (const uint8_t[]){0xFF}, 1);
//         }

//         // Send CMD0 to reset the SD card
//         sd_send_command(0, 0);
//         uint8_t response = sd_get_response();
//         printf("CMD0 response: 0x%02X\n", response);

//         // Additional initialization commands can go here...

//         sleep_ms(5000);
//     }

//     return 0;
// }

bool listFiles() 
{
    time_init();

    printf("Listing Files...\n");

    sd_card_t *sdcard = sd_get_by_num(0);

    FRESULT result = f_mount(&sdcard->fatfs, sdcard->pcName, 1);
    if (FR_OK != result) 
    {
        printf("Mount failed.\n");
        return false;
    }

    const char* path = "0:\\";

	DIR dirs;
	FRESULT fr = f_opendir(&dirs, path);
    if (fr == FR_OK)
    {
        FILINFO fileInfo;
		while (((fr = f_readdir(&dirs, &fileInfo)) == FR_OK) && fileInfo.fname[0]) 
        {
			if (fileInfo.fattrib & AM_DIR == 0) 
            {
				printf("%s/%s\n", path, fileInfo.fname);
			}
		}
	}

    f_unmount(sdcard->pcName);
    printf("Done.\n");
}