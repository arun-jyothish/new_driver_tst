#include <stdio.h>
#include <linux/gpio.h>


void main(){
	int number = 200;
	/* int val =  gpio_is_valid( number ); */
	int val =  gpio_request( number );
	printf("app started .. \n");
	printf("gpio validity %d.. \n", val );
	return;
}
