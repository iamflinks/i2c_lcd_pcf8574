// This example project shows various features basic setup to use the i2c_lcd_pcf8574 driver with 16 chars by 2 lines.

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c_lcd_pcf8574.h"

#define I2C_MASTER_SCL_IO 22        // GPIO number for I2C master clock
#define I2C_MASTER_SDA_IO 21        // GPIO number for I2C master data
#define I2C_MASTER_NUM I2C_NUM_0    // I2C port number for master dev
#define I2C_MASTER_FREQ_HZ 100000   // I2C master clock frequency

#define LCD_ADDR 0x27               // I2C address of the LCD
#define LCD_COLS 16                 // Number of columns in the LCD
#define LCD_ROWS 2                  // Number of rows in the LCD

static const char *TAG = "LCD_EXAMPLE";

void i2c_master_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
}

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing I2C");
    i2c_master_init();

    ESP_LOGI(TAG, "Initializing LCD");

    i2c_lcd_pcf8574_handle_t lcd;
    lcd_init(&lcd, LCD_ADDR, I2C_MASTER_NUM);
    lcd_begin(&lcd, LCD_COLS, LCD_ROWS);

    ESP_LOGI(TAG, "LCD initialized");

    // Turn on the backlight
    lcd_set_backlight(&lcd, 255);

    // Print a message
    lcd_set_cursor(&lcd, 0, 0);
    lcd_print(&lcd, "Hello, ESP32!");
    lcd_set_cursor(&lcd, 0, 1);
    lcd_print(&lcd, "LCD Test");

    int counter = 0;
    while (1) {
        vTaskDelay(20 / portTICK_PERIOD_MS);  // Wait for 1 second

        // Update the counter on the LCD
        lcd_set_cursor(&lcd, 10, 1);
        char buffer[6];
        snprintf(buffer, sizeof(buffer), "%5d", counter);
        lcd_print(&lcd, buffer);

        counter++;

        // Reset counter to avoid display overflow
        if (counter > 99999) counter = 0;
        
    }
}