#include <stdio.h>
#include "i2c_lcd_pcf8574.h"
#include "esp_log.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define TAG "I2C_LCD_PCF8574"

#define I2C_MASTER_TIMEOUT_MS 1000


static void lcd_send(i2c_lcd_pcf8574_handle_t* lcd, uint8_t value, bool is_data);
static void lcd_write_nibble(i2c_lcd_pcf8574_handle_t* lcd, uint8_t half_byte, bool is_data, i2c_cmd_handle_t cmd);
static void lcd_write_i2c(i2c_lcd_pcf8574_handle_t* lcd, uint8_t data, bool is_data, bool enable);

void lcd_init(i2c_lcd_pcf8574_handle_t* lcd, uint8_t i2c_addr, i2c_port_t i2c_port) {
    lcd->i2c_addr = i2c_addr;
    lcd->i2c_port = i2c_port;
    lcd->backlight = 0;
    lcd->entrymode = 0x02;
    lcd->displaycontrol = 0x04;
    lcd->rs_mask = 0x01;
    lcd->rw_mask = 0x00;
    lcd->enable_mask = 0x04;
    lcd->data_mask[0] = 0x10;
    lcd->data_mask[1] = 0x20;
    lcd->data_mask[2] = 0x40;
    lcd->data_mask[3] = 0x80;
    lcd->backlight_mask = 0x08;
}

void lcd_begin(i2c_lcd_pcf8574_handle_t* lcd, uint8_t cols, uint8_t rows) {
    lcd->cols = (cols > 80) ? 80 : cols;
    lcd->lines = (rows > 4) ? 4 : rows;

    lcd->row_offsets[0] = 0x00;
    lcd->row_offsets[1] = 0x40;
    lcd->row_offsets[2] = 0x00 + cols;
    lcd->row_offsets[3] = 0x40 + cols;

    vTaskDelay(50 / portTICK_PERIOD_MS);

    lcd_write_i2c(lcd, 0x00, false, false);
    vTaskDelay(50 / portTICK_PERIOD_MS);

    lcd->displaycontrol = 0x04;
    lcd->entrymode = 0x02;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    
    lcd_write_nibble(lcd, 0x03, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(5 / portTICK_PERIOD_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, 0x03, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(1 / portTICK_PERIOD_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, 0x03, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(1 / portTICK_PERIOD_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, 0x02, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    lcd_send(lcd, 0x20 | (rows > 1 ? 0x08 : 0x00), false);

    lcd_display(lcd);
    lcd_clear(lcd);
    lcd_left_to_right(lcd);
}

void lcd_clear(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd_send(lcd, 0x01, false);
    vTaskDelay(2 / portTICK_PERIOD_MS);
}

void lcd_home(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd_send(lcd, 0x02, false);
    vTaskDelay(2 / portTICK_PERIOD_MS);
}

void lcd_set_cursor(i2c_lcd_pcf8574_handle_t* lcd, uint8_t col, uint8_t row) {
    if (row >= lcd->lines) {
        row = lcd->lines - 1;
    }
    if (col >= lcd->cols) {
        col = lcd->cols - 1;
    }
    lcd_send(lcd, 0x80 | (lcd->row_offsets[row] + col), false);
}

void lcd_no_display(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->displaycontrol &= ~0x04;
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}

void lcd_display(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->displaycontrol |= 0x04;
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}

void lcd_cursor(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->displaycontrol |= 0x02;
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}

void lcd_no_cursor(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->displaycontrol &= ~0x02;
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}

void lcd_blink(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->displaycontrol |= 0x01;
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}

void lcd_no_blink(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->displaycontrol &= ~0x01;
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}

void lcd_scroll_display_left(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd_send(lcd, 0x18, false);
}

void lcd_scroll_display_right(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd_send(lcd, 0x1C, false);
}

void lcd_left_to_right(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->entrymode |= 0x02;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}

void lcd_right_to_left(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->entrymode &= ~0x02;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}

void lcd_autoscroll(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->entrymode |= 0x01;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}

void lcd_no_autoscroll(i2c_lcd_pcf8574_handle_t* lcd) {
    lcd->entrymode &= ~0x01;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}

void lcd_set_backlight(i2c_lcd_pcf8574_handle_t* lcd, uint8_t brightness) {
    lcd->backlight = brightness;
    lcd_write_i2c(lcd, 0x00, true, false);
}

void lcd_create_char(i2c_lcd_pcf8574_handle_t* lcd, uint8_t location, uint8_t charmap[]) {
    location &= 0x7;
    lcd_send(lcd, 0x40 | (location << 3), false);
    for (int i = 0; i < 8; i++) {
        lcd_write(lcd, charmap[i]);
    }
}

void lcd_write(i2c_lcd_pcf8574_handle_t* lcd, uint8_t value) {
    lcd_send(lcd, value, true);
}

void lcd_print(i2c_lcd_pcf8574_handle_t* lcd, const char* str) {
    while (*str) {
        lcd_write(lcd, *str++);
    }
}

void lcd_print_number(i2c_lcd_pcf8574_handle_t* lcd, uint8_t col, uint8_t row, uint8_t buf_len, const char *str, ...) {
    //  Ensure the buffer length is greater than zero
    if (buf_len == 0)
    {
        ESP_LOGE(TAG, "Buffer length must be greater than 0");
        return -1;  // Return an error code
    }

    //  Create a buffer to hold the characters
    char buffer[buf_len];

    va_list args;
    va_start(args, str);

    int chars_written = vsniprintf(buffer, buf_len, str, args);

    va_end(args);

    if (chars_written < 0) {
        ESP_LOGE(TAG, "Encoding error in vsnprintf");
    }

    if ((size_t)chars_written >= buf_len) {
        ESP_LOGW(TAG, "Buffer overflow: %d characters needed, but only %d available", chars_written + 1, buf_len);
    }

    lcd_set_cursor(lcd, col, row);
    lcd_print(lcd, buffer);
    
}

static void lcd_send(i2c_lcd_pcf8574_handle_t* lcd, uint8_t value, bool is_data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, (value >> 4 & 0x0F), is_data, cmd);
    lcd_write_nibble(lcd, (value & 0x0F), is_data, cmd);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send data to LCD: %s", esp_err_to_name(ret));
    }
}

static void lcd_write_nibble(i2c_lcd_pcf8574_handle_t* lcd, uint8_t half_byte, bool is_data, i2c_cmd_handle_t cmd) {
    uint8_t data = is_data ? lcd->rs_mask : 0;
    if (lcd->backlight > 0) {
        data |= lcd->backlight_mask;
    }

    if (half_byte & 0x01) data |= lcd->data_mask[0];
    if (half_byte & 0x02) data |= lcd->data_mask[1];
    if (half_byte & 0x04) data |= lcd->data_mask[2];
    if (half_byte & 0x08) data |= lcd->data_mask[3];

    i2c_master_write_byte(cmd, data | lcd->enable_mask, true);
    i2c_master_write_byte(cmd, data, true);
}

static void lcd_write_i2c(i2c_lcd_pcf8574_handle_t* lcd, uint8_t data, bool is_data, bool enable) {
    if (is_data) {
        data |= lcd->rs_mask;
    }
    if (enable) {
        data |= lcd->enable_mask;
    }
    if (lcd->backlight > 0) {
        data |= lcd->backlight_mask;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write to LCD: %s", esp_err_to_name(ret));
    }
}