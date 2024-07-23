/// \file i2c_lcd_pcf8574.h
/// \brief Liquid Crystal display driver with PCF8574 adapter for esp-idf
///
/// \author Femi Olugbon, https://iamflinks.github.io
/// \copyright Copyright (c) 2024 by Femi Olugbon
///
/// The library work is lincensed under a BSD style license.
///
/// \details
/// This library can drive a LCD based on the Hitachi's HD44790 display chip that is wired through a PCF8574 I2C converter. It uses the esp-idf i2c_driver component for communication. The library was adapted from the LiquidCrystal_PCF8574 (Mathias Hertel) and LiquidCrystal_I2C Arduino libraries.
///
///
/// ChangeLog:
/// ===========
/// * 07/22/2024 --> Created
/// * 07/23/2024 --> Added number printing functionality
///

#pragma once

#ifndef I2C_LCD_PCF8574_H
#define I2C_LCD_PCF8574_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/i2c.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LCD_MAX_ROWS 4

typedef struct
{
    uint8_t i2c_addr;
    uint8_t backlight;
    uint8_t cols;
    uint8_t lines;
    uint8_t entrymode;
    uint8_t displaycontrol;
    uint8_t row_offsets[LCD_MAX_ROWS];
    uint8_t rs_mask;
    uint8_t rw_mask;
    uint8_t enable_mask;
    uint8_t backlight_mask;
    uint8_t data_mask[4];
    i2c_port_t i2c_port;
} i2c_lcd_pcf8574_handle_t;


// Initialize the LCD
void lcd_init(i2c_lcd_pcf8574_handle_t* lcd, uint8_t i2c_addr, i2c_port_t i2c_port);

// Begin using the LCD
void lcd_begin(i2c_lcd_pcf8574_handle_t* lcd, uint8_t cols, uint8_t rows);

// Clear the LCD
void lcd_clear(i2c_lcd_pcf8574_handle_t* lcd);

// Move cursor to home position
void lcd_home(i2c_lcd_pcf8574_handle_t* lcd);

// Set cursor position
void lcd_set_cursor(i2c_lcd_pcf8574_handle_t* lcd, uint8_t col, uint8_t row);

// Turn the display on/off
void lcd_no_display(i2c_lcd_pcf8574_handle_t* lcd);
void lcd_display(i2c_lcd_pcf8574_handle_t* lcd);

// Turn the cursor on/off
void lcd_cursor(i2c_lcd_pcf8574_handle_t* lcd);
void lcd_no_cursor(i2c_lcd_pcf8574_handle_t* lcd);

// Turn blinking cursor on/off
void lcd_blink(i2c_lcd_pcf8574_handle_t* lcd);
void lcd_no_blink(i2c_lcd_pcf8574_handle_t* lcd);

// Scroll the display
void lcd_scroll_display_left(i2c_lcd_pcf8574_handle_t* lcd);
void lcd_scroll_display_right(i2c_lcd_pcf8574_handle_t* lcd);

// Set the direction for text that flows automatically
void lcd_left_to_right(i2c_lcd_pcf8574_handle_t* lcd);
void lcd_right_to_left(i2c_lcd_pcf8574_handle_t* lcd);

// Turn on/off autoscroll
void lcd_autoscroll(i2c_lcd_pcf8574_handle_t* lcd);
void lcd_no_autoscroll(i2c_lcd_pcf8574_handle_t* lcd);

// Set backlight brightness
void lcd_set_backlight(i2c_lcd_pcf8574_handle_t* lcd, uint8_t brightness);

// Create a custom character
void lcd_create_char(i2c_lcd_pcf8574_handle_t* lcd, uint8_t location, uint8_t charmap[]);

// Write a character to the LCD
void lcd_write(i2c_lcd_pcf8574_handle_t* lcd, uint8_t value);

// Print a string to the LCD
void lcd_print(i2c_lcd_pcf8574_handle_t* lcd, const char* str);

void lcd_print_number(i2c_lcd_pcf8574_handle_t* lcd, uint8_t col, uint8_t row, uint8_t buf_len, const char *str, ...);


#ifdef __cplusplus
}
#endif // C++ extern

#endif // I2C_LCD_PCF8574_H