obj-m := pcd_platform_driver.o pcd_device_setup.o
ARCH=arm64
CROSS_COMPILE=aarch64-linux-gnu-
# For RPi 5, we'll need the correct kernel source path. Defaulting to local modules for now.
KERN_DIR = /lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help
host:
	make -C $(KERN_DIR) M=$(PWD) modules
