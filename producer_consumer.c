#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Keep this a power of 2 */
#define MAXQUEUE 32

/* Power of 2 number - 1, gives all F's */
#define MASK (MAXQUEUE - 1)

/* Thread count */
#define PRODUCERCOUNT 1 
#define CONSUMERCOUNT 1

/* Queue structure - Circular Queue at that */
struct queue {
  unsigned int head;
  unsigned int tail;
  unsigned int data[MAXQUEUE];  
};

struct queue ringBuffer;

/* Queue full condition is : head & MASK == tail & MASK && head > tail */ 
void enqueue(unsigned int d) {
  if((ringBuffer.head & MASK) == (ringBuffer.tail & MASK) &&
     (ringBuffer.head > ringBuffer.tail)) {
    printf ("\nQueue full! Dropping data [%u]", d);
    return;
  } else {
    ringBuffer.head++;
    ringBuffer.data[ringBuffer.head & MASK] = d; 
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
void produce() {
  srand(time(NULL));
  /* Right now simple stuff. Just enqueue a number. */
  int r = rand();
  enqueue(r);
  return;
}

/* Function to "consume" aka. dequeue() numbers from the queue */
void consume() {
  /* Right now simple stuff. Just dequeue a number. */
  int a = dequeue();
  printf("\nConsumer just consumed - %u", a); 
  return;  
}

/* The function which the producer thread runs continuously */
void* producer(void *arg) {
  while(1) {
    produce();
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
