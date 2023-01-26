/*
 * This is a kernel module that prints the size of virtual address space and physical address space of a process
 * Moreover the kernel module proves that the linux kernel uses lazzy memory allocation
 * @author Debojeet Das
 *
 * @param pid: pid of the process to check
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched/mm.h>
#include <linux/mm.h>

/* Meta Information */
MODULE_DESCRIPTION("VAS & PAS Size LKM");
MODULE_AUTHOR("Debojeet Das");
MODULE_LICENSE("GPL");

static struct task_struct *task;
static struct mm_struct *mm;
static int pid __initdata = 0;
static unsigned long rss __initdata = 0;
static unsigned long total_memory __initdata = 0;

module_param(pid, int, 0);

/*
 * @brief The module entry point prints the size of virtual and physical address space
 */
static int __init print_running(void)
{
    printk("Size of VAS & PAS LKM\tPid: %d\n", pid);
    task = get_pid_task(find_vpid(pid), PIDTYPE_PID);
    if (task == NULL)
    {
        printk("Task not found\n");
        return 0;
    }
    printk("Task: %s\n", task->comm);
    mm = get_task_mm(task);
    if (mm != NULL)
    {
        rss = (get_mm_rss(mm) << PAGE_SHIFT) / 1024;
        total_memory = (mm->total_vm << PAGE_SHIFT) / 1024;
        printk("rss side: %lu KiB\tvm size: %lu KiB\n", rss, total_memory);
    }
    else
    {
        printk("mm is NULL\n");
        return 0;
    }

    return 0;
}

/*
 * @brief The module exit point prints a kernel module unloaded message
 */
static void __exit print_exit(void)
{
    printk("lkm unloaded\n");
}

module_init(print_running);
module_exit(print_exit);
