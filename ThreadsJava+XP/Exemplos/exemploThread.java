import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;

/**
 *
 * @author helde
 */
public class exemploThread extends Thread {

	public static final int SIZE = 100000000;
	private static int numThreads;
	private long start;
	private long end;
	private static double[] array = new double[SIZE];
	private double global_sum = 0.0;

//    public exemploThread(int numThr) {
//        this.numThreads = numThr;
//    }

    public exemploThread(long st, long e) {
        this.start = st;
		this.end = e;
    }
	

	// ---------------- SEM THREAD ----------------
	private static double sum_single() {
		double sum = 0.0;
		for (int i = 0; i < SIZE; i++) {
			sum += array[i];
		}
		return sum;
	}
/*
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
*/	
	
    public static void main(String args[]) throws IOException {

        numThreads = Integer.parseInt(args[0]);

		// Inicializa array
		for (int i = 0; i < SIZE; i++) {
			array[i] = 1.0;
		}

		// ----------- SEM THREAD -----------
		long start = System.currentTimeMillis();
		double result_single = sum_single();
		long end = System.currentTimeMillis();

		double time_single = (double)(end - start);
		System.out.println("Sem thread: soma = "+String.valueOf(result_single)+", tempo = "+String.valueOf(time_single));

		// ----------- COM THREAD -----------
		long chunk = SIZE / numThreads;

		start = System.currentTimeMillis();

		result_single = 0.0;
		for (int i = 0; i < numThreads; i++) {
			long st = i * chunk;
			long e  = (i == numThreads - 1) ? SIZE : (i + 1) * chunk;
//			System.out.println("main Start = "+String.valueOf(st)+"     End = "+String.valueOf(e));
            exemploThread thread = new exemploThread(st, e);
            thread.start();
		}
    	end = System.currentTimeMillis();
		time_single = (double)(end - start);
		System.out.println("Com thread: soma = "+String.valueOf(result_single)+", tempo = "+String.valueOf(time_single));
	}
	
	@Override
    public void run() {
		System.out.println("Start = "+String.valueOf(this.start)+"     End = "+String.valueOf(this.end));
    }
}





