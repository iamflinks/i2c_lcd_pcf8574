// This example project shows various features of the lcd driver library, including examples for all the functions in the driver.

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

// Create the lcd object
i2c_lcd_pcf8574_handle_t lcd;

int8_t show = -1;

// // Two custom characters
uint8_t dotOff[] = {0b00000, 0b01110, 0b10001, 0b10001,
                  0b10001, 0b01110, 0b00000, 0b00000};
uint8_t dotOn[] = { 0b00000, 0b01110, 0b11111, 0b11111,
                 0b11111, 0b01110, 0b00000, 0b00000};

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

    lcd_init(&lcd, LCD_ADDR, I2C_MASTER_NUM);
    lcd_begin(&lcd, LCD_COLS, LCD_ROWS);

    ESP_LOGI(TAG, "LCD initialized");

    show = 0;
    // create custom character
    lcd_create_char(&lcd, 1, dotOff);
    lcd_create_char(&lcd, 2, dotOn);

    // Turn on the backlight
    lcd_set_backlight(&lcd, 255);

    // Print a message
    lcd_set_cursor(&lcd, 0, 0);
    lcd_print(&lcd, "Hello, ESP32!");
    lcd_set_cursor(&lcd, 0, 1);
    lcd_print(&lcd, "LCD Test");
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    while (1)
    {
        if (show == 0)
        {
            lcd_set_backlight(&lcd, 255);
            lcd_home(&lcd);
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "LCD Test");
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            // turn off the backlight
            lcd_set_backlight(&lcd, 0);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            lcd_set_backlight(&lcd, 255);
        }
        else if (show == 1)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "Cursor On");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            lcd_cursor(&lcd);
        }
        else if (show == 2)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "Cursor Blink");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            lcd_blink(&lcd);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        else if (show == 3)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "Cursor Off");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            lcd_no_blink(&lcd);
            lcd_no_cursor(&lcd);
        }
        else if (show == 4)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "Display Off");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            lcd_no_display(&lcd);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        else if (show == 5)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "Display On");
            lcd_display(&lcd);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        else if (show == 7)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "*** First Line.");
            lcd_set_cursor(&lcd, 0, 1);
            lcd_print(&lcd, "*** Second Line.");
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        else if (show == 8)
        {
            lcd_scroll_display_left(&lcd);
        }
        else if (show == 9)
        {
            lcd_scroll_display_left(&lcd);
        }
        else if (show == 10)
        {
            lcd_scroll_display_left(&lcd);
        }
        else if (show == 11)
        {
            lcd_scroll_display_right(&lcd);
        }
        else if (show == 12)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "Write-");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        else if (show == 13)
        {
            lcd_clear(&lcd);
            lcd_set_cursor(&lcd, 0, 0);
            lcd_print(&lcd, "Custom 1: <\01>");
            lcd_set_cursor(&lcd, 0, 1);
            lcd_print(&lcd, "Custom 1: <\02>");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        else
        {
            lcd_clear(&lcd);
            lcd_print_number(&lcd, 0, 0, 6, "%5d", show - 13);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        vTaskDelay(1400 / portTICK_PERIOD_MS);
        show = (show + 1) % 16;
    }
}
