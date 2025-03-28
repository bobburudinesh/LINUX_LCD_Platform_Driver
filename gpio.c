#include <linux/gpio/consumer.h>
#include <linux/device.h>
#include "gpio.h"
#include "lcd_platform_driver.h"

int gpio_direction_config(u8 desc_id, u8 value, struct device *dev) {
	struct lcd_dev_private_data *lcd_data = dev_get_drvdata(dev);
	struct gpio_desc *desc = lcd_data->desc[desc_id];
	int ret;
	if(value == GPIO_DIR_IN) {
		ret = gpiod_direction_input(desc);
	} else {
		ret = gpiod_direction_output(desc, LOW_VALUE);
	}
	return ret;
}

int gpio_write_value(u8 desc_id, u8 value, struct device *dev) {
	struct lcd_dev_private_data *lcd_data = dev_get_drvdata(dev);
        struct gpio_desc *desc = lcd_data->desc[desc_id];
	gpiod_set_value(desc, value);
	return 0;
}

