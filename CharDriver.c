/* 
Note: 
 Kernel space stack is small so DO NOT make too many varibles inside functions 
it can cause the driver to crash, this is not a problem in user space since user 
space stacks are huge comparatively. Use global space instead.
 */

/* Includes */
#include <linux/module.h>
#include <linux/kernel.h>
/* File operations structure, allows to open/close, read/write to the device */
#include <linux/fs.h> 
/* Char driver for character device drivers */
#include <linux/cdev.h>
/* Allows access to semaphores for sychronization, helps privent corruption due to multiple threads accessing the same data */
#include <linux/semaphore.h>
/* Access and map data from user space to kernel space; copy_to_user,copy_from_user */
#include <linux/uaccess.h>

struct fake_device 
{
    char data[100];
    /* Privent data corruption */
    struct semaphore sem;
}virtual_device;

/* myCDEV */
struct cdev *mcdev;
/* holes major number */
int major_number;
/* holds return value */
int ret;
/* holds major number that kernel gives as a structure*/
dev_t dev_num;

/* Max minor number */
#define MX_MINOR 1

/* define the device name */
#define DEVICE_NAME "CharDriver"

/* Device open function */
int chardriver_open(struct inode *inode, struct file *filp)
{
    /* Lock the device using semiphore to privent other modules from using the device at the same time */
    if(down_interruptible(&virtual_device.sem) != 0)
    {
        printk(KERN_ALERT "CharDriver: could not lock device\n");
        return -1;
    }

    printk(KERN_INFO "CharDriver: Device opened\n");
    return 0;
}

/* Device read function */
ssize_t chardriver_read(struct file *filp, char *bufStoreData, size_t bufCount, loff_t *curoffset)
{
    /* Take data from kernel space (device) to user space (process) */
    printk(KERN_INFO "CharDriver: Reading from device\n");
    /* (<destination>, <source>, <sizeToTransfer>) */
    ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
    return ret;
}

/* Device write function */
ssize_t chardriver_write(struct file *filp, const char *bufStoreData, size_t bufCount, loff_t *curoffset)
{
    /* Take data from user space (process) to kernel space (device)*/
    printk(KERN_INFO "CharDriver: Writing to device\n");
    /* (<destination>, <source>, <sizeToTransfer>) */
    ret = copy_from_user(virtual_device.data, bufStoreData, bufCount);
    return ret;
}

/* Device close function */
int chardriver_close(struct inode *inode, struct file *filp)
{
    /* remove lock on device */
    up(&virtual_device.sem);
    printk(KERN_INFO "CharDriver: Device closed\n");
    return 0;
}

/* File opperations structure */
struct file_operations fops = {
    /* Privents unloading of module when operations are running */
    .owner = THIS_MODULE,
    .open = chardriver_open,
    .release = chardriver_close,
    .write = chardriver_write,
    .read = chardriver_read
};


/* Init function */
static int chardriver_init(void)
{
    /* Registering device */
    /* Step 1 */
    /* Get major number and allocate char device region */
    ret = alloc_chrdev_region(&dev_num, 0, MX_MINOR, DEVICE_NAME);
    /* alloc_cdev_region(<device structure>, <startime major number>, <maximum minor number>, <device name>)*/
    if(ret < 0)
    {
        /* exit on error */
        printk(KERN_ALERT "CharDriver: failed to allocate major number\n");
        return ret;
    }
    /* Extract the major number */
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "CharDriver: major number is %d\n", major_number);
    printk(KERN_INFO "CharDriver: use \"mknod /dev/%s c %d 0\" for device file\n", DEVICE_NAME, major_number);

    /* Step 2 */
    /* Allocate char device structure */
    mcdev = cdev_alloc();
    /* Structure for file opperations */
    mcdev->ops = &fops;
    mcdev->owner = THIS_MODULE;
    /* Add cdev to kernel */
    ret = cdev_add(mcdev, dev_num, MX_MINOR);
    if(ret < 0)
    {
        printk(KERN_ALERT "CharDriver: Failed to add cdev to kernel\n");
    }

    /* Init the semaphore with initial value of 1 */
    sema_init(&virtual_device.sem, 1);

    printk(KERN_INFO "CharDriver: Module loaded\n");
    return 0;
}

/* Exit function */
static void chardriver_exit(void)
{
    /* unregister everything in reverse order */
    cdev_del(mcdev);

    unregister_chrdev_region(dev_num, MX_MINOR);
    printk(KERN_ALERT "CharDriver: unloaded module\n");
}

module_init(chardriver_init);
module_exit(chardriver_exit);
