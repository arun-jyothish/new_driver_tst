#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include "my_driver.h"

#define LED_GPIO_YELLOW		( (7-1)*32 + 8 )			// blue led gpio number
#define LED_GPIO_BLUE		( (3-1)*32 + 26 )			// blue led gpio number
#define LED_GPIO_RED		( (1-1)*32 + 24 )			// blue led gpio number

#define BTN_GPIO		( (3-1)*32 + 27 )			// btn gpio number
									//
char str_cmd[10];

module_init(my_driver_init);
module_exit(my_driver_exit);

static long ioctl_fn(struct file *fl,unsigned int cmd, unsigned long arg){

	printk(KERN_INFO "\n cmd rec: %d\n", cmd);
	switch(cmd){
		case ON:
			printk(KERN_INFO "DRIVER ENABLED SELECTED ..\n", cmd);
			break;	
		case OFF:
			printk(KERN_INFO "DRIVER DISABLED SELECTED ..\n", cmd);
			break;	
		case POLL:
			printk(KERN_INFO "POLLING MODE SELECTED ..\n", cmd);
			int val = gpio_get_value ( BTN_GPIO );
			break;	
		case INT:
			printk(KERN_INFO "INTERRUPT MODE SELECTED ..\n", cmd);
			break;	
	}
	return 0;
}

static int my_driver_init(void){
	printk(KERN_INFO "Module intit fn\n");
	major = register_chrdev(	0,DEVICE_NAME , &fops);
	if (major < 0 ){
		printk(KERN_INFO "Driver register failed\n");
		return major;
	}
	printk(KERN_INFO "My_driver Major NO: %d\n",major);
	///		create class
	my_class = class_create( THIS_MODULE, DEVICE_CLASS );
	if ( IS_ERR( my_class ) ){
		unregister_chrdev ( major , DEVICE_NAME );
		printk(KERN_ALERT "My_driver CLASS CREATE FAILED\n");
		return PTR_ERR(my_class);
	}
	printk(KERN_INFO "My_driver class created \n");

	///		create device 
	my_device = device_create( my_class, NULL ,MKDEV(major,0), NULL, DEVICE_NAME );
	if ( IS_ERR( my_device ) ){
		class_destroy( my_class );
		unregister_chrdev ( major , DEVICE_NAME );
		printk(KERN_ALERT "My_driver DEVICE CREATE FAILED\n");
		return PTR_ERR(my_device);
	}
	printk(KERN_INFO "device node created ... !\n");
	gpio_setup();

	gpio_free( LED_GPIO_YELLOW );
	gpio_free( BTN_GPIO );
	return INIT_SUCC;
}
static void  my_driver_exit(void){
	printk(KERN_INFO "Module exit fn called\n");
	device_destroy(my_class, MKDEV(major,0));
	unregister_chrdev(major, DEVICE_NAME);
	class_destroy(my_class);
	class_unregister(my_class);
	printk(KERN_INFO "Module exit fn\n");
}


int count = 1;
static ssize_t read_fn (struct file *fl, char * ch, size_t e, loff_t *oth){
	if ( gpio_get_value ( BTN_GPIO )){
		gpio_set_value( LED_GPIO_YELLOW , 0 );
		gpio_set_value( LED_GPIO_RED, 0 );
		gpio_set_value( LED_GPIO_BLUE, 0 );
		strcpy(str_cmd,"\r\tOFF");
		copy_to_user(ch, str_cmd , sizeof(str_cmd));
	}
	else
	{
		strcpy(str_cmd,"\r\tON ");
		copy_to_user(ch, str_cmd , sizeof(str_cmd));
		switch (count ){
			case 1:
				gpio_set_value( LED_GPIO_RED, 1 );
				break;
			case 2:
				gpio_set_value( LED_GPIO_YELLOW , 1 );
				break;
			case 3:
				gpio_set_value( LED_GPIO_BLUE, 1 );
				count = 0;
				break;
		}
		count++;
		msleep(500);
	}
	/* msleep(500); */
	/* printk(KERN_INFO "btn: %d\n", gpio_get_value ( BTN_GPIO )); */
	return sizeof(str_cmd);
}
static ssize_t write_fn (struct file *fl, const char *ch, size_t e, loff_t *oth){
	printk(KERN_INFO "writing .. \n");
	return 3;
}
static int open_fn (struct inode *lk, struct file *kl){
	gpio_setup();
	printk(KERN_INFO "open_fn called !\n");
	return 0;
}
static int release_fn(struct inode *lk, struct file *kl){
	gpio_free( LED_GPIO_YELLOW );
	gpio_free( BTN_GPIO );
	printk(KERN_INFO "close_fn called !\n");
	return 0;
}
static void gpio_setup(void){
	printk(KERN_INFO "BTN: %d\t LED: %d\t",BTN_GPIO,LED_GPIO_YELLOW);
	printk(KERN_INFO "RED_LED: %d\t",LED_GPIO_RED);
	printk(KERN_INFO "BLUE_LED: %d\n",LED_GPIO_BLUE);
	/* if ( ! gpio_is_valid(LED_GPIO_YELLOW) ) { */
	/* 	printk(KERN_INFO "GPIO is not valid .. \n"); } */

	if ( ENOSYS == gpio_request(LED_GPIO_YELLOW, "YELLOW_LED") ) {
		printk(KERN_INFO "Invalid syscall gpio_reques LED\n"); }
	if ( gpio_direction_output ( LED_GPIO_YELLOW , 0) ){
		printk(KERN_INFO "can't set gpio as output for LED\n"); }
	/* ------------------------------------------------------------------------- */
	if ( ENOSYS == gpio_request(BTN_GPIO, "BTN") ) {
		printk(KERN_INFO "Invalid syscall gpio_reques BTN\n"); }
	if ( gpio_direction_input( BTN_GPIO   )){
		printk(KERN_INFO "can't set gpio as output for BTN\n"); }
	gpio_set_debounce( BTN_GPIO, 50 );
	/* ------------------------------------------------------------------------- */
	if ( ENOSYS == gpio_request(LED_GPIO_RED, "RED_LED") ) {
		printk(KERN_INFO "Invalid syscall gpio_reques LED\n"); }
	if ( gpio_direction_output ( LED_GPIO_RED, 0) ){
		printk(KERN_INFO "can't set gpio as output for LED\n"); }
	/* ------------------------------------------------------------------------- */
	if ( ENOSYS == gpio_request(LED_GPIO_BLUE, "BLUE_LED") ) {
		printk(KERN_INFO "Invalid syscall gpio_reques LED\n"); }
	if ( gpio_direction_output ( LED_GPIO_BLUE, 0) ){
		printk(KERN_INFO "can't set gpio as output for LED\n"); }
	/* ------------------------------------------------------------------------- */
	/* gpio_set_value (LED_GPIO_YELLOW , 0); 			// Turn off LED */
	/* gpio_set_value (LED_GPIO_RED, 0); 			// Turn off LED */
	/* gpio_set_value (LED_GPIO_BLUE, 0); 			// Turn off LED */
}

