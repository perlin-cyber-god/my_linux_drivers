#include <linux/module.h>

/*Meta information*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("perli");
MODULE_DESCRIPTION("A simple Helloworld kernel module");
MODULE_INFO(board, "Asus Ubuntu laptop");

/*Module initialisation function*/
static int __init helloworld_init(void)
{
    pr_info("Hello, world!\n");
    return 0;
}

/*Module cleanup function*/
static void __exit helloworld_cleanup(void)
{
    pr_info("Goodbye, world!\n");
}

/*Registration*/
module_init(helloworld_init);
module_exit(helloworld_cleanup);