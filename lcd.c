#include <linux/device.h>

#include "lcd_platform_driver.h"
#include "gpio.h"
#include "lcd.h"



int lcd_init(struct device *dev) {
	return 0;
}
void lcd_deinit(struct device *dev) {

}
void lcd_send_command(struct device *dev, u8 command) {

}
void lcd_write_text(struct device *dev, char *message) {

}
void lcd_write_char(struct device *dev, u8 character) {

}
void lcd_clear(struct device *dev) {

}
void lcd_return_home(struct device *dev) {

}
void lcd_set_cursor(struct device *dev, u8 x, u8 y) {

}
void lcd_enable(struct device *dev) {

}

