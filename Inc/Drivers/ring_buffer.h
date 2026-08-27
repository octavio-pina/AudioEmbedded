/*
 * ring_buffer.h
 *
 *  Created on: Aug 17, 2026
 *      Author: octav
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>
#define MAXBUFFER 64
#define BUFFER_MASK (MAXBUFFER - 1)

typedef enum
{
	rb_full = 0,
	rb_empty,
	rb_ok
}status_rb_e;

typedef struct
{
	uint8_t buffer[MAXBUFFER];
	volatile uint8_t head;
	volatile uint8_t tail;
}ringBuffer_t;

status_rb_e ringBuffer_init(ringBuffer_t* rb);
status_rb_e ringBuffer_push(ringBuffer_t* rb, uint8_t data);
status_rb_e ringBuffer_pop(ringBuffer_t* rb, uint8_t* data);
#endif /* RING_BUFFER_H_ */
