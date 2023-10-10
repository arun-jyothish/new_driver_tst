#include <linux/ioctl.h>
#include <stdio.h>
#include <unistd.h>


#define 	WR_VALUE	_IOW('d','a', int32_t *)
#define 	RD_VALUE	_IOW('d','b', int32_t *)


void main(){
	while (1)
}
