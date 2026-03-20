#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

// The dummy release function required by the kernel
static void pcdev_release(struct device *dev) {
    pr_info("Device released!\n");
}

// 1. Create the platform data for FOUR devices
struct pcdev_platform_data pcdev_pdata[4] = {
    [0] = {.size = 512,  .perm = RDWR,   .serial_number = "PCDEVABC1111"},
    [1] = {.size = 1024, .perm = RDWR,   .serial_number = "PCDEVXYZ2222"},
    [2] = {.size = 128,  .perm = RDONLY, .serial_number = "PCDEVXYZ3333"},
    [3] = {.size = 32,   .perm = WRONLY, .serial_number = "PCDEVXYZ4444"}
};

// 2. Define the 4 Platform Devices (Notice the corrected IDs!)
struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device", .id = 0, .dev = { .platform_data = &pcdev_pdata[0], .release = pcdev_release }
};
struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device", .id = 1, .dev = { .platform_data = &pcdev_pdata[1], .release = pcdev_release }
};
struct platform_device platform_pcdev_3 = {
    .name = "pseudo-char-device", .id = 2, .dev = { .platform_data = &pcdev_pdata[2], .release = pcdev_release }
};
struct platform_device platform_pcdev_4 = {
    .name = "pseudo-char-device", .id = 3, .dev = { .platform_data = &pcdev_pdata[3], .release = pcdev_release }
};

// 3. Create an array of POINTERS to those devices
struct platform_device *platform_pcdevs[] = {
    &platform_pcdev_1,
    &platform_pcdev_2,
    &platform_pcdev_3,
    &platform_pcdev_4
};

// 4. Module Init
static int __init pcdev_platform_init(void)
{
    // Register the entire array in one shot!
    platform_add_devices(platform_pcdevs, ARRAY_SIZE(platform_pcdevs));
    pr_info("4 Pseudo Platform Devices inserted into the kernel.\n");
    return 0;
}

// 5. Module Exit
static void __exit pcdev_platform_exit(void)
{
    // Unregister them in a clean loop
    int i;
    for (i = 0; i < ARRAY_SIZE(platform_pcdevs); i++) {
        platform_device_unregister(platform_pcdevs[i]);
    }
    pr_info("4 Pseudo Platform Devices removed.\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module which registers pseudo platform devices");
