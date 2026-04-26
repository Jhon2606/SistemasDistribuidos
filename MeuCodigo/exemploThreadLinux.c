#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define SIZE 100000000   // 100 milhões de elementos
#define NUM_THREADS 4    // Vamos usar 4 threads
double array[SIZE];      // Array global com 100 milhões de doubles
double global_sum = 0.0; // Resultado final (compartilhado entre threads)
pthread_mutex_t mutex;   // mutex é o mecanismo que vai evitar condição de corrida (dois processos escrevendo no mesmo lugar ao mesmo tempo)

// ---------------- SEM THREAD ----------------
double sum_single()
{
    double sum = 0.0;
    for (long i = 0; i < SIZE; i++)
    {
        sum += array[i];
    }
    return sum;
}

// ---------------- COM THREAD ----------------
typedef struct
{
    long start;
    long end;
} ThreadData;

void *sum_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    double local_sum = 0.0;
    for (long i = data->start; i < data->end; i++)
    {
        local_sum += array[i];
    }
    pthread_mutex_lock(&mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&mutex); // Sem o mutex, duas threads poderiam ler global_sum ao mesmo tempo, somar seus valores locais, e a que terminar por último sobrescreveria o resultado da outra — perdendo dados. Isso é chamado de condição de corrida (race condition).
    pthread_exit(NULL);
}

int main()
{
    // Inicializa array
    for (long i = 0; i < SIZE; i++)
    {
        array[i] = 1.0;
    }

    struct timespec start, end;

    // ----------- SEM THREAD -----------
    clock_gettime(CLOCK_MONOTONIC, &start);
    double result_single = sum_single();
    clock_gettime(CLOCK_MONOTONIC, &end);

    double tempo_sem_thread = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Sem thread: soma = %.2f, tempo = %f segundos\n", result_single, tempo_sem_thread);

    // ----------- COM THREAD -----------
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);
    long chunk = SIZE / NUM_THREADS;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].start = i * chunk;
        thread_data[i].end = (i == NUM_THREADS - 1) ? SIZE : (i + 1) * chunk;

        pthread_create(&threads[i], NULL, sum_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // pthread_join faz a main esperar cada thread terminar antes de continuar. Sem isso, o programa poderia imprimir o resultado antes das threads finalizarem.

    clock_gettime(CLOCK_MONOTONIC, &end);
    double tempo_com_thread = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Com threads: soma = %.2f, tempo = %f segundos\n", global_sum, tempo_com_thread);
    pthread_mutex_destroy(&mutex);

    return 0;
}