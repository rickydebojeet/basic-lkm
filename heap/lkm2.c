/*
 * This is a kernel module that prints the process that has the largest heap memory
 * @author Debojeet Das
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>

/* Meta Information */
MODULE_DESCRIPTION("Largest Heap LKM");
MODULE_AUTHOR("Debojeet Das");
MODULE_LICENSE("GPL");

static struct task_struct *task;
static struct mm_struct *mm;
static unsigned long size;
static unsigned long max_size __initdata = 0;
static int pid;

/*
 * @brief The module init prints the pid of the process that is using largest heap memory
 */
static int __init print_largest(void)
{
    printk("largest heap memory lkm loaded\n");
    for_each_process(task)
    {
        mm = get_task_mm(task);
        if (mm != NULL)
        {
            size = mm->brk - mm->start_brk;
            if (task->pid != 0 && max_size < size)
            {
                max_size = size;
                pid = task->pid;
            }
        }
    }
    printk("PID: %d has max heap usage with: %lu\n", pid, max_size);
    return 0;
}

/*
 * @brief The module exit prints the exit message when the lkm is unloaded
 */
static void __exit print_exit(void)
{
    printk("largest heap memory lkm unloaded\n");
}

module_init(print_largest);
module_exit(print_exit);
