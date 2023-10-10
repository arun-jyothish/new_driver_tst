#include <stdio.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <stdint.h>
/* #include <sys/stat.h> */
#include <linux/fcntl.h>
#include <unistd.h>

#include  "../my_driver_generic.h"


void main(){
	printf("Driver application start\n");
	int fd = open ("/dev/my_driver",2);
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

