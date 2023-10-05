obj-m += driver.o

root_dir = /home/arun/Technexion/tech/linux

final:
	make -C ${root_dir} M=${PWD} modules
clean:
	make -C ${root_dir} M=${PWD} clean 
