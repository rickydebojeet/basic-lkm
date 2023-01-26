/*
 * This is a kernel module that check if a virtual address is present in processes' address space or not. If
 * present it prints out the corresponding phyical address
 * @author Debojeet Das
 *
 * @param pid - pid of the process to check
 * @param address - the address to check
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched/mm.h>
#include <linux/mm.h>
#include <linux/io.h>

#define PAGE_OFFSET_MASK 0x0000000000000fff

/* Meta Information */
MODULE_DESCRIPTION("Virtual to Physcial Address Mapper LKM");
MODULE_AUTHOR("Debojeet Das");
MODULE_LICENSE("GPL");

static struct task_struct *task;
static struct mm_struct *mm;
static struct page *pg;
pgd_t *pgd;
p4d_t *p4d;
pud_t *pud;
pmd_t *pmd;
pte_t *pte;
static int pid __initdata = 0;
static int found __initdata = 0;
unsigned long address __initdata = 0x0000000000000000;
unsigned long physical __initdata = 0x0000000000000000;
unsigned long pfn __initdata = 0x0000000000000000;

module_param(pid, int, 0);
module_param(address, ulong, 0);

/*
 * @brief The module entry point check mapping of address in a process
 */
static int __init print_mapping(void)
{
    printk("LKM loaded\tPid: %d\tAddress: %lx\n", pid, address);
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
        pgd = pgd_offset(mm, address);
        if (!pgd_none(*pgd) && !pgd_bad(*pgd))
        {
            printk("Page Global Directory: %lx\n", (unsigned long)pgd);
            p4d = p4d_offset(pgd, address);
            if (p4d_none(*p4d))
            {
                printk("Page 4th Directory is empty.\n");
            }
            else if (!p4d_bad(*p4d))
            {
                printk("Address of Page 4th Directory: %lx\n", (unsigned long)p4d);
                pud = pud_offset(p4d, address);
                if (pud_none(*pud))
                {
                    printk("Page Upper Directory is empty.\n");
                }
                else if (!pud_bad(*pud))
                {
                    printk("Address of Page Upper Directory: %lx\n", (unsigned long)pud);
                    pmd = pmd_offset(pud, address);
                    if (pmd_none(*pmd))
                    {
                        printk("Page Middle Directory is empty.\n");
                    }
                    else if (!pmd_bad(*pmd))
                    {
                        printk("Address of Page Middle Directory: %lx\n", (unsigned long)pmd);
                        pte = pte_offset_map(pmd, address);
                        if (pte_present(*pte))
                        {
                            printk("Address of Page Table Entry: %lx\n", (unsigned long)pte);
                            printk("The address %lx is valid\n", address);
                            found = 1;
                            pg = pte_page(*pte);
                            pfn = page_to_pfn(pg);
                            physical = (pte_val(*pte) & PHYSICAL_PAGE_MASK) + (address & PAGE_OFFSET_MASK);
                            printk("Page Frame Number: %lx\n", pfn);
                            printk("Physical Address: %lx\n", physical);
                        }
                        pte_unmap(pte);
                    }
                }
            }
        }
    }

    if (found == 0)
    {
        printk("The address %lx is not mapped\n", address);
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

module_init(print_mapping);
module_exit(print_exit);
