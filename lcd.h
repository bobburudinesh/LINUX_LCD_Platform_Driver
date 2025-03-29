#ifndef LCD_16x2_H_
#define LCD_16x2_H_

#include <linux/types.h>

#define LCD_CMD_CLEAR	 	0x01
#define LCD_CMD_RETURN_HOME	0x02
#define LCD_CMD_ENTRY_MODE	0x06
#define LCD_CMD_DISPLAY_CNTL	0x0F
#define LCD_CMD_FUNCTION_SET	0x28
#define LCD_CMD_SET_CGRAM_ADDR	0x40
#define LCD_CMD_SET_DDRAM_ADDR	0x80


#define LCD_FIRST_LINE_DDRAM_ADDR	LCD_CMD_SET_DDRAM_ADDR
#define LCD_SECOND_LINE_DDRAM_ADDR	(LCD_CMD_SET_DDRAM_ADDR | 0x40)

#define LCD_ENABLE		1
#define LCD_DISABLE		0


int lcd_init(struct device *dev);
void lcd_deinit(struct device *dev);
void lcd_send_command(struct device *dev, u8 command);
void lcd_write_text(struct device *dev, char *message);
void lcd_write_char(struct device *dev, u8 character);
void lcd_clear(struct device *dev);
void lcd_return_home(struct device *dev);
void lcd_set_cursor(struct device *dev, u8 x, u8 y);
void lcd_enable(struct device *dev);

#endif
