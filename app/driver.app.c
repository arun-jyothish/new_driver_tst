#include <stdio.h>
#include <linux/fs.h>
#include <linux/types.h>
/* #include <sys/stat.h> */
/* #include <linux/fcntl.h> */
#include <unistd.h>

void init(){
	printf("Driver application start");
}

void main(){
	int fd = open ("/dev/my_driver",2);
	init();
	while(1){
	char buf [3];
	read(fd, buf , 3);
	sleep(1);
	write(fd, buf , 3 );
	sleep(1);
	}
}
