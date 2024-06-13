#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 20                  // # numbers to produce
#define N   4                   // # slots in the buffer
#define N_CONSUMERS 4           // # of consumer threads
pthread_mutex_t cnsmr_mutex;    // controls access 'rc'
// pthread_mutex_t buf_idx_mutex; // controls acces to 'buf_idx'
pthread_mutex_t db;             // controls access to the database
volatile int rc = 0;            // # of processes reading or wanting to
sem_t space;                    // counts # space buffer slots
sem_t items;                     // counts # items buffer slots
int *buffer;                    // pointer to the buffer array of size N
size_t buf_idx;
size_t buf_start;

void consumeItem(int *item) {
    printf("C: %d\n", *item);
}

void* producer() {
    int item, idx;
    for(int i=0; i<MAX; i++) {        
        item = i;

        sem_wait(&space);
        pthread_mutex_lock(&db);

        buffer[(buf_idx++) % N] = item;

        pthread_mutex_unlock(&db);
        sem_post(&items);
    }
    pthread_exit(0);
}

void* consumer() {
    // p166
    int item, idx;
    // for(int i=0; i<MAX; i++) {
    while(1) {
        sem_wait(&items);                    // decrement items count

        // pthread_mutex_lock(&cnsmr_mutex);
        // rc_orig = rc;
        // rc++;
        // if(rc == 1)
            pthread_mutex_lock(&db);    // if this is the first consumer
        // pthread_mutex_unlock(&cnsmr_mutex);
        
        // idx = buf_idx % N;
        // buf_idx--;
        // item = buffer[idx];
        item = buffer[buf_start % N];
        buf_start++;

        // pthread_mutex_lock(&cnsmr_mutex);
        // rc--;
        // if(rc == 0)
            pthread_mutex_unlock(&db);
        // pthread_mutex_unlock(&cnsmr_mutex);

        sem_post(&space);

        // pthread_mutex_lock(&cnsmr_mutex);
        consumeItem(&item);
        // pthread_mutex_unlock(&cnsmr_mutex);
    }
    pthread_exit(0);
}

int main(void) {
    pthread_t pro;
    pthread_t con_pool[N_CONSUMERS];
    buffer = (int*)malloc(N*sizeof(int));
    buf_idx = 0;
    buf_start = 0;

    pthread_mutex_init(&cnsmr_mutex, NULL);
    pthread_mutex_init(&db, NULL);
    sem_init(&space, 0, N);
    sem_init(&items, 0, 0);

    pthread_create(&pro, NULL, producer, NULL);
    for(int i=0; i<N_CONSUMERS; i++)
        pthread_create(&(con_pool[i]), NULL, consumer, NULL);

    for(int i=N_CONSUMERS-1; i>=0; i--)
        pthread_join(con_pool[i], NULL);
    pthread_join(pro, NULL);
    
    sem_destroy(&items);
    sem_destroy(&space);
    pthread_mutex_destroy(&db);
    pthread_mutex_destroy(&cnsmr_mutex);

    free(buffer);
}
