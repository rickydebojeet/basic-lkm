/*
 * This is a dummy hello world kernel module
 * @author Debojeet Das
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_DESCRIPTION("Hello world kernel module");
MODULE_AUTHOR("Debojeet Das");
MODULE_LICENSE("GPL");

/*
 * @brief The module prints a hello message
 */
static int __init dummy_init(void)
{
    printk("Hello World from a module\n");
    return 0;
}

/*
 * @brief The module exit point prints a bye message
 */
static void __exit dummy_exit(void)
{
    printk("Bye from a module\n");
}

module_init(dummy_init);
module_exit(dummy_exit);
