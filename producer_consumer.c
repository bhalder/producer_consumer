#include <stdio.h>
#include <pthread.h>

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
    printf ("Queue full! Dropping data [%u]", d);
    return;
  } else {
    ringBuffer.head++;
    ringBuffer.data[ringBuffer.head & MASK] = d; 
  }
}

/* Queue empty condition is : head == tail */
unsigned int dequeue() {
  if(ringBuffer.head == ringBuffer.tail) {
    printf("Queue is empty. No data to be shared.");
    return 0;
  } else {
    unsigned int d = ringBuffer.data[ringBuffer.tail & MASK];
    ringBuffer.tail++;
    return d;
  }
}

/* Function to "produce" aka. enqueue() random numbers */
void producerWork() {

  /* TODO: Add code */

}

/* Function to "consume" aka. dequeue() numbers from the queue */
void consumerWork() {

  /* TODO: Add code */

}

/* The function which the producer thread runs continuously */
void producer() {

  /* TODO: Add code */

}

void consumer() {

  /* TODO: Add code */

}

int main() {
  pthread_t producer[PRODUCERCOUNT];
  pthread_t consumer[CONSUMERCOUNT];

  return 0;  
}
