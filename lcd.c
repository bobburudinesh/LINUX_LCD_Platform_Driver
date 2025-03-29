#include <linux/device.h>
#include <linux/delay.h>
#include "lcd_platform_driver.h"
#include "gpio.h"
#include "lcd.h"

static void gpio_write_4_bits(struct device *dev, u8 value);

int lcd_init(struct device *dev) {
	gpio_direction_config(LCD_RS, GPIO_DIR_OUT, dev);
	gpio_direction_config(LCD_RW, GPIO_DIR_OUT, dev);
	gpio_direction_config(LCD_EN, GPIO_DIR_OUT, dev);
	gpio_direction_config(LCD_D4, GPIO_DIR_OUT, dev);
	gpio_direction_config(LCD_D5, GPIO_DIR_OUT, dev);
	gpio_direction_config(LCD_D6, GPIO_DIR_OUT, dev);
	gpio_direction_config(LCD_D7, GPIO_DIR_OUT, dev);
	

	gpio_write_value(LCD_RS, GPIO_VALUE_LOW, dev);
	gpio_write_value(LCD_RW, GPIO_VALUE_LOW, dev);
	gpio_write_value(LCD_EN, GPIO_VALUE_LOW, dev);
	gpio_write_value(LCD_D4, GPIO_VALUE_LOW, dev);
	gpio_write_value(LCD_D5, GPIO_VALUE_LOW, dev);
	gpio_write_value(LCD_D6, GPIO_VALUE_LOW, dev);
	gpio_write_value(LCD_D7, GPIO_VALUE_LOW, dev);
	/*Wait for more than 15ms*/
	mdelay(30);
	gpio_write_4_bits(dev, 0x03);
	mdelay(10);
	gpio_write_4_bits(dev, 0x03);
	mdelay(10);
	gpio_write_4_bits(dev, 0x03);
	gpio_write_4_bits(dev, 0x02);
	
	
	return 0;
}
void lcd_deinit(struct device *dev) {
	lcd_clear(dev);
	lcd_return_home(dev);
}
void lcd_send_command(struct device *dev, u8 command) {
	gpio_write_value(LCD_RS, LOW_VALUE, dev);

	gpio_write_value(LCD_RW, LOW_VALUE, dev);

	gpio_write_4_bits(dev, (command >> 4));
	gpio_write_4_bits(dev, command);
}
void lcd_write_text(struct device *dev, char *message) {
	do {
		lcd_write_char(dev, (u8)*message++);
	} while(*message != '\0');
}
void lcd_write_char(struct device *dev, u8 character) {
	gpio_write_value(LCD_RS, HIGH_VALUE, dev);
	gpio_write_value(LCD_RW, LOW_VALUE, dev);
	gpio_write_4_bits(dev, (character >> 4));
	gpio_write_4_bits(dev, character);	
}
void lcd_clear(struct device *dev) {
	lcd_send_command(dev, LCD_CMD_CLEAR);
	mdelay(2);
}
void lcd_return_home(struct device *dev) {
	lcd_send_command(dev, LCD_CMD_RETURN_HOME);
	mdelay(2);
}
void lcd_set_cursor(struct device *dev, u8 x, u8 y) {
	y--;
	switch(x){
		case 1:
			lcd_send_command(dev,(y |= LCD_FIRST_LINE_DDRAM_ADDR));
			break;
		case 2:
			lcd_send_command(dev,(y |= LCD_SECOND_LINE_DDRAM_ADDR));
                        break;
		default:
			break;
	}
}
void lcd_enable(struct device *dev) {
	gpio_write_value(LCD_EN, LOW_VALUE, dev);
	udelay(10);
	gpio_write_value(LCD_EN, HIGH_VALUE, dev);
	udelay(10);
	gpio_write_value(LCD_EN, LOW_VALUE, dev);
	udelay(100);
}

static void gpio_write_4_bits(struct device *dev, u8 value) {
	gpio_write_value(LCD_D4, ((value >> 0) & 0x01), dev);
	gpio_write_value(LCD_D5, ((value >> 1) & 0x01), dev);
	gpio_write_value(LCD_D6, ((value >> 2) & 0x01), dev);
	gpio_write_value(LCD_D7, ((value >> 3) & 0x01), dev);
	
	lcd_enable(dev);
}
