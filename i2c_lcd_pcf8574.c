/// \file i2c_lcd_pcf8574.c
/// \brief Liquid Crystal display driver with PCF8574 adapter for esp-idf
///
/// \author Femi Olugbon, https://iamflinks.github.io
/// \copyright Copyright (c) 2024 by Femi Olugbon
///
/// ChangeLog see: i2c_lcd_pcf8574.h

#include <stdio.h>
#include "i2c_lcd_pcf8574.h"
#include "esp_log.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define TAG "I2C_LCD_PCF8574"

#define I2C_MASTER_TIMEOUT_MS 1000


// private functions
static void lcd_send(i2c_lcd_pcf8574_handle_t* lcd, uint8_t value, bool is_data);
static void lcd_write_nibble(i2c_lcd_pcf8574_handle_t* lcd, uint8_t half_byte, bool is_data, i2c_cmd_handle_t cmd);
static void lcd_write_i2c(i2c_lcd_pcf8574_handle_t* lcd, uint8_t data, bool is_data, bool enable);

void lcd_init(i2c_lcd_pcf8574_handle_t* lcd, uint8_t i2c_addr, i2c_port_t i2c_port) {
    lcd->i2c_addr = i2c_addr;
    lcd->i2c_port = i2c_port;
    lcd->backlight = 0;
    lcd->entrymode = 0x02; // Init the LCD with an internal reset
    lcd->displaycontrol = 0x04;
    lcd->rs_mask = 0x01;
    lcd->rw_mask = 0x00;
    lcd->enable_mask = 0x04;
    lcd->data_mask[0] = 0x10;
    lcd->data_mask[1] = 0x20;
    lcd->data_mask[2] = 0x40;
    lcd->data_mask[3] = 0x80;
    lcd->backlight_mask = 0x08;
}   // lcd_begin()

void lcd_begin(i2c_lcd_pcf8574_handle_t* lcd, uint8_t cols, uint8_t rows) {

    // Ensure the cols and rows stay within max limit
    lcd->cols = (cols > 80) ? 80 : cols;
    lcd->lines = (rows > 4) ? 4 : rows;

    lcd->row_offsets[0] = 0x00;
    lcd->row_offsets[1] = 0x40;
    lcd->row_offsets[2] = 0x00 + cols;
    lcd->row_offsets[3] = 0x40 + cols;

    // Initialize the LCD
    lcd_write_i2c(lcd, 0x00, false, false);
    esp_rom_delay_us(50000);

    // This follows after the reset mode
    lcd->displaycontrol = 0x04;
    lcd->entrymode = 0x02;

    // The following are the reset sequence: Please see "Initialization instruction in the PCF8574 datasheet."
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    // We left-shift the device addres and add the read/write command
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    
    lcd_write_nibble(lcd, 0x03, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    esp_rom_delay_us(4500);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, 0x03, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    esp_rom_delay_us(200);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, 0x03, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    esp_rom_delay_us(200);

    // Set the data interface to 4-bit interface (PCF8574 uses 4-bit interface)
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, 0x02, false, cmd);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    // Instruction: function set = 0x20
    lcd_send(lcd, 0x20 | (rows > 1 ? 0x08 : 0x00), false);

    // Set the display parameters (turn on display, clear, and set left to right)
    lcd_display(lcd);
    lcd_clear(lcd);
    lcd_left_to_right(lcd);
}  // lcd_begin()

// Clear the display content
void lcd_clear(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Clear display = 0x01
    lcd_send(lcd, 0x01, false);
    // Clearing the display takes a while: takes approx. 1.5ms
    esp_rom_delay_us(1600);
}  // lcd_clear()

// Set the display to home
void lcd_home(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Return home = 0x02
    lcd_send(lcd, 0x02, false);
    // Same as clearing the display: takes approx. 1.5ms
    esp_rom_delay_us(1600);
}  // lcd_home()

// Set the cursor to a new position.
void lcd_set_cursor(i2c_lcd_pcf8574_handle_t* lcd, uint8_t col, uint8_t row) {
    // Check the display boundaries
    if (row >= lcd->lines) {
        row = lcd->lines - 1;
    }
    if (col >= lcd->cols) {
        col = lcd->cols - 1;
    }
    // Instruction: Set DDRAM address = 080
    lcd_send(lcd, 0x80 | (lcd->row_offsets[row] + col), false);
}  // lcd_set_cursor()

// Turn off the display: fast operation
void lcd_no_display(i2c_lcd_pcf8574_handle_t* lcd) {
    // Display Control: Display on/off control = 0x04
    lcd->displaycontrol &= ~0x04;
    // Instruction: Display mode: 0x08
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}  // lcd_no_display()

// Turn on the display: fast operation
void lcd_display(i2c_lcd_pcf8574_handle_t* lcd) {
    // Display Control: Display on/off control = 0x04
    lcd->displaycontrol |= 0x04;
    // Instruction: Display mode: 0x08
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}  // lcd_display()

// Turn on the cursor
void lcd_cursor(i2c_lcd_pcf8574_handle_t* lcd) {
    // Display Control: Cursor on/off control = 0x02
    lcd->displaycontrol |= 0x02;
    // Instruction: Display mode: 0x08
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}  // lcd_cursor()

// Turn off the cursor
void lcd_no_cursor(i2c_lcd_pcf8574_handle_t* lcd) {
    // Display Control: Cursor on/off control = 0x02
    lcd->displaycontrol &= ~0x02;
    // Instruction: Display mode: 0x08
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}  // lcd_no_cursor()

