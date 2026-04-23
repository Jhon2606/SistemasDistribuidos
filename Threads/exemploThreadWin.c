#include <windows.h>
#include <stdio.h>
#include <time.h>

#define SIZE 100000000
#define NUM_THREADS 4

double array[SIZE];
double global_sum = 0.0;

typedef struct {
    long start;
    long end;
} ThreadData;

HANDLE threads[NUM_THREADS];
DWORD dwThreadId[NUM_THREADS];
ThreadData thread_data[NUM_THREADS];
HANDLE hMutex;

// ---------------- SEM THREAD ----------------
double sum_single() {
    double sum = 0.0;
    for (long i = 0; i < SIZE; i++) {
        sum += array[i];
    }
    return sum;
}

// Função que a thread irá executar
DWORD WINAPI sum_thread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*) lpParam;
    double local_sum = 0.0;

    for (long i = data->start; i < data->end; i++) {
        local_sum += array[i];
    }

    WaitForSingleObject(hMutex, INFINITE);
    global_sum += local_sum;
    ReleaseMutex(hMutex);

    return 0;
}

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

    hMutex = CreateMutex(NULL, FALSE, NULL); // Cria mutex anônimo

    long chunk = SIZE / NUM_THREADS;

    start = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk;
        thread_data[i].end = (i == NUM_THREADS - 1) ? SIZE : (i + 1) * chunk;

	    // Criando a thread
        threads[i] = CreateThread(
            NULL,                   // Atributos de segurança padrão
            0,                      // Tamanho da pilha padrão
            sum_thread,             // Função da thread
            &thread_data[i],         // Parâmetro para a função
            0,                      // Flags de criação padrão
            &dwThreadId[i]          // Recebe o ID da thread
        );

        if (threads[i] == NULL) {
            printf("Erro ao criar thread: %d\n", GetLastError());
            return 1;
        }
		
    }

    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    end = clock();

    double time_thread = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Com threads: soma = %.2f, tempo = %f segundos\n", global_sum, time_thread);
    
    CloseHandle(hMutex);
	for (int i = 0; i< NUM_THREADS; i++)
        CloseHandle(threads[i]);

    return 0;
}
