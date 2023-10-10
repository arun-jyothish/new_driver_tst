
#ifndef MY_DRIVER_H
#define MY_DRIVER_H

// define .begin
#define	gpio_b_base_addr	(0x20A4000)
#define	gpio_y_base_addr	(0x20B4000)
#define	gpio_r_base_addr	(0x209C000)
#define		gpio_b_dr			gpio_b_base_addr
#define		gpio_y_dr			gpio_y_base_addr
#define		gpio_r_dr			gpio_r_base_addr
#define		gpio_b_gdir			(gpio_b_base_addr+0x04)
#define		gpio_y_gdir			(gpio_y_base_addr+0x04)
#define		gpio_r_gdir			(gpio_r_base_addr+0x04)

#define 	led_blue_pin		(26)
#define 	led_yellow_pin		(8)
#define 	led_red_pin			(24)

#define 	WR_VALUE	_IOW('d','a', int32_t *)
#define 	RD_VALUE	_IOW('d','b', int32_t *)
#define 	ON		_IOW('d','b', int32_t *)
#define 	OFF		_IOW('d','c', int32_t *)

#define 	LIGHT_ON	1	
#define 	LIGHT_OFF	0	

static enum status{
	OPEN_SUCC = 8001,
	OPEN_FAIL,
	READ_SUCC,
	READ_FAIL,
	WRITE_SUCC,
	WRITE_FAIL,
	INIT_SUCC,
};

// global var
static void __iomem*		gpio_b_gdir_vm;
static void __iomem*		gpio_b_dr_vm  ;
static void __iomem*		gpio_y_gdir_vm;
static void __iomem*		gpio_y_dr_vm  ;
static void __iomem*		gpio_r_gdir_vm;
static void __iomem*		gpio_r_dr_vm  ;

static struct device * my_device ;
static struct class * my_class ;
static int major	;
static char str_cmd [20] ;

// fn prototype
static void setup (void);
static ssize_t read_fn (struct file *, char *, size_t, loff_t *);
static ssize_t write_fn (struct file *, const char *, size_t, loff_t *);
static int open_fn (struct inode *, struct file *);
static int release_fn (struct inode *, struct file *);
static long ioctl_fn(struct file *fl,unsigned int cmd, unsigned long arg);
static void unMap(void );
static void mapIo(void );
static void ledAll( int arg );


static struct file_operations fops = {
owner: THIS_MODULE,
	 read : read_fn,
	 write : write_fn,
	 open : open_fn,
	 release : release_fn,
	 unlocked_ioctl : ioctl_fn
};

#define DEVICE_NAME  "my_driver"
#define DEVICE_CLASS "my_driver_class" 


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ARUN JYOTHISH K");
MODULE_DESCRIPTION(" device description my device driver .. ");
MODULE_VERSION("1.0");



#endif
