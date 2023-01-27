/*
 * This is a kernel module that prints stack pointer of a task with pid 1
 * @author Debojeet Das
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>

/* Meta Information */
MODULE_DESCRIPTION("Kernel Stack pointer printer");
MODULE_AUTHOR("Debojeet Das");
MODULE_LICENSE("GPL");

static struct task_struct *task;

/*
 * @brief The module entry point prints the stack pointer of a process with pid 1
 */
static int __init print_ksp(void)
{
    printk("stack pointer lkm loaded\n");
    task = get_pid_task(find_vpid(1), PIDTYPE_PID);
    printk("PID: 1 has kernel stack base at %lx and stack pointer at %lx\n", (unsigned long)task->stack, (unsigned long)task->thread.sp);
    return 0;
}

/*
 * @brief The module exit point prints a kernel module unloaded message
 */
static void __exit print_exit(void)
{
    printk("stack pointer lkm unloaded\n");
}

module_init(print_ksp);
module_exit(print_exit);
