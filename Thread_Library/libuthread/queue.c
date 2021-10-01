#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

// Each node stores data, pointer to next node, pointer to prev node.
struct Node {
	void *data;
	struct Node *next;
	struct Node *prev;
};

// Doubly-Linked List implementation.
struct queue {
	struct Node *head;
	struct Node *tail;
	unsigned int cur_size;
};

queue_t queue_create(void)
{
	struct queue *newQueue = malloc(sizeof(*newQueue));

	if (!newQueue) 
		return NULL;

	newQueue->cur_size = 0;
	newQueue->head = NULL;
	newQueue->tail = NULL;
	return newQueue;
}

int queue_destroy(queue_t queue)
{
	// "Return: -1 if @queue is NULL or if @queue is not empty."
	if (!queue || queue->cur_size != 0)
		return -1;

	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{	
	// "Return: -1 if @queue or @data are NULL."
	if (!queue || !data)
		return -1;

	// Create new node to be pushed to end of queue. Initialize variables.
	struct Node *node = malloc(sizeof(*node));

	if (!node)
		return -1;

	node->data = data;
	node->prev = NULL;

	// Set next pointer to NULL or tail. No tail means queue is empty.
	if (!queue->tail)
		node->next = NULL;
	else 
		node->next = queue->tail;

	// If queue already has tail, must set this tail node's prev pointer
	// to new node before updating tail to point to new node.
	if (queue->tail)
		queue->tail->prev = node;
	queue->tail = node;
	
	// If no head present, this is the first node. Becomes a head node.
	if (!queue->head)
		queue->head = node;

	queue->cur_size++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	// "Return: -1 if @queue or @data are NULL."
	if (!queue || !data)
		return -1;

	// "Return: -1 if the queue is empty."
	if (queue->cur_size == 0)
		return -1;

	*data = queue->head->data;
	struct Node *temp = queue->head;
	if (queue->head->prev) {
		queue->head = queue->head->prev;
		queue->head->next = NULL;
		temp->prev = NULL;
	} else {
		// Only node in queue left, set head and tail to NULL.
		queue->head = NULL;
		queue->tail = NULL;
	}
	free(temp);
	queue->cur_size--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	// "Return: -1 if @queue or @data are NULL."
	if (!queue || !data)
		return -1;

	// Iterate down front of queue to delete appropriate node.
	struct Node *temp = queue->head;
	while(temp) {
		if (temp->data == data) {
			// Found data at node, must reroute pointers of neighbors before freeing.
			if (temp->next && temp->prev) {
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			} else if (temp->next && !temp->prev) {
				queue->tail = temp->next;
				queue->tail->prev = NULL;
			} else if (!temp->next && temp->prev) {
				queue->head = temp->prev;
				queue->head->next = NULL;
			}
			temp->next = NULL;
			temp->prev = NULL;
			temp->data = NULL;
			free(temp);
			queue->cur_size--;
			return 0;
		} else 
			temp = temp->prev;
	}
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	// "Return: -1 if @queue or @func are NULL."
	if (!queue || !func)
		return -1;

	// Iterate down the front of the queue.
	struct Node *temp = queue->head;
	while (temp) {
		if (func(queue, temp->data, arg) == 1) {
			if (data)
				*data = temp->data;
			break;
		}
		temp = temp->prev;
	}
	temp = NULL;
	return 0;
}

int queue_length(queue_t queue)
{
	// "Return: -1 if @queue is NULL. Length of @queue otherwise."
	if (!queue)
		return -1;
		
	return queue->cur_size;
}