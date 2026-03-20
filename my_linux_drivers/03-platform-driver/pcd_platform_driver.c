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
 */
struct pcdev_private_data {
    struct pcdev_platform_data pdata;
    char *buffer;
    dev_t dev_num;
    struct cdev cdev;
};

/*
 * DRIVER PRIVATE DATA
 */
struct pcdrv_private_data {
    int total_devices;
    dev_t device_num_base;
    struct class *class_pcd;
    struct device *device_pcd;
};

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

// Probe: Gets called the millisecond the Kernel finds a matching device name
static int pcd_platform_driver_probe(struct platform_device *pdev)
{
    int ret;
    struct pcdev_private_data *dev_data;
    struct pcdev_platform_data *pdata;

    pr_info("A device is detected! [Name: %s, ID: %d]\n", pdev->name, pdev->id);

    pdata = (struct pcdev_platform_data*)dev_get_platdata(&pdev->dev);
    if (!pdata) {
        pr_err("No platform data available\n");
        return -EINVAL;
    }

    // MODERN ALLOCATION 1: Tied to the device lifecycle
    // The kernel will automatically free this when the device is removed!
    dev_data = devm_kzalloc(&pdev->dev, sizeof(*dev_data), GFP_KERNEL);
    if (!dev_data) {
        pr_err("Cannot allocate memory for dev_data\n");
        return -ENOMEM;
    }

    dev_data->pdata.size = pdata->size;
    dev_data->pdata.perm = pdata->perm;
    dev_data->pdata.serial_number = pdata->serial_number;

    // MODERN ALLOCATION 2: Tied to the device lifecycle
    dev_data->buffer = devm_kzalloc(&pdev->dev, dev_data->pdata.size, GFP_KERNEL);
    if (!dev_data->buffer) {
        pr_err("Cannot allocate memory for buffer\n");
        return -ENOMEM; // No goto needed! The kernel frees dev_data automatically!
    }

    dev_data->dev_num = pcdrv_data.device_num_base + pdev->id;

    cdev_init(&dev_data->cdev, &pcd_fops);
    dev_data->cdev.owner = THIS_MODULE;
    
    ret = cdev_add(&dev_data->cdev, dev_data->dev_num, 1);
    if (ret < 0) {
        pr_err("cdev_add failed\n");
        return ret; // Again, no goto needed for memory!
    }

    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, NULL, dev_data->dev_num, NULL, "pcdev-%d", pdev->id);
    if (IS_ERR(pcdrv_data.device_pcd)) {
        pr_err("Device create failed\n");
        ret = PTR_ERR(pcdrv_data.device_pcd);
        cdev_del(&dev_data->cdev); // We still have to manually delete the cdev!
        return ret;
    }

    dev_set_drvdata(&pdev->dev, dev_data);
    pcdrv_data.total_devices++;

    pr_info("Probe was successful!\n\n");
    return 0;
}

// Remove: Gets called when the device is unplugged or module is removed
static int pcd_platform_driver_remove(struct platform_device *pdev)
{
    struct pcdev_private_data *dev_data;

    pr_info("A device is removed! [Name: %s, ID: %d]\n", pdev->name, pdev->id);

    dev_data = dev_get_drvdata(&pdev->dev);
    if (!dev_data) return -EINVAL;

    // 1. Destroy the device file
    device_destroy(pcdrv_data.class_pcd, dev_data->dev_num);

    // 2. Remove the Character Device
    cdev_del(&dev_data->cdev);

    // NO kfree() NEEDED! 
    // Because we used devm_kzalloc, the kernel automatically frees the memory 
    // as soon as the remove function returns. It's like a hotel staff cleaning 
    // the room the moment you check out.

    pcdrv_data.total_devices--;
    pr_info("Device completely removed. Total devices left: %d\n\n", pcdrv_data.total_devices);

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

    ret = alloc_chrdev_region(&pcdrv_data.device_num_base, 0, MAX_DEVICES, "pcdev");
    if (ret < 0) {
        pr_err("Alloc chrdev failed\n");
        return ret;
    }

    pcdrv_data.class_pcd = class_create(THIS_MODULE, "pcd_class");
    if (IS_ERR(pcdrv_data.class_pcd)) {
        pr_err("Class creation failed\n");
        ret = PTR_ERR(pcdrv_data.class_pcd);
        unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);
        return ret;
    }

    platform_driver_register(&pcd_platform_driver);
    
    pr_info("pcd platform driver loaded\n");
    return 0;
}

static void __exit pcd_driver_exit(void)
{
    platform_driver_unregister(&pcd_platform_driver);
    class_destroy(pcdrv_data.class_pcd);
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);
    pr_info("pcd platform driver unloaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Platform + Character Driver with Modern devm_kzalloc Management");
