#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

// 1. Create the platform data for our two devices
struct pcdev_platform_data pcdev_pdata[2] = {
    [0] = {.size = 512,  .perm = RDWR, .serial_number = "PCDEVABC1111"},
    [1] = {.size = 1024, .perm = RDWR, .serial_number = "PCDEVXYZ2222"}
};

// 2. Define Platform Device 1
struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device",
    .id = 0,
    .dev = {
        .platform_data = &pcdev_pdata[0]
    }
};

// 3. Define Platform Device 2
struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device",
    .id = 1,
    .dev = {
        .platform_data = &pcdev_pdata[1]
    }
};

// 4. Module Init: Register devices with the Kernel
static int __init pcdev_platform_init(void)
{
    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);
    
    pr_info("Pseudo Platform Devices inserted into the kernel.\n");
    return 0;
}

// 5. Module Exit: Unregister devices
static void __exit pcdev_platform_exit(void)
{
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
    
    pr_info("Pseudo Platform Devices removed.\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module which registers pseudo platform devices");
