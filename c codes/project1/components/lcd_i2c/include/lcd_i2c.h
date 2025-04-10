#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "driver/i2c.h"
#include "esp_err.h"

#define LCD_I2C_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

void lcd_init(i2c_port_t i2c_num);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_print_centered(const char *str, uint8_t row);

#endif // LCD_I2C_H