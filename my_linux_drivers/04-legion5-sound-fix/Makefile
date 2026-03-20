obj-m := sound_fix.o
HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

clean:
	make -C $(HOST_KERN_DIR) M=$(PWD) clean
