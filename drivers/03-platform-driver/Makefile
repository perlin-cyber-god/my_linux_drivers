obj-m := pcd_device_setup.o pcd_platform_driver.o

# Find the build path for the currently running kernel on your Pi
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
