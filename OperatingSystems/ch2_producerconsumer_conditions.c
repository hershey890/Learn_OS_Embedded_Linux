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

#define MAX 20                  // How many numbers to produce
pthread_mutex_t mutex;          // controls access to critical region
pthread_cond_t condc, condp;    // used for signaling
int buffer = 0;                     


void *producer() {
    for(int i=1; i<=MAX; i++) {
        pthread_mutex_lock(&mutex);     // get exclusive access to buffer
        while(buffer != 0)              
            pthread_cond_wait(&condp, &mutex); // block until another thread signals to it
        buffer = i;                     // put item into buffer
        pthread_cond_signal(&condc);    // wake consumer
        pthread_mutex_unlock(&mutex);   // release access to buffer
    }
    pthread_exit(0);
}


void *consumer() {
    for(int i=1; i<=MAX; i++) {
        pthread_mutex_lock(&mutex);     // get exclusive access to buffer
        while(buffer == 0)
            pthread_cond_wait(&condc, &mutex);
        buffer = 0;
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}


int main() {
    printf("Start\n");
    pthread_t pro, con;
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);
    pthread_join(pro, 0);
    pthread_join(con, 0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&mutex);
    printf("Done\n");
}