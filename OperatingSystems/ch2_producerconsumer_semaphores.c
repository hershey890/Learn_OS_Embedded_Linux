/* Producer-Consumer problem using only semaphores/mutexes, no conditions/signals

From Tanebaum/Bos Modern Operating Systems 4th ed. pg 131 and 138

Resources
---------
* https://man7.org/linux/man-pages/man7/pthreads.7.html
* https://man7.org/linux/man-pages/man0/semaphore.h.0p.html

Compiling/Running
-----------------
>>> gcc ch2_producerconsumer_semaphores.c -o ch2_producerconsumer_semaphores.exe -lpthread
>>> ./ch2_producerconsumer_semaphores.exe
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 20          // How many numbers to produce
#define N 100           // # slots in buffer
pthread_mutex_t mutex;  // controls access to critical region
sem_t empty;            // counts empty buffer slots
sem_t full;             // counts full buffer slots
int *buffer;            // pointer to buffer array of size N


void consume_item(int *item) {
    printf("Removed item: %d\n", *item);
}


void *producer() {
    int item, idx;
    for(int i=0; i<MAX; i++) {
        item = i;                       // produce item to insert in buffer
        sem_wait(&empty);               // decrement empty count
        pthread_mutex_lock(&mutex);     // enter critical region
        sem_getvalue(&empty, &idx);     // insert item into buffer
        buffer[N-idx] = item;
        pthread_mutex_unlock(&mutex);   // leave critical region
        sem_post(&full);                // increment # full slots
    }
    pthread_exit(0);
}


void *consumer() {
    int item, idx;
    for(int i=0; i<MAX; i++) {
        sem_wait(&full);                // decrement full count
        pthread_mutex_lock(&mutex);     // enter critical region
        sem_getvalue(&full, &idx);      // remove item from buffer
        item = buffer[idx];
        pthread_mutex_unlock(&mutex);   // leave critical region
        sem_post(&empty);               // increment # full slots
        consume_item(&item);            // do something with item
    }
    pthread_exit(0);
}


int main() {
    buffer = (int*)malloc(N*sizeof(int));   // allocate buffer
    
    /* Init Mutexes, Semaphores, and Threads */
    pthread_t pro, con;
    pthread_mutex_init(&mutex, 0);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);

    /* Destroy Mutex, Semaphores, and Threads */
    pthread_join(pro, 0);
    pthread_join(con, 0);
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);

    free(buffer);
}