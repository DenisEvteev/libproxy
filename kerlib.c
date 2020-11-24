#include <linux/list.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "debug.h" //xPRINTF
#include "fifo.h"
/**
 * all events within kernel space will be represented via a buffer with at most MESSAGE_SIZE bytes
 * */
#define MESSAGE_SIZE ((4 * PAGE_SIZE))

typedef struct message_from_user {
	size_t size;
	char *buff; //MESSAGE_SIZE will be allocated for this buffer to collect data about an event
}msg_t;

typedef struct message_entry_fifo {
	size_t number_read;
	msg_t  *data;
	struct list_head list;
}message_t;

fifo_t fifo; // GLOBAL FIFO: gather all kernel events (kernel events storage)

size_t number_clients = 0;

void init_storage(void)
{
	fifo_init(&fifo);
}

void register_client(struct file *filp)
{
	if ((filp->f_flags & O_ACCMODE) == O_RDONLY) {
		++number_clients; // to add some number of blocks because two different files can open device special file simultaneously!!!!
	}
}

#define bufk (entry->data->buff)

ssize_t add_msg_queue(const char __user *buff, size_t count, loff_t *offp)
{
	ssize_t retval = -ENOMEM;
	message_t *entry = kmalloc(sizeof(message_t), GFP_KERNEL);
	if (!entry){
		EPRINTF("kmalloc failed");
		goto out;
	}
	entry->number_read = 0;
	size_t mes_size;
	/**we can split the message written to kernel(event size) due to we have a predefined size
	 * of the buffer in kernel
	 * */
	if (count > MESSAGE_SIZE)
		mes_size = MESSAGE_SIZE;
	else mes_size = count;
	bufk = kmalloc(mes_size, GFP_KERNEL);
	if (!bufk) {
		EPRINTF("kmalloc failed for allocating MESSAGE_SIZE");
		goto cleanup1;
	}
	memset(bufk, 0, mes_size);
	if(copy_from_user(bufk, buff, mes_size)) {
		/**
		 * This branch means that some error occurred while
		 * transferring event from user space
		 * */
		 retval = -EFAULT;
		 EPRINTF("copy_from_user failed");
		 goto cleanup2;
	}
	*offp += mes_size;
	entry->data->size = mes_size;
	retval = (ssize_t)mes_size;

	ssize_t r = fifo_push(&(entry->list), &fifo);
	if (r != 0) {
		EPRINTF("interrupted attempt of holding a semaphore");
		retval = r;
		goto cleanup2;
	}
	return retval;

cleanup2: kfree(bufk);
cleanup1: kfree(entry);
out:      return retval;
}

#undef buf