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

- [Hello Program](./hello/mymodule.c): Prints hello world when installed and bye when removed.
    Outputs-
    ```console 
    [197612.209917] Hello World from a module
    [197617.471480] Bye from a module
    ```