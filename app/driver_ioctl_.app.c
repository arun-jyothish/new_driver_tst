#include <stdio.h>
#include <linux/fs.h>
#include <linux/types.h>
/* #include <sys/stat.h> */
/* #include <linux/fcntl.h> */
#include <unistd.h>


void main(){
	printf("Driver application start");
	int fd = open ("/dev/my_driver",2);
	printf("Enter cmd (1: LED ON, 2: LED_OFF, 3: BLINK)\n");
	int cmd;
	char cmd_str [15];
	scanf(" %s",&cmd);
	write(fd, cmd_str , strlen(cmd_str) );
/*
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
}