// Turn on the blinking
void lcd_blink(i2c_lcd_pcf8574_handle_t* lcd) {
    // Display Control: Blink on/off control = 0x01
    lcd->displaycontrol |= 0x01;
    // Instruction: Display mode: 0x08
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}  // lcd_blink()

// Turn off the blinking
void lcd_no_blink(i2c_lcd_pcf8574_handle_t* lcd) {
    // Display Control: Blink on/off control = 0x01
    lcd->displaycontrol &= ~0x01;
    // Instruction: Display mode: 0x08
    lcd_send(lcd, 0x08 | lcd->displaycontrol, false);
}  // lcd_no_blink()

// This command will scroll the display left by one step without changing the RAM
void lcd_scroll_display_left(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Cursor or display shift - 0x10
    // Instruction: Display mode: 0x08
    // Control: Left shift control = 0x00
    // 0x10 | 0x08 | 0x00 = 0x18
    lcd_send(lcd, 0x18, false);
}  // lcd_scroll_display_left()

// This command will scroll the display right by one step without changing the RAM
void lcd_scroll_display_right(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Cursor or display shift - 0x10
    // Instruction: Display mode: 0x08
    // Control: Left shift control = 0x04
    // 0x10 | 0x08 | 0x04 = 0x1C
    lcd_send(lcd, 0x1C, false);
}  // lcd_scroll_display_right()

// Controlling the entry mode: This is for text that flows left to right
void lcd_left_to_right(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Entry mode set, set increment/decrement = 0x02
    lcd->entrymode |= 0x02;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}  // lcd_left_to_right()

// Controlling the entry mode: This is for text that flows right to left
void lcd_right_to_left(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Entry mode set, clear increment/decrement = 0x02
    lcd->entrymode &= ~0x02;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}  // lcd_right_to_left()

// This will justify the text to the right from the cursor
void lcd_autoscroll(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Entry mode set, set shift = 0x01
    lcd->entrymode |= 0x01;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}  // lcd_autoscroll()

// This will justify the text to the left from the cursor
void lcd_no_autoscroll(i2c_lcd_pcf8574_handle_t* lcd) {
    // Instruction: Entry mode set, clear shift = 0x01
    lcd->entrymode &= ~0x01;
    lcd_send(lcd, 0x04 | lcd->entrymode, false);
}  // lcd_no_autoscroll()

// Setting the backlight: It can only be turn on or off.
// Current backlight value is saved in the i2c_lcd_pcf8574_handle_t struct for further data transfers
void lcd_set_backlight(i2c_lcd_pcf8574_handle_t* lcd, uint8_t brightness) {
    // Place the backlight value in the lcd struct
    lcd->backlight = brightness;
    // Send no data
    lcd_write_i2c(lcd, 0x00, true, false);
}  // lcd_set_backlight()

// Custom character creation: allows us to create up to 8 custom characters in the CGRAM locations
void lcd_create_char(i2c_lcd_pcf8574_handle_t* lcd, uint8_t location, uint8_t charmap[]) {
    location &= 0x7;  // Only 8 locations are available
    // Set the CGRAM address
    lcd_send(lcd, 0x40 | (location << 3), false);
    for (int i = 0; i < 8; i++) {
        lcd_write(lcd, charmap[i]);
    }
}  // lcd_create_char()

// Write a byte to the LCD
void lcd_write(i2c_lcd_pcf8574_handle_t* lcd, uint8_t value) {
    lcd_send(lcd, value, true);
}  // lcd_write()

// Print characters to the LCD: cursor set or clear instruction must preceded this instruction, or it will write on the current text.
void lcd_print(i2c_lcd_pcf8574_handle_t* lcd, const char* str) {
    while (*str) {
        lcd_write(lcd, *str++);
    }
}  // lcd_print()

// Additional function to print numbers as formatted string
void lcd_print_number(i2c_lcd_pcf8574_handle_t* lcd, uint8_t col, uint8_t row, uint8_t buf_len, const char *str, ...) {
    //  Ensure the buffer length is greater than zero
    if (buf_len == 0)
    {
        ESP_LOGE(TAG, "Buffer length must be greater than 0");
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
    
}  // lcd_print_number()


// Private functions: derived from the esp32 i2c_master driver


static void lcd_send(i2c_lcd_pcf8574_handle_t* lcd, uint8_t value, bool is_data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (lcd->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    lcd_write_nibble(lcd, (value >> 4 & 0x0F), is_data, cmd);
    lcd_write_nibble(lcd, (value & 0x0F), is_data, cmd);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send data to LCD: %s", esp_err_to_name(ret));
    }
}  // lcd_send()

// Write a nibble / half byte with ACK
static void lcd_write_nibble(i2c_lcd_pcf8574_handle_t* lcd, uint8_t half_byte, bool is_data, i2c_cmd_handle_t cmd) {

    // Map the data to the given pin connections
    uint8_t data = is_data ? lcd->rs_mask : 0;

    // Don't use rw_mask here
    if (lcd->backlight > 0) {
        data |= lcd->backlight_mask;
    }
    // Allow arbitrary pin configuration
    if (half_byte & 0x01) data |= lcd->data_mask[0];
    if (half_byte & 0x02) data |= lcd->data_mask[1];
    if (half_byte & 0x04) data |= lcd->data_mask[2];
    if (half_byte & 0x08) data |= lcd->data_mask[3];

    i2c_master_write_byte(cmd, data | lcd->enable_mask, true);
    i2c_master_write_byte(cmd, data, true);
}  // lcd_write_nibble()

// Private function to change the PCF8574 pins to the given value.
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
    esp_err_t ret = i2c_master_cmd_begin(lcd->i2c_port, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write to LCD: %s", esp_err_to_name(ret));
    }
}  // lcd_write_i2c()