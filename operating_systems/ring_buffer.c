/**
 * @brief : A simple ring buffer implementation using pthreads
 * Innefficient. Only uses 2 threads and has no thread synchronization. Simply check the buffer in each iteration in a while loop.
 * @param[in] : uint32_t maximum number of items to write to the buffer. Writes from [0, max)
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>

#define BUF_SIZE 256

static uint32_t ring_buf[BUF_SIZE];
static uint32_t head_idx = 0;
static uint32_t tail_idx = 0;

/**
 * @brief : Writes values to the ring buffer
 * @param data : value to write to the buffer
 */
uint8_t writeBuffer(uint32_t data) {
    if((tail_idx + 1) % BUF_SIZE == head_idx) // buff full
        return EXIT_FAILURE;
    ring_buf[tail_idx] = data;
    tail_idx = (tail_idx + 1) % BUF_SIZE;
    return EXIT_SUCCESS;
}

/**
 * @brief : Reads values from the ring buffer
 * @param data : pointer to value read from the buffer
 */
uint8_t readBuffer(uint32_t *data) {
    if(tail_idx == head_idx) // buffer empty
        return EXIT_FAILURE;
    *data = ring_buf[head_idx];
    head_idx = (head_idx + 1) % BUF_SIZE;
    return EXIT_SUCCESS;
}

/**
 * @brief : Reads values from the ring buffer
 * Thread function to be called by pthread_create
 * @param max : maximum number of values to read from the buffer
 */
void *readHandler(void *max) {
    uint32_t max_num = (uint32_t)max;
    uint32_t data;

    while(1) {
        if(readBuffer(&data) == EXIT_SUCCESS) {
            printf("Read value %d\n", data);
            if(data == max_num)
                break;
        }
    }

    pthread_exit(NULL);
}

/**
 * @brief : Writes values to the ring buffer
 * Thread function to be called by pthread_create
 * @param max : maximum number of values to write to the buffer
 */
void *writeHandler(void *max) {
    uint32_t max_num = (uint32_t)max;
    for(uint32_t i=0; i<max_num; ) {
        if(writeBuffer(i) == EXIT_SUCCESS) {
            printf("Wrote value %d\n", i);
            i++;
        }
    }

    pthread_exit(NULL);
}

void handle_sigint(int sig) {
    printf("Caught signal %d\n", sig);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("ERROR: ring_buffer expects one uint32_t value as an argument\n");
        exit(EXIT_FAILURE);
    }

    size_t num_elem_write = (size_t)atoi(argv[1]);
    pthread_t read_thread, write_thread;

    signal(SIGINT, handle_sigint);

    if(pthread_create(&read_thread, NULL, readHandler, (void*)num_elem_write)) {
        printf("ERROR: Read thread spawning failure\n");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&write_thread, NULL, writeHandler, (void*)num_elem_write)) {
        printf("ERROR: Write thread spawning failure\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(write_thread, NULL);
    pthread_join(read_thread, NULL);


    return 0;
}