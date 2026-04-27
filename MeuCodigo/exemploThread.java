import java.io.IOException;

public class exemploThread extends Thread {

    public static final int SIZE = 100000000;
    private static int numThreads;
    private long start;
    private long end;
    private static double[] array = new double[SIZE];
    
    // global_sum precisa ser static para ser compartilhada entre threads
    private static double global_sum = 0.0;

    public exemploThread(long st, long e) {
        this.start = st;
        this.end = e;
    }

    private static double sum_single() {
        double sum = 0.0;
        for (int i = 0; i < SIZE; i++) {
            sum += array[i];
        }
        return sum;
    }

    @Override
    public void run() {
        // run() precisa fazer o cálculo real, não só imprimir
        double local_sum = 0.0;
        for (long i = this.start; i < this.end; i++) {
            local_sum += array[(int) i]; // cast: índice de array é int em Java
        }

        // sincronização para evitar race condition ao somar no global
        synchronized (exemploThread.class) {
            global_sum += local_sum;
        }
    }

    //main precisa declarar InterruptedException por causa do join()
    public static void main(String[] args) throws IOException, InterruptedException {

        numThreads = Integer.parseInt(args[0]);

        for (int i = 0; i < SIZE; i++) {
            array[i] = 1.0;
        }

        // ----------- SEM THREAD -----------
        long start = System.currentTimeMillis();
        double result_single = sum_single();
        long end = System.currentTimeMillis();

        System.out.println("Sem thread: soma = " + result_single + ", tempo = " + (end - start) + " ms");

        // ----------- COM THREAD -----------
        global_sum = 0.0; // resetar antes de usar com threads
        long chunk = SIZE / numThreads;

        // guardar referências das threads para fazer join depois
        exemploThread[] threads = new exemploThread[numThreads];

        start = System.currentTimeMillis();

        for (int i = 0; i < numThreads; i++) {
            long st = i * chunk;
            long e = (i == numThreads - 1) ? SIZE : (i + 1) * chunk;
            threads[i] = new exemploThread(st, e);
            threads[i].start();
        }

        // aguardar todas as threads terminarem antes de ler o resultado
        for (int i = 0; i < numThreads; i++) {
            threads[i].join();
        }

        end = System.currentTimeMillis();
        System.out.println("Com thread: soma = " + global_sum + ", tempo = " + (end - start) + " ms");
    }
}