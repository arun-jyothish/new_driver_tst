#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/io.h>
#include "my_driver.h"



// fn definition 

static long ioctl_fn(struct file *fl,unsigned int cmd, unsigned long arg){

	printk(KERN_INFO "\n cmd rec: %d\n", cmd);
	switch(cmd){
		case ON:
			iowrite32( 0b1 << led_blue_pin, gpio_b_dr_vm);		// turn on led 
			iowrite32( 0b0 << led_red_pin, gpio_r_dr_vm);		// turn on led 
			break;	
		case OFF:
			iowrite32( 0b0 << led_blue_pin, gpio_b_dr_vm);		// turn off led 
			iowrite32( 0b1 << led_red_pin, gpio_r_dr_vm);		// turn off led 
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


	mapIo();
	setup ();
	ledAll(LIGHT_OFF);
	unMap();
	return INIT_SUCC;

}
static void  my_driver_exit(void){
	mapIo();
	setup ();
	ledAll(LIGHT_ON);								// turn on all led
	unMap();

	printk(KERN_INFO "Module exit fn called\n");
	device_destroy(my_class, MKDEV(major,0));
	unregister_chrdev(major, DEVICE_NAME);
	class_destroy(my_class);
	class_unregister(my_class);
	printk(KERN_INFO "Module exit fn\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

static ssize_t read_fn (struct file *fl, char * ch, size_t e, loff_t *oth){
	printk(KERN_INFO "read_fn called !\n");
	copy_to_user(ch , str_cmd , sizeof(str_cmd));
	iowrite32( 0b0 << led_blue_pin, gpio_b_dr_vm);		// turn off led 
	return 0;
}
static ssize_t write_fn (struct file *fl, const char *ch, size_t e, loff_t *oth){
	int sz = sizeof(ch);
	printk(KERN_INFO "write_fn called ! cmd len: %d\n",sz);
	printk(KERN_INFO "cmd: %s\n",ch);
	if ( !strncmp("ON",ch ,2 )){
		iowrite32( 0b1 << led_blue_pin, gpio_b_dr_vm);		// turn on led 
		strcpy( str_cmd,"LED ON");
	}
	if ( !strncmp("OFF",ch,3 ) ){
		static char str_cmd[] = "LED OFF";
		iowrite32( 0b0 << led_blue_pin, gpio_b_dr_vm);		// turn off led 
		strcpy( str_cmd,"LED ON");
	}
	return sz;
}

static int open_fn (struct inode *lk, struct file *kl){
	mapIo();
	setup();
	printk(KERN_INFO "open_fn called !\n");
	return 0;
}

static int release_fn(struct inode *lk, struct file *kl){
	unMap();
	printk(KERN_INFO "close_fn called !\n");
	return 0;
}

static void setup (void){
	u32 read  = ioread32(gpio_b_gdir_vm);		//  
	printk(KERN_INFO "reg val: %x\n",read);

	iowrite32( 0b1 << led_blue_pin , gpio_b_gdir_vm);		// sets blue led pin as output
	/* iowrite32( 0b1 << led_yellow_pin, gpio_y_gdir_vm);		// sets yello led pin as output */
	iowrite32( 0b1 << led_red_pin , gpio_r_gdir_vm);		// sets red led pin as output
											//
	read  = ioread32(gpio_b_gdir_vm);		//  
	printk(KERN_INFO "reg val: %x\n",read);
}

static void mapIo(void){
	gpio_b_gdir_vm	= ioremap(gpio_b_gdir, sizeof(u32));
	gpio_y_gdir_vm	= ioremap(gpio_y_gdir, sizeof(u32));
	gpio_r_gdir_vm	= ioremap(gpio_r_gdir, sizeof(u32));

	gpio_b_dr_vm	= ioremap(gpio_b_dr, sizeof(u32));
	gpio_y_dr_vm	= ioremap(gpio_y_dr, sizeof(u32));
	gpio_r_dr_vm	= ioremap(gpio_r_dr, sizeof(u32));
}

static void unMap(void ){
	iounmap(gpio_b_dr_vm);
	iounmap(gpio_y_dr_vm);
	iounmap(gpio_r_dr_vm);

	iounmap(gpio_b_gdir_vm);
	iounmap(gpio_y_gdir_vm);
	iounmap(gpio_r_gdir_vm);
}
static void ledAll( int arg ){
	if (!arg){
		iowrite32( 0b0 << led_blue_pin, gpio_b_dr_vm);		// turn off led 
		iowrite32( 0b0 << led_yellow_pin, gpio_y_dr_vm);		// turn off led 
		iowrite32( 0b0 << led_red_pin, gpio_r_dr_vm);		// turn off led 
	}
	else{
		iowrite32( 0b1 << led_blue_pin, gpio_b_dr_vm);		// turn off led 
		iowrite32( 0b1 << led_yellow_pin, gpio_y_dr_vm);		// turn off led 
		iowrite32( 0b1 << led_red_pin, gpio_r_dr_vm);		// turn off led 

	}
}
