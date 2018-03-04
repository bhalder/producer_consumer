#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Keep this a power of 2 */
#define MAXQUEUE 32

/* Power of 2 number - 1, gives all F's */
#define MASK (MAXQUEUE - 1)

/* Thread count */
#define PRODUCERCOUNT 10 
#define CONSUMERCOUNT 1

/* Queue structure - Circular Queue at that */
struct queue {
  unsigned int head;
  unsigned int tail;
  unsigned int data[MAXQUEUE];  
};

/* Ring buffer and the concurrency parameters. */
struct queue ringBuffer;
pthread_mutex_t ring_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ring_cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t ring_cond_empty = PTHREAD_COND_INITIALIZER;

/* Queue full condition is : head & MASK == tail & MASK && head > tail */ 
void enqueue(unsigned int d) {
  if((ringBuffer.head & MASK) == (ringBuffer.tail & MASK) &&
     (ringBuffer.head > ringBuffer.tail)) {

    printf ("\nQueue full! Dropping data [%u]", d);
    return;
  } else {
    ringBuffer.head++;
    ringBuffer.data[ringBuffer.head & MASK] = d; 
    printf ("\nAdded into the queue [%u]", d);
  }
}

/* Queue empty condition is : head == tail */
unsigned int dequeue() {
  if(ringBuffer.head == ringBuffer.tail) {
    printf("\nQueue is empty. No data to be shared.");
    return 0;
  } else {
    unsigned int d = ringBuffer.data[ringBuffer.tail & MASK];
    ringBuffer.tail++;
    return d;
  }
}

/* Function to "produce" aka. enqueue() random numbers */

void produce(r) {
  pthread_mutex_lock(&ring_mtx);

  while((ringBuffer.head & MASK) == (ringBuffer.tail & MASK) &&
        (ringBuffer.head > ringBuffer.tail)) {
    pthread_cond_wait(&ring_cond_full, &ring_mtx);
  }
 
  enqueue(r);
  pthread_cond_signal(&ring_cond_empty);
  pthread_mutex_unlock(&ring_mtx);
  sleep(1);
  return;
}

/* Function to "consume" aka. dequeue() numbers from the queue */
void consume() {
  int a = -1;
  pthread_mutex_lock(&ring_mtx);

  while(ringBuffer.head == ringBuffer.tail) { 
    pthread_cond_wait(&ring_cond_empty, &ring_mtx);
  }

  a = dequeue();
  pthread_cond_signal(&ring_cond_full);
  pthread_mutex_unlock(&ring_mtx);
  printf("\nConsumer just consumed - %u", a); 
  sleep(1);

  return;  
}

/* The function which the producer thread runs continuously */
void* producer(void *arg) {
  int r = 0;
  while(1) {
    produce(r++);
  }

  printf("\nReturning from producer.");
}

void* consumer(void *arg) {
  while(1) {
    consume();
  }
  printf("\nReturning from consumer.");
}

int main() {
  int i = 0;
  ringBuffer.tail = 0;
  ringBuffer.head = 0;

  pthread_t producer_thread[PRODUCERCOUNT];
  pthread_t consumer_thread[CONSUMERCOUNT];

  /* Start the Consumer threads first. */
  for(i = 0; i < CONSUMERCOUNT; i++) {
    pthread_create(&consumer_thread[i], NULL, &consumer, NULL);
  }

  /* Start the Producer threads. */
  for(i = 0; i < PRODUCERCOUNT; i++) {
    pthread_create(&producer_thread[i], NULL, &producer, NULL);
  }

  for(i = 0; i < CONSUMERCOUNT; i++) {
    pthread_join(consumer_thread[i], NULL);
  }

  for(i = 0; i < PRODUCERCOUNT; i++) {
    pthread_join(producer_thread[i], NULL);
  }
 
  printf("\nThis is the end of the main function.");
  return 0;  
}
