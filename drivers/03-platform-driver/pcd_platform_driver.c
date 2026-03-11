#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

#define MAX_DEVICES 10

/*
 * DEVICE PRIVATE DATA
 * Think of this as the "File Folder" for a single piece of hardware.
 * If you plug in 3 devices, the probe() function will allocate 3 of these.
 */
struct pcdev_private_data {
    struct pcdev_platform_data pdata; // From platform.h (Size, Perms, Serial)
    char *buffer;                     // The actual memory buffer (fake hardware)
    dev_t dev_num;                    // The specific Minor Number for this device
    struct cdev cdev;                 // The Character Device object
};

/*
 * DRIVER PRIVATE DATA
 * Think of this as the "Filing Cabinet". There is only ONE of these.
 * It holds the global state for the entire driver.
 */
struct pcdrv_private_data {
    int total_devices;                // How many devices have we probed?
    dev_t device_num_base;            // The starting Major/Minor number
    struct class *class_pcd;          // The /sys/class/pcd_class folder
    struct device *device_pcd;        // (Used later for device creation)
};

// The single, global instance of the filing cabinet
struct pcdrv_private_data pcdrv_data;

/* ====================================================
 * 1. THE CHARACTER DEVICE HALF (User Space Interface)
 * ==================================================== */

int pcd_open(struct inode *inode, struct file *filp) {
    pr_info("Device opened\n");
    return 0;
}

ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Read requested\n");
    return 0;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Write requested\n");
    return -ENOMEM; 
}

int pcd_release(struct inode *inode, struct file *filp) {
    pr_info("Device closed\n");
    return 0;
}

struct file_operations pcd_fops = {
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .release = pcd_release,
    .owner = THIS_MODULE
};

/* ====================================================
 * 2. THE PLATFORM DRIVER HALF (Hardware Matchmaker)
 * ==================================================== */

static int pcd_platform_driver_probe(struct platform_device *pdev)
{
    pr_info("A device is detected! [Name: %s, ID: %d]\n", pdev->name, pdev->id);
    return 0;
}

static int pcd_platform_driver_remove(struct platform_device *pdev)
{
    pr_info("A device is removed! [Name: %s, ID: %d]\n", pdev->name, pdev->id);
    return 0;
}

struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .driver = {
        .name = "pseudo-char-device"
    }
};

/* ====================================================
 * 3. MODULE INIT / EXIT
 * ==================================================== */

static int __init pcd_driver_init(void)
{
    int ret;

    // 1. Dynamically allocate a device number range for MAX_DEVICES
    ret = alloc_chrdev_region(&pcdrv_data.device_num_base, 0, MAX_DEVICES, "pcdev");
    if (ret < 0) {
        pr_err("Alloc chrdev failed\n");
        return ret;
    }

    // 2. Create device class under /sys/class
    pcdrv_data.class_pcd = class_create(THIS_MODULE, "pcd_class");
    if (IS_ERR(pcdrv_data.class_pcd)) {
        pr_err("Class creation failed\n");
        ret = PTR_ERR(pcdrv_data.class_pcd);
        unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);
        return ret;
    }

    // 3. Register the platform driver
    platform_driver_register(&pcd_platform_driver);
    
    pr_info("pcd platform driver loaded\n");
    return 0;
}

static void __exit pcd_driver_exit(void)
{
    // 1. Unregister the platform driver
    platform_driver_unregister(&pcd_platform_driver);

    // 2. Destroy the class
    class_destroy(pcdrv_data.class_pcd);

    // 3. Unregister device number region
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);

    pr_info("pcd platform driver unloaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Platform + Character Driver with Private Data Structures");
