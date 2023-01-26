/*
 * This is a kernel module that lists all processes either in the running or the runnable state
 * @author Debojeet Das
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

/* Meta Information */
MODULE_DESCRIPTION("Running & Runnable processes kernel module");
MODULE_AUTHOR("Debojeet Das");
MODULE_LICENSE("GPL");

static struct task_struct *task;

/*
 * @brief The module prints all the running processes
 */
static int __init print_running(void)
{
    printk("print running process kernel module loaded\n");
    printk("PID\tPROC\n");
    printk("---------------\n");
    for_each_process(task)
    {
        if (task_is_running(task))
        {
            printk("%d\t%s\n", task->pid, task->comm);
        }
    }
    return 0;
}

/*
 * @brief The module prints a exit message when a kernel module is unloaded
 */
static void __exit print_exit(void)
{
    printk("print running process kernel module unloaded\n");
}

module_init(print_running);
module_exit(print_exit);
