ARCH = x86
CROSS_COMPILE = 

KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

obj-m += chrdevbase.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) modules clean
