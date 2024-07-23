# API Reference

## Header files

- [i2c_lcd_pcf8574.h](#file-i2c_lcd_pcf8574h)

## File i2c_lcd_pcf8574.h

_Driver for the I2C\_LCD\_PCF8574 LCD adapter._

To use this driver:

* Initialize the ESP32 in master mode using [**i2c\_master\_init()**](#function-i2c_master_init)
* Initialize the driver with [**lcd\_init()**](#function-lcd_init) and [**lcd\_init()**](#function-lcd_begin)
* Call the [**functions**](#functions) to set other lcd display characteristics.

## Structures and types

| Type | Name |
| ---: | :--- |
| struct | [**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t) <br>_I2C\_LCD\_PCF8574 driver configuration._ |

## Functions

| Type | Name |
| ---: | :--- |
| void | [**lcd\_init**](#function-lcd_init) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd, uint8_t i2c_addr, i2c_port_t i2c_port) <br> _Initialize the I2C\_LCD\_PCF8574 driver._ |
| void | [**lcd\_begin**](#function-lcd_begin) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd, uint8_t cols, uint8_t rows) <br> _Set the display size of the I2C\_LCD\_PCF8574 driver._ |
| void | [**lcd\_clear**](#function-lcd_clear) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Clear the LCD display._ |
| void | [**lcd\_home**](#function-lcd_home) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Set the LCD to home._ |
| void | [**lcd\_set\_cursor**](#function-lcd_set_cursor) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd, uint8_t col, uint8_t row) <br> _Set the LCD cursor to a new position._ |
| void | [**lcd\_no\_display**](#function-lcd_no_display) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Turn off the display._ |
| void | [**lcd\_display**](#function-lcd_display) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Turn on the display._ |
| void | [**lcd\_cursor**](#function-lcd_cursor) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Turn on the LCD cursor position._ |
| void | [**lcd\_no\_cursor**](#function-lcd_no_cursor) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Turn off the LCD cursor position._ |
| void | [**lcd\_blink**](#function-lcd_blink) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Turn on the LCD cursor blink._ |
| void | [**lcd\_no\_blink**](#function-lcd_no_blink) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _Turn off the LCD cursor blink._ |
| void | [**lcd\_scroll\_display_\left**](#function-lcd_scroll_display_left) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _This command will scroll the display left by one step without changing the RAM._ |
| void | [**lcd\_scroll\_display_\right**](#function-lcd_scroll_display_right) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _This command will scroll the display right by one step without changing the RAM._ |
| void | [**lcd\_left\_to\_right**](#function-lcd_left_to_right) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _This is for text that flows left to right._ |
| void | [**lcd\_right\_to\_left**](#function-lcd_right_to_left) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _This is for text that flows left to right._ |
| void | [**lcd\_auto\_scroll**](#function-lcd_autoscroll) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _This function will justify the text to the right from the cursor._ |
| void | [**lcd\_no\_auto\_scroll**](#function-lcd_no_autoscroll) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd) <br> _This function will justify the text to the left from the cursor._ |
| void | [**lcd\_set\_backlight**](#function-lcd_set_backlight) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd, uint8_t brightness) <br> _This function set the backlight brightness (PS: It can only be turn on or off)._ |
| void | [**lcd\_create\_charl**](#function-lcd_create_char) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd, uint8_t location, uint8_t charmap[]) <br> _This function allows us to create up to 8 custom characters in the CGRAM locations._ |
| void | [**lcd\_print**](#function-lcd_print) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd, const char* str) <br> _This function prints characters to the LCD._ |
| void | [**lcd\_print\_number**](#function-lcd_print_number) ([**i2c\_lcd\_pcf8574\_handle\_t**](#struct-i2c_lcd_pcf8574_handle_t)\* lcd, uint8_t col, uint8_t row, uint8_t buf_len, const char *str, ...) <br> _Additional function to print numbers as formatted string._ |


## Structures and Types Documentation

### struct `i2c_config_t`

_ESP-32 I2C\_MASTER driver configuration._

### struct `i2c_lcd_pcf8574_handle_t`

_I2C\_LCD\_PCF8574 driver configuration._

-  uint8\_t i2c_dev_addr  <br>I2C device address of the GPS. Defualt 0x27.

-  i2c\_port\_t i2c_port  <br>I2C bus number to which the LCD is connected


## Functions Documentation

### function `i2c_master_init`

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


* `void`


**Returns:**

* `void` logs error to the esp32 monitor.

### function `lcd_init`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_init(
    i2c_lcd_pcf8574_handle_t lcd,
    uint8_t i2c_addr,
    i2c_port_t i2c_port
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct. The driver makes a copy, so can point to a local variable.
* `i2c_addr` LCD i2c device address.
* `i2c_port` I2C port number for the master.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_begin`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_begin(
    i2c_lcd_pcf8574_handle_t lcd,
    uint8_t cols,
    uint8_t rows
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct. The driver makes a copy, so can point to a local variable.
* `cols` LCD character length (column length).
* `rows` LCD number of lines.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_clear`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_clear(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_home`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_home(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_set_cursor`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_set_cursor(
    i2c_lcd_pcf8574_handle_t lcd,
    uint8_t col,
    uint8_t row
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.
* `col` Column/Character position.
* `row` Line/Row position.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_no_display`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_no_display(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_display`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_display(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_cursor`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_cursor(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_no_cursor`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_no_cursor(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_blink`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_blink(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_no_blink`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_no_blink(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_scroll_display_left`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_scroll_display_left(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_scroll_display_right`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_scroll_display_right(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_left_to_right`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_left_to_right(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_right_to_left`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_right_to_left(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_autoscroll`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_autoscroll(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_no_autoscroll`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_no_autoscroll(
    i2c_lcd_pcf8574_handle_t lcd
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_set_backlight`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_set_backlight(
    i2c_lcd_pcf8574_handle_t lcd,
    uint8_t brightness
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.
* `brightness` Brightness value (0 - 255).

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_create_char`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_create_char(
    i2c_lcd_pcf8574_handle_t lcd,
    uint8_t location,
    uint8_t charmap[]
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.
* `location` Memory location to store the character value (0 - 7).
* `location` Custom character buffer (8 bytes).

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_print`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_print(
    i2c_lcd_pcf8574_handle_t lcd,
    const char* str
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.
* `str` Character array/strings.

**Returns:**

`void`: logs error to the esp32 monitor.

### function `lcd_print_number`

_Initialize the I2C\_LCD\_PCF8574 driver._

```c
void lcd_print_number(
    i2c_lcd_pcf8574_handle_t lcd,
    uint8_t col,
    uint8_t row,
    uint8_t buf_len,
    const char *str,
    ...
)
```

**Parameters:**

* `lcd` Pointer to the configuration struct.
* `col` Columns position.
* `row` Row position.
* `buf_len` Length of the formatted string buffer.
* `str` Formatted string buffer.
* `...` Formatted string arguments.

**Returns:**

`void`: logs error to the esp32 monitor.