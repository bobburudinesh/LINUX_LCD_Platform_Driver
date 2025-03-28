#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include "lcd_platform_driver.h"


int lcd_probe(struct platform_device *pdev);
void lcd_remove(struct platform_device *pdev);
int create_device_files(struct device *dev, struct lcd_dev_private_data *lcd_data);
ssize_t lcdcmd_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t lcdtext_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t lcdscroll_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t lcdscroll_show (struct device *dev, struct device_attribute *attr, char *buf);
ssize_t lcdxy_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t lcdxy_show (struct device *dev, struct device_attribute *attr, char *buf);




struct lcd_dev_private_data lcd_data = {
        .lcd_scroll = 0,
        .lcdxy = "(1,1)"
};
struct lcd_drv_private_data drv_data;




ssize_t lcdcmd_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
	
	return 0;	
}
DEVICE_ATTR_WO(lcdcmd);



ssize_t lcdtext_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
	return 0;
}

DEVICE_ATTR_WO(lcdtext);



ssize_t lcdscroll_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
	return 0;
}

ssize_t lcdscroll_show (struct device *dev, struct device_attribute *attr, char *buf) {
	return 0;
}

DEVICE_ATTR_RW(lcdscroll);



ssize_t lcdxy_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
	return 0;
}

ssize_t lcdxy_show (struct device *dev, struct device_attribute *attr, char *buf) {
	return 0;
}

DEVICE_ATTR_RW(lcdxy);

struct attribute *lcd_attrs[] = {
	&dev_attr_lcdcmd.attr,
	&dev_attr_lcdtext.attr,
	&dev_attr_lcdscroll.attr,
	&dev_attr_lcdxy.attr,
	NULL
};

struct attribute_group lcd_attr_group = {
	.attrs = lcd_attrs
};

static const struct attribute_group *lcd_attr_groups[] = {
	&lcd_attr_group,
	NULL
};

int create_device_files(struct device *dev,struct lcd_dev_private_data *lcd_data) {
	lcd_data->dev = device_create_with_groups(drv_data.class_lcd, dev, 0, lcd_data, lcd_attr_groups, "LCD16x2");
 	if(IS_ERR(lcd_data->dev)){
		dev_err(dev, "Creation of device files failed\n");
		return PTR_ERR(lcd_data->dev);
	}	
	return 0;
}


int lcd_probe(struct platform_device *pdev) {
	int ret;
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
	ret = create_device_files(dev, &lcd_data);	
	if(ret) {
		dev_err(dev,"LCD sysfs device create failed\n");
		return ret;
	}

	dev_info(dev,"probe sucessful\n");

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


