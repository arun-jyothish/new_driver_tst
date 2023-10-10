#include <stdio.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <stdint.h>
/* #include <sys/stat.h> */
#include <linux/fcntl.h>
#include <unistd.h>

#define 	WR_VALUE	_IOW('d','a', int32_t *)
#define 	ON		_IOW('d','b', int32_t *)
#define 	OFF		_IOW('d','c', int32_t *)
#define 	RD_VALUE	_IOR('d','d', int32_t *)

void main(){
	printf("Driver application start\n");
	int fd = open ("/dev/my_driver",2);
	if ( fd != 0 ){
		printf("errorr ... opening\n");
		return;
	}
	printf("\nEnter cmd (1: LED ON, 2: LED_OFF, 3: BLINK)\n");
	while(1){
		ioctl(fd,ON);
		printf("on.. %ld\n",ON);
		sleep(1);
		ioctl(fd,OFF);
		printf("off.. %ld\n",OFF);
		sleep(1);
	}
}

