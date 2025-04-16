#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1

int buffer[MAX_BUFFER_SIZE];
int in = 0;  
int out = 0;
int buffer_count = 0;

sem_t empty, full, mutex;

int num_producers, num_consumers;

void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 100; 

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        buffer_count++;
        printf("Producer %d produced %d\n", id, item);
        printf("Buffer:%d\n", buffer[in]);

        in = (in + 1) % MAX_BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);

        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        printf("Consumer %d consumed %d\n", id, item);
        buffer_count--;
        printf("Current buffer len: %d\n", buffer_count);

        out = (out + 1) % MAX_BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);

        sleep(1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    printf("Enter the number of Producers: ");
    scanf("%d", &num_producers);

    printf("Enter the number of Consumers: ");
    scanf("%d", &num_consumers);

    printf("Enter buffer capacity: ");
    int cap;
    scanf("%d", &cap);

    if (cap != 1) {
        printf("This example only supports buffer capacity 1 (like your output screenshot).\n");
        return 1;
    }

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, MAX_BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_t prod_threads[num_producers];
    pthread_t cons_threads[num_consumers];

    int ids[num_producers > num_consumers ? num_producers : num_consumers];

    for (int i = 0; i < num_producers; i++) {
        ids[i] = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, &ids[i]);
        printf("Successfully created producer %d\n", i + 1);
    }

    for (int i = 0; i < num_consumers; i++) {
        ids[i] = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, &ids[i]);
        printf("Successfully created consumer %d\n", i + 1);
    }

    for (int i = 0; i < num_producers; i++)
        pthread_join(prod_threads[i], NULL);

    for (int i = 0; i < num_consumers; i++)
        pthread_join(cons_threads[i], NULL);

    return 0;
}
