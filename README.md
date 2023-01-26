# Basic Linux Kernel Module

A Linux kernel module is a piece of code (kernel functionality) that can be loaded and unloaded into the kernel on demand. Kernel modules extend the functionality of the kernel without the need to reboot the system and are typically used for on-demand loading of drives to handle plug-and-play devices, add custom system call like binary functionality to the kernel etc.

## Usage

To compile a LKM it requires linux kernel headers as well as linux binaries. To install linux kernel header run the following command:

```console
sudo apt install linux-headers-$(uname -r) build-essential
```

Usage:

```console
# Compilation
make

# Installation
sudo insmod <module-name>.ko

# Uninstallation
sudo rmmod <module-name>

# To check kernel logs
sudo dmesg
```

## Kernel Modules

0. [Hello Linux Kernel Module (LKM)](./hello/mymodule.c): Prints hello world when installed and bye when removed.
    Outputs-
    ```console 
    [197612.209917] Hello World from a module
    [197617.471480] Bye from a module
    ```

1. [List Running/Runnable Process LKM](./running/lkm1.c): Lists all processes either in running or runnable state. Outputs-
    ```console
    [199171.196640] print running process kernel module loaded
    [199171.196642] PID     PROC
    [199171.196642] ---------------
    [199171.196706] 349110  insmod
    [199174.954766] print running process kernel module unloaded
    ```
    The output might change depending on number of processes that were in running/runnable state when the insmod was running. To check if the module is correctly working. We can start a CPU intensive process, and check if the process name is coming in `dmesg` or not.

2. [Largest Heap meory usage LKM](./heap/lkm2.c): Prints the process that is using the largest heap memory. Outputs-
    ```console
    [200316.613901] largest heap memory lkm loaded
    [200316.613966] PID: 2221 has max heap usage with: 153395200
    [200322.950288] largest heap memory lkm unloaded
    ```
    The largest value of the heap memory usage was found out by traversing through all processes and comparing their heap memory usage. Heap memory usage was found out by `mm->brk - mm->start_brk` where `mm` is the `mm_struct` of the process.

3. [Kernel Stack Pointer LKM](./ksp/lkm3.c): Prints the kernel stack pointer of the task with pid 1. Outputs-
    ```console
    [202413.887261] stack pointer lkm loaded
    [202413.887266] PID: 1 has kernel stack at ffffb5044006c000
    [202418.149797] stack pointer lkm unloaded
    ```
    The lkm prints `task->stack` where `task` is the `task_struct` of the process with pid 1. As we can see in the output the address starts with `0xffffb` which resides in the kernel address space and represent the kernel stack of the process.

4. [Virtual Address Checker LKM](./mapper/lkm4.c): Checks if an address is mapped to process. If mapped prints out the physical address coressponding to it.
    ```console
    [203901.641433] LKM loaded      Pid: 360249     Address: 7fff5f610000
    [203901.641438] Task: htop
    [203901.641439] Page Global Directory: ffff8df79503e7f8
    [203901.641440] Address of Page 4th Directory: ffff8df79503e7f8
    [203901.641440] Address of Page Upper Directory: ffff8df79a2e7fe8
    [203901.641441] Address of Page Middle Directory: ffff8df93f8b17d8
    [203901.641442] Address of Page Table Entry: ffff8df7cde33080
    [203901.641442] The address 7fff5f610000 is valid
    [203901.641443] Page Frame Number: 12c603
    [203901.641443] Physical Address: 12c603000
    [203904.882719] lkm unloaded
    ```
    The module walks through the page tables to find the physical address of the virtual address if found prints it out otherwise prints that the address is not mapped.

5. [Size of VAS & PAS](./size/lkm5.c): Prints out the size of virtual address space and physical address space in Kilo Bytes (KB). Output-
    ```console
    [ 4084.542246] Size of VAS & PAS LKM    Pid: 48677
    [ 4084.542250] Task: htop
    [ 4084.542252] rss side: 5724 KiB       vm size: 11908 KiB
    [ 4087.749883] lkm unloaded
    ```
    The Virtual Size is larger than resident set size, the main reason for this is that linux uses lazy loading and maps memory only when there is a demand for it. If for a process we are using `malloc` to allocate memory, we will notice that vm size will increase but the rss size will not increase. No untill the allocated memory is accessed, once it is accessed it will be mapped to physical address and the rss sise will increase.