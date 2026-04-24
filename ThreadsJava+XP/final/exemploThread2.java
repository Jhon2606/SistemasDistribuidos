//O ganho de desempenho com threads existe mas é menor que o ideal (reducao de +-75% com 4 threads). Por causa do Overhead, Mémoria compartilhada e gargalo de memoria.
public class exemploThread2 {

    public static final int SIZE = 100000000;
    private static int numThreads;
    private static double[] array = new double[SIZE];
    private static double global_sum = 0.0;

    //SEM THREAD
    private static double sum_single() {
        double sum = 0.0;
        for (int i = 0; i < SIZE; i++) {
            sum += array[i];
        }
        return sum;
    }

    public static void main(String[] args) throws InterruptedException {

        numThreads = Integer.parseInt(args[0]);

        for (int i = 0; i < SIZE; i++) {
            array[i] = 1.0;
        }

        //SEM THREAD
        long start = System.currentTimeMillis();
        double result_single = sum_single();
        long end = System.currentTimeMillis();

        double time_single = (double)(end - start);
        System.out.println("Sem thread: soma = " + result_single + ", tempo = " + time_single);

        //COM THREAD (Runnable dentro)
        long chunk = SIZE / numThreads;
        Thread[] threads = new Thread[numThreads];
        double[] partial_sums = new double[numThreads];

        global_sum = 0.0;
        start = System.currentTimeMillis();

        for (int i = 0; i < numThreads; i++) {
            final long st = i * chunk;
            final long e  = (i == numThreads - 1) ? SIZE : (i + 1) * chunk;
            final int index = i;

            // Runnable dentro do codigo
            Runnable r = new Runnable() {
                public void run() {
                    System.out.println("Start = " + st + "     End = " + e);
                    double local_sum = 0.0;
                    for (long j = st; j < e; j++) {
                        local_sum += array[(int) j];
                    }
                    partial_sums[index] = local_sum;
                }
            };

            threads[i] = new Thread(r);
            threads[i].start();
        }

        // Aguarda todas as threads finalizarem
        for (int i = 0; i < numThreads; i++) {
            threads[i].join();
            global_sum += partial_sums[i];
        }

        end = System.currentTimeMillis();
        double time_thread = (double)(end - start);
        System.out.println("Com thread: soma = " + global_sum + ", tempo = " + time_thread);
    }
}
