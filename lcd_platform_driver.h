#ifndef	LCD_PLATFORM_DRIVER
#define LCD_PLATFORM_DRIVER

/*Include files*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
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





#endif
