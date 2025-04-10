#include "lcd_i2c.h"
#include "driver/i2c.h"
#include "esp_log.h"

static const char *TAG = "LCD_I2C";
static i2c_port_t i2c_port;

// LCD commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

static void lcd_send_byte(uint8_t data, uint8_t mode) {
    uint8_t high_nibble = data & 0xF0;
    uint8_t low_nibble = (data << 4) & 0xF0;
    
    uint8_t data_arr[4];
    data_arr[0] = high_nibble | mode | LCD_BACKLIGHT | En;
    data_arr[1] = high_nibble | mode | LCD_BACKLIGHT;
    data_arr[2] = low_nibble | mode | LCD_BACKLIGHT | En;
    data_arr[3] = low_nibble | mode | LCD_BACKLIGHT;
    
    i2c_master_write_to_device(i2c_port, LCD_I2C_ADDRESS, data_arr, 4, pdMS_TO_TICKS(1000));
}

static void lcd_command(uint8_t cmd) {
    lcd_send_byte(cmd, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_init(i2c_port_t i2c_num) {
    i2c_port = i2c_num;
    
    // Initialization sequence
    vTaskDelay(pdMS_TO_TICKS(50));
    lcd_send_byte(0x03 << 4, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_byte(0x03 << 4, 0);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_byte(0x03 << 4, 0);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_byte(0x02 << 4, 0);
    
    // Set 4-bit mode, 2 lines, 5x8 font
    lcd_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
    
    // Display on, cursor off, blink off
    lcd_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    
    // Clear display
    lcd_clear();
    
    // Entry mode set
    lcd_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    
    ESP_LOGI(TAG, "LCD initialized");
}

void lcd_clear(void) {
    lcd_command(LCD_CLEARDISPLAY);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40};
    if (row > 1) row = 1;
    lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_byte(*str++, Rs);
    }
}

void lcd_print_centered(const char *str, uint8_t row) {
    uint8_t length = 0;
    const char *p = str;
    while (*p++) length++;
    
    uint8_t pos = (LCD_COLUMNS - length) / 2;
    lcd_set_cursor(pos, row);
    lcd_print(str);
}