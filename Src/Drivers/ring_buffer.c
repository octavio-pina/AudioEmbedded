/*
 * ring_buffer.c
 *
 *  Created on: Aug 17, 2026
 *      Author: octav
 */

#include "ring_buffer.h"


status_rb_e ringBuffer_init(ringBuffer_t* rb){
	rb->head = 0;
	rb->tail = 0;

	return rb_ok;
}

status_rb_e ringBuffer_push(ringBuffer_t* rb, uint8_t data){
	if(((rb->head + 1) & BUFFER_MASK) == rb->tail){
		return rb_full;
	}

	rb->buffer[rb->head] = data;
	rb->head = (rb->head + 1) & BUFFER_MASK;
	return rb_ok;
}

status_rb_e ringBuffer_pop(ringBuffer_t* rb, uint8_t* data){
	if(rb->tail == rb->head){
		return rb_empty;
	}

	*data = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) & BUFFER_MASK;
	return rb_ok;
}
