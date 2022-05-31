
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include "ringBuff.h"

int circ_bbuf_push(ringBuff *buff, int data) {
	int next;
	next = buff->head + 1; // next is where head will point to after this write.
	if (next >= buff->maxlen) {
		next = 0;
	}

	if (next == buff->tail) { // if the head + 1 == tail, circular buffer is full
		return -1;
	}
	buff->nums[buff->head] = data;  // Load data and then move
	buff->head = next;             // head to next data offset.
	return 0;  // return success to indicate successful push.
}

int circ_bbuf_pop(ringBuff *buff, int *data) {
	int next;

	if (buff->head == buff->tail) // if the head == tail, we don't have any data
		return -1;

	next = buff->tail + 1;  // next is where tail will point to after this read.
	if (next >= buff->maxlen)
		next = 0;

	*data = buff->nums[buff->tail];  // Read data and then move
	buff->tail = next;              // tail to next offset.
	return 0;  // return success to indicate successful push.
}

