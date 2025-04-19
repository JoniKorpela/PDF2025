#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Constants
#define MAX_LINES 2
#define MAX_CHARS 16

// LCD control functions
void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(int line, int position);
void lcd_string(const char *s);
void lcd_send_byte(uint8_t val, int mode);
void lcd_show();

void display_lcd_message(void);

// Internal helpers (you could make these static in the .c file if not needed externally)
void i2c_write_byte(uint8_t val);
void lcd_toggle_enable(uint8_t val);
void lcd_char(char val);

#endif // LCD1602_H