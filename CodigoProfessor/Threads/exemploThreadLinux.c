#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 100000000
#define NUM_THREADS 4

double array[SIZE];
double global_sum = 0.0;

pthread_mutex_t mutex;

// ---------------- SEM THREAD ----------------
double sum_single() {
    double sum = 0.0;
    for (long i = 0; i < SIZE; i++) {
        sum += array[i];
    }
    return sum;
}

// ---------------- COM THREAD ----------------
typedef struct {
    long start;
    long end;
} ThreadData;

void* sum_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double local_sum = 0.0;

    for (long i = data->start; i < data->end; i++) {
        local_sum += array[i];
    }

    pthread_mutex_lock(&mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

// ---------------- MAIN ----------------
int main() {
    // Inicializa array
    for (long i = 0; i < SIZE; i++) {
        array[i] = 1.0;
    }

    clock_t start, end;

    // ----------- SEM THREAD -----------
    start = clock();
    double result_single = sum_single();
    end = clock();

    double time_single = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Sem thread: soma = %.2f, tempo = %f segundos\n", result_single, time_single);

    // ----------- COM THREAD -----------
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    long chunk = SIZE / NUM_THREADS;

    start = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk;
        thread_data[i].end = (i == NUM_THREADS - 1) ? SIZE : (i + 1) * chunk;

        pthread_create(&threads[i], NULL, sum_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    end = clock();

    double time_thread = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Com threads: soma = %.2f, tempo = %f segundos\n", global_sum, time_thread);

    pthread_mutex_destroy(&mutex);

    return 0;
}

