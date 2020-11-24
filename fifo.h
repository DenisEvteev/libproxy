//
// Created by user on 17/11/2020.
//

#ifndef _FIFO_H_
#define _FIFO_H_

#include <linux/list.h>
#include <linux/semaphore.h>

typedef struct events_fifo {
	struct list_head head;
	struct semaphore lock;
}fifo_t;


void fifo_init(fifo_t *fifo);
ssize_t fifo_push(struct list_head *new, fifo_t *head);

#endif //_FIFO_H_
