# API Reference

## Header files

- [i2c_lcd_pcf8574.h](#file-i2c_lcd_pcf8574h)

## File i2c_lcd_pcf8574.h

_Driver for the I2C\_LCD\_PCF8574 LCD adapter._

To use this driver:

* Initialize the ESP32 in master mode using [**i2c\_master\_init**](#function-i2c_master_init)
* Initialize the driver with [**lcd\_init()**](#function-lcd_init)


_Initialize the esp32 i2c master._
```c
#define I2C_MASTER_SCL_IO 22        // GPIO PIN for I2C master clock
#define I2C_MASTER_SDA_IO 21        // GPIO PIN for I2C master data
#define I2C_MASTER_NUM I2C_NUM_0    // I2C port number for master dev
#define I2C_MASTER_FREQ_HZ 100000   // I2C master clock frequency (100kHz or 400kHz)

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
```

**Parameters:**


* `cconf` Pointer to the i2c configuration struct. The driver takes ata refrence to the struct.


**Returns:**

void, logs error to the esp32 monitor.

_Initialize the I2C\_LCD\_PCF8574 driver._
```c
#define LCD_ADDR 0x27               // I2C address of the LCD (default address)
#define LCD_COLS 16                 // Number of columns in the LCD
#define LCD_ROWS 2                  // Number of rows in the LCD

// Create the i2c_lcd_pcf8574_handle_t struct
i2c_lcd_pcf8574_handle_t lcd;

void app_main(void)
{
    i2c_master_init();
    lcd_init(&lcd, LCD_ADDR, I2C_MASTER_NUM);
    lcd_begin(&lcd, LCD_COLS, LCD_ROWS);
}
```

**Parameters:**


* `lcd` Pointer to the configuration struct. The driver makes a copy, so can point to a local variable.
* `LCD_ADDR` LCD i2c device address 
* `LCD_COLS` LCD number of characters (columns).
* `LCD_ROWS` LCD number of lines.
* `I2C_MASTER_NUM` I2C port number for the master. 


**Returns:**

void: logs error to the esp32 monitor.