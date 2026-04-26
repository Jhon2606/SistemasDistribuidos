#include <windows.h>
#include <stdio.h>
#include <time.h>

#define SIZE 100000000
#define NUM_THREADS 4

double array[SIZE];
double global_sum = 0.0;

typedef struct
{
    long start;
    long end;
} ThreadData;

HANDLE threads[NUM_THREADS];
DWORD dwThreadId[NUM_THREADS];
ThreadData thread_data[NUM_THREADS];
HANDLE hMutex;

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

DWORD WINAPI sum_thread(LPVOID lpParam)
{
    ThreadData *data = (ThreadData *)lpParam;
    double local_sum = 0.0;

    for (long i = data->start; i < data->end; i++)
    {
        local_sum += array[i];
    }

    WaitForSingleObject(hMutex, INFINITE);
    global_sum += local_sum;
    ReleaseMutex(hMutex);

    return 0;
}

int main()
{
    LARGE_INTEGER freq, start, end;
    for (long i = 0; i < SIZE; i++)
    {
        array[i] = 1.0;
    }
    // ----------- SEM THREAD -----------
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    double result_single = sum_single();
    QueryPerformanceCounter(&end);

    double tempo_sem_thread = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    printf("Sem thread: soma = %.2f, tempo = %f segundos\n", result_single, tempo_sem_thread);

    // ----------- COM THREAD -----------
    hMutex = CreateMutex(NULL, FALSE, NULL);
    long chunk = SIZE / NUM_THREADS;
    QueryPerformanceCounter(&start);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].start = i * chunk;
        thread_data[i].end = (i == NUM_THREADS - 1) ? SIZE : (i + 1) * chunk;

        threads[i] = CreateThread(NULL, 0, sum_thread, &thread_data[i], 0, &dwThreadId[i]);
        if (threads[i] == NULL)
        {
            printf("Erro ao criar thread: %d\n", GetLastError());
            return 1;
        }
    }

    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);
    QueryPerformanceCounter(&end);
    double tempo_com_thread = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    printf("Com threads: soma = %.2f, tempo = %f segundos\n", global_sum, tempo_com_thread);

    CloseHandle(hMutex);
    for (int i = 0; i < NUM_THREADS; i++)
        CloseHandle(threads[i]);

    return 0;
}