#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>


// define .begin
#define		gpio_b_gdir			(gpio_b_base_addr+0x04)
#define 	led_blue_pin		(26)
#define	gpio_b_base_addr	(0x20A4000)
#define fnP(sht)	void (* sht) ( char *dev )
// define .end


// global var
static void __iomem*		gpio_b_gdir_vm;
static void __iomem*		gpio_b_dr_vm  ;
struct device * my_device ;
struct class * my_class ;
int major	;


// fn prototype
/* int __init driver_init(void); */
ssize_t read_fn (struct file *, char *, size_t, loff_t *);
ssize_t write_fn (struct file *, const char *, size_t, loff_t *);
int open_fn (struct inode *, struct file *);
/* void device_release(void); */
 /* int (*release) (struct inode *, struct file *); */

struct file_operations fops = {
	owner: THIS_MODULE,
	read : read_fn,
	write : write_fn,
	open : open_fn,
	/* release: device_release, */
};

#define DEVICE_NAME  "my_driver"
#define DEVICE_CLASS "my_driver_class" 


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ARUN JYOTHISH K");
MODULE_DESCRIPTION(" device description my device driver .. ");
MODULE_VERSION("1.0");

// fn definition 

static int my_driver_init(void){

	printk(KERN_INFO "Module intit fn");
	major = register_chrdev(	0,DEVICE_NAME , &fops);
	if (major < 0 ){
	printk(KERN_INFO "Driver register failed\n");
	return major;
	}
	printk(KERN_INFO "My_driver Major NO: %d",major);
///		create class
	my_class = class_create( THIS_MODULE, DEVICE_CLASS );
	if ( IS_ERR( my_class ) ){
		unregister_chrdev ( major , DEVICE_NAME );
		printk(KERN_ALERT "My_driver CLASS CREATE FAILED");
		return PTR_ERR(my_class);
	}
	printk(KERN_INFO "My_driver class created ");

///		create device 
	my_device = device_create( my_class, NULL ,MKDEV(major,0), NULL, DEVICE_NAME );
	if ( IS_ERR( my_device ) ){
		class_destroy( my_class );
		unregister_chrdev ( major , DEVICE_NAME );
		printk(KERN_ALERT "My_driver DEVICE CREATE FAILED");
		return PTR_ERR(my_device);
	}
	printk(KERN_INFO "device node created ... !");

	gpio_b_gdir_vm	= ioremap(gpio_b_gdir, sizeof(u32));
/*
	static struct device_driver drv_serial = {
		.name = "serial_drv_tst",
		.owner = "Arun jyothish k",
		.shutdown = &shutdown,
	};
*/
	iowrite32( 0b0 << led_blue_pin, gpio_b_dr_vm);		// turn off led 
	return 0;

}

static void shutdown(void){
	printk(KERN_INFO "shutdown ..");
}


static void  my_driver_exit(void){
	device_destroy(my_class, MKDEV(major,0));
	class_unregister(my_class);
	class_destroy(my_class);
	unregister_chrdev(major, DEVICE_NAME);
	iowrite32( 0b1 << led_blue_pin, gpio_b_dr_vm);		// turn on led 
	iounmap(gpio_b_gdir_vm);
	printk(KERN_INFO "Module exit fn");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

ssize_t read_fn (struct file *fl, char * ch, size_t e, loff_t *oth){
	iowrite32( 0b0 << led_blue_pin, gpio_b_dr_vm);		// turn off led 
	return 0;
}
ssize_t write_fn (struct file *fl, const char *ch, size_t e, loff_t *oth){
	iowrite32( 0b1 << led_blue_pin, gpio_b_dr_vm);		// turn on led 
	return 0;
}
