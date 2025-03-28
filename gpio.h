#ifndef	LCD_GPIO_H
#define LCD_GPIO_H

#include <linux/types.h>

#define HIGH_VALUE	1
#define LOW_VALUE	0

#define GPIO_DIR_OUT	HIGH_VALUE
#define GPIO_DIR_IN	LOW_VALUE

#define GPIO_VALUE_LOW	LOW_VALUE
#define GPIO_VALUE_HIGH	HIGH_VALUE

int gpio_direction_config(u8 desc_id, u8 value, struct device *dev);
int gpio_write_value(u8 desc_id, u8 value, struct device *dev);

#endif
