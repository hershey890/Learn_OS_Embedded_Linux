/* Producer-Consumer problem using mutexes and conditions

From Tanebaum/Bos Modern Operating Systems 4th ed. pg 138

Resources
---------
* https://man7.org/linux/man-pages/man7/pthreads.7.html
* https://man7.org/linux/man-pages/man0/semaphore.h.0p.html

Compiling/Running
-----------------
>>> gcc ch2_producerconsumer_conditions.c -o ch2_producerconsumer_conditions.exe -lpthread
>>> ./ch2_producerconsumer_conditions.exe
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 20                  // number of elements to remove from buffer
#define N 20                     // buffer size
pthread_mutex_t mutex;
pthread_cond_t condc, condp;
int* buffer;
size_t buf_start, buf_end;

void *producer() {
    for(int i=0; i<MAX; i++) {
        pthread_mutex_lock(&mutex);
        while((buf_end + 1) % N == buf_start) // buffer full
            pthread_cond_wait(&condp, &mutex);
        buffer[buf_end] = i;
        buf_end = (buf_end + 1) % N;
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void *consumer() {
    int item;
    for(int i=0; i<MAX; i++) {
        pthread_mutex_lock(&mutex);
        while(buf_start == buf_end) // buffer empty
            pthread_cond_wait(&condc, &mutex);
        item = buffer[buf_start];
        buf_start = (buf_start + 1) % N;
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&mutex);

        printf("C: %d\n", item);
    }
    pthread_exit(0);
}

int main(void) {
    buffer = (int*)malloc(N*sizeof(int));
    buf_start = 0;
    buf_end = 0;

    pthread_t pro, con;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condc, NULL);
    pthread_cond_init(&condc, NULL);

    pthread_create(&pro, NULL, producer, NULL);
    pthread_create(&con, NULL, consumer, NULL);
    pthread_join(con, NULL);
    pthread_join(pro, NULL);

    pthread_cond_destroy(&condp);
    pthread_cond_destroy(&condc);
    pthread_mutex_destroy(&mutex);
    free(buffer);
}