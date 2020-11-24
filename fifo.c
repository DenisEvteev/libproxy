//
// Created by user on 17/11/2020.
//
#include "fifo.h"

void fifo_init(fifo_t *fifo)
{
	INIT_LIST_HEAD(&(fifo->head));
	sema_init(&(fifo->lock), 1);
}

ssize_t fifo_push(struct list_head *new, fifo_t *fifo)
{
	if(down_interruptible(&(fifo->lock)))
		return -ERESTART;
	list_add_tail(new, &(fifo->head));
	up(&(fifo->lock));
	return 0;
}