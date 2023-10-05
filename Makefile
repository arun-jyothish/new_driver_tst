KBUILD_EXTRA_SYMBOLS=n
CONFIG_MODULE_SIG=n
CONFIG_MODVERSIONS=n

obj-m += my_driver.o
# root_dir = /home/arun/Technexion/tech/linux
root_dir = /home/arun/new/linux


# make -C ${root_dir} M=${PWD} modules
final:
	make ARCH=arm CROSS_COMPILE=/home/arun/new/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- -C ${root_dir} M=${PWD} modules
clean:
	make -C ${root_dir} M=${PWD} clean 
