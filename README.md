# I2C_LCD_PCF8574 I2C Liquid Crystal Display driver

[![Component Registry](https://components.espressif.com/components/iamflinks/i2c_lcd_pcf8574/badge.svg)](https://components.espressif.com/components/iamflinks/i2c_lcd_pcf8574) 

This repository contains an ESP-IDF driver for a Hitachi HD44790 compatible LCD attached to a PCF8574 I2C adapter connected over I2C.

## Using the component

Run the following command in your ESP-IDF project to install this component
```bash
idf.py add-dependency "iamflinks/i2c_lcd_pcf8574"
```

## Example

To run the provided examples, create it as follows:

```bash
idf.py create-project-from-example- "iamflinks/i2c_lcd_pcf8574:i2c_lcd_pcf8574_example"
```

OR

```bash
idf.py create-project-from-example- "iamflinks/i2c_lcd_pcf8574:i2c_lcd_pcf8574_lcd_function"
```

Then build the project as usual:
```bash
cd i2c_lcd_pcf8574_example
idf.py build
```

OR

```bash
cd 2c_lcd_pcf8574_lcd_function
idf.py build
```

And flash the project to the board:
```bash
idf.py -p PORT flash monitor
```

The example uses GPIOs 21 and 22 for the SDA and SCL, respectively.

## Licence

This component is provided under Apache 2.0 license, see [LICENSE](LICENSE.md) file for details.


## Documentation

Please check the [API](api.md) file for detailed API documentation.