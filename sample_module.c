/**
 * this module was written for testing the kerlib.c library.
 * It's character device driver. So we have corresponding operations implemented.
 * */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>

#include "debug.h"
//NAME OF A DEVICES WILL BE testmodule it will appear in /proc/devices representing our device according to its major number
static const char *name = "testmodule";

#ifdef MAJOR_DEFAULT
static int my_major = MAJOR_DEFAULT;
#else
static int my_major = 0;
#endif

static dev_t first;
static struct cdev *my_cdev;

ssize_t dread(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	return 0;
}

ssize_t dwrite(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	return 0;
}

int drelease(struct inode *inode, struct file *filp) {
	return 0;
}

int dopen(struct inode *inode, struct file *filp) {
	return 0;
}

static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.read    = dread,
	.write   = dwrite,
	.open    = dopen,
	.release = drelease
};

static int __init initialize_service(void)
{
	if(my_major) {
		first = MKDEV(my_major, 0);
		if(register_chrdev_region(first, 1, name) != 0) {
			EPRINTF("register_chrdev_region");
			return -1;
		}
	} else {
		if(alloc_chrdev_region(&first, 0, 1, name) != 0) {
			EPRINTF("alloc_chrdev_region");
			return -1;
		}
	}
	my_cdev = cdev_alloc();
	if(my_cdev == NULL)
		goto fail;
	cdev_init(my_cdev, &fops);
	my_cdev->owner = THIS_MODULE;

	if (cdev_add(my_cdev, first, 1) != 0)
		goto fail;

	IPRINTF("module load success");
	return 0;

fail:
	unregister_chrdev_region(first, 1);
	NPRINTF("module load wasn't completed");
	return -1;
}

static void __exit cleanup_service(void)
{
	unregister_chrdev_region(first, 1);
	cdev_del(my_cdev);
	IPRINTF("module unload success");
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Acro Team");
MODULE_DESCRIPTION("module for testing kernel library");
module_init(initialize_service);
module_exit(cleanup_service);