#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

// 1. The Probe Function: Called automatically when the Kernel finds a name match!
static int pcd_probe(struct platform_device *pdev)
{
    struct pcdev_platform_data *pdata;

    pr_info("A device is detected! [Name: %s, ID: %d]\n", pdev->name, pdev->id);

    // Extract the platform data pointer from the device structure
    pdata = (struct pcdev_platform_data*) dev_get_platdata(&pdev->dev);
    
    if (!pdata) {
        pr_info("No platform data available\n");
        return -EINVAL;
    }

    // Print out the hardware specs to prove we got them
    pr_info("Device Serial Number: %s\n", pdata->serial_number);
    pr_info("Device Size: %d bytes\n", pdata->size);
    pr_info("Device Permission: %d\n", pdata->perm);
    pr_info("Probe successful!\n\n");

    return 0;
}

// 2. The Remove Function: Called when the device or driver is removed
static int pcd_remove(struct platform_device *pdev)
{
    pr_info("A device is removed! [Name: %s, ID: %d]\n", pdev->name, pdev->id);
    return 0;
}

// 3. The Platform Driver Structure
struct platform_driver pcd_platform_driver = {
    .probe = pcd_probe,
    .remove = pcd_remove,
    .driver = {
        // THIS IS THE CRITICAL MATCHING STRING
        .name = "pseudo-char-device"
    }
};

// 4. Module Init: Register the driver with the Platform Bus
static int __init pcd_driver_init(void)
{
    platform_driver_register(&pcd_platform_driver);
    pr_info("Platform Driver loaded.\n");
    return 0;
}

// 5. Module Exit: Unregister the driver
static void __exit pcd_driver_exit(void)
{
    platform_driver_unregister(&pcd_platform_driver);
    pr_info("Platform Driver unloaded.\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A pseudo character platform driver");
