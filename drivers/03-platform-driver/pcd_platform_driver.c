#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

/* ====================================================
 * 1. THE CHARACTER DEVICE HALF (User Space Interface)
 * ==================================================== */

// Dummy open
int pcd_open(struct inode *inode, struct file *filp) {
    pr_info("Device opened\n");
    return 0;
}

// Dummy read
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Read requested\n");
    return 0;
}

// Dummy write (Returns ENOMEM like the instructor asked)
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Write requested\n");
    return -ENOMEM; 
}

// Dummy release
int pcd_release(struct inode *inode, struct file *filp) {
    pr_info("Device closed\n");
    return 0;
}

// The File Operations Map
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
    pr_info("A device is detected! [Name: %s, ID: %d]\n", pdev->name, pdev->id);
    // (In the next lecture, the instructor will dynamically create the /dev/ files here!)
    return 0;
}

// Remove: Gets called when the device is unplugged or module is removed
static int pcd_platform_driver_remove(struct platform_device *pdev)
{
    pr_info("A device is removed! [Name: %s, ID: %d]\n", pdev->name, pdev->id);
    return 0;
}

// The Platform Driver Structure (The literal glue)
struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .driver = {
        .name = "pseudo-char-device" // MUST MATCH THE SETUP MODULE EXACTLY
    }
};

/* ====================================================
 * 3. MODULE INIT / EXIT
 * ==================================================== */

static int __init pcd_driver_init(void)
{
    platform_driver_register(&pcd_platform_driver);
    pr_info("Platform Driver Loaded\n");
    return 0;
}

static void __exit pcd_driver_exit(void)
{
    platform_driver_unregister(&pcd_platform_driver);
    pr_info("Platform Driver Unloaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Platform + Character Driver Merger");
