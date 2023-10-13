#ifndef MY_DRIVER_GENERIC_H
#define MY_DRIVER_GENERIC_H

#define 	WR_VALUE	_IOW('d','a', int32_t *)
#define 	RD_VALUE	_IOR('d','b', int32_t *)
#define 	ON		_IOW('d','b', int32_t *)
#define 	OFF		_IOW('d','c', int32_t *)

#define 	POLL		_IOW('d','d', int32_t *)
#define 	INT		_IOW('d','e', int32_t *)

#define 	LIGHT_ON	1	
#define 	LIGHT_OFF	0	

static enum status{
	INIT_SUCC ,
	OPEN_SUCC ,
	OPEN_FAIL,
	READ_SUCC,
	READ_FAIL,
	WRITE_SUCC,
	WRITE_FAIL,
};

#endif
