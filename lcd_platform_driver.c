#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include "lcd_platform_driver.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : "fmt,__func__

#define DEVICE_COUNT    1
#define LCD_LINES (4+1+1+1)

enum {LCD_RS,LCD_RW,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7};

struct lcd_dev_private_data {
	int lcd_scroll;
	char lcdxy[8]; /*(16,16)*/
	struct device *dev;
	struct gpio_desc *desc[LCD_LINES];	
};

struct lcd_drv_private_data {
	struct class *class_lcd;
};


int lcd_probe(struct platform_device *pdev);
void lcd_remove(struct platform_device *pdev);


struct lcd_dev_private_data lcd_data = {
	.lcd_scroll = 0,
	.lcdxy = "(1,1)"
};
struct lcd_drv_private_data drv_data;

int lcd_probe(struct platform_device *pdev) {
	struct device *dev = &pdev->dev;
	dev_set_drvdata(dev, &lcd_data);
	lcd_data.desc[LCD_RS] = gpiod_get(dev, "rs", GPIOD_OUT_LOW);
	lcd_data.desc[LCD_RW] = gpiod_get(dev, "rw", GPIOD_OUT_LOW);
	lcd_data.desc[LCD_EN] = gpiod_get(dev, "en", GPIOD_OUT_LOW);
	lcd_data.desc[LCD_D4] = gpiod_get(dev, "d4", GPIOD_OUT_LOW);
	lcd_data.desc[LCD_D5] = gpiod_get(dev, "d5", GPIOD_OUT_LOW);
	lcd_data.desc[LCD_D6] = gpiod_get(dev, "d6", GPIOD_OUT_LOW);
	lcd_data.desc[LCD_D7] = gpiod_get(dev, "d7", GPIOD_OUT_LOW);
	if(IS_ERR(lcd_data.desc[LCD_RS]) || \
			IS_ERR(lcd_data.desc[LCD_RW]) || \
			IS_ERR(lcd_data.desc[LCD_EN]) || \
			IS_ERR(lcd_data.desc[LCD_D4]) || \
			IS_ERR(lcd_data.desc[LCD_D5]) || \
			IS_ERR(lcd_data.desc[LCD_D6]) || \
			IS_ERR(lcd_data.desc[LCD_D7])) {
	dev_err(dev, "Unable to acquire GPIO\n");
	return -EINVAL;
	}
	/*Configure direction of GPIO'S and init LCD*/
	
	dev_info(dev,"LCD init successful\n");

	return 0;
}
void lcd_remove(struct platform_device *pdev) {
	
}

struct of_device_id lcd_device_match[] = {
	{.compatible = "org,lcd16x2"},
	{ }
};

struct platform_driver lcd_platform_driver = {
	.probe = lcd_probe,
	.remove = lcd_remove,
	.driver = {
		.name = "lcd-sysfs-driver",
		.of_match_table = of_match_ptr(lcd_device_match)
	}
};


static int __init lcd_sysfs_init(void) {
	drv_data.class_lcd = class_create("lcd");
	platform_driver_register(&lcd_platform_driver);
	pr_info("Module Loaded\n");
	return 0;
}

static void __exit lcd_sysfs_exit(void) {
	platform_driver_unregister(&lcd_platform_driver);
	class_destroy(drv_data.class_lcd);
	pr_info("Module Unloaded\n");
}

module_init(lcd_sysfs_init);
module_exit(lcd_sysfs_exit);

MODULE_AUTHOR("Dinesh Bobburu");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Platform driver for 16x2 LCD display");


