
/**
 * @file queue.h
 * @brief Queue.
 *
 * Queue.
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pyas/list.h> /* list_t */

typedef struct link_t *queue_t;

/**
 * create a new queue
 */
queue_t queue_new( void );

/**
 * @return if the queue is empty
 */
int     queue_empty( queue_t q );

/**
 * adds a new element at the end of the queue
 */
queue_t enqueue( queue_t q, void* object );

/**
 * transformes a queue to a list_t
 * @param *q is the adress of the queue
 */
list_t  queue_to_list( queue_t *q );

#ifdef __cplusplus
}
#endif

#endif /* _QUEUE_H_ */
