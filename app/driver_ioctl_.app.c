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
	printf("Driver application start");
	int fd = open ("/dev/my_driver",2);
	if ( fd != 0 ){
		printf("errorr ... opening");
	}
	printf("Enter cmd (1: LED ON, 2: LED_OFF, 3: BLINK)\n");
	while(1){
		ioctl(fd,ON);
		printf("on.. %ld\n",ON);
		sleep(1);
		ioctl(fd,OFF);
		printf("off.. %ld\n",OFF);
		sleep(1);
	}
}

/*
	int cmd;
	char cmd_str [15];
	scanf(" %s",&cmd);
   write(fd, cmd_str , sizeof(cmd_str) );
   sleep(1)
   write(fd, cmd_str , sizeof(cmd_str) );
   sleep(1)

   switch(cmd){
   case 1:
   printf("LED_BLINK\n");
   write(fd, cmd_str , strlen(cmd_str) );
   break;
   case 2:
   printf("LED_BLINK\n");
   write (fd,cmd_str);
   break;
   case 3:
   printf("LED_BLINK\n");
   write (fd,"LED_BLINK");
   while(1){
   write(fd, cmd_str , strlen(cmd_str) );
   }
   sleep(1);
   }
   */
